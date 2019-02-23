#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>
using namespace std;
// 设x�?�?二进�?4�?16进制数变量，lx为低两位数变量，mx为中两位数变量，hx高两位数变量，则�?
// hx=x>>16&0xFF; �? 23-16 23,8
// mx=x>>8&0xFF;  �? 15-8  15,8
// lx=x>>0&0xFF;  �? 7-0   7,8
//#define getnbit(bits, begin, n)  bits>>(begin-n+1) & (1 << (n + 1)) - 1)
#define getnbit(bits, begin, n) (bits>>(begin-n+1) & ((1 << (n + 1)) - 1))
// n -> 111l
// 4 -> 1111
// 1 << n

int main(int argc, char** argv)
{
	
	list<int> ticketLnk;
	ticketLnk.push_back()


	return 0;
}