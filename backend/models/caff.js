const Schema = require('mongoose').Schema;
const db = require('../config/db');

const Caff = db.model('CAFF',  {
	content: Schema.Types.Buffer,
	_comments: [{
		type: Schema.Types.ObjectId,
		ref: 'Comment'
	}],
	_caffOwner: {
		type: Schema.Types.ObjectId,
		ref: 'User'
	}
});

module.exports = Caff;