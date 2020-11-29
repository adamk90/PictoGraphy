<template>
  <div class="image-container">
    <b-img-lazy class="image-aspect" :src="$store.state.imageServer + src" fluid />

    <div class="overlay">
      <slot />
    </div>
  </div>
</template>

<script>
export default {
  props: {
    src: {
      type: String,
      required: true
    }
  }
}
</script>

<style lang="scss" scoped>
.image-container {
  position: relative;
  overflow: hidden;
  border-radius: $border-radius;
  padding-top: 100%;
  height: 0;
  background: grey;

  .image-aspect {
    position: absolute;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    margin: auto;
    min-width: 100%;
    min-height: 100%;
    object-fit: cover;
    cursor: pointer;
  }

  .overlay {
    position: absolute;
    top: 0;
    display: flex;
    flex-direction: column;
    padding: 16px;
    height: 100%;
    width: 100%;
  }
}
</style>
