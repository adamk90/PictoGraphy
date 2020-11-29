#include <iostream>
#include <napi.h>
#include "../CaffParser/caff.h"

using namespace Napi;
using namespace std;

struct membuf : streambuf {
    membuf(int8_t* begin, int8_t* end) {
        this->setg(reinterpret_cast<char*>(begin), reinterpret_cast<char*>(begin), reinterpret_cast<char*>(end));
    }
};

void getCaffInfos(const Caff& caff, Object& obj, const CallbackInfo& info) {
    obj.Set("Creator", caff.getCreator());
    obj.Set("Year", caff.getYear());
    obj.Set("Month", caff.getMonth());
    obj.Set("Day", caff.getDay());
    obj.Set("Hour", caff.getHour());
    obj.Set("Minute", caff.getMinute());
    vector<Animation> anims = caff.getAnimations();
    if (anims.size() > 0) {
        ull bmpSize = 0;
        auto bmpBytes = anims[0].img.getBMP(bmpSize);
        Object bmpObj = Object::New(info.Env());
        for (ull i = 0; i < bmpSize; ++i) {
            bmpObj.Set(i, bmpBytes.get()[i]);
        }
        obj.Set("Preview", bmpObj);
    }
    Object tags = Object::New(info.Env());
    ull tagIndex = 0;
    for (ull i = 0; i < anims.size(); ++i) {
        for (auto s : anims[i].img.getTags()) {
            tags.Set(tagIndex++, s);
        }
    }
    obj.Set("Tags", tags);
}

Object Parse(const CallbackInfo& info) {
    auto obj = Object::New(info.Env());
    if (info.Length() != 1) {
        obj.Set("Error", "Wrong call!");
    } else {
        auto arg = info[0];
        Int8Array arr = arg.As<Int8Array>();
        membuf buf(arr.Data(), arr.Data() + arr.ElementLength());
        istream is(&buf);
        try {
            Caff caff = Caff::parse(is);
            getCaffInfos(caff, obj, info);
        } catch (const exception& ex) {
            obj.Set("Error", ex.what());
        }
    }
    return obj;
}

Object Init(Env env, Object exports) {
    exports.Set("parse", Function::New(env, Parse));
    return exports;
}

NODE_API_MODULE(addon, Init)