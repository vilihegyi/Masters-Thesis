sap.ui.define([
    "sap/m/MessageBox",
    "../controller/BaseController",
    "../lib/Blockchain"
], function (MessageBox, BaseController, Blockchain) {
    "use strict";

    return BaseController.extend("CarChain.controller.Details", {

        /**
         * Initializing method for creating Details page
         * @public
         */
        onInit : function () {
            this.blockchainInit();
        },

        /**
         * Initializing method for creating the Blockchain instance
         * @public
         */
        blockchainInit: function() {
            this.mBlockchainConnected = false;

            if (this.mBlockchain === undefined) {
                this.mBlockchainUrl = "//" + window.location.hostname + ":8080";
                this.mBlockchain = Blockchain.instance(this.mBlockchainUrl);

                this.mBlockchain.attachOpen(function() {
                    this.mBlockchainConnected = true;
                }, this);
                this.mBlockchain.attachError(function () {
                    if (!this.mBlockchainConnected) {
                        this.showError("Connection error", "Failed to connect to blockchain (" + this.mBlockchainUrl + ").", true);
                    }
                }, this);
                this.mBlockchain.attachClose(function () {
                    if (this.mBlockchainConnected) {
                        this.showError("Connection error", "Connection unexpectedly closed.", true);
                    }
                }, this);
            }
        },

        /**
         * Method appealed upon clicking on an image to do a reservation for a car
         * @param oEvent - Event object containing metadata about the clicked element
         * @private
         */
        bImagePress: function(oEvent) {
            const carName = oEvent.getSource().getAlt();
            console.log("Clicky click");
            this.mBlockchain.request({
                request: "car.addToBlockchain",
                data: {
                    details: carName
                }
            }, function(reply) {
                MessageBox.show("The reservation for " + carName + " was a success! \nYour hash is: " + reply.result);
            }, function(reply) {
                that.showError("Error", reply.result, false);
            });
        },

        /**
         * Initializing method for creating the Blockchain instance
         * @param title - Title of the dialog
         * @param message - Message to display in the dialog
         * @param reconnect - Pressing ok will refresh the page
         * @private
         */
        showError: function(title, message, reconnect) {
            let options = {};

            options.icon = MessageBox.Icon.ERROR;
            options.title = title;

            if (reconnect) {
                options.actions = sap.m.MessageBox.Action.RETRY;
                options.onClose = function () {
                    window.location.reload();
                };
                options.initialFocus = MessageBox.Action.RETRY;
            } else {
                options.actions = sap.m.MessageBox.Action.OK;
                options.initialFocus = MessageBox.Action.OK;
            }

            MessageBox.show(message, options);
        }
    });
});
