const fs = require('fs');

module.exports = function (objectRepository) {
	return async function(req, res, next) {
		let itemid;
		if (res.params && res.params.itemid) {
			itemid = res.params.itemid;
		}
		if (itemid) {
			try {
				let caff = await objectRepository.Caff.findOne({'_id': itemid}).exec();
				objectRepository.Comment.deleteMany({'_id:':{$in:caff._comments}});
				objectRepository.Caff.deleteOne({_id:itemid});
				fs.unlinkSync("./static" + caff.preview);
				fs.unlinkSync(caff.content);
				res.status(200).end();
			} catch (err) {
				console.log(err);
			}	
		}
		return res.status(400).end();
	};
};