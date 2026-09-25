#include <conversion.hpp>

namespace Melon::Conversion
{
        namespace
        {
                const char *baseStrings[] = {
                        "Binary",
                        "Octal",
                        "Decimal",
                        "Hexadecimal"
                };
        } // anonymous namespace

        const char *baseToString(Base base)
        {
                switch (base) {
                case Base::Binary:
                        return baseStrings[0];
                        break;

                case Base::Octal:
                        return baseStrings[1];
                        break;

                case Base::Decimal:
                        return baseStrings[2];
                        break;

                case Base::Hexadecimal:
                        return baseStrings[3];
                        break;
                }
        }
} // namespace Melon::Conversion