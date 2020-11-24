<template>
  <div class="form-container m-auto">
    <b-form class="border p-4" @submit.stop.prevent="login">
      <h1 class="text-center mb-4">
        Pictography
      </h1>
      <b-form-group>
        <b-form-input v-model="user.email" placeholder="Felhasználónév" />
      </b-form-group>
      <b-form-group>
        <b-form-input v-model="user.password" type="password" placeholder="Jelszó" />
      </b-form-group>
      <b-button variant="primary" type="submit">
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
export default {
  layout: 'auth',

  data () {
    return {
      user: {
        email: '',
        password: ''
      }
    }
  },

  methods: {
    login () {
      this.$auth.loginWith('local', { data: this.user })
    }
  }
}
</script>

<style lang="scss" scoped>

</style>
