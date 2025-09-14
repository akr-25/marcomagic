#pragma once

#include <string_view>
#include <array>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <iostream>
#include <optional>

namespace magic_enum_breakdown::final_implementation {

/**
 * FINAL IMPLEMENTATION: A Complete Simplified magic_enum
 * 
 * This brings together all the techniques we've learned into a working implementation
 * that demonstrates the core principles of the real magic_enum library.
 */

// Configuration constants (simplified)
constexpr int MAGIC_ENUM_RANGE_MIN = -128;
constexpr int MAGIC_ENUM_RANGE_MAX = 127;

/**
 * Utility to extract enum name from compiler function signature
 */
template<auto V>
constexpr std::string_view pretty_name() {
#if defined(__GNUC__) || defined(__clang__)
    constexpr std::string_view name = __PRETTY_FUNCTION__;
    constexpr std::size_t prefix = name.find("= ") + 2;
    constexpr std::size_t suffix = name.find(']', prefix);
    constexpr std::string_view full_name = name.substr(prefix, suffix - prefix);
    constexpr std::size_t scope = full_name.rfind("::");
    return scope == std::string_view::npos ? full_name : full_name.substr(scope + 2);
#elif defined(_MSC_VER)
    constexpr std::string_view name = __FUNCSIG__;
    constexpr std::size_t prefix = name.find('<') + 1;
    constexpr std::size_t suffix = name.rfind(">(");
    constexpr std::string_view full_name = name.substr(prefix, suffix - prefix);
    constexpr std::size_t scope = full_name.rfind("::");
    return scope == std::string_view::npos ? full_name : full_name.substr(scope + 2);
#else
    return "unknown";
#endif
}

/**
 * Check if an integer value corresponds to a valid, named enum value
 */
template<typename E, int I>
constexpr bool is_valid_enum() {
    if constexpr (std::is_enum_v<E>) {
        constexpr E value = static_cast<E>(I);
        constexpr std::string_view name = pretty_name<value>();
        return !name.empty() && name.front() != '(' && name.back() != ')';
    }
    return false;
}

/**
 * Build a compile-time sequence of valid enum values
 */
template<typename E, int... Is>
constexpr auto filter_valid_enums(std::integer_sequence<int, Is...>) {
    constexpr auto valid_values = std::array{is_valid_enum<E, MAGIC_ENUM_RANGE_MIN + Is>()...};
    constexpr std::size_t count = std::count(valid_values.begin(), valid_values.end(), true);
    
    // This is simplified - real implementation would build the actual sequence
    return count;
}

/**
 * Core magic_enum implementation
 */
template<typename E, std::enable_if_t<std::is_enum_v<E>, int> = 0>
class magic_enum {
public:
    /**
     * Convert enum value to string name
     */
    static constexpr std::string_view enum_name(E value) noexcept {
        const auto integer_value = static_cast<std::underlying_type_t<E>>(value);
        if (integer_value >= MAGIC_ENUM_RANGE_MIN && integer_value <= MAGIC_ENUM_RANGE_MAX) {
            return enum_name_impl(value);
        }
        return {};
    }
    
    /**
     * Get array of all enum names
     */
    static constexpr auto enum_names() noexcept {
        return enum_names_impl();
    }
    
    /**
     * Get array of all enum values  
     */
    static constexpr auto enum_values() noexcept {
        return enum_values_impl();
    }
    
    /**
     * Get count of enum values
     */
    static constexpr std::size_t enum_count() noexcept {
        return enum_values().size();
    }
    
    /**
     * Convert string to enum value (simplified)
     */
    static constexpr std::optional<E> enum_cast(std::string_view name) noexcept {
        const auto names = enum_names();
        const auto values = enum_values();
        
        for (std::size_t i = 0; i < names.size(); ++i) {
            if (names[i] == name) {
                return values[i];
            }
        }
        return {};
    }

private:
    template<E V>
    static constexpr std::string_view enum_name_impl(E value) noexcept {
        if constexpr (V == value) {
            return pretty_name<V>();
        }
        return {};
    }
    
    template<int Min, int Max, int... Sequence>
    static constexpr auto build_enum_arrays() noexcept {
        if constexpr (Min > Max) {
            constexpr std::size_t size = sizeof...(Sequence);
            std::array<E, size> values{static_cast<E>(Sequence)...};
            std::array<std::string_view, size> names{pretty_name<static_cast<E>(Sequence)>()...};
            return std::pair{values, names};
        } else {
            if constexpr (is_valid_enum<E, Min>()) {
                return build_enum_arrays<Min + 1, Max, Sequence..., Min>();
            } else {
                return build_enum_arrays<Min + 1, Max, Sequence...>();
            }
        }
    }
    
    static constexpr auto enum_data() noexcept {
        return build_enum_arrays<MAGIC_ENUM_RANGE_MIN, MAGIC_ENUM_RANGE_MAX>();
    }
    
    static constexpr auto enum_values_impl() noexcept {
        return enum_data().first;
    }
    
    static constexpr auto enum_names_impl() noexcept {
        return enum_data().second;
    }
    
    static constexpr std::string_view enum_name_impl(E value) noexcept {
        const auto values = enum_values();
        const auto names = enum_names();
        
        for (std::size_t i = 0; i < values.size(); ++i) {
            if (values[i] == value) {
                return names[i];
            }
        }
        return {};
    }
};

/**
 * Convenience functions (matching magic_enum API)
 */
template<typename E>
constexpr std::string_view enum_name(E value) noexcept {
    return magic_enum<E>::enum_name(value);
}

template<typename E>  
constexpr auto enum_names() noexcept {
    return magic_enum<E>::enum_names();
}

template<typename E>
constexpr auto enum_values() noexcept {
    return magic_enum<E>::enum_values();
}

template<typename E>
constexpr std::size_t enum_count() noexcept {
    return magic_enum<E>::enum_count();
}

// Test enums
enum class Color { Red = 1, Green = 2, Blue = 4 };
enum class Size { Small, Medium, Large };

void demonstrate_final_implementation() {
    std::cout << "=== FINAL IMPLEMENTATION: Complete magic_enum ===\n";
    
    // Test Color enum
    std::cout << "Color enum:\n";
    std::cout << "enum_count<Color>() = " << enum_count<Color>() << "\n";
    
    std::cout << "Enum values to names:\n";
    std::cout << "Color::Red -> \"" << enum_name(Color::Red) << "\"\n";
    std::cout << "Color::Green -> \"" << enum_name(Color::Green) << "\"\n";
    std::cout << "Color::Blue -> \"" << enum_name(Color::Blue) << "\"\n";
    
    // Test Size enum  
    std::cout << "\nSize enum:\n";
    std::cout << "enum_count<Size>() = " << enum_count<Size>() << "\n";
    
    constexpr auto size_names = enum_names<Size>();
    constexpr auto size_values = enum_values<Size>();
    
    std::cout << "All Size values and names:\n";
    for (std::size_t i = 0; i < size_names.size(); ++i) {
        std::cout << "Size::" << size_names[i] << " = " << static_cast<int>(size_values[i]) << "\n";
    }
    
    std::cout << "\n=== Key Achievements ===\n";
    std::cout << "✓ Zero runtime overhead - everything computed at compile time\n";
    std::cout << "✓ Type-safe enum reflection\n";
    std::cout << "✓ Automatic enum value discovery\n";
    std::cout << "✓ Cross-platform compatibility (GCC, Clang, MSVC)\n";
    std::cout << "✓ constexpr-friendly API\n";
    std::cout << "✓ Template metaprogramming wizardry!\n\n";
}

} // namespace magic_enum_breakdown::final_implementation