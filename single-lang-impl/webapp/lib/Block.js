sap.ui.define([
    "../controller/BaseController",
    "./SHA256"
], function (BaseController) {
    "use strict";

    return BaseController.extend("CarChain.lib.Block", {

        /**
         * Constructs a new Block that will be used in the Blockchain
         * @public
         */
        constructor: function(data, prevHash) {
            if (this.index) {
                this.index++;
            } else {
                this.index = 0;
            }
            this.timestamp = Date.now();
            this.data = data;
            this.prevHash = prevHash;
            this.mNonce = 0;
            this.thisHash = sha256(
                this.index + this.timestamp + this.data + this.mNonce + this.prevHash
            );

            // Repeat procedure until we get a string that starts
            // with substring 0000, as difficulty is set to be 4
            while (this.thisHash.substring(0, 4) != "0000") {
                this.mNonce++;
                this.thisHash = sha256(
                    this.index + this.timestamp + this.data + this.mNonce + this.prevHash
                );
            }
        },

        /**
         * Returns the hash of the current block
         * @public
         */
        hash: function() {
            return this.thisHash;
        }
    });
});
