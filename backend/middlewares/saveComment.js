module.exports = function (objectRepository) {
    return async function(req, res, next) {
        if (res.locals.user && res.locals.caff && req.body.comment) {
            let comment = new objectRepository.Comment({
                'text': req.body.comment,
                '_owner': res.locals.user._id,
                'username': res.locals.user.userName,
                'date': new Date()
            });
            try {
                let savedComment = await comment.save();
                res.locals.caff._comments.push(savedComment._id);
                await res.locals.caff.save();
                res.data = {
                    'id': savedComment._id,
                    'comment': savedComment.text,
                    'user': savedComment.username,
                    'date': savedComment.date
                };
                let log = new objectRepository.Log({
                    'text': "Comment " + savedComment._id + " added",
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