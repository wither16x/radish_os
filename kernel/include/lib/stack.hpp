#pragma once

#include <kernel.hpp>
#include <lib/typing.hpp>
#include <lib/vector.hpp>
#include <lib/memory.hpp>
#include <mem/page.hpp>
#include <mem/pmm.hpp>
#include <mem/pml4t.hpp>

namespace Kiwi::Lib
{
        template<typename T>
        class Stack {
                uptr bottom;
                uptr top;
                T *pointer;
                Mem::PML4T *pml4t;

        public:
                Stack(Mem::PML4T &pml4t, uptr bottom, uptr top)
                        : pml4t(&pml4t)
                {
                        this->bottom = bottom;
                        this->top = top;
                        this->pointer = reinterpret_cast<T *>(this->top);

                        for (uptr vaddr = this->bottom; vaddr < this->top; vaddr += Mem::PAGE_SIZE) {
                                uptr frame = Mem::Pmm::allocateFrame();
                                this->pml4t->mapPage(vaddr,
                                        frame,
                                        Mem::PageFlag::ReadWriteUser | Mem::PageFlag::NoExec
                                );
                        }
                }

                Stack(const Stack<T> &other, Mem::PML4T &pml4t)
                        : pml4t(&pml4t)
                {
                        this->bottom    = other.bottom;
                        this->top       = other.top;
                        this->pointer   = other.pointer;
                }

                Stack(const Stack<T> &) = delete;

                ~Stack()
                {
                        // TODO
                }

                void push(this Stack<T> &self, T &value)
                {
                        if (not self.pointer)
                                return;

                        *(--self.pointer) = value;
                }

                uptr *push_string_array(this Stack<T> &self, char **s, int count)
                {
                        if (not s)
                                return nullptr;

                        uptr *s_uaddrs = count > 0 ? new uptr[count] : nullptr;

                        for (int i = count - 1; i >= 0; i--) {
                                usize len = strlen(s[i]) + 1;
                                self.grow(len);

                                for (usize j = 0; j < len; j++) {
                                        uptr uaddr = reinterpret_cast<uptr>(self.pointer) + j;
                                        uptr kaddr = self.pml4t->virtToPhys(uaddr) + getKernelHhdmOffset();
                                        *reinterpret_cast<char *>(kaddr) = s[i][j];
                                }

                                s_uaddrs[i] = reinterpret_cast<uptr>(self.pointer);
                        }

                        return s_uaddrs;
                }

                void push_qword(this Stack<T> &self, u64 value)
                {
                        self.grow(sizeof(value));
                        uptr uaddr = reinterpret_cast<uptr>(self.pointer);
                        uptr kaddr = self.pml4t->virtToPhys(uaddr) + getKernelHhdmOffset();
                        *reinterpret_cast<u64 *>(kaddr) = value;
                }

                void grow(this Stack<T> &self, usize n)
                {
                        if (not self.pointer)
                                return;

                        for (usize i = 0; i < n; i++)
                                --self.pointer;
                }

                T pop(this Stack<T> &self)
                {
                        if (not self.pointer)
                                return T(0);

                        return *(self.pointer++);
                }

                T shrink(this Stack<T> &self, usize n)
                {
                        T last;
                        for (usize i = 0; i < n; i++)
                                last = self.pop();
                        return last;
                }

                void align(this Stack<T> &self, usize n)
                {
                        if (not self.pointer)
                                return;

                        self.pointer = reinterpret_cast<T *>(
                                reinterpret_cast<uptr>(self.pointer) & ~static_cast<uptr>(n - 1)
                        );
                }

                void reset(this Stack<T> &self, Mem::PML4T &pml4t, uptr bottom, uptr top)
                {
                        self.pml4t = &pml4t;
                        self.bottom = bottom;
                        self.top = top;
                        self.pointer = reinterpret_cast<T *>(self.top);

                        for (uptr vaddr = self.bottom; vaddr < self.top; vaddr += Mem::PAGE_SIZE) {
                                uptr frame = Mem::Pmm::allocateFrame();
                                self.pml4t->mapPage(vaddr, frame, Mem::PageFlag::ReadWriteUser | Mem::PageFlag::NoExec);
                        }
                }

                uptr get_bottom(this const Stack<T> &self)
                {
                        return self.bottom;
                }

                uptr get_top(this const Stack<T> &self)
                {
                        return self.top;
                }

                uptr virt_to_phys(this const Stack<T> &self, uptr vaddr)
                {
                        return self.pml4t->virtToPhys(vaddr);
                }

                const T *get(this const Stack<T> &self)
                {
                        return self.pointer;
                }

                T *get_raw(this const Stack<T> &self)
                {
                        return self.pointer;
                }

                Stack<T> &operator =(const Stack<T> &) = delete;
        };
} // namespace Kiwi::Lib