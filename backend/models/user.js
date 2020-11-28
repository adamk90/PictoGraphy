const Schema = require('mongoose').Schema;
const db = require('../config/db');

const User = db.model('User', {
	userName: {
		type: String,
		unique: true
	},
	password: String,
	email: {
		type: String,
		unique: true,
	},
	admin: Boolean
});

module.exports = User;