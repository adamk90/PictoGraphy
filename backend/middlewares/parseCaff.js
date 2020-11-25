const caffParser = require('../build/Release/caffparser');

/*
 * Gets uploaded file and tries to parse it.
 * If successful then it puts:
 *   preview.bmp byte content into res.locals.caff.preview
 *   metadata content into res.locals.caff.metadata
 * If parsing failed, then it puts error message into res.locals.error
 */

module.exports = function () {
	return async function(req, res, next) {
		return next();
	};
};