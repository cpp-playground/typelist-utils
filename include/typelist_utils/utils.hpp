#pragma once

#include <tuple>

#include "traits.hpp"

namespace tl
{
/**
 * @brief Checks if a tuple-like type starts with a given type.
 *
 * @tparam T The tuple-like type.
 * @tparam E The type to check for.
 */
template <tl::concepts::tuple T, typename E>
struct start_with
{
    static constexpr bool value = false;
};

template <typename T, typename... Ts, typename E>
struct start_with<std::tuple<T, Ts...>, E>
{
    static constexpr bool value = std::is_same_v<T, E>;
};

template <tl::concepts::tuple T, typename E>
constexpr auto start_with_v = start_with<T, E>::value;

/**
 * @brief Counts the number of times a type appears in a tuple-like type.
 *
 * @tparam T The tuple-like type.
 * @tparam E The type to count.
 */
template <tl::concepts::tuple T, typename E>
class count
{
    template <std::size_t... indexes>
    static constexpr auto impl(std::index_sequence<indexes...>)
    {
        return ((std::is_same_v<E, std::tuple_element_t<indexes, T>> ? 1 : 0) + ... + 0);
    }

  public:
    static constexpr auto value =
        impl(std::make_integer_sequence<std::size_t, std::tuple_size_v<T>>{});
};

template <tl::concepts::tuple T, typename E>
constexpr auto count_v = count<T, E>::value;

template <tl::concepts::tuple T, typename E>
constexpr auto contains_v = 0 < count_v<T, E>;

/**
 * @brief Checks if a tuple-like type contains all of the given types.
 * @tparam T The tuple-like type.
 * @tparam Es The types to check for.
 */
template <tl::concepts::tuple T, typename... Es>
struct contains_all
{
    static constexpr auto value =
        (static_cast<std::size_t>(contains_v<T, Es>) + ... + 0) == sizeof...(Es);
};

template <tl::concepts::tuple T, typename... Es>
constexpr auto contains_all_v = contains_all<T, Es...>::value;

/**
 * @brief Concatenates two tuple-like types.
 * @tparam T The first tuple-like type.
 * @tparam U The second tuple-like type.
 */
template <tl::concepts::tuple T, tl::concepts::tuple U>
struct concat;
template <typename... Ts, typename... Us>
struct concat<std::tuple<Ts...>, std::tuple<Us...>>
{
    using type = std::tuple<Ts..., Us...>;
};
template <tl::concepts::tuple T, tl::concepts::tuple U>
using concat_t = typename tl::concat<T, U>::type;

/**
 * @brief split a tuple-like type in 2 at the given index.
 * @tparam T The tuple-like type.
 * @tparam split_index The index to split at.
 */
template <tl::concepts::tuple T, std::size_t split_index>
    requires(split_index <= std::tuple_size_v<T>)
class split
{
    template <std::size_t start, std::size_t... indexes>
    static constexpr auto impl(std::index_sequence<indexes...>)
    {
        return std::tuple<std::tuple_element_t<start + indexes, T>...>{};
    }

  public:
    using l = decltype(impl<0>(std::make_integer_sequence<std::size_t, split_index>()));
    using r = decltype(impl<split_index>(
        std::make_integer_sequence<std::size_t, std::tuple_size_v<T> - split_index>()));
};

template <tl::concepts::tuple T, std::size_t split_index>
    requires(split_index <= std::tuple_size_v<T>)
using split_r_t = typename split<T, split_index>::r;

template <tl::concepts::tuple T, std::size_t split_index>
    requires(split_index <= std::tuple_size_v<T>)
using split_l_t = typename split<T, split_index>::l;

/**
 * @brief Applies a unary type predicate to each type in a tuple-like type.
 * @tparam T The tuple-like type.
 * @tparam F The unary type predicate.
 */
template <tl::concepts::tuple T, template <typename> typename F>
struct for_each;

template <template <typename> typename F>
struct for_each<std::tuple<>, F>
{
    using type = std::tuple<>;
};

template <typename T, typename... Ts, template <typename> typename F>
    requires tl::concepts::unary_type_predicate<F, T>
struct for_each<std::tuple<T, Ts...>, F>
{
    using type = tl::concat_t<std::tuple<typename F<T>::type>, std::tuple<typename F<Ts>::type...>>;
};

template <tl::concepts::tuple T, template <typename> typename F>
using for_each_t = typename for_each<T, F>::type;

/**
 * @brief Swap 2 elements in a tuple-like type.
 * @tparam T The tuple-like type.
 * @tparam first The first index.
 * @tparam second The second index.
 */
template <tl::concepts::tuple T, std::size_t first, std::size_t second>
    requires tl::concepts::valid_swap_indices<T, first, second>
class swap_elements
{
    template <std::size_t... start_to_first_indexes,
              std::size_t... first_to_second_indexes,
              std::size_t... second_to_end_indexes>
    static constexpr auto swap_helper(std::index_sequence<start_to_first_indexes...>,
                                      std::index_sequence<first_to_second_indexes...>,
                                      std::index_sequence<second_to_end_indexes...>)
    {
        return std::tuple_cat(
            std::tuple<std::tuple_element_t<start_to_first_indexes, T>...>{},
            std::tuple<std::tuple_element_t<second, T>>{},
            std::tuple<std::tuple_element_t<first_to_second_indexes + first + 1, T>...>{},
            std::tuple<std::tuple_element_t<first, T>>{},
            std::tuple<std::tuple_element_t<second_to_end_indexes + second + 1, T>...>{});
    }

    static constexpr auto swap_helper_boilerplate()
    {
        auto start_to_first = std::make_integer_sequence<std::size_t, first>();
        auto first_to_second = std::make_integer_sequence<std::size_t, second - (first + 1)>();
        auto second_to_end =
            std::make_integer_sequence<std::size_t, std::tuple_size_v<T> - (second + 1)>();

        return swap_helper(start_to_first, first_to_second, second_to_end);
    }

  public:
    using type = decltype(swap_helper_boilerplate());
};

template <tl::concepts::tuple T, std::size_t first, std::size_t second>
using swap_elements_t = typename swap_elements<T, first, second>::type;

}  // namespace tl
