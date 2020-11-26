const fs = require('fs');
const crypto = require('crypto');
const caffParser = require('../build/Release/caffparser');

function decryptCaff(encoded) {
	let combined = Buffer.from(encoded, 'hex');
	const ENC_KEY = Buffer.from(process.env.AES_KEY, 'hex');
	const IV = Buffer.alloc(16);
	combined.copy(IV, 0, 0, 16);
	const encryptedData = combined.slice(16).toString('binary');
	let decipher = crypto.createDecipheriv('aes-256-cbc', ENC_KEY, IV);
	let decrypted = decipher.update(encryptedData, 'binary', 'utf8') + decipher.final('utf8');
	return decrypted;
}

module.exports = function () {
	return async function(req, res, next) {
		if (res.locals.caff) {
			let caffFileName = './caffs/' + res.locals.caff._id + "_encrypted.caff";
			fs.readFile(caffFileName, 'binary', (err, encryptedData) => {
				if (err) {
					console.log(err);
					res.status(400).end();
				}
				const caff = Buffer.from(decryptCaff(encryptedData), 'binary');
				res.data = {
					'caffBytes': caff
				};
				return next();
			})
		} else {
			res.status(400).end();
		}
	};
};