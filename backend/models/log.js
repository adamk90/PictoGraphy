const Schema = require('mongoose').Schema;
const db = require('../config/db');

const Log = db.model('Log', {
	text: String,
	_timeStamp: {
		type: Date,
		default: new Date()
	},
	_user:  {
		type: Schema.Types.ObjectId,
        ref: 'User'
    }
});

module.exports = Log;