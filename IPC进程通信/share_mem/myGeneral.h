#pragma once

#define getnbit(bits, begin, n) (bits >> (begin - n + 1) & ((1 << (n + 1)) - 1))
#define BCD_TO_BIN(val) ((val) = ((val)&15) + ((val) >> 4) * 10)

//处理错误信息, 致命错误, 程序直接退出
void handle_error(const char* msg);
//显示信息, 非致命错误, 被调程序返回错误码
void show_msg(const char* msg);

