const caffParser = require('../build/Release/caffparser');
const fs = require('fs');

/*
 * Gets uploaded file and tries to parse it.
 * If successful then it puts:
 *   preview.bmp byte content into res.locals.caff.preview
 *   metadata content into res.locals.caff.metadata
 * If parsing failed, then it puts error message into res.locals.error
 */

module.exports = function () {
	return async function(req, res, next) {
		let caffBytes;
		for (file in req.files) {
			if (req.files.hasOwnProperty(file)) {
				caffBytes = req.files[file];
				break;
			}
		}
		if (caffBytes) {
			let obj = caffParser.parse(caffBytes.data);
			if (obj.Error) {
				console.log("Error while parsing Caff: ", obj.Error);
				return res.status(400).end();
			} else {
				let previewBytes = Uint8Array.from(Object.values(obj.Preview));
				let tags = [...new Set(Object.values(obj.Tags))];
				let creationDate = new Date(obj.Year, obj.Month - 1, obj.Day, obj.Hour, obj.Minute, 0, 0);
				let creator = obj.Creator;
				res.locals.caff = {
					'caffBytes': caffBytes.data,
					'previewBmp': previewBytes,
					'uploader': res.locals.user.username,
					'creator': creator,
					'creationDate': creationDate,
					'tags': tags
				};
			}
		}
		return next();
	};
};