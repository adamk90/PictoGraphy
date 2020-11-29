module.exports = function (objectRepository) {
    return async function (req, res, next) {
        try {
            let user = await objectRepository.User.findOne({ 'userName': res.locals.authUser.username }).exec();
            res.data = {
                user: {
                    username: user.userName,
                    email: user.email,
                    isAdmin: res.locals.authUser.isAdmin,
                    id: user._id
                }
            }
            return next();
        } catch (err) {
            console.log(err);
        }
    }
}