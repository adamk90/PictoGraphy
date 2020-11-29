export default (ctx, inject) => {
  const validator = {
    state (field) {
      const { $dirty, $error } = field

      return $dirty ? !$error : null
    },

    validateField (field) {
      if (field) {
        const rule = Object.keys(field.$params).find(elem => !field[elem])

        switch (rule) {
          case 'required': return 'A mező kitöltése kötelező.'
          case 'minLength': return `Legalább ${field.$params[rule].min} karakter hosszúnak kell lennie.`
          case 'hasLowercase': return 'A mezőnek kisbetűt tartalmaznia kell'
          case 'hasUppercase': return 'A mezőnek nagybetűt tartalmaznia kell'
          case 'hasNumber': return 'A mezőnek számot tartalmaznia kell'
          case 'hasSpecial': return 'A mezőnek speciális karaktert tartalmaznia kell'
        }
      }
    }
  }

  inject('validator', validator)
}
