module.exports = function (objectRepository, shouldFind) {
    return async function(req, res, next) {
        if (res.locals.user && res.locals.caff) {
            try {
                if (res.locals.caff._owner._id && res.locals.user._id.toString() === res.locals.caff._owner._id.toString()) {
                    return next();
                }
                let transaction = await objectRepository.Transaction.findOne({
                    '_product': res.locals.caff._id,
                    '_customer': res.locals.user._id
                }).exec();
                if (transaction === null && shouldFind) {
                    console.log('User (' + res.locals.user.userName + ') have not bought this caff yet: ', res.locals.caff._id);
                    return res.status(400).end();
                } else if (transaction !== null && !shouldFind) {
                    console.log('User (' + res.locals.user.userName + ') have already bought this caff: ', res.locals.caff._id);
                    return res.status(200).json({'transactionId': transaction._id});
                } else {
                    return next();
                }
            } catch (err) {
                console.log(err);
            }
        }
        return res.status(400).end();
    };
};