module.exports = function (objectRepository) {
    return async function(req, res, next) {
        if (res.locals.user && res.locals.caff) {
            let transaction = new objectRepository.Transaction({
                '_product': res.locals.caff._id,
                '_customer': res.locals.user._id,
            });
            try {
                let savedTransaction = await transaction.save();
                res.data = {
                    'transactionId': savedTransaction._id
                };
                let log = new objectRepository.Log({
                    'text': "User just buyed caff: " + res.locals.caff._id,
                    '_timeStamp': new Date(),
                    '_user': res.locals.user._id
                });
                await log.save();
                return next();
            } catch (err) {
                console.log(err);
            }
        }
        return res.status(400).end();
    };
};