module.exports = function (objectRepository) {
    return async function(req, res, next) {
        try {
            let caffs = [];
            if (req.query && req.query.searchBy === 'tag' && req.query.q) {
                caffs = await objectRepository.Caff.find({ "tags": { "$regex": req.query.q, "$options": "i"}}).populate('_owner').exec();
            } else {
                caffs = await objectRepository.Caff.find().populate('_owner').exec();
            }
            caffs = caffs.map((caff) => {
                return {
                    'id': caff._id,
                    'previewBmp': caff.preview,
                    'uploader': caff._owner.userName,
                    'creator': caff.creator,
                    'creationDate': caff.creationDate,
                    'tags': caff.tags,
                };
            });
            res.data = {
                'caffs': caffs
            };
            return next();
        } catch (err) {
            console.log(err);
        }
        return res.status(400).end();
    };
};