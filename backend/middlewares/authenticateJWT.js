const jwt = require("jsonwebtoken");

module.exports = function () {
	return async function(req, res, next) {
		if (req.headers && req.headers.authorization) {
			const token = req.headers.authorization.split(' ')[1];
			if (token) {
				try {
					const decoded = jwt.verify(token, process.env.JWT_SECRET);
					if (decoded.username && decoded.isAdmin !== undefined) {
						res.locals.authUser = decoded;
						console.log("JWT Authenticated: ", decoded);
						return next();
					}
				} catch (err) {
					console.log(err);
				}
			}
		}
		return res.status(400).end();
	};
};