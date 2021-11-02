sap.ui.define([
    'sap/ui/core/ws/WebSocket'
], function (WebSocket) {
    "use strict";

    var blockchainInstance = null;

    var Blockchain = WebSocket.extend("CarChain.lib.Blockchain", {
        constructor: function () {
            WebSocket.apply(this, arguments);

            this.requests = {};
            this.requestIdCounter = 0;

            this.attachMessage(null, function (message) {
                const reply = JSON.parse(message.getParameter("data"));

                if (!this.requests.hasOwnProperty(reply.id)) {
                    return;
                }

                let delRequest = true;
                console.log(reply);

                if (reply.status === "success") {
                    this.requests[reply.id].success(reply);
                } else if (reply.status === "progress") {
                    this.requests[reply.id].progress(reply);
                    delRequest = false;
                } else {
                    this.requests[reply.id].failure(reply);
                }
                if (delRequest) {
                    delete this.requests[reply.id];
                }
            }, this);
        }
    });

    /**
     * Check if the instance is still open
     * @public
     */
    Blockchain.prototype.isConnectionAlive = function() {
        return this.getReadyState() === sap.ui.core.ws.ReadyState.OPEN;
    };

    /**
     * Create a new request to send to the back-end API
     * @param data - JSON storing the details
     * @param success - Success callback function
     * @param failure - Failure callback function
     * @param progress - Progress callback function
     * @public
     */
    Blockchain.prototype.request = function(data, success, failure, progress) {
        const requestId = ++this.requestIdCounter;
        console.log(requestId);

        // Send the data (as a string)
        data.id = requestId;
        this.send(JSON.stringify(data));

        // Place the request in the list of requests.
        this.requests[requestId] = {request: data, success: success, failure: failure, progress: progress};

        // Return ourself, for chaining.
        return this;
    };

    /**
     * Sends data to the back-end API
     * @param stringData - Stringified JSON to send to the API
     * @public
     */
    Blockchain.prototype.sendData = function(stringData) {
        this.send(stringData);

        return this;
    };

    /**
     * Create a new instance for the first time we initialize the blockchain, otherwise return
     * the instance we created so everyone uses the same Blockchain instance.
     * @public
     */
    return {
        instance: function () {
            if (blockchainInstance == null) {
                // We need a null-argument as this...
                Array.prototype.unshift.call(arguments, null);
                blockchainInstance = new (Function.prototype.bind.apply(Blockchain, arguments))();
            }
            return blockchainInstance;
        }
    };
});
