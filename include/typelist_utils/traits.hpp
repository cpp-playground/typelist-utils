#pragma once

#include <tuple>
#include <type_traits>

/**
 * @brief Namespace containing trait types
 */
namespace tl::traits
{

/**
 * @brief Trait type used to check if a given type is a tuple
 * Inherits std::true_type if so, std::false_type otherwise.
 *
 * @tparam T Type to check
 */
template <typename>
struct is_tuple : std::false_type
{};

/**
 * @brief Trait type used to check if a given type is a tuple
 * Inherits std::true_type if so, std::false_type otherwise.
 *
 * @tparam T Type to check
 */
template <typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type
{};

template <typename T>
constexpr auto is_tuple_v = is_tuple<T>::value;

}  // namespace tl::traits


/**
 * @brief Namespace containing concepts
 */
namespace tl::concepts
{

/**
 * @brief Concept checking that 2 given types are the same
 * @tparam T First type to check
 * @tparam U Second type to check
 */
template <typename T, typename U>
concept same_as = std::is_same_v<T, U>;

/**
 * @brief Concept checking a given type is a tuple type
 * @tparam T Type to check
 */
template <typename T>
concept tuple = tl::traits::is_tuple_v<T>;

/**
 * @brief Concept checking that a given type can be used as a binary value predicate for 
 * 2 other types.
 *
 * A binary value predicate is a type that, given 2 types exposes a static field
 * value of type bool
 *
 * @tparam P Predicate type to check
 * @tparam T First type for the predicate to be used on
 * @tparam U Second type for the predicate to be used on
 */
template <template <typename, typename> typename P, typename T, typename U>
concept binary_value_predicate = requires
{
    {
        P<T, U>::value
        } -> same_as<const bool&>;
};

/**
 * @brief Concept checking that a given type can be used as a unary value predicate for 
 * another type.
 *
 * A unary value predicate is a type that, given another types exposes a static field
 * value of type bool
 *
 * @tparam P Predicate type to check
 * @tparam T Type for the predicate to be used on
 */
template <template <typename> typename P, typename T>
concept unary_value_predicate = requires
{
    {
        P<T>::value
        } -> same_as<const bool&>;
};

/**
 * @brief Concept checking that a given type can be used as a binary type predicate for 
 * 2 other types.
 *
 * A binary type predicate is a type that, given 2 types exposes a type member named type
 *
 * @tparam P Predicate type to check
 * @tparam T First type for the predicate to be used on
 * @tparam U Second type for the predicate to be used on
 */
template <template <typename, typename> typename P, typename T, typename U>
concept binary_type_predicate = requires
{
    typename P<T, U>::type;
};

/**
 * @brief Concept checking that a given type can be used as a unary type predicate for 
 * another type.
 *
 * A unary type predicate is a type that, given another type exposes a type member named type
 *
 * @tparam P Predicate type to check
 * @tparam T Type for the predicate to be used on
 */
template <template <typename> typename P, typename T>
concept unary_type_predicate = requires
{
    typename P<T>::type;
};

/**
 * @brief Concept checking that a pair of indices are valid swap indices for a given tuple
 *
 * 2 indices are valid if the first is strictly smaller than the second and that the second
 * is strictly smaller than the provided tuple size
 *
 * @tparam T Tuple to check indices validity against
 * @tparam first First index
 * @tparam second Second index
 */
template <typename T, std::size_t first, std::size_t second>
concept valid_swap_indices = requires {
    requires tl::concepts::tuple<T>;
    requires first < second;
    requires second < std::tuple_size_v<T>;
};

}  // namespace tl::concepts
