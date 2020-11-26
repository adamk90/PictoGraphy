const express = require('express');
const app = express();
const dotenv = require("dotenv");
const fileUpload = require("express-fileupload");

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

const server = app.listen(3000, function() {
	console.log("Server listening at :3000");
});