const User = require('../models/user');
const Comment = require('../models/comment');
const Log = require('../models/log');
const Transaction = require('../models/transaction');
const Caff = require('../models/caff');

const authenticateJWT = require('../middlewares/authenticateJWT');
const getUser = require('../middlewares/getUser');
const getTransaction = require('../middlewares/getTransaction');
const checkPermissions = require('../middlewares/checkPermissions');
const getCaff = require('../middlewares/getCaff');
const getCaffs = require('../middlewares/getCaffs');
const sendResponse = require('../middlewares/sendResponse');
const deleteCaff = require('../middlewares/deleteCaff');
const saveComment = require('../middlewares/saveComment');
const saveTransaction = require('../middlewares/saveTransaction');
const deleteComment = require('../middlewares/deleteComment');
const checkUserData = require('../middlewares/checkUserData');
const saveUserData = require('../middlewares/saveUserData');
const checkPassword = require('../middlewares/checkPassword');
const generateJWT = require('../middlewares/generateJWT');
const searchCaffs = require('../middlewares/searchCaffs');
const saveCaff = require('../middlewares/saveCaff');
const parseCaff = require('../middlewares/parseCaff');
const getCaffBytes = require('../middlewares/getCaffBytes');
 
module.exports = function(app) {
	let objectRepository = {
		User: User,
		Comment: Comment,
		Log: Log,
		Transaction: Transaction,
		Caff: Caff
	};

	/*
	 * Successful response: streaming the download as octet stream
	 */
	app.get('/caff/:itemid/download',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		//getTransaction(objectRepository),
		getCaff(objectRepository),
		getCaffBytes()
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
		checkPermissions('comment'),
		deleteComment(objectRepository)
	);


	/*
	 * Successful response:
	 * { 'caff': {
	 *		'id': id,
	 *		'previewBmp': '...', <- bytes
	 *		'uploader': '...', < string USERID
	 *      'creator': '...', <- string
	 *      'creationDate': ..., <- Date 
	 *      'tags': [
	 *			'tag1',
	 *			'tag2',
	 *			...
	 *		],
	 *      'comments': [
	 *			{
	 *				'id': id,
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
		getCaff(objectRepository),
		sendResponse()
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
		getUser(objectRepository),
		checkUserData(),
		saveUserData(objectRepository)
	);

	/*
	 * Request payload:
	 * { 
	 *   'username': username,
	 *   'password': password,
	 * }
	 * Successful response:
	 * {
	 *	'token': ...
	 * }
	 */
	app.post('/login',
		getUser(objectRepository),
		checkPassword(),
		generateJWT()
	);

	/*
	 * Request params:
	 * no params -> all caffs
	 * ?searchBy=tag&q=tagName -> all caffs containing tag 'tagName'
	 * Successful response:
	 * { 'caffs': [
	 *		{
	 *			'id': id,
	 *			'previewBmp': '...', <- bytes
	 *			'uploader': '...', < string USERID
	 *      	'creator': '...', <- string
	 *      	'creationDate': ..., <- Date 
	 *      	'tags': [
	 *				'tag1',
	 *				'tag2',
	 *				...
	 *			],
	 *      	'comments': [
	 *				{
	 *					'id': id,
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

	app.get('/my-caffs',
		authenticateJWT(),
		getUser(objectRepository),
		checkPermissions('user'),
		getCaffs(objectRepository),
	);


	/*
	 * Request payload: multiform data
	 * Successful response:
	 * { 'caff': {
	 *		'id': id,
	 *		'previewBmp': '...', <- bytes
	 *		'uploader': '...', < string USERID
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
	);

};