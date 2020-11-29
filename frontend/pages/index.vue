<template>
  <div class="container py-3">
    <CaffContainer :images="images" />
  </div>
</template>

<script>
export default {
  middleware: 'auth',

  asyncData ({ store }) {
    return store.dispatch('getCaffs')
  },

  computed: {
    images () {
      return this.$store.state.caffs
    }
  }
}
</script>
