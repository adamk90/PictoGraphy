const Schema = require('mongoose').Schema;
const db = require('../config/db');

const User = db.model('User', {
	userName: String,
	password: String,
	email: String,
	admin: Boolean
});

module.exports = User;