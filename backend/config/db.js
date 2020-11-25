const mongoose = require('mongoose');

mongoose.connect('mongodb://localhost/caffs', { useNewUrlParser: true, useUnifiedTopology: true });

module.exports = mongoose;