#ifndef MACROS_H
#define MACROS_H

#define RETURN_IF_NULL(item, failure) do { \
    if ((item) == NULL) { \
        return (failure); \
    } \
} while (0)

#define RETURN_IF_EMPTY(item, failure) do { \
    if (!(item)) { \
        return (failure); \
    } \
} while (0)

#define CONDITIONAL_RETURN(item, value, failure) do { \
    if ((item) == (value)) { \
        return (failure); \
    } \
} while (0)

#endif