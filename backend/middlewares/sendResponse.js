module.exports = function () {
	return async function(req, res, next) {
		if (res.data) {
			res.json(res.data);
		}
		return res.status(400).end();
	};
};