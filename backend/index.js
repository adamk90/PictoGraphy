const express = require('express');
const app = express();
const bodyParser = require('body-parser');

const server = app.listen(3000, function() {
	console.log("Server listening at :3000");
});