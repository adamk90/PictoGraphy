const Schema = require('mongoose').Schema;
const db = require('../config/db');

const Caff = db.model('CAFF',  {
	content: String,
	preview: String,
	creator: String,
	creationDate: Date,
	tags: [String],
	_comments: [{
		type: Schema.Types.ObjectId,
		ref: 'Comment'
	}],
	_owner: {
		type: Schema.Types.ObjectId,
		ref: 'User'
	}
});

module.exports = Caff;