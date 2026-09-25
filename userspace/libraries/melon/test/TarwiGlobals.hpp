#if defined(TARWI_OUTPUT)
        #undef TARWI_OUTPUT
        #include <stdio.h>
        #define TARWI_OUTPUT(...) \
                do { \
                        printf(__VA_ARGS__); \
                        printf("\n"); \
                } while (0)
#endif