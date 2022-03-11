#ifndef ASSERT_H
#define ASSERT_H

extern void __assert(const char *msg, const char *file, int line);
#define assert(EX) (void) ((EX) || (__assert (#EX, __FILE__, __LINE__),0))

#endif // ASSERT_H