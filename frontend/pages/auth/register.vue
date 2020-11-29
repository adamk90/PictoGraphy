<template>
  <div class="form-container m-auto">
    <b-form class="border p-4" @submit.stop.prevent="register">
      <h1 class="text-center mb-4">
        Pictography
      </h1>
      <p class="text-center font-weight-bold text-secondary">
        Ha regisztrálsz, több ezer CAFF animáció közül válogathatsz.
      </p>

      <picto-input :field="$v.user.email" label="E-mail" />

      <picto-input :field="$v.user.username" label="Felhasználónév" />

      <picto-input :field="$v.user.password" type="password" label="Jelszó" />

      <picto-form-error />

      <b-button variant="primary" type="submit" :disabled="$v.$invalid || $store.getters.hasError">
        Regisztráció
      </b-button>

      <p class="text-center text-secondary terms mt-3 mb-0">
        A regisztrációval vállalod a <b>Felhasználási feltételeinket.</b>
      </p>
    </b-form>

    <div class="border py-3 px-4 mt-3 text-center">
      <span>Van fiókod?</span>
      <nuxt-link to="/auth/login">
        Bejelentkezés
      </nuxt-link>
    </div>
  </div>
</template>

<script>
import { required, email, minLength } from 'vuelidate/lib/validators'
import endpoints from '~/utils/endpoints'

export default {
  layout: 'auth',

  data () {
    return {
      user: {
        email: '',
        username: '',
        password: ''
      }
    }
  },

  validations: {
    user: {
      email: { required, email },
      username: { required },
      password: {
        required,
        minLength: minLength(8),
        hasLowercase: value => value.toUpperCase() !== value,
        hasUppercase: value => value.toLowerCase() !== value,
        hasNumber: value => /[0-1]/g.test(value),
        hasSpecial: value => /[ `!@#$%^&*()_+\-=[\]{};':"\\|,.<>/?~]/.test(value)
      }
    }
  },

  methods: {
    register () {
      this.$axios.post(endpoints.register, this.user)
        .then(() => {
          this.$router.push('/auth/login')
        })
        .catch(() => {
          this.$store.commit('setError', 'Hiba történt a regisztráció során. Kérjük próbálja újra.')
        })
    }
  }
}
</script>

<style lang="scss" scoped>
.terms {
  font-size: 14px;
}
</style>
