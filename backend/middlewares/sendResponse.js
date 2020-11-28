module.exports = function () {
	return async function(req, res, next) {
		if (res.data) {
			return res.json(res.data);
		}
		return res.status(400).end();
	};
};