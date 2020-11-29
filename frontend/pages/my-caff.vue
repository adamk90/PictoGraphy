<template>
  <b-container class="py-3">
    <h1 class="mb-4">
      <b-icon-file-earmark-image class="text-primary" />
      Saját CAFF-ok
    </h1>

    <CaffContainer :images="images" @caff-removed="images.splice(images.findIndex(image => image.id === $event), 1)" />
  </b-container>
</template>

<script>
import endpoints from '~/utils/endpoints'

export default {
  middleware: 'auth',

  asyncData ({ app }) {
    return app.$axios.get(endpoints.myCaff).then(({ data }) => ({ images: data.caffs }))
  }
}
</script>
