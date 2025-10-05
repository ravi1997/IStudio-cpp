#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [-o output_file]" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = "a.out"; // default output
    
    // Parse arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) {
            output_file = argv[i + 1];
            i++; // skip next argument
        }
    }

    // Check if input file exists and has .ipl extension
    if (input_file.substr(input_file.find_last_of(".") + 1) != "ipl") {
        std::cerr << "Error: Input file must have .ipl extension" << std::endl;
        return 1;
    }

    // Read the IPL source file
    std::ifstream infile(input_file);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file " << input_file << std::endl;
        return 1;
    }

    // For now, just create a simple output file with a basic executable
    // In a real implementation, this would parse and compile the IPL code
    std::ofstream outfile(output_file);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not create output file " << output_file << std::endl;
        return 1;
    }

    // Write a simple executable placeholder
    outfile << "# IPL Compiled Output Placeholder" << std::endl;
    outfile << "# Input: " << input_file << std::endl;
    outfile << "# This is where the compiled IPL code would go" << std::endl;

    std::cout << "Compiled " << input_file << " -> " << output_file << std::endl;

    return 0;
}