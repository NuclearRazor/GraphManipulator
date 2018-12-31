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

#ifndef HTTPRESPONSEDATA_H
#define HTTPRESPONSEDATA_H

/* This data belongs to the HttpResponse */

#include "HttpParser.h"
#include "AsyncSocketData.h"
#include <functional>

namespace uWS {

template <bool SSL>
struct HttpResponseData : HttpParser, AsyncSocketData<SSL> {
    template <bool> friend struct HttpResponse;
    template <bool> friend struct HttpContext;
private:
    /* Bits of status */
    enum {
        HTTP_STATUS_CALLED = 1, // used
        HTTP_WRITE_CALLED = 2, // used
        HTTP_END_CALLED = 4, // used
        HTTP_UPGRADED_TO_WEBSOCKET = 8, // not used
        HTTP_ENDED_STREAM_OUT = 16 // not used
    };

    /* Per socket event handlers */
    std::function<bool(int)> onWritable;
    std::function<void()> onAborted;
    //std::function<void()> onData;

    std::function<void(std::string_view)> inStream;
    std::function<std::pair<bool, std::string_view>(int)> outStream;
    /* Outgoing offset */
    int offset = 0;

    /* Current state (content-length sent, status sent, write called, etc */
    int state = 0;
};

}

#endif // HTTPRESPONSEDATA_H
