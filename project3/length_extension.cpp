#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace std;

const vector<uint32_t> IV = { 0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e };

uint32_t ROTL(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

vector<uint8_t> put_uint32_be(uint32_t data) {
    vector<uint8_t> bytes(4);
    for (int i = 3; i >= 0; i--) {
        bytes[i] = data & 0xff;
        data >>= 8;
    }
    return bytes;
}

vector<uint8_t> padding(vector<uint8_t> message) {
    int len = message.size();
    int k = 448 - ((len * 8) % 512);
    if (k <= 0) k += 512;

    for (int i = 0; i < k / 8 - 2; i++) {
        message.push_back(0x00);
    }
    message.push_back(0x80);


    // 修改计算消息长度的方式
    uint64_t message_len = (uint64_t)len * 8;
    for (int i = 7; i >= 0; i--) {
        message.push_back((message_len >> (i * 8)) & 0xff);
    }

    return message;
}

uint32_t xor_op(uint32_t a, uint32_t b) {
    return a ^ b;
}

uint32_t sm3_ff_j(uint32_t x, uint32_t y, uint32_t z, int j) {
    if (j >= 0 && j <= 15) {
        return x ^ y ^ z;
    }
    else {
        return (x & y) | (x & z) | (y & z);
    }
}

uint32_t sm3_gg_j(uint32_t x, uint32_t y, uint32_t z, int j) {
    if (j >= 0 && j <= 15) {
        return x ^ y ^ z;
    }
    else {
        return (x & y) | ((~x) & z);
    }
}

uint32_t sm3_p_0(uint32_t x) {
    return x ^ ROTL(x, 9) ^ ROTL(x, 17);
}

uint32_t sm3_p_1(uint32_t x) {
    return x ^ ROTL(x, 15) ^ ROTL(x, 23);
}

vector<uint32_t> sm3_cf(vector<uint32_t> v_i, vector<uint8_t> b_i) {
    vector<uint32_t> w;
    for (int i = 0; i < 16; i++) {
        uint32_t data = 0;
        for (int j = 0; j < 4; j++) {
            data <<= 8;
            data |= b_i[i * 4 + j];
        }
        w.push_back(data);
    }

    for (int j = 16; j < 68; j++) {
        w.push_back(xor_op(sm3_p_1(w[j - 16] ^ w[j - 9] ^ ROTL(w[j - 3], 15)), ROTL(w[j - 13], 7) ^ w[j - 6]));
    }

    vector<uint32_t> w_1;
    for (int j = 0; j < 64; j++) {
        w_1.push_back(w[j] ^ w[j + 4]);
    }

    uint32_t a, b, c, d, e, f, g, h;
    a = v_i[0];
    b = v_i[1];
    c = v_i[2];
    d = v_i[3];
    e = v_i[4];
    f = v_i[5];
    g = v_i[6];
    h = v_i[7];

    uint32_t ss1, ss2, tt1, tt2;
    for (int j = 0; j < 64; j++) {
        ss1 = ROTL((ROTL(a, 12) + e + ROTL(sm3_p_0(j), j % 32)), 7);
        ss2 = ss1 ^ ROTL(a, 12);
        tt1 = sm3_ff_j(a, b, c, j) + d + ss2 + w_1[j];
        tt2 = sm3_gg_j(e, f, g, j) + h + ss1 + w[j];
        d = c;
        c = ROTL(b, 9);
        b = a;
        a = tt1;
        h = g;
        g = ROTL(f, 19);
        f = e;
        e = sm3_p_0(tt2);
    }

    vector<uint32_t> v_j(8);
    v_j[0] = a ^ v_i[0];
    v_j[1] = b ^ v_i[1];
    v_j[2] = c ^ v_i[2];
    v_j[3] = d ^ v_i[3];
    v_j[4] = e ^ v_i[4];
    v_j[5] = f ^ v_i[5];
    v_j[6] = g ^ v_i[6];
    v_j[7] = h ^ v_i[7];

    return v_j;
}

vector<uint8_t> sm3_hash(vector<uint8_t> message) {
    message = padding(message);

    int n = message.size() / 64;

    vector<uint32_t> v_i = IV;
    for (int i = 0; i < n; i++) {
        vector<uint8_t> b_i(message.begin() + i * 64, message.begin() + (i + 1) * 64);
        vector<uint32_t> v_j = sm3_cf(v_i, b_i);
        v_i = v_j;
    }

    vector<uint8_t> hash_bytes;
    for (int i = 0; i < 8; i++) {
        vector<uint8_t> bytes = put_uint32_be(v_i[i]);
        for (int j = 0; j < 4; j++) {
            hash_bytes.push_back(bytes[j]);
        }
    }

    return hash_bytes;
}

string sm3_digest(string message) {
    vector<uint8_t> message_bytes(message.begin(), message.end());
    vector<uint8_t> hash_bytes = sm3_hash(message_bytes);
    stringstream ss;
    for (int i = 0; i < hash_bytes.size(); i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash_bytes[i];
    }
    return ss.str();
}

int main() {
    string message = "202100150110";
    string append_m = "This is an additional message";
    string guess_hash = "7c34e2debf8a2078dfc0b3b8ba37291e1f44da7ad5b6db196f9d727e17a1912d";

    // Calculate hash of original message
    string cip_hash = sm3_digest(message);

    // Construct message with additional message
    string new_msg_str = message + append_m;

    // Calculate hash of new message
    string new_hash = sm3_digest(new_msg_str);

    cout << "Message: " << message << endl;
    cout << "Hashed message: " << cip_hash << endl;
    cout << "Appended message: " << append_m << endl;
    cout << "Manually constructed message's hash: " << guess_hash << endl;
    cout << "New message:\n" << new_msg_str << endl;
    cout << "Hash of new message: " << new_hash << endl;

    if (new_hash == guess_hash) {
        cout << "Success" << endl;
    }
    else {
        cout << "Fail" << endl;
    }

    return 0;
}
