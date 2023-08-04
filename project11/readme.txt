首先定义一个用于生成密钥对的函数 generate_keypair()，它通过EC_KEY_new_by_curve_name 函数创建一个基于 SM2 曲线的 EC 密钥对。然后使用 EC_KEY_generate_key 函数生成 EC 密钥对。

然后定义一个用于使用 RFC6979 算法进行签名的函数 sm2_sign()。接受消息、私钥、密钥对和签名缓冲区作为参数。函数内部调用 ECDSA_do_sign 函数生成 ECDSA_SIG 结构体表示的签名。然后将 r 和 s 值转换为字节数组形式，并将它们拼接到签名缓冲区中。

在 main() 函数中，首先调用 generate_keypair() 生成 SM2 密钥对。然后获取私钥。接下来，定义了待签名的消息，并计算其长度。

创建一个用于保存签名结果的缓冲区 signature。

调用 sm2_sign() 函数进行签名。如果签名成功，将打印签名结果。

最后，释放密钥对的资源并返回。