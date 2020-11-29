const mongoose = require('mongoose');

mongoose.connect('mongodb://localhost/caffs', { useNewUrlParser: true, useUnifiedTopology: true, useCreateIndex: true });

module.exports = mongoose;