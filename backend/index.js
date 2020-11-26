const express = require('express');
const app = express();
const dotenv = require("dotenv");
const fileUpload = require("express-fileupload");
const fs = require('fs');
const https = require('https');

dotenv.config();

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