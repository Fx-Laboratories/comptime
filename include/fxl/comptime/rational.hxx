#pragma once

#ifndef FXL_CTLIB_RATIONAL
#    define FXL_CTLIB_RATIONAL

#    include <type_traits>

#    include <concepts>
#    include <ratio>

namespace fxl::comptime {

    namespace detail {

        template <typename type_tp>
        struct is_ratio : std::false_type { };

        template <std::intmax_t num, std::intmax_t den>
        struct is_ratio<std::ratio<num, den>> : std::true_type { };

        struct rational_base { };

    } // namespace detail

    namespace concepts {

        template <typename type_tp>
        concept standard_ratio_type = detail::is_ratio<type_tp>::value;

        template <typename type_tp>
        concept rational_type
            = std::is_base_of_v<detail::rational_base, type_tp>;

    } // namespace concepts

    template <std::intmax_t num, std::intmax_t den>
    struct rational;

    template <concepts::standard_ratio_type ratio_tp>
    using extend_ratio_to_rational = rational<ratio_tp::num, ratio_tp::den>;

    template <std::intmax_t num, std::intmax_t den = 1>
    struct rational : detail::rational_base, std::ratio<num, den> {
        template <concepts::rational_type other>
        using plus = extend_ratio_to_rational<
            std::ratio_add<typename rational::type, typename other::type>>;

        template <concepts::rational_type other>
        using minus = extend_ratio_to_rational<
            std::ratio_subtract<typename rational::type, typename other::type>>;

        template <concepts::rational_type other>
        using times = extend_ratio_to_rational<
            std::ratio_multiply<typename rational::type, typename other::type>>;

        template <concepts::rational_type other>
        using over = extend_ratio_to_rational<
            std::ratio_divide<typename rational::type, typename other::type>>;

        using reciprocal = extend_ratio_to_rational<std::conditional_t<
            rational::type::num == 0,
            std::ratio<0>,
            std::ratio<rational::type::den, rational::type::num>>>;

        template <concepts::rational_type other>
        static constexpr bool equals
            = std::ratio_equal_v<typename rational::type, typename other::type>;

        template <concepts::rational_type other>
        static constexpr bool greater_than
            = std::ratio_greater_v<typename rational::type,
                                   typename other::type>;

        template <concepts::rational_type other>
        static constexpr bool greater_than_or_equal
            = std::ratio_greater_equal_v<typename rational::type,
                                         typename other::type>;

        template <concepts::rational_type other>
        static constexpr bool less_than
            = std::ratio_less_v<typename rational::type, typename other::type>;

        template <concepts::rational_type other>
        static constexpr bool less_than_or_equal
            = std::ratio_less_equal_v<typename rational::type,
                                      typename other::type>;

        template <std::floating_point output_type>
        static constexpr auto
        convert_to() -> output_type {
            return output_type { static_cast<output_type>(rational::type::num)
                                 / static_cast<output_type>(
                                     rational::type::den) };
        }
    };

} // namespace fxl::comptime

#endif
