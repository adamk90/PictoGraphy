<template>
  <b-modal
    id="create-modal"
    title="Animáció feltöltése"
    centered
    size="lg"
    static
    hide-footer
    body-class="p-5"
    @hidden="image = null"
  >
    <b-form-file
      v-if="!image"
      v-model="image"
      style="height: 200px"
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
    </b-form-file>

    <div v-else class="text-center">
      <div>{{ image.name }}</div>

      <b-button variant="primary" class="mt-3">
        Feltöltés
      </b-button>
    </div>
  </b-modal>
</template>

<script>
export default {
  data () {
    return {
      image: null
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
