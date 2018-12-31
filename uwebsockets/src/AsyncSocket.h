/*
 * Copyright 2018 Alex Hultman and contributors.

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ASYNCSOCKET_H
#define ASYNCSOCKET_H

/* This class implements async socket memory management strategies */

#include "StaticDispatch.h"
#include "LoopData.h"
#include "AsyncSocketData.h"

namespace uWS {

template <bool SSL>
struct AsyncSocket : StaticDispatch<SSL> {
    template <bool> friend struct HttpContext;
    template <bool, bool> friend struct WebSocketContext;
protected:
    using SOCKET_TYPE = typename StaticDispatch<SSL>::SOCKET_TYPE;
    using StaticDispatch<SSL>::static_dispatch;

    /* Get loop data for socket */
    LoopData *getLoopData() {
        return (LoopData *) us_loop_ext(
                    static_dispatch(us_ssl_socket_context_loop, us_socket_context_loop)(
                        static_dispatch(us_ssl_socket_get_context, us_socket_get_context)((SOCKET_TYPE *) this))
                    );
    }

    /* Get socket extension */
    void *getExt() {
        return static_dispatch(us_ssl_socket_ext, us_socket_ext)((SOCKET_TYPE *) this);
    }

    /* Socket timeout */
    void timeout(unsigned int seconds) {
        static_dispatch(us_ssl_socket_timeout, us_socket_timeout)((SOCKET_TYPE *) this, seconds);
    }

    /* Shutdown socket without any automatic drainage */
    void shutdown() {
        static_dispatch(us_ssl_socket_shutdown, us_socket_shutdown)((SOCKET_TYPE *) this);
    }

    /* Immediately close socket */
    SOCKET_TYPE *close() {
        return static_dispatch(us_ssl_socket_close, us_socket_close)((SOCKET_TYPE *) this);
    }

    /* Cork this socket. Only one socket may ever be corked per-loop at any given time */
    void cork() {
        /* What if another socket is corked? */
        getLoopData()->corkedSocket = this;
    }

    /* Returns wheter we are corked or not */
    bool isCorked() {
        return getLoopData()->corkedSocket == this;
    }

    /* Returns a suitable buffer for temporary assemblation of send data */
    std::pair<char *, bool> getSendBuffer(size_t size) {
        /* If we are corked and we have room, return the cork buffer itself */
        LoopData *loopData = getLoopData();
        if (loopData->corkedSocket == this && loopData->corkOffset + size < LoopData::CORK_BUFFER_SIZE) {
            char *sendBuffer = loopData->corkBuffer + loopData->corkOffset;
            loopData->corkOffset += size;
            return {sendBuffer, false};
        } else {
            // slow path for now

            return {(char *) malloc(size), true};

            // if we are out of buffer, fail this completely?

        }
    }

    /* Returns the user space backpressure. */
    int getBufferedAmount() {
        AsyncSocketData<SSL> *asyncSocketData = (AsyncSocketData<SSL> *) getExt();

        return asyncSocketData->buffer.size();
    }

