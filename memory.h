#ifndef MEMORY_H
#define MEMORY_H

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount)      \
    (type *)reallocate(pointer, sizeof(type) * (oldCount), \
                       sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void *reallocate(void *pointer, int old_size, int new_size);

#endif