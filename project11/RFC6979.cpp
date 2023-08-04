#include <iostream>
#include <cstring>
#include <cstdio>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/rand.h>

using namespace std;

EC_KEY* generate_keypair() {
    EC_KEY* keypair = EC_KEY_new_by_curve_name(NID_sm2);
    if (!keypair) {
        cout << "�޷����� EC ��Կ��" << endl;
        return nullptr;
    }

    if (EC_KEY_generate_key(keypair) != 1) {
        cout << "�޷����� EC ��Կ��" << endl;
        EC_KEY_free(keypair);
        return nullptr;
    }

    return keypair;
}

bool sm2_sign(const unsigned char* message, size_t length, const BIGNUM* private_key, const EC_KEY* keypair, unsigned char* signature) {
    ECDSA_SIG* sig = nullptr;
    bool result = false;

    do {
        if (private_key == nullptr || keypair == nullptr || signature == nullptr) {
            break;
        }

        sig = ECDSA_do_sign(message, length, const_cast<EC_KEY*>(keypair));
        if (!sig) {
            cout << "SM2 ǩ��ʧ��" << endl;
            break;
        }

        const BIGNUM* r = nullptr;
        const BIGNUM* s = nullptr;
        ECDSA_SIG_get0(sig, &r, &s);

        // �� r��s ת��Ϊ�ֽ�������ʽ
        unsigned char* r_bytes = new unsigned char[32];
        unsigned char* s_bytes = new unsigned char[32];
        BN_bn2binpad(r, r_bytes, 32);
        BN_bn2binpad(s, s_bytes, 32);

        // ƴ�� r��s������ǩ��
        memcpy(signature, r_bytes, 32);
        memcpy(signature + 32, s_bytes, 32);

        result = true;
    } while (false);

    if (sig) {
        ECDSA_SIG_free(sig);
    }

    return result;
}

int main() {
    // ������Կ��
    EC_KEY* keypair = generate_keypair();
    if (!keypair) {
        return 1;
    }

    // ����˽Կ
    const BIGNUM* private_key = EC_KEY_get0_private_key(keypair);

    // ��ǩ������Ϣ
    unsigned char message[] = "Hello, SM2 with RFC6979";
    size_t length = strlen(reinterpret_cast<char*>(message));

    // ǩ�����
    unsigned char signature[64];

    // ʹ�� RFC6979 �㷨����ǩ��
    if (!sm2_sign(message, length, private_key, keypair, signature)) {
        cout << "SM2 ǩ��ʧ��" << endl;
        return 1;
    }

    // ���ǩ�����
    printf("ǩ�������");
    for (int i = 0; i < 64; i++) {
        printf("%02X", signature[i]);
    }
    printf("\n");

    // �ͷ���Դ
    EC_KEY_free(keypair);

    return 0;
}
