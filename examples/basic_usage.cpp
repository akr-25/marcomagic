#include "final_implementation_simple.hpp"
#include <iostream>

using namespace magic_enum_breakdown::final_implementation;

// Example enums for demonstration
enum class LogLevel { Debug, Info, Warning, Error };
enum class FilePermission { Read = 1, Write = 2, Execute = 4 };

int main() {
    std::cout << "=== Basic magic_enum Usage Examples ===\n\n";
    
    // Basic enum to string conversion
    std::cout << "1. Enum to String Conversion:\n";
    LogLevel level = LogLevel::Warning;
    std::cout << "LogLevel::Warning -> \"" << enum_name(level) << "\"\n";
    std::cout << "LogLevel::Error -> \"" << enum_name(LogLevel::Error) << "\"\n\n";
    
    // Get all enum names and values
    std::cout << "2. All Enum Values and Names:\n";
    
    std::cout << "LogLevel has " << enum_count<LogLevel>() << " values:\n";
    std::cout << "  LogLevel::Debug = " << static_cast<int>(LogLevel::Debug) << "\n";
    std::cout << "  LogLevel::Info = " << static_cast<int>(LogLevel::Info) << "\n";  
    std::cout << "  LogLevel::Warning = " << static_cast<int>(LogLevel::Warning) << "\n";
    std::cout << "  LogLevel::Error = " << static_cast<int>(LogLevel::Error) << "\n\n";
    
    // Enum with custom values
    std::cout << "3. Enum with Custom Values:\n";
    
    std::cout << "FilePermission has " << enum_count<FilePermission>() << " values:\n";
    std::cout << "  FilePermission::Read = " << static_cast<int>(FilePermission::Read) << "\n";
    std::cout << "  FilePermission::Write = " << static_cast<int>(FilePermission::Write) << "\n";
    std::cout << "  FilePermission::Execute = " << static_cast<int>(FilePermission::Execute) << "\n\n";
    
    // Practical usage example: logging
    auto log_message = [](LogLevel level, const std::string& message) {
        std::cout << "[" << enum_name(level) << "] " << message << "\n";
    };
    
    std::cout << "4. Practical Usage - Logging:\n";
    log_message(LogLevel::Debug, "Application started");
    log_message(LogLevel::Info, "Processing request");  
    log_message(LogLevel::Warning, "Resource running low");
    log_message(LogLevel::Error, "Connection failed");
    std::cout << "\n";
    
    // Compile-time operations
    std::cout << "5. Compile-time Validation:\n";
    static_assert(enum_count<LogLevel>() == 4, "LogLevel should have 4 values");
    // Note: static_assert with string comparison requires more complex constexpr setup
    std::cout << "All compile-time assertions passed!\n\n";
    
    return 0;
}