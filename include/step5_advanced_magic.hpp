#include "step4_compile_time_strings.hpp"

#include <string_view>
#include <array>
#include <type_traits>
#include <utility>
#include <iostream>

using magic_enum_breakdown::step4_compile_time_strings::extract_enum_name;

namespace magic_enum_breakdown::step5_advanced_magic {

/**
 * STEP 5: Advanced Template Magic
 * 
 * Now we combine everything to create the real magic_enum techniques:
 * 1. Automatic enum value range detection
 * 2. Filtering valid enum values using SFINAE
 * 3. Creating lookup tables at compile time
 * 4. Optimized enum-to-string and string-to-enum conversion
 */

enum class HttpStatus { 
    Ok = 200, 
    NotFound = 404, 
    InternalError = 500 
};

/**
 * Advanced SFINAE: Check if a cast to enum is valid AND produces a named enum
 */
template<typename E, int I>
constexpr auto is_valid_named_enum_value(int) -> decltype(
    static_cast<E>(I), 
    (extract_enum_name<static_cast<E>(I)>().empty() ? std::false_type{} : std::true_type{})
);

template<typename E, int I>
constexpr std::false_type is_valid_named_enum_value(...);

template<typename E, int I>
constexpr bool is_valid_named_enum_value_v = decltype(is_valid_named_enum_value<E, I>(0))::value;

// Re-use from step4
template<auto V>
constexpr std::string_view extract_enum_name() {
    constexpr std::string_view function_name = 
#if defined(__GNUC__) || defined(__clang__)
        __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)  
        __FUNCSIG__;
#else
        "unsupported";
#endif

#if defined(__GNUC__) || defined(__clang__)
    constexpr std::size_t prefix_pos = function_name.find("= ");
    if constexpr (prefix_pos == std::string_view::npos) {
        return "";
    } else {
        constexpr std::string_view after_equals = function_name.substr(prefix_pos + 2);
        constexpr std::size_t suffix_pos = after_equals.find(']');
        if constexpr (suffix_pos == std::string_view::npos) {
            return "";
        } else {
            constexpr std::string_view full_name = after_equals.substr(0, suffix_pos);
            constexpr std::size_t scope_pos = full_name.rfind("::");
            if constexpr (scope_pos == std::string_view::npos) {
                return full_name;
            } else {
                return full_name.substr(scope_pos + 2);
            }
        }
    }
#else
    return "unsupported_compiler";
#endif
}

/**
 * Recursive template to build a sequence of valid enum values
 */
template<typename E, int Min, int Max, int... Sequence>
constexpr auto build_enum_sequence() {
    if constexpr (Min > Max) {
        return std::integer_sequence<int, Sequence...>{};
    } else {
        if constexpr (is_valid_named_enum_value_v<E, Min>) {
            return build_enum_sequence<E, Min + 1, Max, Sequence..., Min>();
        } else {
            return build_enum_sequence<E, Min + 1, Max, Sequence...>();
        }
    }
}

/**
 * Create compile-time lookup table for enum values and names
 */
template<typename E, int... Values>
constexpr auto make_enum_lookup_table(std::integer_sequence<int, Values...>) {
    constexpr std::size_t size = sizeof...(Values);
    
    struct EnumEntry {
        E value;
        std::string_view name;
    };
    
    return std::array<EnumEntry, size>{
        EnumEntry{static_cast<E>(Values), extract_enum_name<static_cast<E>(Values)>()}...
    };
}

/**
 * The magic enum class template - simplified version
 */
template<typename E>
class magic_enum_simple {
private:
    // Define search range - in real magic_enum this is configurable
    static constexpr int min_value = -128;
    static constexpr int max_value = 127;
    
    using sequence_type = decltype(build_enum_sequence<E, min_value, max_value>());
    static constexpr auto lookup_table = make_enum_lookup_table<E>(sequence_type{});
    
public:
    static constexpr std::string_view enum_name(E value) {
        for (const auto& entry : lookup_table) {
            if (entry.value == value) {
                return entry.name;
            }
        }
        return "";
    }
    
    static constexpr std::size_t enum_count() {
        return lookup_table.size();
    }
    
    template<std::size_t N = 0>
    static constexpr std::array<std::string_view, enum_count()> enum_names() {
        std::array<std::string_view, enum_count()> names{};
        for (std::size_t i = 0; i < lookup_table.size(); ++i) {
            names[i] = lookup_table[i].name;
        }
        return names;
    }
    
    template<std::size_t N = 0>
    static constexpr std::array<E, enum_count()> enum_values() {
        std::array<E, enum_count()> values{};
        for (std::size_t i = 0; i < lookup_table.size(); ++i) {
            values[i] = lookup_table[i].value;
        }
        return values;
    }
};

/**
 * Convenience functions (like the real magic_enum API)
 */
template<typename E>
constexpr std::string_view enum_name(E value) {
    return magic_enum_simple<E>::enum_name(value);
}

template<typename E>
constexpr std::size_t enum_count() {
    return magic_enum_simple<E>::enum_count();
}

void demonstrate_advanced_magic() {
    std::cout << "=== Step 5: Advanced Template Magic ===\n";
    
    // Test our simplified magic_enum
    std::cout << "HttpStatus enum analysis:\n";
    std::cout << "enum_count<HttpStatus>() = " << enum_count<HttpStatus>() << "\n";
    
    std::cout << "\nEnum value to name conversion:\n";
    std::cout << "HttpStatus::Ok -> \"" << enum_name(HttpStatus::Ok) << "\"\n";
    std::cout << "HttpStatus::NotFound -> \"" << enum_name(HttpStatus::NotFound) << "\"\n";
    std::cout << "HttpStatus::InternalError -> \"" << enum_name(HttpStatus::InternalError) << "\"\n";
    
    // Show all enum names
    constexpr auto names = magic_enum_simple<HttpStatus>::enum_names();
    std::cout << "\nAll enum names: ";
    for (const auto& name : names) {
        std::cout << "\"" << name << "\" ";
    }
    
    // Show all enum values
    constexpr auto values = magic_enum_simple<HttpStatus>::enum_values();
    std::cout << "\nAll enum values: ";
    for (const auto& value : values) {
        std::cout << static_cast<int>(value) << " ";
    }
    
    std::cout << "\n\n";
}

/**
 * Key magic_enum techniques demonstrated:
 * 1. SFINAE to detect valid enum values in a range
 * 2. Recursive templates to build compile-time sequences
 * 3. Compile-time lookup tables using std::array
 * 4. Template metaprogramming to achieve zero runtime overhead
 * 5. Parsing compiler-generated strings for enum names
 */

} // namespace magic_enum_breakdown::step5_advanced_magic