const fs = require('fs');
const crypto = require('crypto');
const caffParser = require('../build/Release/caffparser');
const streamBuffers = require('stream-buffers');

function decryptCaff(encoded) {
    let combined = Buffer.from(encoded, 'hex');
    const ENC_KEY = Buffer.from(process.env.AES_KEY, 'hex');
    const IV = Buffer.alloc(16);
    combined.copy(IV, 0, 0, 16);
    const encryptedData = combined.slice(16).toString('binary');
    let decipher = crypto.createDecipheriv('aes-256-cbc', ENC_KEY, IV);
    let decrypted = decipher.update(encryptedData, 'binary', 'utf8') + decipher.final('utf8');
    return decrypted;
}

module.exports = function () {
    return async function(req, res, next) {
        if (res.locals.caff) {
            let caffFileName = './caffs/' + res.locals.caff._id + "_encrypted.caff";
            fs.readFile(caffFileName, 'utf8', (err, encryptedData) => {
                if (err) {
                    console.log(err);
                    return res.status(400).end();
                }
                const caff = Buffer.from(decryptCaff(encryptedData), 'binary');
                const caffTempFileName = './tmp/' + Date.now() + res.locals.user._id + "_" + res.locals.caff.id + "_tmp.caff";
                fs.writeFile(caffTempFileName, caff, "utf8", (err) => {
                    if (err) {
                        console.log(err);
                        return res.status(400),end();
                    }
                    readStream = fs.createReadStream(caffTempFileName);
                    readStream.on('open', () => {
                        res.writeHead(200, {
                            "Content-Type": "application/octet-stream",
                            "Content-Disposition": "attachment; filename=" + res.locals.caff._id + ".caff"
                        });
                        readStream.pipe(res);
                    });
                    readStream.on('error', (err) => {
                        console.log(err);
                        fs.unlinkSync(caffTempFileName);
                        return res.status(400).end();
                    });
                    readStream.on('end', () => {
                        fs.unlinkSync(caffTempFileName);
                    });
                });
            });
        } else {
            return res.status(400).end();
        }
    };
};