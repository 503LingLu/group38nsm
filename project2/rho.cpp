#include <iostream>
#include <random>
#include <chrono>
#include <sstream>

std::string IV = "7380166f";

std::string toHex(int x, int length) {
    std::stringstream tmp;
    tmp << std::hex << x;
    std::string result(tmp.str());
    if (result.length() < length) {
        result = std::string(length - result.length(), '0') + result;
    }
    return result;
}

int toDec(std::string s) {
    std::stringstream ss;
    ss << std::hex << s;
    int ret;
    ss >> ret;
    return ret;
}

int t(int j) {
    return j < 16 ? 0x79cc4519 : 0x7a879d8a;
}

int csl(int x, int k) {
    if (k == 0 || k == 32) {
        return x;
    }
    else {
        int ret, tmp;
        tmp = x >> (32 - k);
        ret = x << k;
        ret += tmp;
        return ret;
    }
}

int ff(int x, int y, int z, int j) {
    if (j < 16) {
        return x ^ y ^ z;
    }
    else {
        return (x & y) | (y & z) | (z & x);
    }
}

int gg(int x, int y, int z, int j) {
    if (j < 16) {
        return x ^ y ^ z;
    }
    else {
        return (x & y) | (~x & z);
    }
}

int p0(int x) {
    return x ^ csl(x, 9) ^ csl(x, 17);
}

int p1(int x) {
    return x ^ csl(x, 15) ^ csl(x, 23);
}

std::string fill(std::string m) {
    int l = m.length() * 4;
    m += '8';
    int k = 112 - (m.length() % 128);
    m += std::string(k, '0') + toHex(l, 16);
    return m;
}

std::vector<std::string> grouping(std::string m) {
    int n = m.length() / 32;
    std::vector<std::string> b(n);
    for (int i = 0; i < n; i++) {
        b[i] = m.substr(i * 32, 32);
    }
    return b;
}

std::vector<int> extend(std::vector<std::string> bi) {
    std::vector<int> w(132);
    for (int i = 0; i < 132; i++) {
        if (i < 16) {
            w[i] = toDec(bi[i]);
        }
        else if (i < 68) {
            w[i] = p1(w[i - 16] ^ w[i - 9] ^ csl(w[i - 3], 15)) ^ csl(w[i - 13], 7) ^ w[i - 6];
        }
        else {
            w[i] = w[i - 68] ^ w[i - 64];
        }
    }
    return w;
}

std::string cf(std::string vi, std::string bi) {
    std::vector<int> w = extend(grouping(bi));
    int a = toDec(vi.substr(0, 8));
    for (int i = 0; i < 16; i++) {
        int ss1 = csl((csl(a, 12) + a + csl(t(i), i)) & 0xFFFFFFFF, 7);
        int ss2 = ss1 ^ csl(a, 12);
        int tt1 = (a + ss2 + w[i + 68]) & 0xFFFFFFFF;
        int tt2 = (a + ss1 + w[i]) & 0xFFFFFFFF;
        a = csl(a, 9);
        a += tt1;
        a ^= p0(tt2);
    }
    std::stringstream tmp;
    tmp << std::hex << (a ^ toDec(vi));
    std::string ret(tmp.str());
    if (ret.length() < 8) {
        ret = std::string(8 - ret.length(), '0') + ret;
    }
    std::string result;
    for (int i = 0; i < 8; i++) {
        result += char(std::stoi(ret.substr(i * 2, 2), nullptr, 16) ^ vi[i]);
    }
    return result;
}

std::string iteration(std::vector<std::string> b) {
    std::string v = IV;
    for (auto bi : b) {
        v = cf(v, bi);
    }
    return v;
}

std::string SM3(std::string m) {
    m = fill(m);
    std::vector<std::string> b = grouping(m);
    return iteration(b);
}

std::string randStr(int length) {
    std::string ret;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);
    for (int i = 0; i < length; i++) {
        ret += std::to_string(dis(gen));
    }
    return ret;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, INT_MAX);
    std::string str1 = "";
    std::string str2 = "";
    std::string h1 = "";
    std::string h2 = "";
    while (true) {
        str2 = "";
        str1 = randStr(16);
        std::cout << str1 << std::endl;
        h1 = SM3(str1);
        for (int i = 0; i < h1.length(); i++) {
            if (h1[i] >= '1' && h1[i] <= '9') {
                str2 += '3' + (h1[i] - '0');
            }
            else {
                str2 += '6' + (h1[i] - 'a' + 1);
            }
        }
        h2 = SM3(str2);
        if (h1 == h2) {
            std::cout << "success" << std::endl;
            std::cout << str1 << std::endl;
            std::cout << str2 << std::endl;
            break;
        }
    }
    return 0;
}
