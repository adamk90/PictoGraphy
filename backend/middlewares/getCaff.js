module.exports = function (objectRepository) {
	return async function(req, res, next) {
		if (req.params && req.params.itemid) {
			try {
				let caff = await objectRepository.Caff.findOne({_id: req.params.itemid}).exec();
				res.locals.caff = caff;
			} catch (err) {
				console.log(err);
				res.status(400).end();
			}
		}
		return next();
	};
};