#include <iostream>
#include <sstream>
#include <random>
#include "SM2.h"
#include "SM4.h"

std::string str36 = "shandonguniversity202100150110nimeng";
unsigned char iv[16] = {0};

std::string private_key = "010203040506070809000A0B0C0D0E0F010203040506070809000A0B0C0D0E0F01";
std::string public_key = "04E9C91B81872260BEF331A83081A693747D7EA88042927317EB06B9C9A6EF5DDEB0BB2FF6CF5AF347B483F7B9487C018FB4162B8993F3F5D6EADDDE24620207";

// Generate a random k
std::string generate_random_k() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 35);

    std::stringstream k;
    for (int i = 0; i < 16; ++i) {
        k << str36[dis(gen)];
    }
    return k.str();
}

// Sender: encrypts the message and key
std::pair<std::string, std::string> sender(const std::string& msg) {
    // Generate a random k
    std::string K = generate_random_k();

    // SM4 encryption
    SM4 sm4_enc;
    sm4_enc.SetKey((const unsigned char*)K.c_str(), SM4_ENCRYPT);

    // Encrypt the message
    std::string enc_msg = sm4_enc.CryptECB((const unsigned char*)msg.c_str(), msg.length());

    // SM2 encryption, private key is not required in this process
    SM2 sm2_enc;
    sm2_enc.SetPublicKey(public_key);

    // Encrypt the key
    std::string enc_key = sm2_enc.Encrypt(K);

    return std::make_pair(enc_msg, enc_key);
}

// Receiver: decrypts the message and key
std::string receiver(const std::string& enc_msg, const std::string& enc_key) {
    // Configure SM2 encryption
    SM2 sm2_enc;
    sm2_enc.SetPrivateKey(private_key);
    sm2_enc.SetPublicKey(public_key);

    // Decrypt the key
    std::string K = sm2_enc.Decrypt(enc_key);

    // Configure SM4 decryption
    SM4 crypt_sm4;
    crypt_sm4.SetKey((const unsigned char*)K.c_str(), SM4_DECRYPT);

    // Decrypt the message
    std::string decrypt_m = crypt_sm4.CryptECB((const unsigned char*)enc_msg.c_str(), enc_msg.length());

    return decrypt_m;
}

int main() {
    // Input the data to be encrypted
    std::string msg;
    std::cout << "Enter the data to be encrypted:\n";
    std::getline(std::cin, msg);

    std::clock_t start = std::clock();
    auto [enc_msg, enc_key] = sender(msg);
    std::string dec_m = receiver(enc_msg, enc_key);
    std::clock_t end = std::clock();

    std::cout << "Encrypted message: " << enc_msg << std::endl;
    std::cout << "Encrypted session key:\n" << enc_key << std::endl;
    if (dec_m == msg) {
        std::cout << "Success!" << std::endl;
    }
    std::cout << (end - start) / (double)CLOCKS_PER_SEC << " seconds" << std::endl;

    return 0;
}
