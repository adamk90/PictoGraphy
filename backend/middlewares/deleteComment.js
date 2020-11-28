const validObjectId = require('mongoose').Types.ObjectId.isValid;

module.exports = function (objectRepository) {
	return async function(req, res, next) {
		let commentid;
		if (req.params && req.params.commentid) {
			commentid = req.params.commentid;
		}
		if (commentid && res.locals.user && res.locals.caff) {
			try {
				if (!validObjectId(commentid)) {
					console.log("Invalid comment id: ", commentid);
					return res.status(400).end();
				}
				let comment = null;
				if (res.locals.authUser.isAdmin) {
					comment = await objectRepository.Comment.findOneAndDelete({_id: commentid});
				} else {
					comment = await objectRepository.Comment.findOneAndDelete({_id: commentid, _owner: res.locals.user._id});
				}
				if (comment === null) {
					console.log("User (" + res.locals.user._id + ") tried to delete non-existing or not owned comment: ", commentid);
				} else {
					res.locals.caff._comments = res.locals.caff._comments.filter((id) => id != comment._id);
					await res.locals.caff.save();
				}
				return res.status(200).end();
			} catch (err) {
				console.log(err);
			}	
		} else {
			console.log('Error: no user, caff or commentid in request. Probably wrong usage of this middleware.');
		}
		return res.status(400).end();
	};
};