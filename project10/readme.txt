代码说明：
sha3_256Hash(msg)：用于计算输入消息的SHA3-256哈希值。
create_secp256k1_context()：创建secp256k1上下文。
destroy_secp256k1_context(ctx)：销毁secp256k1上下文。
hexStringToBytes(hexString, bytes)：将十六进制字符串转换为字节数组。
bytesToHexString(bytes, size, hexString)：将字节数组转换为十六进制字符串。
signECDSAsecp256k1(msg, privKeyHex, signatureR, signatureS)：使用私钥对消息进行ECDSA签名，并将签名结果保存在signatureR和signatureS中。
verifyECDSAsecp256k1(msg, signatureR, signatureS, pubKeyHex)：使用公钥验证ECDSA签名的有效性。
在main函数中：
使用sodium_init()初始化libsodium库。
定义要签名的消息msg和私钥privKeyHex。
调用signECDSAsecp256k1函数对消息进行签名，并将签名结果保存在signatureR和signatureS中。
输出签名结果。
定义要验证的公钥pubKeyHex。
调用verifyECDSAsecp256k1函数验证签名的有效性，并输出验证结果。
修改消息为"Tampered message"，再次调用verifyECDSAsecp256k1函数验证签名的有效性，并输出验证结果。