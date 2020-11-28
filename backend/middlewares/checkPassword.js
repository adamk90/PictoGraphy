const crypto = require('crypto');

module.exports = function () {
    return async function(req, res, next) {
        if (res.locals.user && req.body && req.body.password) {
            let salt = res.locals.user.password.slice(-16);
            let hash = crypto.createHmac('sha512', salt);
            hash.update(req.body.password);
            let hashedPassword = hash.digest('hex');
            if ((hashedPassword + salt) === res.locals.user.password) {
                return next();
            } else {
                console.log("Trying wrong password for user: ", res.locals.user.userName);
            }
        }
        return res.status(400).end();
    };
};