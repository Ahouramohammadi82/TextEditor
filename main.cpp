#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

const string EXIT_COMMAND = "exit"; //  command to exit the program
const string CREATE_COMMAND = "create"; //  command to create a new file
const string SEARCH_COMMAND = "search"; //  command to search in a file
const string EDIT_COMMAND = "edit"; //  command for interactive editing
const string REPLACE_COMMAND = "replace"; //  command for replacing

// A function to create a new file with the given name and content
void create_file(const string& file_name, const string& content) {
    // Open the file in output mode
    ofstream file(file_name);
    // Check if the file is opened successfully
    if (file.is_open()) {
        file << content;
        file.close();
        cout << "File " << file_name << " created successfully.\n";
    }
    else {
        cout << "Error: Could not create file " << file_name << ".\n";
    }
}

// A function to search for a phrase in a file and print the matching lines
void search_file(const string& file_name, const string& phrase) {
    // Open the file in input mode
    ifstream file(file_name);
    // Check if the file is opened successfully
    if (file.is_open()) {
        // Declare a variable to store each line of the file
        string line;
        // Declare a variable to store the line number
        int line_number = 0;
        // Declare a variable to store the number of matches
        int match_count = 0;
        // Loop through each line of the file
        while (getline(file, line)) {
            // Increment the line number
            line_number++;
            // Check if the phrase is found in the line
            if (line.find(phrase) != string::npos) {
                // Increment the match count
                match_count++;
                // Print the line number and the line
                cout << "Line " << line_number << ": " << line << "\n";
            }
        }
        // Close the file
        file.close();
        // Print the number of matches
        cout << "Found " << match_count << " matches for " << phrase << " in " << file_name << ".\n";
    }
    else {
        // Print an error message
        cout << "Error: Could not open file " << file_name << ".\n";
    }
}

void edit_file(const string& file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << file_name << ".\n";
        return;
    }

    // Read the existing content
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Split the content into lines
    vector<string> lines;
    istringstream iss(content);
    string line;
    while (getline(iss, line)) {
        lines.push_back(line);
    }

    // Interactive editing loop
    while (true) {
        // Display the current content
        cout << "Current content:\n";
        for (size_t i = 0; i < lines.size(); ++i) {
            cout << i + 1 << ": " << lines[i] << "\n";
        }

        // Prompt the user for input
        cout << "Enter line number to edit (0 to add a new line, -1 to save and exit): ";
        int line_number;
        cin >> line_number;

        // Check if the user wants to save and exit
        if (line_number == -1) {
            ofstream new_file(file_name);
            for (const auto& edited_line : lines) {
                new_file << edited_line << "\n";
            }
            new_file.close();
            cout << "Changes saved. Exiting interactive edit mode.\n";
            break;
        }

        // Check if the user wants to add a new line
        if (line_number == 0) {
            cout << "Enter new text for the new line: ";
            string new_line;
            cin.ignore(); // Clear newline from previous input
            getline(cin, new_line);
            lines.push_back(new_line);
        } else if (line_number > 0 && line_number <= lines.size()) {
            // Edit an existing line
            cout << "Enter new text for line " << line_number << ": ";
            string new_text;
            cin.ignore(); // Clear newline from previous input
            getline(cin, new_text);
            lines[line_number - 1] = new_text;
        } else {
            cout << "Invalid line number. Please try again.\n";
        }
    }
}

void replace_occurrences(vector<string>& lines, const string& old_string, const string& new_string) {
    for (auto& line : lines) {
        size_t pos = 0;
        while ((pos = line.find(old_string, pos)) != string::npos) {
            line.replace(pos, old_string.length(), new_string);
            pos += new_string.length();
        }
    }
}

void handle_replace_command(const string& file_name) {
    ifstream file(file_name);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << file_name << ".\n";
        return;
    }

    // Read the existing content
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Split the content into lines
    vector<string> lines;
    istringstream iss(content);
    string line;
    while (getline(iss, line)) {
        lines.push_back(line);
    }

    string old_string, new_string;
    cout << "Enter the string to replace: ";
    cin >> old_string;

    cout << "Enter the new string: ";
    cin >> new_string;

    replace_occurrences(lines, old_string, new_string);

    // Save the modified content back to the file
    ofstream new_file(file_name);
    for (const auto& edited_line : lines) {
        new_file << edited_line << "\n";
    }
    new_file.close();

    cout << "All occurrences of '" << old_string << "' replaced with '" << new_string << "'.\n";
}



// The main function
int main() {
    // Declare a variable to store the user's command
    string command;
    // Declare a variable to store the file name
    string file_name;
    // Declare a variable to store the file content
    string content;
    // Declare a variable to store the search phrase
    string search_phrase;
    // Declare a variable to store the old phrase
    string old_phrase;
    // Declare a variable to store the new phrase
    string new_phrase;
    // Print a welcome message
    cout << "Welcome to the command-line text editor.\n";
    // Print the available commands
    cout << "The available commands are:\n";
    cout << CREATE_COMMAND << " - to create a new file\n";
    cout << SEARCH_COMMAND << " - to search in a file\n";
    cout << EDIT_COMMAND << " - to edit in a file\n";
    cout << EXIT_COMMAND << " - to exit the program\n";
    cout << REPLACE_COMMAND << " - to replace something\n";
    // Loop until the user enters the exit command
    while (true) {
        // Prompt the user to enter a command
        cout << "Enter a command: ";
        // Read the command from the standard input
        cin >> command;
        // Check if the command is the exit command
        if (command == EXIT_COMMAND) {
            // Break the loop
            break;
        }
            // Check if the command is the create command
        else if (command == CREATE_COMMAND) {
            // Prompt the user to enter the file name
            cout << "Enter the file name: ";
            // Read the file name from the standard input
            cin >> file_name;
            // Prompt the user to enter the file content
            cout << "Enter the file content (end with EOF): ";
            // Ignore the newline character from the previous input
            cin.ignore();
            // Read the file content from the standard input until EOF
            // Read the file content from the standard input until the delimiter "EOF"
            cout << "Enter the file content (type 'EOF' on a new line to finish):\n";
            content = "";
            while (true) {
                string line;
                getline(cin, line);
                if (line == "EOF") {
                    break;
                }
                content += line + "\n";
            }
            // Call the create_file function with the file name and content
            create_file(file_name, content);
        }
            // Check if the command is the search command
        else if (command == SEARCH_COMMAND) {
            // Prompt the user to enter the file name
            cout << "Enter the file name: ";
            // Read the file name from the standard input
            cin >> file_name;
            // Prompt the user to enter the search phrase
            cout << "Enter the search phrase: ";
            // Ignore the newline character from the previous input
            cin.ignore();
            // Read the search phrase from the standard input
            getline(cin, search_phrase);
            // Call the search_file function with the file name and search phrase
            search_file(file_name, search_phrase);
        }
            // Check if the command is the edit command
        else if (command == EDIT_COMMAND) {
            cout << "Enter the file name to edit: ";
            cin >> file_name;
            edit_file(file_name);
            
        } else if (command == REPLACE_COMMAND){
            cout << "Enter the file name to replace: ";
            cin >> file_name;
            handle_replace_command(file_name);
        }
            // If the command is invalid
        else {
            cout << "Error: Invalid command.\n";
        }
    }
}
