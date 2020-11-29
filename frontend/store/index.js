import endpoints from '~/utils/endpoints'

export const state = () => ({
  error: null,
  imageServer: 'https://192.168.10.122:3000',
  caffs: []
})

export const actions = {
  getCaffs ({ commit, state }, { text = null, force = false } = {}) {
    if (!state.caffs.length || force) {
      return this.$axios.get(endpoints.search, { params: { searchBy: 'tag', q: text } })
        .then(({ data }) => {
          commit('setCaffs', data.caffs)

          return data.caffs
        })
    }

    return state.caffs
  },

  deteleCaff ({ commit }, caffId) {
    return this.$axios.get(endpoints.deleteCaff(caffId))
      .then((data) => {
        commit('removeCaff', caffId)

        return data
      })
  }
}

export const mutations = {
  setError (state, data) {
    state.error = data
  },

  clearError (state) {
    state.error = null
  },

  setCaffs (state, data) {
    state.caffs = data
  },

  addCaff (state, data) {
    state.caffs.push(data)
  },

  removeCaff (state, id) {
    state.caffs.splice(state.caffs.findIndex(caff => caff.id === id), 1)
  }
}

export const getters = {
  hasError (state) {
    return Boolean(state.error)
  }
}
