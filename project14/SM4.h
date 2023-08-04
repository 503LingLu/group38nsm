#pragma once
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

unsigned HexToDec(char c);

string NLTransform(string str);

string LTransform(string str);

string L2Transform(string str);

string T(string str);

string T2(string str);

string KeyExtension(string MK);

string SM4_enc(string pln, string key);

string SM4_dec(string cip, string key); #pragma once
