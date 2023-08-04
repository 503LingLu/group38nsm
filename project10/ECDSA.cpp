#include <iostream>
#include <secp256k1.h>
#include <secp256k1_ecdsa.h>
#include <secp256k1_recovery.h>
#include <sodium.h>

std::string sha3_256Hash(std::string msg) {
    std::string hash;
    unsigned char hashBytes[crypto_hash_sha3_256_BYTES];
    crypto_hash_sha3_256(hashBytes, (const unsigned char*)msg.c_str(), msg.size());

    for (int i = 0; i < crypto_hash_sha3_256_BYTES; ++i) {
        hash += hashBytes[i];
    }

    return hash;
}

secp256k1_context* create_secp256k1_context() {
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    return ctx;
}

void destroy_secp256k1_context(secp256k1_context* ctx) {
    secp256k1_context_destroy(ctx);
}

void hexStringToBytes(const std::string& hexString, unsigned char* bytes) {
    for (int i = 0, j = 0; i < hexString.size(); i += 2, ++j) {
        int c1 = (hexString[i] >= 'A' ? (hexString[i] & 0x7) + 9 : (hexString[i] - '0')) << 4;
        int c2 = hexString[i + 1] >= 'A' ? (hexString[i + 1] & 0x7) + 9 : (hexString[i + 1] - '0');
        bytes[j] = c1 + c2;
    }
}

void bytesToHexString(const unsigned char* bytes, int size, std::string& hexString) {
    static const char* hexDigits = "0123456789abcdef";
    hexString.resize(size * 2);

    for (int i = 0; i < size; ++i) {
        hexString[i * 2] = hexDigits[bytes[i] >> 4];
        hexString[i * 2 + 1] = hexDigits[bytes[i] & 0xf];
    }
}

void signECDSAsecp256k1(const std::string& msg, const std::string& privKeyHex, std::string& signatureR, std::string& signatureS) {
    secp256k1_context* ctx = create_secp256k1_context();

    unsigned char privKeyBytes[32];
    hexStringToBytes(privKeyHex, privKeyBytes);

    std::string hash = sha3_256Hash(msg);
    const unsigned char* hashBytes = (const unsigned char*)hash.c_str();

    secp256k1_ecdsa_signature sig;
    int result = secp256k1_ecdsa_sign(ctx, &sig, hashBytes, privKeyBytes, nullptr, nullptr);

    if (result == 1) {
        unsigned char signatureBytes[64];
        size_t signatureSize = sizeof(signatureBytes);
        secp256k1_ecdsa_signature_serialize_compact(ctx, signatureBytes, &sig);

        bytesToHexString(signatureBytes, signatureSize, signatureR);
        bytesToHexString(signatureBytes + 32, signatureSize, signatureS);
    }

    destroy_secp256k1_context(ctx);
}

bool verifyECDSAsecp256k1(const std::string& msg, const std::string& signatureR, const std::string& signatureS, const std::string& pubKeyHex) {
    secp256k1_context* ctx = create_secp256k1_context();

    std::string hash = sha3_256Hash(msg);
    const unsigned char* hashBytes = (const unsigned char*)hash.c_str();

    unsigned char pubKeyBytes[64];
    hexStringToBytes(pubKeyHex, pubKeyBytes);

    secp256k1_pubkey pubkey;
    int result = secp256k1_ec_pubkey_parse(ctx, &pubkey, pubKeyBytes, sizeof(pubKeyBytes));

    if (result == 1) {
        secp256k1_ecdsa_signature sig;
        unsigned char signatureBytes[64];
        size_t signatureSize = sizeof(signatureBytes);

        hexStringToBytes(signatureR, signatureBytes);
        hexStringToBytes(signatureS, signatureBytes + 32);

        secp256k1_ecdsa_signature_parse_compact(ctx, &sig, signatureBytes);
        result = secp256k1_ecdsa_verify(ctx, &sig, hashBytes, &pubkey);

        destroy_secp256k1_context(ctx);
        return result == 1;
    }

    destroy_secp256k1_context(ctx);
    return false;
}

int main() {
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium" << std::endl;
        return 1;
    }

    std::string msg = "Message for ECDSA signing";
    std::string privKeyHex = "8a33f2e4bf75e5d47348199d871f398762c489bdc2667269646f5eabf9f456c6";
    std::string signatureR, signatureS;

    signECDSAsecp256k1(msg, privKeyHex, signatureR, signatureS);

    std::cout << "Message: " << msg << std::endl;
    std::cout << "Private Key: " << privKeyHex << std::endl;
    std::cout << "Signature: r=" << signatureR << ", s=" << signatureS << std::endl;

    std::string pubKeyHex = "027b1d2e8cf4e32f12731223984d0da0b8b3494bcf8140273b4730e8ea26df689e";
    bool valid = verifyECDSAsecp256k1(msg, signatureR, signatureS, pubKeyHex);

    std::cout << "\nMessage: " << msg << std::endl;
    std::cout << "Public Key: " << pubKeyHex << std::endl;
    std::cout << "Is Signature Valid? " << (valid ? "Yes" : "No") << std::endl;

    std::string tamperedMsg = "Tampered message";
    valid = verifyECDSAsecp256k1(tamperedMsg, signatureR, signatureS, pubKeyHex);

    std::cout << "\nMessage: " << tamperedMsg << std::endl;
    std::cout << "Is Signature Valid? (Tampered) " << (valid ? "Yes" : "No") << std::endl;

    return 0;
}
