<template>
  <div class="row">
    <slot />
  </div>
</template>

<style lang="scss" scoped>
.row {
  margin-left: -5px;
  margin-right: -5px;

  & > [class*=" col"], & > [class^="col"] {
    padding: 5px;
  }
}
</style>
