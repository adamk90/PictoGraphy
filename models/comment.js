const Schema = require('mongoose').Schema;
const db = require('../config/db');

const Comment = db.model('Comment', {
	text: String,
	_owner: {
		type: Schema.Types.ObjectId,
		ref: 'User'
	}
});

module.export = Comment;