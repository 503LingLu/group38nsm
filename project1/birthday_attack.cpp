#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <random>

std::string toHex(int x, int length) {
    char c[] = "0123456789abcdef";
    std::string tmp(length, '0');
    int r, i = length - 1;
    while (x != 0) {
        r = x % 16;
        tmp[i] = c[r];
        i--;
        x >>= 4;
    }
    return tmp;
}

int toDec(const std::string& s) {
    int ret = 0;
    int base = 1;
    for (int i = s.length() - 1; i >= 0; i--) {
        int num = 0;
        if (isdigit(s[i])) {
            num = s[i] - '0';
        } else {
            num = s[i] - 'a' + 10;
        }
        ret += num * base;
        base <<= 4;
    }
    return ret;
}

int t(int j) {
    if (j < 16) {
        return 0x79cc4519;
    } else {
        return 0x7a879d8a;
    }
}

int csl(int x, int k) {
    if (k == 0 || k == 32) {
        return x;
    } else {
        int ret = 0;
        int tmp = 0;
        tmp = x >> (32 - k);
        ret = x << k;
        ret += tmp;
        return ret;
    }
}

int ff(int x, int y, int z, int j) {
    if (j < 16) {
        return x ^ y ^ z;
    } else {
        return (x & y) | (y & z) | (z & x);
    }
}

int gg(int x, int y, int z, int j) {
    if (j < 16) {
        return x ^ y ^ z;
    } else {
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

std::vector<std::string> grouping(const std::string& m) {
    int n = m.length() / 32;
    std::vector<std::string> b;
    for (int i = 0; i < n; i++) {
        b.push_back(m.substr(i * 32, 32));
    }
    return b;
}

std::vector<int> extend(const std::vector<std::string>& bi) {
    std::vector<int> w(132);
    for (int i = 0; i < 132; i++) {
        if (i < 16) {
            w[i] = toDec(bi[i]);
        } else if (i < 68) {
            w[i] = p1(w[i - 16] ^ w[i - 9] ^ csl(w[i - 3], 15)) ^ csl(w[i - 13], 7) ^ w[i - 6];
        } else {
            w[i] = w[i - 68] ^ w[i - 64];
        }
    }
    return w;
}

std::string cf(const std::string& vi, const std::vector<std::string>& bi) {
    std::string tmp;
    std::string ret = "00000000";
    std::vector<int> w = extend(bi);
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
    tmp = toHex(a ^ toDec(vi), 8);
    for (int i = 0; i < 8; i++) {
        ret[i] = tmp[i] ^ vi[i];
    }
    return ret;
}

std::string iteration(const std::vector<std::string>& b) {
    std::string v = "7380166f";
    for (const std::string& bi : b) {
        v = cf(v, b);
    }
    return v;
}

std::string SM3(const std::string& m) {
    std::string filled = fill(m);
    std::vector<std::string> b = grouping(filled);
    return iteration(b);
}

std::string randstr(int len) {
    std::string ret;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);
    for (int i = 0; i < len; i++) {
        ret += std::to_string(dis(gen));
    }
    return ret;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    while (true) {
        std::string str1 = randstr(16);
        std::string str2 = randstr(16);
        std::string h1 = SM3(str1);
        std::string h2 = SM3(str2);
        if (h1 == h2) {
            std::cout << "success" << std::endl;
            std::cout << str1 << std::endl;
            std::cout << str2 << std::endl;
            break;
        }
    }
    return 0;
}
