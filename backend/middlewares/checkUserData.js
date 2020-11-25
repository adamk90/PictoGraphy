module.exports = function (objectRepository) {
	return async function(req, res, next) {
		return next();
	};
};