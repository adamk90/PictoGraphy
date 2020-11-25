module.exports = function (objectRepository) {
	return async function(req, res, next) {
		let commentid;
		if (res.params && res.params.commentid) {
			commentid = res.params.commentid;
		}
		if (commentid) {
			try {
				objectRepository.Comment.deleteOne({_id:commentid});
				res.status(200).end();
			} catch (err) {
				console.log(err);
			}	
		}
		return res.status(400).end();
	};
};