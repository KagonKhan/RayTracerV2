#include <iostream>
#include <print>
#include <ranges>
#include <source_location>

int main(){
    std::print(std::cout, std::source_location::current().function_name());
    // test 
}
