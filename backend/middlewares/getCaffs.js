module.exports = function (objectRepository) {
	return async function(req, res, next) {
		if (res.locals.user) {
			try {
				let caffs = await objectRepository.Caff.find({'_owner': res.locals.user._id}).exec();
				caffs = caffs.map((caff) => {
					return {
						'id': caff._id,
						'previewBmp': caff.preview,
						'uploader': res.locals.user.userName,
						'creator': caff.creator,
						'creationDate': caff.creationDate,
						'tags': caff.tags,
					};
				});
				res.data = {
					'caffs': caffs
				};
				return next();
			} catch (err) {
				console.log(err);
			}
		}
		return res.status(400).end();
	};
};