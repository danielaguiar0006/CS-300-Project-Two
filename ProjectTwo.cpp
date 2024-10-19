#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

// --- GLOBAL VARIABLES ---
enum class MenuOption { LOAD_DATA_STRUCTURE, PRINT_COURSE_LIST, PRINT_COURSE, EXIT_PROGRAM };
const std::unordered_map<ushort, MenuOption> MENU_OPTIONS_MAP = {{1, MenuOption::LOAD_DATA_STRUCTURE},
                                                                 {2, MenuOption::PRINT_COURSE_LIST},
                                                                 {3, MenuOption::PRINT_COURSE},
                                                                 {9, MenuOption::EXIT_PROGRAM}};
// --- END GLOBAL VARIABLES ---

// --- COURSE OBJECT ---
struct Course {
    std::string name;
    std::string description;
    std::vector<std::string> prerequisites;
};
// --- END COURSE OBJECT ---

// --- BINARY SEARCH TREE ---
struct Node {
    std::string data;
    Node* left;
    Node* right;

    Node(const std::string& value) : data(value), left(nullptr), right(nullptr) {}
};

class BST {
  public:
    BST() : root(nullptr) {}

    void Insert(const std::string& value) {
        root = InsertRec(root, value);
    }

    void PrintInOrder() {
        PrintInOrderRec(root);
    }

  private:
    Node* root;

    Node* InsertRec(Node* node, const std::string& value) {
        if (node == nullptr) {
            return new Node(value);
        }

        if (value < node->data) {
            node->left = InsertRec(node->left, value);
        } else if (value > node->data) {
            node->right = InsertRec(node->right, value);
        }

        return node;
    }

    void PrintInOrderRec(Node* node) {
        if (node != nullptr) {
            PrintInOrderRec(node->left);
            std::cout << node->data << std::endl;
            PrintInOrderRec(node->right);
        }
    }
};
// --- END BINARY SEARCH TREE ---

// --- MENU FUNCTIONS ---
void LoadDataStructure(BST* structure, const std::string& filePath) {
    // OPEN FILE
    std::ifstream file(filePath);
    if (!file.is_open()) { // Check if the file was opened successfully
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    // READ FILE
    std::vector<std::vector<std::string>> data; // 2D vector to store all of the csv data
    std::string line;                           // Represents a single line of the file

    while (std::getline(file, line)) { // Read the file line by line (Will do this until the end of the file)
        // HACK: Remove carriage return characters from the line
        // This is needed because it could cause undefined behavior
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        std::stringstream ss(line);   // Create a stringstream from the line
        std::string value;            // Represents a single value in a row
        std::vector<std::string> row; // Represents a single row of the csv file

        while (std::getline(ss, value, ',')) { // Read each value in the row
            if (value.empty()) {               // Check if the value is empty
                continue;                      // Skip the empty value
            }

            row.push_back(value); // Add the value to the row
        }

        data.push_back(row); // Add the row to the 2D vector
    }

    // PROCESS DATA
    for (const auto& row : data) {
        Course course;

        // Load the name and description of the course into the course object
        course.name        = row[0];
        course.description = row[1];

        // Load the variable prerequisites into the course object
        for (int i = 2; i < row.size(); ++i) {
            course.prerequisites.push_back(row[i]);
        }

        // --- DEBUG ---
        std::cout << "Course Name: " << course.name << std::endl;
        std::cout << "Course Description: " << course.description << std::endl;
        std::cout << "Course Prerequisites: ";
        for (const auto& prerequisite : course.prerequisites) {
            std::cout << prerequisite << " ";
        }
        std::cout << std::endl;
        // --- END DEBUG ---

        // TODO: structure->Insert(course);
    }
}

void PrintCourseList(BST* structure) {
    structure->PrintInOrder();
}
// --- END MENU FUNCTIONS ---

// --- MENU ---
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

bool MainMenu(BST* structure, const std::string& filePath) {
    MenuOption userSelection = GetValidMenuSelection();

    switch (userSelection) {
        case MenuOption::LOAD_DATA_STRUCTURE:
            std::cout << "Loading data structure..." << std::endl;
            LoadDataStructure(structure, filePath);
            break;
        case MenuOption::PRINT_COURSE_LIST:
            std::cout << "Printing course list..." << std::endl;
            PrintCourseList(structure);
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
// --- END MENU ---

// --- MAIN ENTRY POINT ---
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
    BST structure;

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
        // MainMenu() returns a boolean value indicating whether the program should continue
        isRunning = MainMenu(&structure, filePath);
    }

    return 0;
}
// --- END MAIN ENTRY POINT ---
