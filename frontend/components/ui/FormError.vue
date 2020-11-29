<template>
  <b-form-invalid-feedback :state="!$store.state.error" class="my-3 text-center">
    {{ $store.state.error }}
  </b-form-invalid-feedback>
</template>
