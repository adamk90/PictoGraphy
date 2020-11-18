const Schema = require('mongoose').Schema;
const db = require('../config/db');

const User = db.model('User', {
	userName: String,
	password: String
});

module.export = User;