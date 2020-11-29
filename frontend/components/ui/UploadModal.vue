<template>
  <b-modal
    id="create-modal"
    title="Animáció feltöltése"
    centered
    size="lg"
    static
    hide-footer
    body-class="p-5"
    @hidden="file = null; $store.commit('clearError')"
  >
    <b-form-file
      v-model="file"
      style="height: 200px"
      @input="uploadFile"
    >
      <template v-slot:placeholder>
        <div class="text-center">
          <b-icon-upload class="h1" />

          <div class="mt-3">
            Húzd ide a feltölteni kívánt animációt
          </div>

          <div class="btn mt-3 btn-primary">
            Fájl kiválasztása
          </div>
        </div>
      </template>

      <template v-slot:file-name>
        <div class="text-center">
          <div>{{ file.name }}</div>
          <b-spinner variant="primary" />
        </div>
      </template>
    </b-form-file>

    <picto-form-error />
  </b-modal>
</template>

<script>
import endpoints from '~/utils/endpoints'
export default {
  data () {
    return {
      file: null
    }
  },

  methods: {
    uploadFile () {
      if (this.file) {
        this.$store.commit('clearError')
        const formData = new FormData()

        formData.append('files', this.file)

        this.$axios.post(endpoints.upload, formData)
          .then(({ data }) => {
            this.$bvModal.hide('create-modal')
            this.$toast(this.$bvToast, 'Sikeres fájlfeltöltés', this.file.name)
            this.$store.commit('addCaff', data.caff)
          })
          .catch(() => {
            this.file = null
            this.$store.commit('setError', 'Hiba a feltöltés közben.')
          })
      }
    }
  }
}
</script>

<style lang="scss">
.custom-file-input ~ .custom-file-label::after {
  display: none;
}

.custom-file-label {
  overflow: visible;
  height: 200px;
  display: flex;
  flex-direction: column;
  justify-content: center;
}
</style>
