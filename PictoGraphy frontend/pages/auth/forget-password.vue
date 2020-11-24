<template>
  <div class="form-container m-auto">
    <b-form class="border p-4">
      <h4 class="text-center mb-4">
        Nem tudsz bejelentkezni?
      </h4>
      <p class="text-center text-secondary">
        Add meg az email címed és jelszó emlékeztetőt küldünk neked.
      </p>
      <b-form-group>
        <b-form-input v-model="email" placeholder="E-mail" />
      </b-form-group>
      <b-button variant="primary">
        Jelszó emlékeztető küldése
      </b-button>

      <div class="container">
        <b-row align-v="center" class="mt-3">
          <b-col class="divider" />
          <b-col cols="auto" class="text-secondary">
            VAGY
          </b-col>
          <b-col class="divider" />
        </b-row>
      </div>

      <div class="text-center mt-3 ">
        <nuxt-link to="/auth/register" class="text-secondary">
          Új fiók létrehozása
        </nuxt-link>
      </div>
    </b-form>

    <div class="mt-3 text-center">
      <nuxt-link to="/auth/login">
        Vissza a bejelentkezéshez
      </nuxt-link>
    </div>
  </div>
</template>

<script>
export default {
  layout: 'auth',

  data () {
    return {
      email: ''
    }
  }
}
</script>

<style lang="scss" scoped>
.divider {
  height: 1px;
  background-color: grey;
}
</style>
