#pragma once

#include <string_view>
#include <type_traits>
#include <iostream>

namespace magic_enum_breakdown::step2_template_fundamentals {

/**
 * STEP 2: Template Metaprogramming Fundamentals
 * 
 * To understand magic_enum, we need to master several template techniques:
 * 1. Non-type template parameters (auto in C++17)
 * 2. Template specialization
 * 3. constexpr functions
 * 4. Template parameter packs and fold expressions
 */

// Example enum for demonstrations
enum class Status { Pending, Running, Complete, Failed };

/**
 * Non-type template parameters allow us to pass enum values as template arguments
 */
template<Status S>
struct StatusInfo {
    static constexpr std::string_view name = "Unknown";
};

// Template specialization - we can provide specific implementations for specific values
template<>
struct StatusInfo<Status::Pending> {
    static constexpr std::string_view name = "Pending";
};

template<>
struct StatusInfo<Status::Running> {
    static constexpr std::string_view name = "Running";
};

/**
 * C++17's auto template parameter makes this more flexible
 */
template<auto Value>
struct EnumInfo {
    static constexpr std::string_view name = "Unknown";
};

template<>
struct EnumInfo<Status::Complete> {
    static constexpr std::string_view name = "Complete";
};

/**
 * Template functions can also use auto parameters
 */
template<auto EnumValue>
constexpr std::string_view get_name() {
    if constexpr (EnumValue == Status::Pending) {
        return "Pending";
    } else if constexpr (EnumValue == Status::Running) {
        return "Running";
    } else if constexpr (EnumValue == Status::Complete) {
        return "Complete";
    } else if constexpr (EnumValue == Status::Failed) {
        return "Failed";
    }
    return "Unknown";
}

/**
 * Template parameter packs for handling multiple enum values at once
 */
template<auto... Values>
constexpr auto make_name_array() {
    return std::array<std::string_view, sizeof...(Values)>{
        get_name<Values>()...
    };
}

/**
 * Compile-time iteration using index sequences
 */
template<typename E, E... Values>
constexpr auto enum_names_for_values(std::integer_sequence<E, Values...>) {
    return std::array<std::string_view, sizeof...(Values)>{
        get_name<Values>()...
    };
}

void demonstrate_template_fundamentals() {
    std::cout << "=== Step 2: Template Metaprogramming Fundamentals ===\n";
    
    // Non-type template parameters
    std::cout << "StatusInfo<Status::Pending>::name = " 
              << StatusInfo<Status::Pending>::name << "\n";
    
    // Auto template parameters
    std::cout << "EnumInfo<Status::Complete>::name = " 
              << EnumInfo<Status::Complete>::name << "\n";
    
    // Template functions with auto parameters
    std::cout << "get_name<Status::Running>() = " 
              << get_name<Status::Running>() << "\n";
    
    // Template parameter packs
    std::cout << "Template parameter packs - individual calls:\n";
    std::cout << "get_name<Status::Pending>() = " << get_name<Status::Pending>() << "\n";
    std::cout << "get_name<Status::Running>() = " << get_name<Status::Running>() << "\n";
    std::cout << "get_name<Status::Complete>() = " << get_name<Status::Complete>() << "\n\n";
}

} // namespace magic_enum_breakdown::step2_template_fundamentals