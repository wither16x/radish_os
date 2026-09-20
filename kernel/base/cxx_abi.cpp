#include <panic_simple.hpp>

using namespace Kiwi;

// The functions and variables below are required by the
// Itanium C++ ABI.

extern "C"
{
        int __cxa_atexit(void (*)(void *), void *, void *)
        {
                return 0;
        }

        void __cxa_pure_virtual()
        {
                panic_simple("call to pure virtual method");
        }

        int atexit()
        {
                return 0;
        }

        void *__dso_handle;
} // extern "C"