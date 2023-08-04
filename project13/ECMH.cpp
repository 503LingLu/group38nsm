#include <iostream>
#include <string>
#include <vector>
#include <openssl/sm2.h>
#include <openssl/sha.h>
#include <openssl/obj_mac.h>

using namespace std;

// 椭圆曲线官方推荐参数
const char* p = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
const char* a = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
const char* b = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
const char* n = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
const char* g = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7"
"BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";

// ECMH函数,返回msghash过后椭圆曲线点横纵坐标拼接:x||y
string ECMH(const string& msg) {
    unsigned char hash_msg[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(msg.c_str()), msg.length(), hash_msg);

    BIGNUM* pri = BN_new();
    BN_bin2bn(hash_msg, SHA256_DIGEST_LENGTH, pri);

    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_sm2);
    EC_POINT* pub = EC_POINT_new(group);
    if (EC_POINT_mul(group, pub, pri, NULL, NULL, NULL) != 1) {
        cout << "Error: Failed to perform ECMH." << endl;
        exit(1);
    }

    BN_CTX* ctx = BN_CTX_new();
    char* x_hex = BN_bn2hex(pub->X);
    char* y_hex = BN_bn2hex(pub->Y);
    string result(x_hex);
    result += y_hex;

    BN_CTX_free(ctx);
    EC_POINT_free(pub);
    EC_GROUP_free(group);
    BN_free(pri);

    return result;
}

// 哈希过后添加新的元素(另一个输入)并继续哈希
string ECMH_append(const string& hash, const string& msg) {
    unsigned char hash_msg[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(msg.c_str()), msg.length(), hash_msg);

    BIGNUM* pri = BN_new();
    BN_bin2bn(hash_msg, SHA256_DIGEST_LENGTH, pri);

    EC_GROUP* group = EC_GROUP_new_by_curve_name(NID_sm2);
    EC_POINT* pub = EC_POINT_new(group);
    if (EC_POINT_mul(group, pub, pri, NULL, NULL, NULL) != 1) {
        cout << "Error: Failed to perform ECMH_append." << endl;
        exit(1);
    }

    EC_POINT* result = EC_POINT_new(group);
    if (EC_POINT_add(group, result, pub, hash, NULL) != 1) {
        cout << "Error: Failed to perform ECMH_append." << endl;
        exit(1);
    }

    BN_CTX* ctx = BN_CTX_new();
    char* x_hex = BN_bn2hex(result->X);
    char* y_hex = BN_bn2hex(result->Y);
    string final_result(x_hex);
    final_result += y_hex;

    BN_CTX_free(ctx);
    EC_POINT_free(result);
    EC_POINT_free(pub);
    EC_GROUP_free(group);
    BN_free(pri);

    return final_result;
}

int main() {
    string cip1, cip2;
    cout << "字符串1：" << endl;
    getline(cin, cip1);
    cout << "字符串2：" << endl;
    getline(cin, cip2);

    string cip_hash1 = ECMH(cip1);
    string cip_hash2 = ECMH(cip2);
    string cip_app_hash1_cip2 = ECMH_append(cip_hash1, cip2);
    string cip_app_hash2_cip1 = ECMH_append(cip_hash2, cip1);

    cout << "字符串1 hash 值：" << cip_hash1 << endl;
    cout << "字符串2 hash 值：" << cip_hash2 << endl;
    cout << "先 hash 第一个字符串再添加第二个：" << cip_app_hash1_cip2 << endl;
    cout << "先 hash 第二个字符串再添加第一个：" << cip_app_hash2_cip1 << endl;

    return 0;
}
