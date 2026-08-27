#pragma once

#if defined(TARWI_OUTPUT)
        #undef TARWI_OUTPUT
        #include <lib/print.hpp>
        #define TARWI_OUTPUT(...) \
                do { \
                        Kiwi::Lib::printf("[tarwi] " __VA_ARGS__); \
                } while (0)
#endif