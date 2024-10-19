# CS 300 Project Two Guidelines

## Competency

In this project, you will demonstrate your mastery of the following competency:
- Develop code using algorithms and data structures to solve basic programming problems

## Scenario

The academic advisors in the Computer Science department at ABCU are very happy with the planning and design you have completed for the advising assistance software. Now, they would like you to write the code for the application so the department advisors can use it to help when they talk with students.

## Directions

Complete all of your coding in the integrated development environment (IDE). Additional references on the use of this IDE are linked in the Supporting Materials section. As you begin coding, use the data structure that you recommended in Project One to complete the following actions:

1. **Input: Design code to correctly read the course data file.** The program you submit should be a command-line program written in C++. The program should prompt the user to ask for the file name that contains the course data and read that file into course objects that are stored in your chosen data structure. Your data structure will hold course objects.

2. **Menu: Design code to create a menu that prompts a user for menu options.** The menu should include the following options:
    a. Option 1: Load the file data into the data structure. Note that before you can print the course information or the sorted list of courses, you must load the data into the data structure. 
    b. Option 2: Print an alphanumeric list of all the courses in the Computer Science department.
    c. Option 3: Print the course title and the prerequisites for any individual course.
    d. Option 9: Exit the program.

3. **Loading Data Structure: Develop working code to load data from the file into the data structure.**

4. **Course List: Develop working code to sort and print out a list of the courses in the Computer Science program in alphanumeric order, including all math courses.** To print out a course list, use the pseudocode you created previously to guide your work. Then create code that will allow advisers to print a course list in alphanumeric order. Remember that this code should do the following actions:
    a. Sort the course information alphanumerically from lowest to highest.
    b. Print the sorted list to a display.

5. **Course Information: Develop working code to print course information.** This code should allow users to look up a course and print out information about its title and prerequisites. Your program must prompt the user to enter the course number. You will then print the name of the course and the prerequisite course numbers and titles. See Project Two Sample Program Output in the Supporting Documents section.

6. **Industry Standard Best Practices: Apply industry standard best practices in code design.** Your program should display an error message when user input does not fall within the parameters. You should also use in-line comments and appropriate naming conventions to enhance readability and maintainability.

## What to Submit

To complete this project, you must submit the following item:

**Advising Assistance Program**
Submit your ProjectTwo.cpp C++ code file. Ideally, all of your code should be in a single CPP file with all of the necessary functions working correctly without any CSVParser or other header files being used. Do not zip or compress your submission. Make sure the code compiles and runs and has been thoroughly tested.
