import endpoints from './utils/endpoints.js'
import fs from 'fs'

export default {
  // Global page headers (https://go.nuxtjs.dev/config-head)
  head: {
    title: 'pictography',
    meta: [
      { charset: 'utf-8' },
      { name: 'viewport', content: 'width=device-width, initial-scale=1' },
      { hid: 'description', name: 'description', content: '' },
    ],
    link: [{ rel: 'icon', type: 'image/x-icon', href: '/favicon.ico' }],
  },

  // Global CSS (https://go.nuxtjs.dev/config-css)
  css: [
    '~/assets/scss/app.scss'
  ],

  // Plugins to run before rendering page (https://go.nuxtjs.dev/config-plugins)
  plugins: [
    '~/plugins/injector.js',
    '~/plugins/validator.js',
    '~/plugins/wrapper.js',
  ],

  // Auto import components (https://go.nuxtjs.dev/config-components)
  components: true,

  // Modules for dev and build (recommended) (https://go.nuxtjs.dev/config-modules)
  buildModules: [
    // https://go.nuxtjs.dev/eslint
    '@nuxtjs/eslint-module',
  ],

  // Modules (https://go.nuxtjs.dev/config-modules)
  modules: [
    // https://go.nuxtjs.dev/bootstrap
    'bootstrap-vue/nuxt',
    // https://go.nuxtjs.dev/axios
    '@nuxtjs/axios',
    // https://auth.nuxtjs.org/
    '@nuxtjs/auth',
    // Doc: https://github.com/nuxt-community/style-resources-module
    '@nuxtjs/style-resources',
  ],

  bootstrapVue: {
    bootstrapCSS: false,
    bootstrapVueCSS: false
  },

  router: {
    middleware: 'clearError'
  },

  styleResources: {
    scss: [
      '~/assets/scss/_variables.scss',
      'bootstrap/scss/_functions.scss',
      'bootstrap/scss/_variables.scss',
    ]
  },

  server: {
    host: '0.0.0.0',
    port: '3001',
    https: {
      key: fs.readFileSync('./ssl/key.pem'),
      cert: fs.readFileSync('./ssl/cert.pem')
    }
  },

  auth: {
    strategies: {
      local: {
        endpoints: {
          login: { url: endpoints.login, method: 'post', propertyName: 'token' },
          logout: { url: endpoints.logout, method: 'post' },
          user: { url: endpoints.user, method: 'get', propertyName: 'user' }
        },
        tokenRequired: true,
      },
    },
    redirect: {
      login: '/auth/login',
      logout: '/auth/login',
      home: '/'
    },
    resetOnError: true
  },

  // Axios module configuration (https://go.nuxtjs.dev/config-axios)
  axios: {
    proxyHeaders: true,
    credentials: false,
    proxy: true
  },

  proxy: {
    '/api/': {
      target: 'https://127.0.0.1:3000',
      xfwd: true,
      changeOrigin: true,
      secure: false,
    }
  },

  // Build Configuration (https://go.nuxtjs.dev/config-build)
  build: {
    extend(config, { isDev, isClient }) {
      if (isDev && isClient) {
        config.module.rules.push({
          enforce: 'pre',
          test: /\.(js|vue)$/,
          loader: 'eslint-loader',
          exclude: /(node_modules)/,
          options: {
            fix: true
          }
        })
      }
    }
  }
}
