export default (ctx, inject) => {
  const errorToast = (bvToast, title, message) => {
    if (bvToast) {
      bvToast.toast(message, {
        title,
        autoHideDelay: 3000,
        toaster: 'b-toaster-bottom-center',
        variant: 'danger',
        solid: true
      })
    }
  }

  const toast = (bvToast, title, message) => {
    if (bvToast) {
      bvToast.toast(message, {
        title,
        autoHideDelay: 3000,
        toaster: 'b-toaster-bottom-center',
        variant: 'primary',
        solid: true
      })
    }
  }

  inject('toast', toast)
  inject('errorToast', errorToast)
}
