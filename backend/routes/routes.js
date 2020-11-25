const User = require('../models/user');
const Comment = require('../models/comment');
const Log = require('../models/log');
const Transaction = require('../models/transaction');
const Caff = require('../models/caff');
 
module.exports = function(app) {
	let objectRepository = {
		User: User,
		Comment: Comment,
		Log: Log,
		Transaction: Transaction,
		Caff: Caff
	};

	/*
	 * Successful response:
	 * {
	 *	'caffBytes': ...
	 * }
	 */
	app.get('/caff/:itemid/download',
		authenticateJWT(),
		getUser(objectRepository),
		getTransaction(objectRepository),
		checkPermissions('transaction'),
		getCaff(objectRepository)
	);

	/*
	 * Success: 200
	 * Error: 400
	 */
	app.get('/caff/:itemid/delete',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		deleteCaff(objectRepository)
	);

	/*
	 * Request payload:
	 * { 
	 *   'comment': comment
	 * }
	 * Successful response:
	 * {
	 *	'commentId': id
	 * }
	 */
	app.post('/caff/:itemid/comment',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		getCaff(objectRepository),
		saveComment(objectRepository)
	);


	/*
	 * Successful response:
	 * {
	 *	'transactionId': id
	 * }
	 */
	app.get('/caff/:itemid/buy',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		getCaff(objectRepository),
		saveTransaction(objectRepository)
	);

	/*
	 * Success: 200
	 * Error: 400
	 */
	app.get('/caff/:itemid/:commentid/delete',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		getCaff(objectRepository),
		saveComment(objectRepository)
	);


	/*
	 * Successful response:
	 * { 'caff': {
	 *		'previewBmp': '...', <- bytes
	 *		'uploader': '...', < string USERID
	 *      'capture': '...', <- string
	 *      'creator': '...', <- string
	 *      'creationDate': ..., <- Date 
	 *      'tags': [
	 *			'tag1',
	 *			'tag2',
	 *			...
	 *		],
	 *      'comments': [
	 *			{
	 *  			'comment': comment,
	 *				'user': user,
	 *				'date': Date,
	 *			},
	 *			...
	 *		]
	 *	 }
	 * }
	 */
	app.get('/caff/:itemid',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		getCaff(objectRepository)
	);

	/*
	 * Request payload:
	 * { 
	 *   'username': username,
	 *   'password': password,
	 *   'email': email
	 * }
	 * Success: 200
	 * Error: 400
	 */
	app.post('/register',
		checkUserData(),
		saveUserData(objectRepository),
	);

	/*
	 * Request payload:
	 * { 
	 *   'username': username,
	 *   'password': password,
	 *   'email': email
	 * }
	 * Successful response:
	 * {
	 *	'token': ...
	 * }
	 */
	app.post('/login',
		getUser(objectRepository),
		checkPassword(objectRepository),
		generateJWT(),
	);

	/*
	 * Request params:
	 * no params -> all caffs
	 * ?searchBy=tag&q=tagName -> all caffs containing tag 'tagName'
	 * ?searchBy=creator&q=creatorName -> all caffs with creator 'creatorName'
	 * Successful response:
	 * { 'caffs': [
	 *		{
	 *			'previewBmp': '...', <- bytes
	 *			'uploader': '...', < string USERID
	 *      	'capture': '...', <- string
	 *      	'creator': '...', <- string
	 *      	'creationDate': ..., <- Date 
	 *      	'tags': [
	 *				'tag1',
	 *				'tag2',
	 *				...
	 *			],
	 *      	'comments': [
	 *				{
	 *  				'comment': comment,
	 *					'user': user,
	 *					'date': Date,
	 *				},
	 *				...
	 *			]
	 *		},
	 *		...
	 *	 ]
	 * }
	 */
	app.get('/search',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		searchCaffs(objectRepository),
	);


	/*
	 * Request payload:
	 * { 
	 *   'fileBytes': ... 
	 * }
	 * Successful response:
	 * { 'caff': {
	 *		'previewBmp': '...', <- bytes
	 *		'uploader': '...', < string USERID
	 *      'capture': '...', <- string
	 *      'creator': '...', <- string
	 *      'creationDate': ..., <- Date 
	 *      'tags': [
	 *			'tag1',
	 *			'tag2',
	 *			...
	 *		]
	 *	 }
	 * }
	 */
	app.post('/upload',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		parseCaff(),
		saveCaff(objectRepository),
		sendCaffResponse()
	);

};