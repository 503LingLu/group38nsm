show_points(p, a, b)函数：用于生成椭圆曲线上的所有点。

GCD(a, b)函数和XGCD(a, b)函数：用于计算最大公约数和扩展欧几里得算法。

get_inverse(a, b)函数：用于计算模b下a的逆元。

pow_mod(a, b, n)函数：用于计算a的b次方模n的结果。

get_miller(m)函数：用于计算m的最小奇数因子。

rabin_miller(p)函数：用于进行Rabin-Miller素性测试，判断p是否为素数。

get_gcd(x, y)函数：用于计算x和y的最大公约数。

get_Qr(n, p)函数：用于生成模p下的平方剩余和非平方剩余。

calculate_p_q(x1, y1, x2, y2, a, b, p)函数：用于计算两点的相加（即求p+q）。

calculate_2p(p_x, p_y, a, b, p)函数：用于计算两倍点（即求2p）。

calculate_np(p_x, p_y, n, a, b, p)函数：用于计算n倍点（即求np）。

calculate_Tp(p_x, p_y, a, b, p)函数：用于计算点的阶反元（即求-p）。

get_key()函数：得到一个随机的私钥dA，并计算对应的公钥PA。

get_bitsize(num)函数：用于计算一个整数的位数。

int_to_bytes(num)和bytes_to_int(bytes)函数：用于将整数转换为字节流和将字节流转换为整数。

sign(msg, ID)函数：进行签名操作，包括生成随机数k，计算r和s的值。

ALice_sign(msg, IDA)和Bob_sign(msg, IDB)函数：分别为Alice和Bob生成签名，并打印出公钥。

get_sK_wen_Alice_and_Bob_has_the_one_k(msg,IDA,IDB)函数：模拟Alice和Bob使用相同的随机数k进行签名，然后通过计算获得他们共享的密钥。