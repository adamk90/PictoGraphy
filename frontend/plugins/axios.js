import https from 'https'

export default function ({ $axios }) {
  // self signed certificate miatt
  $axios.defaults.httpsAgent = new https.Agent({ rejectUnauthorized: false })
}
