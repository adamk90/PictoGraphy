module.exports = function () {
	return async function(req, res, next) {
		return next();
	};
};