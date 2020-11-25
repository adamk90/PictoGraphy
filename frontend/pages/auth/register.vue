<template>
  <div class="form-container m-auto">
    <b-form class="border p-4">
      <h1 class="text-center mb-4">
        Pictography
      </h1>
      <p class="text-center font-weight-bold text-secondary">
        Ha regisztrálsz, több ezer CAFF animáció közül válogathatsz.
      </p>
      <b-form-group>
        <b-form-input v-model="user.email" placeholder="E-mail" />
      </b-form-group>
      <b-form-group>
        <b-form-input v-model="user.username" placeholder="Felhasználónév" />
      </b-form-group>
      <b-form-group>
        <b-form-input v-model="user.password" type="password" placeholder="Jelszó" />
      </b-form-group>
      <b-button variant="primary">
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
  }
}
</script>

<style lang="scss" scoped>
.terms {
  font-size: 14px;
}
</style>
