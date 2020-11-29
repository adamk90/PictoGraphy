export default {
  register: '/api/register',
  login: '/api/login',
  user: '/api/user',
  logout: '/api/logout',
  search: '/api/search',
  upload: '/api/upload',
  caff: '/api/caff/',
  comment: id => parse('/api/caff/@/comment', id),
  deleteComment: '/api/comment',
  download: id => parse('/api/caff/@/download', id),
  buy: id => parse('/api/caff/@/buy', id),
  myCaff: '/api/my-caffs',
  deleteCaff: id => parse('/api/caff/@/delete', id)
}

const parse = (endpoint, param) => {
  return endpoint.replace('@', param)
}
