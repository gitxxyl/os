#ifndef STIVALE_TAG_H
#define STIVALE_TAG_H
#include <stdint.h>
#include <stddef.h>
#include <thirdparty/stivale2.h>

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id);

#endif //STIVALE_TAG_H