module.exports = function (objectRepository) {
	return async function(req, res, next) {
		let username;
		if (res.locals.authUser && res.locals.authUser.username) {
			username = res.locals.authUser.username;
		} else if (req.body && req.body.username) {
			username = req.body.username;
		}
		if (username) {
			try {
				let user = await objectRepository.User.findOne({'userName': username}).exec();
				if (user === null) {
					console.log("Authentication error, no such user found: ", username);
					return res.status(400).end();
				} else {
					console.log('User found: ', user);
				}
				res.locals.user = user;
				return next();
			} catch (err) {
				console.log(err);
			}	
		}
		return res.status(400).end();
	};
};