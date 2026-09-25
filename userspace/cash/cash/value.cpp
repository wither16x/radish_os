#include <cash/value.hpp>

#include <melon/conversion.hpp>
#include <variant>

using namespace Melon;

namespace Cash
{
        String::String EvalValue::toString(this const EvalValue &self)
        {
                return std::visit(Overloaded{
                        [](std::monostate) {return String::String("null");},
                        [](const String::String &s) {return s;},
                        [](int i) {return Conversion::intToString(i, Conversion::Base::Decimal);},
                        [](bool b) {return b ? String::String("true") : String::String("false");},
                        [](const Vector::Vector<EvalValue> &v) {
                                String::String str = "[";
                                for (Typing::USize i = 0; i < v.length(); ++i) {
                                        str += v[i].toString();
                                        if (i + 1 < v.length())
                                                str += ", ";
                                }
                                str.appendChar(']');

                                return str;
                        }
                }, self.value);
        }

        int EvalValue::toInt(this const EvalValue &self)
        {
                return std::visit(Overloaded{
                        [](std::monostate) {return 0;},
                        [](const String::String &s) {return Conversion::stringToInt<int>(s);},
                        [](int i) {return i;},
                        [](bool b) {return b ? 1 : 0;},
                        [](const Vector::Vector<EvalValue> &) {return 0;}
                }, self.value);
        }

        Vector::Vector<EvalValue> EvalValue::toVector(this const EvalValue &self)
        {
                return std::visit(Overloaded{
                        [](std::monostate) { return Vector::Vector<EvalValue>{}; },
                        [](const String::String &) { return Vector::Vector<EvalValue>{}; },
                        [](int) { return Vector::Vector<EvalValue>{}; },
                        [](bool) { return Vector::Vector<EvalValue>{}; },
                        [](const Vector::Vector<EvalValue> &v) { return v; }
                }, self.value);
        }
} // namespace Cash