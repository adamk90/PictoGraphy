const jwt = require("jsonwebtoken");

module.exports = function () {
    return async function(req, res, next) {
        if (res.locals.user) {
            const jwtPayload = {
                'username': res.locals.user.userName,
                'isAdmin': res.locals.user.admin
            };
            const token = jwt.sign(jwtPayload, process.env.JWT_SECRET, { expiresIn: '2400s' });
            res.data = {
                'token': token,
                'isAdmin': res.locals.user.admin
            };
            console.log("JWT Token generated: ", res.data);
            return next();
        }
        return res.status(400).end();
    };
};