const express = require('express');
const app = express();
const session = require('express-session');
const bodyParser = require('body-parser');


app.set('view engine', 'ejs');
app.use('*.ico', express.static('/static/images/favicon.ico'))
app.use(express.static('static'));

const server = app.listen(3000, function() {
	console.log("Server listening at :3000");
});