const fs = require('fs');
const crypto = require('crypto');

function encryptCaff(caff) {
    const data = Buffer.from(caff).toString('binary');
    const ENC_KEY = Buffer.from(process.env.AES_KEY, "hex");
    const IV = crypto.randomBytes(16);
    let cipher = crypto.createCipheriv('aes-256-cbc', ENC_KEY, IV);
    let encrypted = cipher.update(data, 'utf8', 'binary') + cipher.final('binary');
    let encoded = Buffer.from(IV, 'binary').toString("hex") + Buffer.from(encrypted, 'binary').toString('hex');
    return encoded;
}

module.exports = function (objectRepository) {
    return async function(req, res, next) {
        if (res.locals.caff) {
            let caff = new objectRepository.Caff({
                'creator': res.locals.caff.creator,
                'creationDate': res.locals.caff.creationDate,
                'tags': res.locals.caff.tags,
                '_owner': res.locals.user._id
            });
            try {
                let savedCaff = await caff.save();
                res.locals.caff.id = savedCaff._id;
                console.log("Caff saved: ", res.locals.caff);
                const previewFileName = "./static/images/previews/" + res.locals.caff.id + "_preview.bmp";
                const caffFileName = "./caffs/" + res.locals.caff.id + "_encrypted.caff";
                fs.writeFile(previewFileName, res.locals.caff.previewBmp, "binary", (err) => {
                    if (err) {
                        console.log(err)
                        console.log("Error while saving preview to: ", previewFileName);
                        delete res.locals.caff.previewBmp;
                        return res.status(400).end();
                    } else {
                        console.log("Successfully saved preview to: ", previewFileName);
                    }
                });
                fs.writeFile(caffFileName, encryptCaff(res.locals.caff.caffBytes), "binary", (err) => {
                    if (err) {
                        console.log("Error while saving caff to: ", caffFileName);
                        delete res.locals.caff.caffBytes;
                        return res.status(400).end();
                    } else {
                        console.log("Successfully saved caff to: ", caffFileName);
                        try {
                            savedCaff.preview = previewFileName.substring("./static".length);
                            savedCaff.content = caffFileName;
                            await savedCaff.save();
                            let log = new objectRepository.Log({
                                'text': "Caff " + savedCaff._id + " uploaded",
                                '_timeStamp': new Date(),
                                '_user': res.locals.user._id
                            });
                            await log.save();
                            res.data = {
                                'caff': {
                                    'id': savedCaff._id,
                                    'previewBmp': savedCaff.preview,
                                    'uploader': res.locals.user.userName,
                                    'creator': savedCaff.creator,
                                    'creationDate': savedCaff.creationDate,
                                    'tags': savedCaff.tags,
                                }
                            };
                            return next();
                        } catch (err) {
                            console.log(err);
                            return res.status(400).end();
                        }
                    }
                });
            } catch (err) {
                console.log(err);
                return res.status(400).end();
            }
        } else {
            console.log("Error: no caff on request!");
            return res.status(400).end();
        }
    };
};