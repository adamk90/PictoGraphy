<template>
  <b-form-group
    :label="label"
    :invalid-feedback="$validator.validateField(field)"
  >
    <b-form-input
      v-model="field.$model"
      :state="$validator.state(field)"
      :placeholder="label"
      :type="type"
      @input="$store.commit('clearError')"
    />
  </b-form-group>
</template>

<script>
export default {
  props: {
    label: {
      type: String,
      required: true
    },

    type: {
      type: String,
      required: false,
      default: 'text'
    },

    field: {
      type: Object,
      required: true
    }
  }
}
</script>
