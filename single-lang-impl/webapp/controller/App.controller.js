sap.ui.define([
    "sap/m/MessageBox",
    "../controller/BaseController"
], function (MessageBox, BaseController) {
    "use strict";

    return BaseController.extend("CarChain.controller.App", {

        /**
         * Initializing method for displaying a welcome message
         * @public
         */
        onInit : function () {
            this.showWelcomeMessage();
        },

        /**
         * MessageBox with a welcome message for the user
         * with the option to close the opened dialog.
         * @private
         */
        showWelcomeMessage: function() {
            let options = {};

            options.icon = MessageBox.Icon.INFORMATION;
            options.title = "Welcome";
            options.actions = sap.m.MessageBox.Action.OK;
            options.initialFocus = MessageBox.Action.OK;

            MessageBox.show("Welcome to CarChain! Please pick a car to start!", options);
        }
    });
});
