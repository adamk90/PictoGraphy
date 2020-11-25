const Schema = require('mongoose').Schema;
const db = require('../config/db');

const CAFF = db.model('CAFF', {
	content: Binary,
	_comments: [{
		type: Schema.Types.ObjectId,
		ref: 'Comment'
	}],
	_caffOwner: {
		type: Schema.Types.ObjectId,
		ref: 'User'
	}
});

module.export = CAFF;