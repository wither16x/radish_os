#include <cpu/assembly.hpp>

using namespace kernel;

extern "C" {

int __cxa_atexit(void (*)(void *), void *, void *)
{
        return 0;
}

void __cxa_pure_virtual()
{
        while (true)
                cpu::hlt();
}

void *__dso_handle;

}