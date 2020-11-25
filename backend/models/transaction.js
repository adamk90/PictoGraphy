const Schema = require('mongoose').Schema;
const db = require('../config/db');

const Transaction = db.model('Transaction', {
    _customer:  {
		type: Schema.Types.ObjectId,
        ref: 'User'
    },
    _product: [{
		type: Schema.Types.ObjectId,
        ref: 'CAFF'
    } ]
});

module.exports = Transaction;