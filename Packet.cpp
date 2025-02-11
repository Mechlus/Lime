#include "Packet.h"

Packet::Packet() : outp(net::SOutPacket()), inp(net::SInPacket(nullptr, 0)) {}

Packet::Packet(const net::SInPacket out) : inp(out), inPacket(true) {}

void Packet::append(int type, sol::object value) {
    switch (type) {
    case 0: // INT_8BIT_UNSIGNED
        if (value.is<uint8_t>()) outp << value.as<uint8_t>();
        break;
    case 1: // INT_16BIT_UNSIGNED
        if (value.is<uint16_t>()) outp << value.as<uint16_t>();
        break;
    case 2: // INT_32BIT_UNSIGNED
        if (value.is<uint32_t>()) outp << value.as<uint32_t>();
        break;
    case 3: // INT_16BIT_SIGNED
        if (value.is<int16_t>()) outp << value.as<int16_t>();
        break;
    case 4: // INT_32BIT_SIGNED
        if (value.is<int32_t>()) outp << value.as<int32_t>();
        break;
    case 5: // FLOATING_32BIT
        if (value.is<float>()) outp << value.as<float>();
        break;
    case 6: // FLOATING_64BIT
        if (value.is<double>()) outp << value.as<double>();
        break;
    case 7: // CHAR_8BIT
        if (value.is<char>()) outp << value.as<char>();
        break;
    case 8: // CHAR_CONST_8BIT
        if (value.is<std::string>()) outp << value.as<std::string>().c_str();
        break;
    case 9: // STRING
        if (value.is<std::string>()) outp << value.as<std::string>();
        break;
    case 10: { // FILE
        if (value.is<std::string>()) {
            std::ifstream file(value.as<std::string>(), std::ios::binary);
            if (file) {
                std::ostringstream buffer;
                buffer << file.rdbuf();
                outp << buffer.str();
            }
            else if (irrHandler->verbose) {
                std::string msg = "Failed to append data to packet: Could not open file ";
                msg += value.as<std::string>();
                dConsole.sendMsg(msg.c_str(), MESSAGE_TYPE::WARNING);
            }
        }
        break;
    }
    default:
        if (irrHandler->verbose) dConsole.sendMsg("Failed to append data to packet: Unknown type", MESSAGE_TYPE::WARNING);
    }
}

void Packet::clear() {
    outp.clearData();
}

int Packet::getSize() {
    return inPacket ? inp.getSize() : outp.getSize();
}

sol::object Packet::pop(int type) {
    switch (type) {
    case 0: { // INT_8BIT_UNSIGNED
        uint8_t value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, value);
    }
    case 1: { // INT_16BIT_UNSIGNED
        uint16_t value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, value);
    }
    case 2: { // INT_32BIT_UNSIGNED
        uint32_t value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, value);
    }
    case 3: { // INT_16BIT_SIGNED
        int16_t value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, value);
    }
    case 4: { // INT_32BIT_SIGNED
        int32_t value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, value);
    }
    case 5: { // FLOATING_32BIT
        float value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, value);
    }
    case 6: { // FLOATING_64BIT
        double value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, value);
    }
    case 7: { // CHAR_8BIT
        char value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, std::string(1, value));
    }
    case 8: { // CHAR_CONST_8BIT
        char value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, std::string(1, value));
    }
    case 9: { // STRING
        std::string value;
        inp >> value;
        return sol::object(*(lua), sol::in_place, value);
    }
    case 10: { // FILE
        // Probably make a special function that writes/saves this to file?
    }
    default:
        if (irrHandler->verbose)
            dConsole.sendMsg("Failed to pop data from packet: Unknown type", MESSAGE_TYPE::WARNING);
        return sol::nil;
    }
}

void Packet::compress() {
    if (inPacket) inp.compressPacket(); else outp.compressPacket();
}

void Packet::decompress() {
    if (inPacket) inp.deCompressPacket(); else outp.deCompressPacket();
}

void Packet::encrypt() {
    if (inPacket) return; else outp.encryptPacket(irrHandler->key.c_str());
}

void Packet::decrypt() {
    if (inPacket) inp.decryptPacket(irrHandler->key.c_str());
}

bool Packet::isValid() {
    if (inPacket) return inp.isValid(); else return false;
}

int Packet::getSenderID() {
    if (inPacket) return inp.getPlayerId(); else return 0;
}