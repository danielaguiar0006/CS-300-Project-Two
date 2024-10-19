#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// --- GLOBAL VARIABLES ---
enum class MenuOption { LOAD_DATA_STRUCTURE, PRINT_COURSE_LIST, PRINT_COURSE, EXIT_PROGRAM };
const std::unordered_map<ushort, MenuOption> MENU_OPTIONS_MAP = {{1, MenuOption::LOAD_DATA_STRUCTURE},
                                                                 {2, MenuOption::PRINT_COURSE_LIST},
                                                                 {3, MenuOption::PRINT_COURSE},
                                                                 {9, MenuOption::EXIT_PROGRAM}};
// --- END GLOBAL VARIABLES ---

// --- COURSE OBJECT ---
/**
 * @brief Represents a course with its details.
 *
 * This structure holds information about a course, including its name,
 * description, and a list of prerequisites.
 */
struct Course {
    std::string name;                       ///< The name of the course.
    std::string description;                ///< A brief description of the course.
    std::vector<std::string> prerequisites; ///< List of course prerequisites.
};
// --- END COURSE OBJECT ---

// --- BINARY SEARCH TREE ---
/**
 * @brief Represents a node in the binary search tree which holds a Course object.
 */
struct Node {
    Course data; ///< The course data held by the node.
    Node* left;  ///< Pointer to the left child node.
    Node* right; ///< Pointer to the right child node.

    /**
     * @brief Constructs a new Node with the given course data.
     *
     * @param value The course data to be held by the node.
     */
    Node(Course value) : data(value), left(nullptr), right(nullptr) {}
};

/**
 * @brief Represents a binary search tree that holds nodes, which themselves hold Course objects.
 */
class BST { // Represents a binary search tree that hold nodes, which themselves hold Course objects
  public:
    BST() : root(nullptr) {}
    ~BST() {
        Clear(root);
    }

    /**
     * @brief Checks if the binary search tree is empty.
     *
     * @return true if the tree has no nodes, false otherwise.
     */
    bool IsEmpty() {
        return root == nullptr;
    }

    /**
     * @brief Inserts a course into the binary search tree.
     *
     * @param value The course to be inserted.
     */
    void Insert(Course value) {
        root = InsertRec(root, &value);
    }

    /**
     * @brief Prints the courses in the binary search tree in order.
     */
    void PrintInOrder() {
        PrintInOrderRec(root);
    }

    /**
     * @brief Finds a course by its name.
     *
     * @param courseName The name of the course to find.
     * @return An optional containing a pointer to the found course, or nullopt if not found.
     */
    std::optional<Course*> Find(const std::string& courseName) {
        // Initialize an empty string to hold the courseName in uppercase
        std::string upperCaseCourseName(courseName.size(), ' ');

        // Convert the courseName to uppercase to match the case of the Course object names
        std::transform(courseName.begin(), courseName.end(), upperCaseCourseName.begin(), ::toupper);

        // Call the recursive FindRec function with the uppercase courseName
        return FindRec(root, upperCaseCourseName);
    }

  private:
    Node* root; ///< The root node of the binary search tree.

    /**
     * @brief Recursively inserts a course into the binary search tree.
     *
     * @param node The current node in the tree.
     * @param value The course to be inserted.
     * @return The new root of the subtree.
     */
    Node* InsertRec(Node* node, Course* value) {
        if (node == nullptr) {
            return new Node(*value);
        }

        // DECIDE WHICH SUBTREE TO INSERT INTO
        if (value->name < node->data.name) {
            node->left = InsertRec(node->left, value);
        } else if (value->name > node->data.name) {
            node->right = InsertRec(node->right, value);
        }

        return node;
    }

    /**
     * @brief Recursively prints the courses in the binary search tree in order.
     *
     * @param node The current node in the tree.
     */
    void PrintInOrderRec(Node* node) {
        if (node != nullptr) {
            // RECURSIVELY PRINT THE LEFT SUBTREE
            PrintInOrderRec(node->left);

            // ACTUALLY PRINTING THE COURSE
            std::cout << node->data.name << ", " << node->data.description << std::endl;

            // RECURSIVELY PRINT THE RIGHT SUBTREE
            PrintInOrderRec(node->right);
        }
    }

