const User = require('../models/user');
const Comment = require('../models/comment');
const Log = require('../models/log');
const Transaction = require('../models/transaction');
const Caff = require('../models/caff');

const authenticateJWT = require('../middlewares/authenticateJWT');
const getUser = require('../middlewares/getUser');
const checkTransaction = require('../middlewares/checkTransaction');
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
const sanitizeMW = require('../middlewares/sanitizeMW');
const getUserData = require('../middlewares/getUserData');
 
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
    app.get('/api/caff/:itemid/download',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        getCaff(objectRepository, false),
        checkTransaction(objectRepository, true),
        getCaffBytes()
    );

    /*
     * Success: 200
     * Error: 400
     */
    app.get('/api/caff/:itemid/delete',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        deleteCaff(objectRepository)
    );

    /*
     * Request payload:
     * { 
     *   'comment': comment
     * }
     * Successful response:
     * {
     *  'comment': comment
     * }
     */
    app.post('/api/caff/:itemid/comment',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        getCaff(objectRepository, false),
        saveComment(objectRepository),
        sendResponse()
    );


    /*
     * Successful response:
     * {
     *  'transactionId': id
     * }
     */
    app.get('/api/caff/:itemid/buy',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        getCaff(objectRepository, false),
        checkTransaction(objectRepository, false),
        saveTransaction(objectRepository),
        sendResponse()
    );

    /*
     * Success: 200
     * Error: 400
     */
    app.delete('/api/comment/:itemid/:commentid',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        getCaff(objectRepository, false),
        deleteComment(objectRepository)
    );


    /*
     * Successful response:
     * { 'caff': {
     *      'id': id,
     *      'previewBmp': '...', <- string path to server static image
     *      'uploader': '...', < string USERID
     *      'creator': '...', <- string
     *      'creationDate': ..., <- Date 
     *      'tags': [
     *          'tag1', <- string
     *          'tag2',
     *          ...
     *      ],
     *      'comments': [
     *          {
     *              'id': id,
     *              'comment': comment,
     *              'user': user,
     *              'date': Date,
     *          },
     *          ...
     *      ]
     *   }
     * }
     */
    app.get('/api/caff/:itemid',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        getCaff(objectRepository, true),
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
    app.post('/api/register',
        sanitizeMW(),
        checkUserData(),
        saveUserData(objectRepository),
    );

    /*
     * Request payload:
     * { 
     *   'username': username,
     *   'password': password,
     * }
     * Successful response:
     * {
     *  'token': ..., <- hex string
     *  'isAdmin': ... <- bool
     * }
     */
    app.post('/api/login',
        sanitizeMW(),
        getUser(objectRepository),
        checkPassword(),
        generateJWT(),
        sendResponse()
    );


    /*
    * Request params:
    * -
    * Successful respone:
    * {
    *   user:  
    *           userName
    *           email
    *           isAdmin
    *           _id
    *}
    */
    app.get('/api/user',
        authenticateJWT(),
        getUserData(objectRepository),
        sendResponse()
    );


    /*
     * Request params:
     * no params -> all caffs
     * ?searchBy=tag&q=tagName -> all caffs containing tag 'tagName'
     * Successful response:
     * { 'caffs': [
     *      {
     *          'id': id,
     *          'previewBmp': '...', <- string path to server static image
     *          'uploader': '...', < string USERID
     *          'creator': '...', <- string
     *          'creationDate': ..., <- Date 
     *          'tags': [
     *              'tag1',
     *              'tag2',
     *              ...
     *          ],
     *      },
     *      ...
     *   ]
     * }
     */
    app.get('/api/search',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        searchCaffs(objectRepository),
        sendResponse()
    );


    /*
     * Successful response:
     * { 'caffs': [
     *      {
     *          'id': id,
     *          'previewBmp': '...', <- string path to server static image
     *          'uploader': '...', < string USERID
     *          'creator': '...', <- string
     *          'creationDate': ..., <- Date 
     *          'tags': [
     *              'tag1',
     *              'tag2',
     *              ...
     *          ]
     *      },
     *      ...
     *   ]
     * }
     */
    app.get('/api/my-caffs',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        getCaffs(objectRepository),
        sendResponse()
    );


    /*
     * Request payload: multiform data
     * Successful response:
     * { 'caff': {
     *      'id': id,
     *      'previewBmp': '...', <- string path to server static image
     *      'uploader': '...', < string USERID
     *      'creator': '...', <- string
     *      'creationDate': ..., <- Date 
     *      'tags': [
     *          'tag1',
     *          'tag2',
     *          ...
     *      ]
     *   }
     * }
     */
    app.post('/api/upload',
        authenticateJWT(),
        sanitizeMW(),
        getUser(objectRepository),
        parseCaff(),
        saveCaff(objectRepository),
        sendResponse()
    );

};