#include <cpu/tss.hpp>
#include <lib/typing.hpp>

namespace Kiwi::Cpu
{
        namespace
        {
                extern "C" void __tss_flush();
        } // anonymous namespace

        void Tss::init(this Tss &self, Lib::uptr kernel_rsp)
        {
                self.data.rsp0 = kernel_rsp;
        }

        void Tss::flush() const
        {
                __tss_flush();
        }

        void Tss::resetStack(this Tss &self, Lib::uptr kernel_rsp)
        {
                self.data.rsp0 = kernel_rsp;
        }

        const TssData &Tss::getData(this const Tss &self)
        {
                return self.data;
        }
} // namespace Kiwi::Cpu