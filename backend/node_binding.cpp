#include <iostream>
#include <napi.h>
#include "../CaffParser/caff.h"

using namespace Napi;
using namespace std;

Object Parse(const CallbackInfo& info) {
	auto obj = Object::New(info.Env());
	if (info.Length() != 1) {
		obj.Set("Error", "Wrong call!");
	} else {
		auto arg = info[0];
		cout << arg.As<String>().Utf8Value() << endl;
	}
	return obj;
}

Object Init(Env env, Object exports) {
	exports.Set("parse", Function::New(env, Parse));
	return exports;
}

NODE_API_MODULE(addon, Init)