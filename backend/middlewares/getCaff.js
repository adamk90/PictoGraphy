const validObjectId = require('mongoose').Types.ObjectId.isValid;

module.exports = function (objectRepository, needResult) {
	return async function(req, res, next) {
		if (req.params && req.params.itemid && res.locals.user) {
			if (!validObjectId(req.params.itemid)) {
				console.log('Invalid caff id: ', req.params.itemid);
				return res.status(400).end();
			}
			try {
				let caff = await objectRepository.Caff.findOne({'_id': req.params.itemid})
													  .populate('_owner')
													  .populate({
													  				path: '_comments',
													  				populate: {path: '_owner'}
													  			})
													  .exec();
				if (caff !== null) {
					console.log('Caff found: ', caff);
					res.locals.caff = caff;
					if (needResult) {
						res.data = {
							'caff': {
								'id': caff._id,
								'previewBmp': caff.preview,
								'uploader': caff._owner.userName,
								'creator': caff.creator,
								'creationDate': caff.creationDate,
								'tags': caff.tags,
								'comments': caff._comments.map((comment) => {return {'id': comment._id, 'comment': comment.text, 'user': comment.username, 'date': comment.date}})
							}
						};
					}
				} else {
					console.log('Error: no such caff: ', res.locals.user._id);
					return res.status(400).end();
				}
			} catch (err) {
				console.log(err);
				return res.status(400).end();
			}
		}
		return next();
	};
};