const Schema = require('mongoose').Schema;
const db = require('../config/db');

const User = db.model('User', {
	userName: String,
	password: String,
	admin: Boolean
});

module.export = User;