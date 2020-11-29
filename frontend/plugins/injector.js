import Vue from 'vue'

import { BootstrapVueIcons } from 'bootstrap-vue'
import 'bootstrap-vue/dist/bootstrap-vue-icons.min.css'
import Vuelidate from 'vuelidate'

import Input from '~/components/input/Input'
import FormError from '~/components/ui/FormError'

Vue.use(BootstrapVueIcons)
Vue.use(Vuelidate)

Vue.component('picto-input', Input)
Vue.component('picto-form-error', FormError)
