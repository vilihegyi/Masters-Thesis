sap.ui.define([
    "../controller/BaseController",
    "../lib/Block"
], function (BaseController, Block) {
    "use strict";

    var blockchainInstance = null;

    var Blockchain = BaseController.extend("CarChain.lib.Blockchain", {
        constructor: function () {
            this.mBlockchain = [this.createGenesisBlock()];
            this.mPreviousBlock = this.mBlockchain[0];
        }
    });

    /**
     * Create the genesis Block for the blockchain (the root/first block)
     * @public
     */
    Blockchain.prototype.createGenesisBlock = function () {
        return new Block("Genesis Block", "0");
    };

    /**
     * Returns a new block for the chain to be appended to the chain
     * @param data - The details of the current block
     * @param previoushHash - The previous block's hash
     * @public
     */
    Blockchain.prototype.nextBlock = function (data, previoushHash) {
        return new Block(data, previoushHash);
    };

    /**
     * Add a new block with the data 
     * @param message - The message we will store in the block
     * @public
     */
    Blockchain.prototype.addNewBlock = function (message) {
        const blockToAdd = this.nextBlock("Block for car: " + message, this.mPreviousBlock.hash());
        this.mBlockchain.push(blockToAdd);
        this.mPreviousBlock = blockToAdd;
    };

    /**
     * Get the hash of the previous block
     * @public
     */
    Blockchain.prototype.getLastHash = function () {
        return this.mPreviousBlock.hash();
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
