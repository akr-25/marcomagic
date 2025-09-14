#pragma once

#include <string_view>
#include <array>
#include <iostream>

namespace magic_enum_breakdown::step4_compile_time_strings {

/**
 * STEP 4: Compile-time String Manipulation
 * 
 * The heart of magic_enum is parsing compiler-generated strings to extract enum names.
 * This requires compile-time string manipulation techniques.
 */

enum class Operation { Create, Read, Update, Delete };

/**
 * Basic compile-time string utilities
 */
constexpr std::size_t strlen_constexpr(const char* str) {
    std::size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

constexpr bool starts_with(std::string_view str, std::string_view prefix) {
    return str.size() >= prefix.size() && str.substr(0, prefix.size()) == prefix;
}

constexpr bool ends_with(std::string_view str, std::string_view suffix) {
    return str.size() >= suffix.size() && 
           str.substr(str.size() - suffix.size()) == suffix;
}

constexpr std::string_view trim_prefix(std::string_view str, std::string_view prefix) {
    return starts_with(str, prefix) ? str.substr(prefix.size()) : str;
}

constexpr std::string_view trim_suffix(std::string_view str, std::string_view suffix) {
    return ends_with(str, suffix) ? str.substr(0, str.size() - suffix.size()) : str;
}

/**
 * The magic: Extracting enum name from __PRETTY_FUNCTION__
 */
template<auto V>
constexpr std::string_view get_raw_function_name() {
#if defined(__GNUC__) || defined(__clang__)
    return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    return __FUNCSIG__;
#else
    return "unsupported compiler";
#endif
}

/**
 * Parse the function signature to extract just the enum name
 * 
 * For GCC/Clang, __PRETTY_FUNCTION__ might look like:
 * "constexpr std::string_view magic_enum_breakdown::step4_compile_time_strings::get_raw_function_name() [with auto V = Operation::Create]"
 * 
 * We need to extract "Create" from this.
 */
template<auto V>
constexpr std::string_view extract_enum_name() {
    constexpr std::string_view function_name = get_raw_function_name<V>();
    
#if defined(__GNUC__) || defined(__clang__)
    // Find the pattern "= EnumType::EnumValue]"
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
            // Find the last "::" to get just the enum value name
            constexpr std::size_t scope_pos = full_name.rfind("::");
            if constexpr (scope_pos == std::string_view::npos) {
                return full_name;
            } else {
                return full_name.substr(scope_pos + 2);
            }
        }
    }
    
#elif defined(_MSC_VER)
    // MSVC has different format - would need different parsing logic
    return "MSVC_parsing_not_implemented";
#else
    return "";
#endif
}

/**
 * Compile-time validation that our parsing works
 */
template<auto V>
constexpr bool has_valid_name() {
    constexpr std::string_view name = extract_enum_name<V>();
    return !name.empty() && name != "MSVC_parsing_not_implemented";
}

/**
 * Building arrays of enum names at compile time
 */
template<auto... Values>
constexpr auto make_enum_names_array() {
    return std::array<std::string_view, sizeof...(Values)>{
        extract_enum_name<Values>()...
    };
}

void demonstrate_compile_time_strings() {
    std::cout << "=== Step 4: Compile-time String Manipulation ===\n";
    
    std::cout << "Raw function signature for Operation::Create:\n";
    std::cout << get_raw_function_name<Operation::Create>() << "\n\n";
    
    std::cout << "Extracted enum names:\n";
    std::cout << "Operation::Create -> \"" << extract_enum_name<Operation::Create>() << "\"\n";
    std::cout << "Operation::Read -> \"" << extract_enum_name<Operation::Read>() << "\"\n";
    std::cout << "Operation::Update -> \"" << extract_enum_name<Operation::Update>() << "\"\n";
    std::cout << "Operation::Delete -> \"" << extract_enum_name<Operation::Delete>() << "\"\n";
    
    // Demonstrate compile-time array creation
    constexpr auto names = make_enum_names_array<
        Operation::Create, 
        Operation::Read, 
        Operation::Update, 
        Operation::Delete
    >();
    
    std::cout << "\nEnum names array: ";
    for (const auto& name : names) {
        std::cout << "\"" << name << "\" ";
    }
    std::cout << "\n\n";
}

/**
 * Key insights:
 * 1. __PRETTY_FUNCTION__ gives us enum names at compile time
 * 2. constexpr string manipulation lets us parse these names
 * 3. Template parameter packs let us build arrays of names
 * 4. All of this happens at compile time - zero runtime overhead!
 */

} // namespace magic_enum_breakdown::step4_compile_time_strings