<template>
  <div class="form-container m-auto">
    <b-form class="border p-4" @submit.stop.prevent="login">
      <h1 class="text-center mb-4">
        Pictography
      </h1>

      <picto-input :field="$v.user.username" label="Felhasználónév" />

      <picto-input :field="$v.user.password" type="password" label="Jelszó" />

      <picto-form-error />

      <b-button variant="primary" type="submit" :disabled="$v.$invalid || $store.getters.hasError">
        Bejelentkezés
      </b-button>
    </b-form>

    <div class="border py-3 px-4 mt-3 text-center">
      <span>Nincs fiókod?</span>
      <nuxt-link to="/auth/register">
        Regisztráció
      </nuxt-link>
    </div>

    <div class="mt-3 text-center">
      <nuxt-link to="/auth/forget-password">
        Elfelejtetted a jelszavadat?
      </nuxt-link>
    </div>
  </div>
</template>

<script>
import { required } from 'vuelidate/lib/validators'

export default {
  layout: 'auth',

  data () {
    return {
      user: {
        username: '',
        password: ''
      }
    }
  },

  validations: {
    user: {
      username: { required },
      password: { required }
    }
  },

  methods: {
    login () {
      if (!this.$v.$invalid) {
        this.$auth.loginWith('local', { data: this.user }).catch(() => {
          this.$store.commit('setError', 'Hibás felhasználónév vagy jelszó.')
        })
      }
    }
  }
}
</script>

<style lang="scss" scoped>

</style>
