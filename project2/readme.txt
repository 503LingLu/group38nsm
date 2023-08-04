代码说明：
toHex(int x, int length): 将整数转换为指定长度的十六进制字符串。
toDec(std::string s): 将十六进制字符串转换为整数。
t(int j): 根据输入参数返回常数值。
csl(int x, int k): 循环左移操作，将输入整数x的二进制表示左移k位。
ff(int x, int y, int z, int j): 哈希函数中的逻辑运算。
gg(int x, int y, int z, int j): 哈希函数中的逻辑运算。
p0(int x): 哈希函数中的置换函数。
p1(int x): 哈希函数中的置换函数。
fill(std::string m): 对消息进行填充，使其长度满足SM3算法的要求。
grouping(std::string m): 将填充后的消息分组，每个分组为32位。
extend(std::vector<std::string> bi): 扩展消息分组，生成132个扩展字。
cf(std::string vi, std::string bi): 压缩函数，对一个分组进行压缩并更新中间状态。
iteration(std::vector<std::string> b): 迭代压缩函数，对所有分组进行迭代压缩。
SM3(std::string m): SM3哈希算法的入口函数，对输入消息进行哈希计算。
randStr(int length): 生成指定长度的随机字符串。
在main函数中，首先使用randStr函数生成一个16位的随机字符串str1，然后使用SM3函数计算其哈希值h1。然后根据每个字符的ASCII码值对哈希值进行特定的处理，生成新的字符串str2。然后再次使用SM3函数计算str2的哈希值h2。
如果h1和h2相等，则输出"success"以及生成的两个字符串str1和str2。
