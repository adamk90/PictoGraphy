module.exports = function (type) {
	return async function(req, res, next) {
		return next();
	};
};