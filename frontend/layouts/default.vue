<template>
  <div class="main-container">
    <Navbar />
    <nuxt />

    <UploadModal />
  </div>
</template>

<script>
import Navbar from '~/components/ui/Navbar'
import UploadModal from '~/components/ui/UploadModal'

export default {
  components: {
    Navbar,
    UploadModal
  }
}
</script>

<style lang="scss" scoped>
.main-container {
  min-height: 100vh;
  background-color: #dcefff;
}
</style>
