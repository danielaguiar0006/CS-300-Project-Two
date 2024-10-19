#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

// GLOBAL VARIABLES
enum class MenuOption { LOAD_DATA_STRUCTURE = 1, PRINT_COURSE_LIST = 2, PRINT_COURSE = 3, EXIT_PROGRAM = 9 };
const std::unordered_map<ushort, MenuOption> MENU_OPTIONS_MAP = {{1, MenuOption::LOAD_DATA_STRUCTURE},
                                                                 {2, MenuOption::PRINT_COURSE_LIST},
                                                                 {3, MenuOption::PRINT_COURSE},
                                                                 {9, MenuOption::EXIT_PROGRAM}};

void DisplayMenuOptions() {
    std::cout << "\n  1. Load Data Structure" << std::endl;
    std::cout << "  2. Print Course List" << std::endl;
    std::cout << "  3. Print Course" << std::endl;
    std::cout << "  9. Exit" << std::endl;
    std::cout << "\nWhat would you like to do? ";
}

std::optional<MenuOption> GetUserInput() {
    ushort userInput;

    // Get user input from stdin
    std::cin >> userInput;

    if (std::cin.fail()) {              // Check if user input is valid (i.e. not empty, wrong type, etc...)
        std::cin.clear();               // Clear the error flag
        std::cin.ignore(9999999, '\n'); // Ignores 9999999 characters up to the next newline character
        std::cerr << "Invalid input. Please enter a valid menu option." << std::endl;
        return std::nullopt;
    } else if (MENU_OPTIONS_MAP.find(userInput) == MENU_OPTIONS_MAP.end()) {
        std::cerr << "Invalid input. Please enter a valid menu option." << std::endl;
        return std::nullopt;
    }

    // If it passes all of the above conditions, userInput is (should be) valid and the
    // corresponding MenuOption can be returned
    return MENU_OPTIONS_MAP.at(userInput);
}

MenuOption GetValidMenuSelection() {
    DisplayMenuOptions();

    std::optional<MenuOption> userSelection = GetUserInput();

    // If the methods GetUserInput() returns a value, it must be valid and can be returned
    if (userSelection.has_value()) {
        return userSelection.value();
    } else {
        return GetValidMenuSelection(); // Recursive call itself until a valid input is received
    }
}

bool MainMenu() {
    MenuOption userSelection = GetValidMenuSelection();

    switch (userSelection) {
        case MenuOption::LOAD_DATA_STRUCTURE:
            std::cout << "Loading data structure..." << std::endl;
            break;
        case MenuOption::PRINT_COURSE_LIST:
            std::cout << "Printing course list..." << std::endl;
            break;
        case MenuOption::PRINT_COURSE:
            std::cout << "Printing course..." << std::endl;
            break;
        case MenuOption::EXIT_PROGRAM:
            std::cout << "Exiting program..." << std::endl;
            return false;
            break;
    }

    return true;
}

// NOTE: argv[1] should be able to contain a file path to a CSV file
// example: ./ProjectTwoProgram CS_300_ABCU_Advising_Program_Input.csv
int main(int argc, char* argv[]) {
    // --- DEBUG ---
    std::cout << "Number of arguments: " << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }
    std::cout << "\n";
    // --- END DEBUG ---

    const std::string DEFAULT_FILE_PATH = "CS_300_ABCU_Advising_Program_Input.csv"; // Default file path
    std::string filePath;
    bool isRunning = true;

    // GET FILE PATH
    if (argc > 1) { // Check if there are any command-line arguments (1 is reserved for the program name)
        std::cout << "File path provided as argument: " << argv[1] << "\n" << std::endl;
        filePath = argv[1];
    } else {
        std::cout << "No file path provided as argument, instead using default file path: " << DEFAULT_FILE_PATH << "\n"
                  << std::endl;
        filePath = DEFAULT_FILE_PATH;
    }

    // SETUP
    std::cout << "Welcome to the course planner." << std::endl;

    // APPLICATION LOOP
    while (isRunning) {
        isRunning = MainMenu(); // MainMenu() returns a boolean value indicating whether the program should continue
    }

    return 0;
}
