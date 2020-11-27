function checkPassword(pw) {
	if (pw.length < 8) {
		return false;
	}
	containsLowerCase = false;
	containsUpperCase = false;
	containsNumber = false;
	containsSpecial = false;

	for (let i = 0; i < pw.length; ++i) {
		let c = pw[i];
		if (c >= '0' && c <= '9') {
			containsNumber = true;
		} else if (c >= 'a' && c <= 'z') {
			containsLowerCase = true;
		} else if (c >= 'A' && c <= 'Z') {
			containsUpperCase = true;
		} else {
			containsSpecial = true;
		}
	}
	return containsSpecial && containsNumber && containsUpperCase && containsLowerCase;
}

function checkUserName(user) {
	return user === null;
}

function checkEmail(email) {
	return email.indexOf('@') >= 1 && email.indexOf('@') < email.length - 1 && email.indexOf('@', email.indexOf('@') + 1) === -1;
}


module.exports = function (objectRepository) {
	return async function(req, res, next) {
		if (req.body && req.body.username && req.body.email && req.body.password) {
			if (checkEmail(req.body.email) && checkUserName(res.locals.user) && checkPassword(req.body.password)) {
				return next();
			}
		}
		res.status(400).end();
	};
};