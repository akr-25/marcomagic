#pragma once

#include <string_view>
#include <iostream>

namespace magic_enum_breakdown::step1_basic_concepts {

/**
 * STEP 1: Basic Enum Reflection Concepts
 * 
 * Before diving into the magic, let's understand what we want to achieve:
 * - Convert enum values to strings at compile time
 * - Zero runtime overhead
 * - Type-safe and constexpr-friendly
 */

// Simple enum for demonstration
enum class Color { Red, Green, Blue };

// Traditional approach (not what we want - runtime overhead)
const char* color_to_string_runtime(Color c) {
    switch(c) {
        case Color::Red: return "Red";
        case Color::Green: return "Green"; 
        case Color::Blue: return "Blue";
        default: return "Unknown";
    }
}

/**
 * The Goal: We want to achieve the same result but at compile time!
 * 
 * Key insights:
 * 1. C++ compilers generate debug symbols with enum names
 * 2. We can use __PRETTY_FUNCTION__ (GCC/Clang) or __FUNCSIG__ (MSVC) 
 *    to extract type information at compile time
 * 3. Template specialization allows us to generate different code for different enum values
 */

// Demo function to show what __PRETTY_FUNCTION__ gives us
template<auto V>
constexpr std::string_view get_function_name() {
#if defined(__GNUC__) || defined(__clang__)
    return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    return __FUNCSIG__;
#else
    return "unsupported compiler";
#endif
}

/**
 * When called with Color::Red, __PRETTY_FUNCTION__ might return something like:
 * "constexpr std::string_view magic_enum_breakdown::step1_basic_concepts::get_function_name() [with auto V = Color::Red]"
 * 
 * The magic is in parsing this string to extract "Red"!
 */

void demonstrate_basic_concepts() {
    std::cout << "=== Step 1: Basic Enum Reflection Concepts ===\n";
    
    std::cout << "Runtime approach: " << color_to_string_runtime(Color::Red) << "\n";
    
    std::cout << "Function signature for Color::Red:\n";
    std::cout << get_function_name<Color::Red>() << "\n\n";
    
    std::cout << "Function signature for Color::Green:\n";
    std::cout << get_function_name<Color::Green>() << "\n\n";
}

} // namespace magic_enum_breakdown::step1_basic_concepts