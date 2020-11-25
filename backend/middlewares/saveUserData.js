const crypto = require('crypto');

function getRandomByteString(length) {
	return crypto.randomBytes(length).toString('hex').slice(0, length);
}

module.exports = function (objectRepository) {
	return async function(req, res, next) {
		if (req.body && req.body.username && req.body.email && req.body.password) {
			let salt = getRandomByteString(16);
			let hash = crypto.createHmac('sha512', salt);
			hash.update(req.body.password);
			let hashedPw = hash.digest('hex');
			let user = new objectRepository.User({
				userName: req.body.username,
				email: req.body.email,
				password: hashedPw + salt,
				admin: false
			});
			try {
				let savedUser = await user.save();
				console.log("User registered: ", savedUser);
				res.status(200).end();

			} catch (err) {
				console.log(err);
			}
		}
		res.status(400).end();
	};
};