    /**
     * @brief Recursively finds a course by its name.
     *
     * @param node The current node in the tree.
     * @param upperCaseCourseName The name of the course to find, in uppercase.
     * @return An optional containing a pointer to the found course, or nullopt if not found.
     */
    std::optional<Course*> FindRec(Node* node, const std::string& upperCaseCourseName) {
        if (node == nullptr) {
            return std::nullopt;
        }

        // DECIDE WHICH SUBTREE TO FIND
        if (upperCaseCourseName < node->data.name) {
            return FindRec(node->left, upperCaseCourseName);
        } else if (upperCaseCourseName > node->data.name) {
            return FindRec(node->right, upperCaseCourseName);
        } else {
            return &node->data;
        }
    }

    /**
     * @brief Recursively clears the binary search tree by deleting all nodes.
     *
     * @param node The current node in the tree.
     */
    void Clear(Node* node) {
        if (node != nullptr) {
            Clear(node->left);
            Clear(node->right);
            delete node;
        }
    }
};
// --- END BINARY SEARCH TREE ---

// --- MENU FUNCTIONS ---
/**
 * @brief Loads data from a CSV file into the binary search tree.
 *
 * This function reads a CSV file specified by the file path and loads its data into the provided
 * binary search tree structure. Each row in the CSV file represents a course, with the first column
 * being the course name, the second column being the course description, and subsequent columns
 * being the prerequisites for the course.
 *
 * @param structure Pointer to the binary search tree to load data into.
 * @param filePath The path to the CSV file containing the course data.
 * @return true if the data was loaded successfully, false otherwise.
 */
bool LoadDataStructure(BST* structure, const std::string& filePath) {
    try {
        // OPEN FILE
        std::ifstream file(filePath);
        if (!file.is_open()) { // Check if the file was opened successfully
            std::cerr << "Could not open the file: " << filePath << std::endl;
            return false;
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
            // Create a new Course object and load its data from the row
            Course course;

            // Load the name and description of the course into the course object
            course.name = row[0];
            // Enforce uppercase for the course name (Used for searching)
            std::transform(course.name.begin(), course.name.end(), course.name.begin(), ::toupper);

            course.description = row[1];

            // Load the variable prerequisites into the course object
            for (int i = 2; i < row.size(); ++i) {
                course.prerequisites.push_back(row[i]);
            }

            // Insert the course into the binary search tree
            structure->Insert(course);
        }
        // Return true to indicate successful data loading
        return true;

    } catch (std::exception& e) {
        std::cerr << "An error occurred while loading the data structure: " << e.what() << std::endl;
        // Return false to indicate unsuccessful data loading
        return false;
    }
}

/**
 * @brief Prints the list of all courses in the binary search tree in order.
 *
 * @param structure Pointer to the binary search tree containing the courses.
 */
void PrintCourseList(BST* structure) {
    structure->PrintInOrder();
}

/**
 * @brief Prints the details of a specific course.
 *
 * This function searches for a course by its name in the binary search tree and prints its details,
 * including its name, description, and prerequisites. If the course is not found, an error message is displayed.
 *
 * @param structure Pointer to the binary search tree containing the courses.
 * @param courseName The name of the course to be printed.
 */
void PrintCourse(BST* structure, const std::string& courseName) {
    std::optional<Course*> course = structure->Find(courseName);
    if (course.has_value()) {
        std::cout << course.value()->name << ", " << course.value()->description << std::endl;
        std::cout << "Prerequisites: ";
        for (const auto& prerequisite : course.value()->prerequisites) {
            std::cout << prerequisite << " ";
        }
        std::cout << std::endl;
    } else {
        std::cerr << "Course: " << courseName << " not found." << std::endl;
    }
}
// --- END MENU FUNCTIONS ---

// --- MENU ---
/**
 * @brief Displays the menu options to the user.
 *
 * This function prints the available menu options for the user to choose from.
 */
void DisplayMenuOptions() {
    std::cout << "\n  1. Load Data Structure" << std::endl;
    std::cout << "  2. Print Course List" << std::endl;
    std::cout << "  3. Print Course" << std::endl;
    std::cout << "  9. Exit" << std::endl;
    std::cout << "\nWhat would you like to do? ";
}

/**
 * @brief Gets the user's menu selection input.
 *
 * This function reads the user's input from stdin and validates it against the available menu options.
 * If the input is invalid, an error message is displayed and std::nullopt is returned.
 *
 * @return An optional containing the selected MenuOption if valid, or std::nullopt if invalid.
 */