    /* Write in three levels of prioritization: cork-buffer, syscall, socket-buffer. Always drain if possible.
     * Returns pair of bytes written (anywhere) and wheter or not this call resulted in the polling for
     * writable (or we are in a state that implies polling for writable). */
    std::pair<int, bool> write(const char *src, int length, bool optionally = false, int nextLength = 0) {
        /* Fake success if closed, simpel fix to allow uncork of closed socket to succeed */
        if (us_socket_is_closed((us_socket *) this)) {
            std::cout << "Faking successful send due to closed socket!" << std::endl;
            return {length, false};
        }

        LoopData *loopData = getLoopData();
        AsyncSocketData<SSL> *asyncSocketData = (AsyncSocketData<SSL> *) getExt();

        /* We are limited if we have a per-socket buffer */
        if (asyncSocketData->buffer.length()) {
            /* Write off as much as we can */
            int written = static_dispatch(us_ssl_socket_write, us_socket_write)((SOCKET_TYPE *) this, asyncSocketData->buffer.data(), asyncSocketData->buffer.length(), /*nextLength != 0 | */length);

            /* On failure return, otherwise continue down the function */
            if (written < asyncSocketData->buffer.length()) {

                /* Update buffering (todo: we can do better here if we keep track of what happens to this guy later on) */
                asyncSocketData->buffer = asyncSocketData->buffer.substr(written);

                if (optionally) {
                    /* Thankfully we can exit early here */
                    return {0, true};
                } else {
                    /* This path is horrible and points towards erroneous usage */
                    asyncSocketData->buffer.append(src, length);

                    // todo: remove this when we are no longer morons
                    std::cout << "Warning: fudge this is horrible!!" << std::endl;

                    return {length, true};
                }
            }

            /* At this point we simply have no buffer and can continue as normal */
            asyncSocketData->buffer.clear();
        }

        if (length) {
            if (loopData->corkedSocket == this) {
                /* We are corked */
                if (LoopData::CORK_BUFFER_SIZE - loopData->corkOffset >= length) {
                    /* If the entire chunk fits in cork buffer */
                    memcpy(loopData->corkBuffer + loopData->corkOffset, src, length);
                    loopData->corkOffset += length;
                    /* Fall through to default return */
                } else {
                    /* Strategy differences between SSL and non-SSL regarding syscall minimizing */
                    if constexpr (SSL) {
                        /* Cork up as much as we can */
                        int stripped = LoopData::CORK_BUFFER_SIZE - loopData->corkOffset;
                        memcpy(loopData->corkBuffer + loopData->corkOffset, src, stripped);
                        loopData->corkOffset = LoopData::CORK_BUFFER_SIZE;

                        auto [written, failed] = uncork(src + stripped, length - stripped, optionally);
                        return {written + stripped, failed};
                    }

                    /* For non-SSL we take the penalty of two syscalls */
                    return uncork(src, length, optionally);
                }
            } else {
                /* We are not corked */
                int written = static_dispatch(us_ssl_socket_write, us_socket_write)((SOCKET_TYPE *) this, src, length, nextLength != 0);

                /* Did we fail? */
                if (written < length) {
                    /* If the write was optional then just bail out */
                    if (optionally) {
                        return {written, true};
                    }

                    std::cout << "Buffering at bottom of write (okay)!" << std::endl;

                    /* Fall back to worst possible case (should be very rare for HTTP) */
                    /* At least we can reserve room for next chunk if we know it up front */
                    if (nextLength) {
                        asyncSocketData->buffer.reserve(asyncSocketData->buffer.length() + length - written + nextLength);
                    }

                    /* Buffer this chunk */
                    asyncSocketData->buffer.append(src + written, length - written);

                    /* Return the failure */
                    return {length, true};
                }
                /* Fall through to default return */
            }
        }

        /* Default fall through return */
        return {length, false};
    }

    /* Uncork this socket and flush or buffer any corked and/or passed data. It is essential to remember doing this. */
    /* It does NOT count bytes written from cork buffer (they are already accounted for in the write call responsible for its corking)! */
    std::pair<int, bool> uncork(const char *src = nullptr, int length = 0, bool optionally = false) {
        LoopData *loopData = getLoopData();

        if (loopData->corkedSocket == this) {
            loopData->corkedSocket = nullptr;

            if (loopData->corkOffset) {
                /* Corked data is already accounted for via its write call */
                auto [written, failed] = write(loopData->corkBuffer, loopData->corkOffset, false, length);
                loopData->corkOffset = 0;

                if (failed) {
                    /* We do not need to care for buffering here, write does that */
                    return {0, true};
                }
            }

            /* We should only return with new writes, not things written to cork already */
            return write(src, length, optionally, 0);
        } else {
            /* We are not even corked! */
            return {0, false};
        }
    }
};

}

#endif // ASYNCSOCKET_H
