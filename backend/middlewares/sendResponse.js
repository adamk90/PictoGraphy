module.exports = function () {
	return async function(req, res, next) {
		if (res.data) {
			res.json(res.data);
		}
		res.status(400).end();
	};
};