<template>
  <div>
    <div v-if="!images || !images.length" class="text-center">
      <h1 class="my-3 shake">
        😭 Nincs megjeleníthető CAFF 😭
      </h1>
      <b-img src="/sad-face.jpg" class="w-50" />
    </div>
    <Grid v-else>
      <b-col v-for="image in images" :key="image.id" cols="3" @click="openPreview(image)">
        <Photo :src="image.previewBmp">
          <div class="ml-auto mt-auto download-button">
            <b-icon-eye-fill class="h3 mb-0 m-2" />
          </div>
        </Photo>
      </b-col>
    </Grid>

    <PreviewModal :image="previewImage" @caff-removed="$emit('caff-removed', $event)" />
  </div>
</template>

<script>
import endpoints from '~/utils/endpoints'

export default {
  props: {
    images: {
      type: [Array, Object],
      required: true
    }
  },

  data () {
    return {
      previewImage: null
    }
  },

  methods: {
    openPreview (image) {
      this.$axios.get(endpoints.caff + image.id).then(({ data }) => {
        this.previewImage = data.caff
        this.$bvModal.show('preview-modal')
      })
    }
  }
}
</script>

<style lang="scss" scoped>
.download-button {
  display: flex;
  background: $primary;
  color: white;
  width: 35px;
  height: 35px;
  cursor: pointer;
  border-radius: 50%;
  align-items: center;
  justify-content: center;

  &:hover {
    background: darken($primary, 5);
  }
}

.shake {
  transform-origin: center;
  animation-name: shake;
  animation-duration: 0.8s;
  animation-iteration-count: infinite;
}

@keyframes shake {
  20% {
    transform: rotate(1deg);
  }

  40% {
    transform: rotate(-1deg);
  }

  60% {
    transform: rotate(0.5deg);
  }

  80% {
    transform: rotate(-0.5deg);
  }

  to {
    transform: rotate(0deg);
  }
}
</style>
