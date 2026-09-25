#pragma once

#include <melon/string.hpp>

#include <variant>

namespace Cash
{
        class EvalValue
        {
                template<class... Ts>
                struct Overloaded : Ts...
                {
                        using Ts::operator()...;
                };

        public:
                std::variant<
                        std::monostate,
                        Melon::String::String,
                        int,
                        bool,
                        Melon::Vector::Vector<EvalValue>
                > value;
        
                Melon::String::String toString(this const EvalValue &self);
                int toInt(this const EvalValue &self);
                Melon::Vector::Vector<EvalValue> toVector(this const EvalValue &self);

                bool operator ==(const EvalValue &) const = default;
        };

        inline constexpr EvalValue null_value = {std::monostate()};

        using eval_values_t = Melon::Vector::Vector<EvalValue>;
} // namespace Cash