sap.ui.define([
    "sap/m/MessageBox",
    "../controller/BaseController",
    "../lib/Blockchain"
], function (MessageBox, BaseController, Blockchain) {
    "use strict";

    // Blockchain object
    var oBlockchain;

    return BaseController.extend("CarChain.controller.Details", {

        /**
         * Initializing method for creating the Blockchain instance
         * @public
         */
        onInit : function () {
            oBlockchain = Blockchain.instance();
        },

        /**
         * Method appealed upon clicking on an image to do a reservation for a car
         * @param oEvent - Event object containing metadata about the clicked element
         * @private
         */
        bImagePress: function (oEvent) {
            const carName = oEvent.getSource().getAlt();
            oBlockchain.addNewBlock(carName);
            MessageBox.show("The reservation for " + carName + " was a success! \nYour hash is: " + oBlockchain.getLastHash());
        }
    });
});
