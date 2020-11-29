const fs = require('fs');
const validObjectId = require('mongoose').Types.ObjectId.isValid;

module.exports = function (objectRepository) {
    return async function(req, res, next) {
        let itemid;
        if (req.params && req.params.itemid) {
            itemid = req.params.itemid;
        }
        if (itemid && res.locals.user) {
            try {
                if (!validObjectId(itemid)) {
                    console.log("Invalid caff id: ", itemid);
                    return res.status(400).end();
                }
                let caff = null;
                if (res.locals.authUser.isAdmin) {
                    caff = await objectRepository.Caff.findOne({'_id': itemid}).exec();
                } else {
                    caff = await objectRepository.Caff.findOne({'_id': itemid, '_owner': res.locals.user._id}).exec();
                }
                if (caff !== null) {
                    await objectRepository.Comment.deleteMany({'_id:':{$in:caff._comments}});
                    await objectRepository.Caff.deleteOne({'_id':itemid});
                    fs.unlinkSync("./static" + caff.preview);
                    fs.unlinkSync(caff.content);
                    let log = new objectRepository.Log({
                        'text': "Caff " + itemid + " deleted",
                        '_timeStamp': new Date(),
                        '_user': res.locals.user._id
                    });
                    await log.save();
                    return res.status(200).end();
                } else {
                    console.log("User (" + res.locals.user._id + ") tried to delete non-existing or not owned caff: ", itemid);
                    let log = new objectRepository.Log({
                        'text': "There was a try to delete a non-existing or not owned caff by",
                        '_timeStamp': new Date(),
                        '_user': res.locals.user._id
                    });
                    await log.save();
                }
            } catch (err) {
                console.log(err);
            }   
        }
        return res.status(400).end();
    };
};