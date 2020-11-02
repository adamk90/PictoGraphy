#include "caff.h"

const Ciff& Caff::getCiff(ull i) const {
    if (i >= anims.size()) {
        throw out_of_range("Index is out of bound");
    }
    return anims[i].img;
}


void Caff::parseBlock(istream& in, ull& currLength, ParsingStatus& status, Credits& credits, vector<Animation>& anims) {
    int id;
    ull length;

    char idByte[1];
    if (in.read(idByte, 1) && in.gcount() == 1) {
        id = Utils::intFromBytes(idByte, 1);
        if (id == 1 && status.headerDone) {
            throw domain_error("Invalid Caff: multiple headers found");
        } else if (id != 1 && !status.headerDone) {
            throw domain_error("Invalid Caff: the first block must be the header");
        } else if (id == 2 && status.creditsDone) {
            throw domain_error("Invalid Caff: multiple credits found");
        }
        char lengthBytes[8];
        if (in.read(lengthBytes, 8) && in.gcount() == 8) {
            length = Utils::intFromBytes(lengthBytes, 8);
            currLength += length;
            if (currLength < length) {
                throw overflow_error("Invalid Caff: length overflow");
            }
            switch (id) {
                case 1:
                    status.numAnims = parseHeader(in, length);
                    if (status.numAnims < status.processedAnims) {
                        throw domain_error("Invalid Caff: more animations present than provided in header");
                    }
                    status.headerDone = true;
                    break;
                case 2:
                    parseCredits(in, length, credits);
                    status.creditsDone = true;
                    break;
                case 3:
                    parseAnimation(in, length, anims);
                    status.processedAnims += 1;
                    if (status.numAnims < status.processedAnims) {
                        throw domain_error("Invalid Caff: more animations present than provided in header");
                    }
                    break;
                default:
                    throw domain_error("Invalid Caff block: no such ID: " + id);
            }
        } else {
            throw domain_error("Invalid Caff block: short length");
        }
    } else {
        throw domain_error("Invalid Caff block: missing ID");
    } 
}

/*
 * Parsing the header.
 *
 * @throws domain_error Thrown if not enough bytes in the input stream
 * @throws domain_error Thrown if header size declared in header does not match real size 
 * 
 * @param in The input stream that contains the header content
 * @param remainingBytes Tells how many bytes to parse from the input stream
 * @return Number of animations declared in the header
 */
ull Caff::parseHeader(istream& in, const ull& remainingBytes) {
    if (remainingBytes != Caff::HEADER_SIZE) {
        throw domain_error("Invalid Caff header: size should be 20");
    }
    char* content = new char[remainingBytes];
    if (in.read(content, remainingBytes) && in.gcount() > 0 && (ull)in.gcount() == remainingBytes) {
        if (content[0] == 'C' && content[1] == 'A' && content[2] == 'F' && content[3] == 'F') {
            ull headerSize = Utils::intFromBytes(&content[4], 8);
            if (headerSize != Caff::HEADER_SIZE) {
                delete[] content;
                throw domain_error("Invalid Caff header: size should be 20");
            }
            ull numAnims = Utils::intFromBytes(&content[12], 8);
            delete[] content;
            return numAnims;
        } else {
            delete[] content;
            throw domain_error("Invalid Caff header: wrong magic");
        }
    } else {
        delete[] content;
        throw domain_error("Invalid Caff header: actual size is smaller than declared");
    }
    return 0;
}

/*
 * Parsing the credits and storing the fields.
 *
 * @throws domain_error Thrown if not enough bytes in the input stream
 * @throws domain_error Thrown if creator_len declared in header does not match real length of creator
 * @throws domain_error Thrown if any fields not found or incomplete
 * 
 * @param in The input stream that contains the header content
 * @param remainingBytes Tells how many bytes to parse from the input stream
 * @param credits The Credits structure where the parsed fields should be stored.
 */
void Caff::parseCredits(istream& in, const ull& remainingBytes, Credits& credits) {
    if (remainingBytes < Caff::MIN_CREDITS_SIZE) {
        throw domain_error("Invalid Caff credits: size should be at least 14");
    }
    char* content = new char[remainingBytes];
    if (in.read(content, remainingBytes) && in.gcount() > 0 && (ull)in.gcount() == remainingBytes) {
        credits.year = Utils::intFromBytes(content, 2);
        credits.month = Utils::intFromBytes(&content[2], 1);
        credits.day = Utils::intFromBytes(&content[3], 1);
        credits.hour = Utils::intFromBytes(&content[4], 1);
        credits.minute = Utils::intFromBytes(&content[5], 1);
        ull creatorLen = Utils::intFromBytes(&content[6], 8);
        ull fullLength = Caff::MIN_CREDITS_SIZE + creatorLen;
        if (fullLength < creatorLen) {
            delete[] content;
            throw domain_error("Invalid Caff credits: creatorLen too big");
        }
        if (fullLength != remainingBytes) {
            delete[] content;
            throw domain_error("Invalid Caff credits: actual size does not match declared size");
        }
        credits.creator = string{&content[14], creatorLen};
        delete[] content;
    } else {
        throw domain_error("Invalid Caff credits: actual size is smaller than declared");
    }
}

/*
 * Parsing the credits and storing the fields.
 *
 * @throws domain_error Thrown if not enough bytes in the input stream
 * @throws domain_error Thrown if any fields not found or incomplete
 * 
 * @param in The input stream that contains the header content
 * @param remainingBytes Tells how many bytes to parse from the input stream
 * @param anims The vector where the parsed Animation should be stored.
 */
void Caff::parseAnimation(istream& in, const ull& remainingBytes, vector<Animation>& anims) {
    if (remainingBytes < Caff::MIN_ANIMATION_SIZE) {
        throw domain_error("Invalid Caff animation: size invalid");
    }
    char content[8];
    if (in.read(content, 8) && in.gcount() == 8) {
        Animation anim{};
        anim.duration = Utils::intFromBytes(content, 8);
        anim.img = Ciff::parse(in);
        anims.push_back(anim);
    } else {
        throw domain_error("Invalid Caff animation: actual size is smaller than declared");
    }
}

Caff Caff::parse(istream& in){
    ParsingStatus status{};
    Credits credits{};
    vector<Animation> anims{};
    ull currLength = 0;

    while (in.peek() != ifstream::traits_type::eof()) {
        parseBlock(in, currLength, status, credits, anims);
    }

    if (status.numAnims != status.processedAnims) {
        throw domain_error("Invalid Caff: number of anims declared in header not matching actual");
    } else if (!status.creditsDone) {
        //Should we do this?
        //throw domain_error("Invalid Caff: no credits block found");
    }

    return Caff{credits, anims};
}