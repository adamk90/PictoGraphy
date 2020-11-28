const express = require('express');
const app = express();
const dotenv = require("dotenv");
const fileUpload = require("express-fileupload");
const fs = require('fs');
const https = require('https');
const crypto = require('crypto');
const User = require('./models/user');

dotenv.config();

(async () => {
	try {
		let admin = await User.findOne({'userName': 'admin'}).exec();
		if (admin === null) {
			let salt = crypto.randomBytes(16).toString('hex').slice(0, 16);
			let hash = crypto.createHmac('sha512', salt);
			hash.update(process.env.ADMIN_PASSWORD);
			let hashedPw = hash.digest('hex');
			admin = new User({
				userName: 'admin',
				email: 'admin@pictography.hu',
				password: hashedPw + salt,
				admin: true
			});
			admin = await admin.save();
		}
		console.log("Admin user: ", admin);
	} catch (err) {
		console.log("Admin user could not found or created: ", err);
	}
})();

app.use(express.json());
app.use(express.urlencoded({'extended': true}));
app.use(fileUpload());
app.use(express.static('static'));

require('./routes/routes')(app);

app.use((err, req, res, next) => {
    res.end('Problem...');
    console.log(err);
});

const httpsOptions = {
	key: fs.readFileSync('./ssl/key.pem'),
	cert: fs.readFileSync('./ssl/cert.pem')
};

const server = https.createServer(httpsOptions, app).listen(3000, function() {
	console.log("Server listening at :3000");
});