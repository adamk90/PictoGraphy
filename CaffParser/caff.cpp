#include "caff.h"

uint intFromBytes(char* bytes, uint size) {
    uint res = 0;
    for (uint i = 0; i < size; ++i) {
        uint val = static_cast<byte>(bytes[i]);
        res = res | val << (size - i - 1) * 4;
    }
    return res;
}

Ciff Caff::getCiff(uint i) {
    if (i >= ciffs.size()) {
        throw out_of_range("Index is out of bound");
    }
    return ciffs[i];
}

Caff Caff::parse(istream& in, int main_stage = CAFF::MAIN_STAGES, int sub_stage){
    uint headerSize;
    uint numAnim;
    byte[] year;
    byte month;
    byte day;
    byte hour;
    byte minute;
    uint creatorLen;
    string creator;
    uint duration;

    bool header = false;
    bool credit = false;
    bool animation = false;
    switch (main_stage){
        case 1:
            break;
        case 2:
            header = true;
            break;
        case 3:
            header = true;
            credit = true;
            break;
        default:
            throw domain_error("Undefined stage counter");
            break;
    }

    if (main_stage >=1) {
        if (sub_stage >= 1 || header){
            char magic[5];
            if (in.get(magic, 5) && in.gcount() == 4) {
                if (string{magic} != string("CAFF")) {
                    throw domain_error("CAFF should start with 'CAFF'");
                }
            } else {
                throw domain_error("Invalid CAFF format: short magic");
            }
        }
        if (sub_stage >= 2 || header){
            char headerS[9];
            if (in.get(headerS, 9) && in.gcount() == 8) {
                headerSize = intFromBytes(headerS, 8);
                if (headerSize < Caff::MIN_HEADER_SIZE) {
                    throw domain_error("Invalid headerSize");
                }
            } else {
                throw domain_error("Invalid CAFF format: short headerSize");
            }
        }
        if (sub_stage >= 3 || header){
            char numA[9];
            if (in.get(numA, 9) && in.gcount() == 8) {
                numAnim = intFromBytes(numA, 8);
            } else {
                throw domain_error("Invalid CAFF format: short numAnim");
            }
        }
    }

    if (main_stage >=2) {
        if (sub_stage >= 1 || credit){
            byte ye[3];
            if (in.get(ye, 3) && in.gcount() == 2){
                year=ye;
            } else {
                throw domain_error("Invalid CAFF format: short Year");
            }
        }
        if (sub_stage >= 2 || credit){
            byte mo[2];
            if (in.get(mo,2) && in.gcount() == 1){
                month=mo;
            } else {
                throw domain_error("Invalid CAFF format: short month");
            }
        }
        if (sub_stage >= 3 || credit){
            byte da[2];
            if (in.get(da,2) && in.gcount() == 1){
                day=da;
            } else {
                throw domain_error("Invalid CAFF format: short day");
            }
        }
        if (sub_stage >= 4 || credit){
            byte ho[2];
            if (in.get(ho,2) && in.gcount() == 1){
                hour=ho;
            } else {
                throw domain_error("Invalid CAFF format: short hour");
            }
        }
        if (sub_stage >= 5 || credit){
            byte mi[2];
            if (in.get(mi,2) && in.gcount() == 1){
                minute=mi;
            } else {
                throw domain_error("Invalid CAFF format: short minute");
            }
        }
        if (sub_stage >= 6 || credit){
            char crLen[9];
            if (in.get(crLen, 9) && in.gcount() == 8) {
                creatorLen = intFromBytes(crLen, 8);
            } else {
                throw domain_error("Invalid CAFF format: short creatorLen");
            }
        }
        if (sub_stage >= 7 || credit){
            char cr[creatorLen];
            if (in.get(cr, creatorlen+1) && in.gcount() == creatorLen){
                creator = string(cr);
            } else {
                throw domain_error("Invalid CAFF format: short creator");
            }
        }
    }

    if (main_stage >= 3){
        if (sub_stage >=1){
            char dur[9];
            if (in.get(dur, 9) && in.gcount() == 8) {
                duration = intFromBytes(dur, 8);
            } else {
                throw domain_error("Invalid CAFF format: short duration");
            }
        }
        if (sub_stage >= 2){
            //TODO:PARSING THE CIFFS INTO
        }
    }

    return Caff{headerSize, numAnim, year, month, day, hour, minute, creatorLen, "", duration, vector<Ciff>(0)};
}