#include <stdio.h>
#include <stdlib.h>

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
	//192.168.1.1
	//3232235777	unsigned int   0 ~ 4294967295  
	//C0 A8 01 01
	//11000000 10101000 00000001 00000001
	unsigned int a = 3232235777;
	int ret = getnbit(a, 31, 8); //11000000
	printf("a=0x%x\n", a);
	printf("Get 8 bits of variable a starting at position 31 bit: 0x%x\n", ret);


	return 0;
}