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

#ifndef WEBSOCKETDATA_H
#define WEBSOCKETDATA_H

#include "WebSocketProtocol.h"
#include "AsyncSocketData.h"
#include "PerMessageDeflate.h"

#include <string>

namespace uWS {

// take care with get_ext here !
struct WebSocketData : AsyncSocketData<false>, WebSocketState<true> {
    template <bool, bool> friend struct WebSocketContext;
    template <bool, bool> friend struct WebSocket;
private:
    std::string fragmentBuffer;
    int controlTipLength = 0;
    bool isShuttingDown = 0;
    enum CompressionStatus : char {
        DISABLED,
        ENABLED,
        COMPRESSED_FRAME
    } compressionStatus;

    /* We might have a dedicated compressor */
    DeflationStream *deflationStream = nullptr;
public:
    WebSocketData(bool perMessageDeflate, bool slidingCompression) : WebSocketState<true>() {
        compressionStatus = perMessageDeflate ? ENABLED : DISABLED;

        /* Initialize the dedicated sliding window */
        if (perMessageDeflate && slidingCompression) {
            deflationStream = new DeflationStream;
        }
    }

    ~WebSocketData() {
        std::cout << "destruting webocketdata" << std::endl;

        if (deflationStream) {
            delete deflationStream;
        }
    }
};

}

#endif // WEBSOCKETDATA_H
