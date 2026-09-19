#pragma once

#if defined(TARWI_OUTPUT)
        #undef TARWI_OUTPUT
        #include <lib/print.hpp>
        #define TARWI_OUTPUT(...) \
                do { \
                        Kiwi::Lib::println("[tarwi] " __VA_ARGS__); \
                } while (0)
#endif

#if defined(TARWI_FMT_STRING)
        #undef TARWI_FMT_STRING
        #define TARWI_FMT_STRING "{}" 
#endif

#if defined(TARWI_FMT_INTEGER)
        #undef TARWI_FMT_INTEGER
        #define TARWI_FMT_INTEGER "{}"
#endif