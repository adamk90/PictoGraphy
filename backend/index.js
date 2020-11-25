const express = require('express');
const app = express();
const bodyParser = require('body-parser');
const dotenv = require("dotenv");

dotenv.config();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true}));

require('./routes/routes')(app);

app.use((err, req, res, next) => {
    res.end('Problem...');
    console.log(err);
});

const server = app.listen(3000, function() {
	console.log("Server listening at :3000");
});