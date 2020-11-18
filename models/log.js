const Schema = require('mongoose').Schema;
const db = require('../config/db');

const Log = db.model('Log', {
	text: String
});

module.export = Log;