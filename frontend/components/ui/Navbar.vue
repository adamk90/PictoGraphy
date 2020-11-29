<template>
  <b-navbar toggleable="lg" type="dark" variant="primary" sticky>
    <div class="container">
      <b-navbar-brand to="/">
        <h3 class="m-0">
          🖼PictoGraphy
        </h3>
      </b-navbar-brand>

      <b-navbar-toggle target="nav-collapse" />

      <b-collapse id="nav-collapse" is-nav>
        <b-navbar-nav class="ml-auto d-flex align-items-center">
          <b-nav-form v-if="$route.fullPath !== '/my-caff'" @submit.stop.prevent>
            <div class="border rounded-pill px-2">
              <b-button size="sm" class="search-button" type="submit">
                <b-icon-search />
              </b-button>
              <b-form-input
                v-model="search.keyword"
                size="sm"
                class="search-input"
                placeholder="Keresés"
                @input="startSearch"
              />
            </div>
          </b-nav-form>

          <b-nav-item v-b-modal.create-modal class="h5 m-0">
            <b-icon-plus-circle class="text-white" />
          </b-nav-item>

          <b-nav-item-dropdown right>
            <template #button-content>
              <b-avatar variant="info" size="36px" />
            </template>

            <div class="container mt-2 text-center">
              <div class="w-100">
                <b-avatar variant="info" size="lg" class="m-auto" />
              </div>

              <div class="mt-2 w-100">
                {{ $auth.user.username }}
              </div>
              <div class="w-100">
                {{ $auth.user.email }}
              </div>

              <b-button variant="outline-primary" class="mt-2 w-100">
                <nuxt-link to="/my-caff">
                  Animációim
                </nuxt-link>
              </b-button>

              <b-button variant="outline-dark" class="mt-4 w-100" @click="$auth.logout()">
                <nuxt-link to="/my-caff">
                  Kilépés
                </nuxt-link>
              </b-button>

              <div class="mt-2 text-center w-100 remove-button" @click="removeUser">
                Fiók törlése
              </div>
            </div>
          </b-nav-item-dropdown>
        </b-navbar-nav>
      </b-collapse>
    </div>
  </b-navbar>
</template>

<script>
export default {
  data () {
    return {
      search: {
        timeOut: null,
        clearTime: 500,
        keyword: ''
      }
    }
  },

  methods: {
    removeUser () {
      this.$bvModal.msgBoxConfirm('Biztosan törölni szeretnéd a profilod?', {
        title: 'Profil törlése',
        size: 'md',
        okVariant: 'danger',
        okTitle: 'Igen',
        cancelTitle: 'Nem',
        cancelVariant: 'outline-dark',
        centered: true
      })
        .then((value) => {
          if (value) {
            this.$auth.logout()
          }
        })
    },

    startSearch () {
      clearTimeout(this.search.timeOut)

      this.search.timeOut = setTimeout(function () {
        this.$store.dispatch('getCaffs', { text: this.search.keyword, force: true })
      }.bind(this), this.search.clearTime)
    }
  }
}
</script>

<style lang="scss" scoped>
.search-button {
  background: none;
  border: none;

  &:active, &:focus {
    background-color: transparent!important;
    border: none!important;
    box-shadow: none!important;
  }
}

.search-input {
  background: none;
  color: white;
  border: none;

  &::placeholder {
    color: white;
  }
}
</style>

<style lang="scss">
.nav-link.dropdown-toggle {
  display: flex;
  align-items: center;
}
</style>
