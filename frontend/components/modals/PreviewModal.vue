<template>
  <b-modal
    id="preview-modal"
    size="lg"
    hide-footer
    body-class="p-4"
    centered
    @hidden="bought = false"
  >
    <template v-slot:modal-header>
      <b-row v-if="image" align-h="between" no-gutters class="w-100">
        <b-col cols align-self="center">
          <h4 class="m-0">
            {{ image.previewBmp.split('/').pop() }}
          </h4>
        </b-col>
        <b-col cols="auto">
          <b-button v-if="image.uploader === $auth.user.username || $auth.user.isAdmin" variant="danger" @click="deleteCaff(image.id)">
            <b-icon-trash />
            CAFF törlése
          </b-button>

          <b-button v-if="image.isOwner || bought" variant="primary" @click="downloadCaff(image.id)">
            <b-icon-download />
            Letöltés
          </b-button>

          <b-button v-else variant="success" @click="buyCaff(image.id)">
            <b-icon-cart4 />
            Vásárlás
          </b-button>
        </b-col>
      </b-row>
    </template>

    <div v-if="image">
      <div class="my-3 d-flex">
        <b-avatar variant="info" size="26px" class="mr-1" />
        <span><b>Feltöltő:</b> {{ image.uploader }}</span>

        <b-avatar variant="success" size="26px" class="ml-3 mr-1" />
        <span><b>Készítő:</b> {{ image.creator }}</span>

        <span class="ml-auto font-weight-bold">
          <b-icon-calendar variant="primary" />
          {{ image.creationDate.split('T').shift() }}
        </span>
      </div>

      <div class="text-center">
        <b-img fluid class="preview-image" :src="$store.state.imageServer + image.previewBmp" />
      </div>

      <h4 class="text-right mt-2">
        <b-badge v-for="tag in image.tags" :key="tag" pill variant="primary" class="ml-1">
          {{ tag }}
        </b-badge>
      </h4>

      <h4 class="mt-3 ">
        Megjegyzések
      </h4>
      <b-list-group class="comment-container">
        <b-list-group-item v-if="!image.comments.length">
          Ehhez a caff-hoz még nem érkezett hozzászólás
        </b-list-group-item>

        <b-list-group-item v-for="comment in image.comments" v-else :key="comment.id" class="text-left border mt-2">
          <b-row>
            <b-col cols="12" align-self="center" class="pb-2 mb-2 border-bottom d-flex">
              <b-avatar variant="info" size="26px" />
              <span class="font-weight-bold ml-2">{{ comment.user }}</span>

              <span class="ml-auto font-weight-bold">
                <b-icon-calendar variant="info" />
                {{ comment.date.split('T').shift() }}
              </span>
            </b-col>

            <b-col>
              {{ comment.comment }}
            </b-col>

            <b-col v-if="comment.user === $auth.user.username || $auth.user.isAdmin" cols="1" align-self="center">
              <div v-b-tooltip.hover title="Megjegyzés törlése" class="text-center remove-button" @click="deleteConfirm(comment.id)">
                <b-icon-trash-fill class="text-danger" />
              </div>
            </b-col>
          </b-row>
        </b-list-group-item>
      </b-list-group>

      <b-form class="mt-3" @submit.prevent.stop="sendComment(image)">
        <b-form-row class="align-items-center">
          <b-col>
            <b-form-input v-model="newComment" type="text" placeholder="Saját megjegyzés..." />
          </b-col>
          <b-col cols="auto" class="icon-button">
            <b-icon-arrow-right-circle-fill class="h3 mb-0 m-2 text-primary" @click="sendComment(image)" />
          </b-col>
        </b-form-row>
      </b-form>
    </div>
  </b-modal>
</template>

<script>
import endpoints from '~/utils/endpoints'

const download = require('downloadjs')

export default {
  props: {
    image: {
      type: Object,
      required: false,
      default: null
    }
  },

  data () {
    return {
      comments: [],
      newComment: '',
      bought: false
    }
  },

  methods: {
    sendComment () {
      if (this.newComment.length) {
        this.$axios.post(endpoints.comment(this.image.id), { comment: this.newComment }).then(({ data }) => {
          this.image.comments.unshift(data)
          this.newComment = ''
        })
      }
    },

    deleteConfirm (id) {
      this.$bvModal.msgBoxConfirm('Biztosan törölni szeretnéd a megjegyzésedet?', {
        title: 'Megjegyzés törlése',
        size: 'sm',
        okVariant: 'danger',
        okTitle: 'Igen',
        cancelTitle: 'Nem',
        cancelVariant: 'outline-dark',
        centered: true
      })
        .then((value) => {
          if (value) {
            this.deleteComment(id)
          }
        })
    },

    deleteComment (id) {
      this.$axios.delete(`${endpoints.deleteComment}/${this.image.id}/${id}`)
        .then(({ data }) => {
          this.image.comments = this.image.comments.filter(comment => comment.id !== id)
        })
        .catch(() => {
          this.$errorToast(this.$bvToast, 'Hiba', 'Hiba a megjegyzés törlése közben.')
        })
    },

    downloadCaff (caffId) {
      this.$axios({
        url: endpoints.download(caffId),
        method: 'GET',
        responseType: 'blob'
      }).then((response) => {
        download(response.data, caffId + '.caff')
      })
    },

    buyCaff (caffId) {
      this.$axios.get(endpoints.buy(caffId)).then((data) => {
        this.bought = true
        this.$toast(this.$bvToast, 'Sikeres vásárlás', caffId)
      })
    },

    deleteCaff (caffId) {
      this.$store.dispatch('deteleCaff', caffId)
        .then((data) => {
          this.$toast(this.$bvToast, 'CAFF kitörölve', caffId)
          this.$emit('caff-removed', caffId)
          this.$bvModal.hide('preview-modal')
        })
        .catch(() => {
          this.$errorToast(this.$bvToast, 'Hiba a CAFF törlése közben.', caffId)
        })
    }
  }
}
</script>

<style lang="scss" scoped>
.preview-image {
  max-height: 300px;
}

.comment-container {
  max-height: 280px;
  overflow: auto;
}
</style>
