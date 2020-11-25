const mongoose = require('mongoose');

mongoose.connect('mongodb://localhost/caffs', { useNewUrlParser: true });

module.exports = mongoose;