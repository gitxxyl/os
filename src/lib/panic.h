#pragma once 

extern void __panic(const char *msg, const char *file, int line);
#define panic(msg) __panic((msg), __FILE__, __LINE__)