std::optional<MenuOption> GetUserInput() {
    ushort userInput;

    // Get user input from stdin
    std::cin >> userInput;

    // Check if user input is valid (i.e. not empty, wrong type, etc...)
    if (std::cin.fail()) {
        std::cin.clear();               // Clear the error flag
        std::cin.ignore(9999999, '\n'); // Ignores 9999999 characters up to the next newline character
        std::cerr << "Invalid input. Please enter a valid menu option." << std::endl;
        return std::nullopt;
    } else if (MENU_OPTIONS_MAP.find(userInput) == MENU_OPTIONS_MAP.end()) {
        std::cerr << userInput << " is invalid input. Please enter a valid menu option." << std::endl;
        return std::nullopt;
    }

    // If it passes all of the above conditions, userInput is (should be) valid and the
    // corresponding MenuOption can be returned
    return MENU_OPTIONS_MAP.at(userInput);
}

/**
 * @brief Gets a valid menu selection from the user.
 *
 * This function displays the menu options and repeatedly prompts the user for input until a valid
 * menu option is selected.
 *
 * @return The valid MenuOption selected by the user.
 */
MenuOption GetValidMenuSelection() {
    DisplayMenuOptions();

    std::optional<MenuOption> userSelection = GetUserInput(); // MIGHT? contain a valid MenuOption

    // If the methods GetUserInput() returns a value, it must be valid and can be returned
    if (userSelection.has_value()) {
        return userSelection.value();
    } else {
        return GetValidMenuSelection(); // Recursive call itself until a valid input is received
    }
}

/**
 * @brief Main menu loop for the course planner.
 *
 * This function handles the main menu loop, allowing the user to select various options such as
 * loading the data structure, printing the course list, printing a specific course, or exiting the program.
 *
 * @param structure Pointer to the binary search tree containing the courses.
 * @param filePath The path to the CSV file containing the course data.
 * @return false if the user chooses to exit the program, true otherwise.
 */
bool MainMenu(BST* structure, const std::string& filePath) {
    MenuOption userSelection = GetValidMenuSelection(); // Holds a valid MenuOption from the user
    std::string inputCourseName;

    // HANDLE USER SELECTION
    switch (userSelection) {
        case MenuOption::LOAD_DATA_STRUCTURE: // Menu Option 1: Load Data Structure
            std::cout << "Loading data structure..." << std::endl;
            if (LoadDataStructure(structure, filePath)) {
                std::cout << "Data structure loaded successfully." << std::endl;
            } else {
                std::cerr << "Data structure could not be loaded." << std::endl;
            }
            break;
        case MenuOption::PRINT_COURSE_LIST: // Menu Option 2: Print Course List
            // ENSURE THE COURSE LIST IS NOT EMPTY
            if (structure->IsEmpty()) {
                std::cout << "The course list is empty. Please load the data structure first." << std::endl;
                break;
            }

            std::cout << "Course List:" << std::endl;
            PrintCourseList(structure);
            break;
        case MenuOption::PRINT_COURSE: // Menu Option 3: Print Course
            // ENSURE THE COURSE LIST IS NOT EMPTY
            if (structure->IsEmpty()) {
                std::cout << "The course list is empty. Please load the data structure first." << std::endl;
                break;
            }

            std::cout << "Enter the course name: ";
            std::cin >> inputCourseName;
            PrintCourse(structure, inputCourseName);
            break;
        case MenuOption::EXIT_PROGRAM: // Menu Option 9: Exit Program
            std::cout << "Thank you for using the course planner!" << std::endl;
            return false;
    }

    return true;
}
// --- END MENU ---

// --- MAIN ENTRY POINT ---
/**
 * @brief Main entry point for the course planner program.
 *
 * This function initializes the program, sets up the file path for the CSV file containing course data,
 * and enters the main application loop. The file path can be provided as a command-line argument; if not,
 * a default file path is used.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments - Should contain a file path to a CSV file.
 * @return int Exit status of the program.
 */
int main(int argc, char* argv[]) {
    const std::string DEFAULT_FILE_PATH = "CS_300_ABCU_Advising_Program_Input.csv"; // Default file path
    std::string filePath;  // The file path to the CSV file containing course data.
    bool isRunning = true; // Indicates whether the program should continue running.
    BST structure;         // The binary search tree that holds the courses.

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
