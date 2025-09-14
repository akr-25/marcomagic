#pragma once

#include <string_view>
#include <type_traits>
#include <iostream>

namespace magic_enum_breakdown::step3_sfinae_techniques {

/**
 * STEP 3: SFINAE and Type Traits Techniques
 * 
 * SFINAE (Substitution Failure Is Not An Error) is crucial for magic_enum:
 * 1. Detecting valid enum values
 * 2. Filtering out invalid template instantiations
 * 3. Creating conditional template behavior
 */

enum class Priority { Low = 1, Medium = 5, High = 10 };
enum class Invalid {}; // Empty enum for demonstration

/**
 * SFINAE Example 1: Detecting if a value is a valid enum value
 * We'll use decltype and comma operator tricks
 */

// This template will only be instantiated if E(I) is a valid enum value
template<typename E, int I>
constexpr auto is_valid_enum_value(int) -> decltype(static_cast<E>(I), std::true_type{});

// Fallback when the above fails
template<typename E, int I>
constexpr std::false_type is_valid_enum_value(...);

// Helper alias
template<typename E, int I>
constexpr bool is_valid_enum_value_v = decltype(is_valid_enum_value<E, I>(0))::value;

/**
 * SFINAE Example 2: std::enable_if for conditional compilation
 */
template<typename E, int I, std::enable_if_t<is_valid_enum_value_v<E, I>, int> = 0>
constexpr std::string_view get_enum_name_if_valid() {
    return "ValidEnumValue";
}

template<typename E, int I, std::enable_if_t<!is_valid_enum_value_v<E, I>, int> = 0>
constexpr std::string_view get_enum_name_if_valid() {
    return "";
}

/**
 * C++17 constexpr if makes some SFINAE patterns simpler
 */
template<typename E, int I>
constexpr std::string_view get_enum_name_constexpr_if() {
    if constexpr (is_valid_enum_value_v<E, I>) {
        return "ValidEnumValue";
    } else {
        return "";
    }
}

/**
 * SFINAE Example 3: Detecting enum properties
 */

// Detect if enum is scoped (enum class) vs unscoped (enum)
template<typename E>
constexpr auto is_scoped_enum_impl(int) -> decltype(static_cast<std::underlying_type_t<E>>(E{}), std::false_type{});

template<typename E>
constexpr auto is_scoped_enum_impl(...) -> std::true_type;

template<typename E>
constexpr bool is_scoped_enum_v = decltype(is_scoped_enum_impl<E>(0))::value;

/**
 * SFINAE Example 4: Creating type lists of valid enum values
 * This is getting closer to how magic_enum actually works
 */

template<typename E, int Min, int Max, int... Is>
constexpr auto make_valid_enum_sequence(std::integer_sequence<int, Is...>) {
    // Filter only valid enum values using fold expressions (C++17)
    constexpr std::size_t count = (is_valid_enum_value_v<E, Min + Is> + ...);
    
    // This is simplified - real implementation would build the sequence
    return count;
}

void demonstrate_sfinae_techniques() {
    std::cout << "=== Step 3: SFINAE and Type Traits Techniques ===\n";
    
    // Test valid enum value detection
    std::cout << "is_valid_enum_value_v<Priority, 1> = " 
              << is_valid_enum_value_v<Priority, 1> << "\n";
    std::cout << "is_valid_enum_value_v<Priority, 2> = " 
              << is_valid_enum_value_v<Priority, 2> << "\n";
    std::cout << "is_valid_enum_value_v<Priority, 5> = " 
              << is_valid_enum_value_v<Priority, 5> << "\n";
    
    // Test scoped enum detection
    std::cout << "Priority is scoped enum: " << is_scoped_enum_v<Priority> << "\n";
    
    // Test conditional compilation
    std::cout << "get_enum_name_if_valid<Priority, 1>(): \"" 
              << get_enum_name_if_valid<Priority, 1>() << "\"\n";
    std::cout << "get_enum_name_if_valid<Priority, 2>(): \"" 
              << get_enum_name_if_valid<Priority, 2>() << "\"\n";
    
    std::cout << "\n";
}

/**
 * Key takeaways for magic_enum:
 * 1. SFINAE lets us test if static_cast<E>(I) is valid at compile time
 * 2. We can build sequences of only valid enum values
 * 3. Template specialization + SFINAE = conditional behavior
 * 4. This forms the foundation for iterating over enum value ranges
 */

} // namespace magic_enum_breakdown::step3_sfinae_techniques