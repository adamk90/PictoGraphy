module.exports = function (type) {
	return async function(req, res, next) {
		if (res.locals.authUser && !res.locals.authUser.isAdmin) {
			if (type === 'user' && res.locals.user === null) {
				res.status(400).end();
			}
		}
		return next();
	};
};