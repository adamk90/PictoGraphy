module.exports = function (objectRepository) {
	return async function(req, res, next) {
		let username;
		if (req.body && req.body.username) {
			username = req.body.userName
		} else if (res.locals.authUser && res.locals.authUser.username) {
			username = res.locals.authUser.username;
		}
		if (username) {
			try {
				let user = await objectRepository.User.findOne({'userName': username}).exec();
				res.locals.user = user;
				console.log("User found: ", user);
				return next();
			} catch (err) {
				console.log(err);
			}	
		}
		return res.status(400).end();
	};
};