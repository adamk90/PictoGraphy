<template>
  <div class="container py-3">
    <p-grid>
      <b-col v-for="image in images" :key="image.id" cols="3" @click="openPreview(image)">
        <p-image :src="image.src">
          <div class="ml-auto mt-auto download-button">
            <b-icon-download class="h3 mb-0 m-2" />
          </div>
        </p-image>
      </b-col>
    </p-grid>

    <b-modal id="preview-modal" size="lg" hide-footer body-class="p-4">
      <template v-slot:modal-header>
        <div class="d-flex align-items-center justify-content-between w-100">
          <h4>
            {{ previewImage.name }}
          </h4>
          <div>
            <a :href="previewImage.src" class="btn btn-warning" download="">
              Letöltés
            </a>
          </div>
        </div>
      </template>

      <div class="text-center">
        <b-img :src="previewImage.src" fluid class="preview-image" />

        <b-list-group class="mt-3 comment-container">
          <b-list-group-item v-for="comment in previewImage.comments" :key="comment.id" class="text-left">
            <div class="d-flex align-items-center">
              <div>
                {{ comment.text }}
              </div>
              <b-icon-trash-fill v-if="comment.userId === $auth.user.id" class="text-danger ml-auto icon-button" @click="removeComment(comment.id)" />
            </div>
          </b-list-group-item>
        </b-list-group>

        <b-form class="mt-3" @submit.prevent.stop="sendComment(previewImage)">
          <b-form-row class="d-flex align-items-center">
            <b-col>
              <b-form-input v-model="comment" type="text" placeholder="Saját megjegyzés..." />
            </b-col>
            <b-col cols="auto" class="icon-button">
              <b-icon-arrow-right-circle-fill class="h3 mb-0 m-2 text-primary" @click="sendComment(previewImage)" />
            </b-col>
          </b-form-row>
        </b-form>
      </div>
    </b-modal>
  </div>
</template>

<script>
import Image from '~/components/ui/Image'
import Grid from '~/components/ui/Grid'

export default {
  components: {
    'p-image': Image,
    'p-grid': Grid
  },

  middleware: 'auth',

  data () {
    return {
      previewImage: '',
      comment: '',
      images: [
        { id: 1, name: 'macskacica', src: 'http://placekitten.com/250/350', userId: 1, comments: [{ id: 1, text: 'cica', userId: 10 }, { id: 2, text: 'cica', userId: 10 }] },
        { id: 2, name: 'cica', src: 'http://placekitten.com/350/350', userId: 2, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 3, name: 'cica', src: 'http://placekitten.com/450/350', userId: 3, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 4, name: 'cica', src: 'http://placekitten.com/350/300', userId: 6, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 5, name: 'cica', src: 'http://placekitten.com/150/350', userId: 10, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 6, name: 'cica', src: 'http://placekitten.com/200/200', userId: 4, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 7, name: 'cica', src: 'http://placekitten.com/200/200', userId: 111, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 8, name: 'cica', src: 'http://placekitten.com/450/550', userId: 17, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 9, name: 'cica', src: 'http://placekitten.com/550/550', userId: 9, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 10, name: 'cica', src: 'http://placekitten.com/600/550', userId: 22, comments: [{ id: 1, text: 'cica', userId: 1 }, { id: 2, text: 'cica', userId: 1 }] },
        { id: 11, name: 'cica', src: 'http://placekitten.com/500/500', userId: 41, comments: [{ id: 1, text: 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce scelerisque id justo a ullamcorper. Curabitur lobortis tincidunt eros vel tempor. Donec sagittis tortor vel ex molestie sollicitudin. Nam tempus sodales quam, tempus congue ipsum finibus quis. Etiam rutrum leo quam, ut feugiat mauris ultricies at. Vestibulum eu ipsum dui. Suspendisse dictum efficitur nisi laoreet dapibus. Morbi pulvinar est ipsum, in fermentum quam blandit sit amet. Vestibulum accumsan mauris eu magna molestie commodo. Maecenas nec pulvinar mauris. Morbi efficitur iaculis felis eget aliquet. Nullam massa lacus, elementum sed iaculis non, ultrices eget tellus. Nam est mauris, facilisis id scelerisque sed, euismod eget lorem. Vivamus ultrices feugiat mi ac semper. Aliquam varius in nibh at efficitur. Nullam tempus felis id dolor porta euismod.', userId: 1 }, { id: 2, text: 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce scelerisque id justo a ullamcorper. Curabitur lobortis tincidunt eros vel tempor. Donec sagittis tortor vel ex molestie sollicitudin. Nam tempus sodales quam, tempus congue ipsum finibus quis. Etiam rutrum leo quam, ut feugiat mauris ultricies at. Vestibulum eu ipsum dui. Suspendisse dictum efficitur nisi laoreet dapibus. Morbi pulvinar est ipsum, in fermentum quam blandit sit amet. Vestibulum accumsan mauris eu magna molestie commodo. Maecenas nec pulvinar mauris. Morbi efficitur iaculis felis eget aliquet. Nullam massa lacus, elementum sed iaculis non, ultrices eget tellus. Nam est mauris, facilisis id scelerisque sed, euismod eget lorem. Vivamus ultrices feugiat mi ac semper. Aliquam varius in nibh at efficitur. Nullam tempus felis id dolor porta euismod.', userId: 1 }] }
      ]
    }
  },

  methods: {
    openPreview (image) {
      this.previewImage = image
      this.$bvModal.show('preview-modal')
    },

    sendComment (previewImage) {
      if (this.comment.length) {
        previewImage.comments.push({ id: previewImage.comments[previewImage.comments.length - 1].id + 1, text: this.comment, userId: this.$auth.user.id })
        this.comment = ''
      }
    },

    removeComment (commentId) {
      this.previewImage.comments = this.previewImage.comments.filter(comment => comment.id !== commentId)
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

.preview-image {
  max-height: 300px;
}

.comment-container {
  max-height: 300px;
  overflow: auto;
}

.icon-button {
  cursor: pointer;
}
</style>
