#include <panic.hpp>

using namespace kernel;

extern "C" {

int __cxa_atexit(void (*)(void *), void *, void *)
{
        return 0;
}

void __cxa_pure_virtual()
{
        panic("call to pure virtual method");
}

// not sure about this
int atexit()
{
        return 0;
}

void *__dso_handle;

}