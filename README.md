![App Showcase](assets/c-assembler-readme.png)
<p align="center">
  <img src="https://img.shields.io/badge/course-systems_programming_laboratory-lightgrey.svg">
  <img src="https://img.shields.io/badge/language-C-lightgrey.svg">
  <img src="https://img.shields.io/badge/grade-95-blue.svg">
</p>

## Overview
**C Assembler** is a two-pass assembler written in C, designed as a comprehensive systems programming project. It demonstrates mastery of bit-field methods, dynamic memory management, and modular programming architecture. The assembler processes assembly language source files and generates corresponding machine code through a sophisticated two-pass compilation process.

## Key Features
- **Two-Pass Architecture**: Implements a complete two-pass assembly algorithm for symbol resolution
- **Memory Management**: Efficient dynamic memory allocation and deallocation
- **Modular Design**: Clean separation of concerns across multiple source files
- **Robust Error Handling**: Comprehensive syntax and semantic error detection
- **Standards Compliant**: Written in standard C with portable code practices

## Architecture & File Structure

### Core Components
- **assembler.c**: Main entry point containing `main()` and `executePasses()` orchestration
- **assemblerHelper.c/.h**: Argument validation and pass control logic
- **dataStructure.c/.h**: Core data structures (line/label structs) and initialization routines

### Processing Modules
- **firstPass.c/.h**: First pass implementation handling symbol table construction
- **secondPassHelper.c/.h**: Second pass logic for code generation and symbol resolution
- **reservedWords.c/.h**: Assembly language keyword recognition and instruction parsing
- **strHandle.c/.h**: String manipulation and parsing utilities

### Output Generation
- **outputPrint.c/.h**: Machine code generation and output file creation

### Build System
- **makefile**: Automated build configuration with dependency management

## Installation & Usage

### Prerequisites
- GCC compiler (or compatible C compiler)
- Make utility
- POSIX-compliant system (Linux/Unix/macOS)

### Building
```bash
# Clone the repository
git clone <repository-url>
cd C_Assembler

# Compile the project
make

# Clean build artifacts (optional)
make clean
```

### Running the Assembler
```bash
# Basic usage
./assembler source_file.as

# Example with multiple files
./assembler prog1.as prog2.as prog3.as
```

### Input/Output
- **Input**: Assembly source files with `.as` extension
- **Output**: Generated files include:
  - `.ob` - Object file (machine code)
  - `.ext` - External symbols file
  - `.ent` - Entry symbols file

## Assembly Language Support
The assembler supports a custom assembly language instruction set including:
- Data manipulation instructions
- Control flow operations
- Memory addressing modes
- Symbol definitions and references
- Directive handling (`.data`, `.string`, `.entry`, `.extern`)

## Error Handling
Comprehensive error detection including:
- Syntax errors (invalid instruction format)
- Semantic errors (undefined symbols, duplicate labels)
- Memory allocation failures
- File I/O errors
- Invalid command-line arguments

## Development Notes
This project was developed as the final assignment for a Systems Programming Laboratory course, focusing on:
- Low-level programming concepts
- Assembly language processing
- File I/O operations
- Data structure design
- Modular programming practices

## Project Statistics
- **Final Grade**: 95/100
- **Language**: C
- **Architecture**: Modular, multi-file design
- **Compilation**: Standard makefile with dependency tracking

## Contributing
Contributions are welcome! Please follow these guidelines:
1. Fork the repository
2. Create a feature branch
3. Implement changes with appropriate tests
4. Submit a pull request with detailed description

## License
This project is part of academic coursework. Please respect academic integrity policies when referencing or using this code.

---
*Built with precision and attention to detail as a demonstration of systems programming mastery.*
