var sanitize = require("mongo-sanitize");
module.exports = function () {
    return async function(req, res, next) {
        if (req.params) {
            req.params = sanitize(req.params);
        }
        if (req.query) {
            req.query = sanitize(req.query);
        }
        if (req.body) {
            req.body = sanitize(req.body);
        }
        return next();
    };
};