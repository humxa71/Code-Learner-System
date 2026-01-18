#include <iostream>  // For standard input and output operations
#include <fstream>   // For file stream operations like reading and writing
#include <string>    // For using the string data type

using namespace std; // Uses the standard namespace to simplify code

// ============================================================
// CLASS: SystemEngine
// Purpose: Handles checking for existing credentials and login logic.
// ============================================================
class SystemEngine
{
public:
    // Checks if a specific credential file exists to decide if setup is needed
    bool isSetupNeeded(string fileName)
    {
        ifstream credentialFile(fileName); // Tries to open the specific file
        bool fileMissing = !credentialFile.is_open(); // If not open, the file does not exist
        credentialFile.close(); // Closes the file handle
        return fileMissing; // Returns true if setup is required
    }

    // Interactive setup to define custom usernames and passwords for a role
    void runRoleSetup(string fileName, string roleTitle)
    {
        string setupUserName; // Descriptive variable for the chosen username
        string setupPassword; // Descriptive variable for the chosen password
        cout << "==== " << roleTitle << " INITIAL SETUP ====" << endl; // Title
        cout << "Set " << roleTitle << " Username: "; // Prompt
        cin >> setupUserName; // Input name
        cout << "Set " << roleTitle << " Password: "; // Prompt
        cin >> setupPassword; // Input password

        ofstream credentialOutFile(fileName); // Opens the specific file for writing
        credentialOutFile << setupUserName << " " << setupPassword; // Saves credentials
        credentialOutFile.close(); // Closes the file
        cout << roleTitle << " setup complete! Please login now." << endl; // Confirmation
    }

    // Verifies if both name and password match the stored credentials in a file
    bool loginCheck(string fileName, string inputName, string inputPass)
    {
        ifstream loginFile(fileName); // Opens the specific credential file
        string storedUserName; // Descriptive variable to hold name from file
        string storedPassword; // Descriptive variable to hold pass from file
        if (loginFile >> storedUserName >> storedPassword) // Reads both from the file
        {
            loginFile.close(); // Closes file
            return (storedUserName == inputName && storedPassword == inputPass); // Compare
        }
        loginFile.close(); // Closes if file is empty
        return false; // Return failure if no match
    }

    // Utility function to print any text file content line by line
    void showData(string fileName, string title)
    {
        ifstream dataFile(fileName); // Opens the requested log file
        string dataLine; // Variable for each line
        cout << "\n--- " << title << " ---" << endl; // Prints a header
        if (!dataFile) // Handles missing files
        {
            cout << "No records found." << endl;
            return;
        }
        while (getline(dataFile, dataLine)) // Reads and prints every line
        {
            cout << dataLine << endl;
        }
        dataFile.close(); // Closes the file
    }
};

// ============================================================
// BASE CLASS: User (Principle: Abstraction & Inheritance)
// ============================================================
class User
{
protected:
    string name; // Stores the user's name for the session
    bool loggedIn; // Controls if the user stays in the portal menu
public:
    User(string n) // Constructor called when an object is created
    {
        name = n; // Assigns the user's name
        loggedIn = true; // Sets the session to active
        cout << "\nWelcome to the Portal, " << name << "!" << endl; // Welcome message
    }
    virtual ~User() // Virtual Destructor called on logout
    {
        cout << "Goodbye, " << name << "! Session closed." << endl; // Goodbye message
    }

    virtual void displayMenu() = 0; // Pure virtual function for menus
    virtual void executeTask(SystemEngine& engine) = 0; // Pure virtual for tasks

    bool isSessionActive() // Getter for portal loop control
    {
        return loggedIn;
    }
};

// ============================================================
// ROLE: Admin (Includes Auto-Add and Statistics)
// ============================================================
class Admin : public User
{
public:
    Admin(string n) : User(n) {} // Call base constructor

    void displayMenu() override
    {
        cout << "\n[ADMIN PORTAL]" << endl; // Header
        cout << "1. Auto-Add Default Quiz Questions" << endl; // Option 1
        cout << "2. Manually Add Questions" << endl; // Option 2
        cout << "3. View Class Average Score" << endl; // Option 3
        cout << "4. Logout" << endl; // Option 4
    }

    void executeTask(SystemEngine& engine) override
    {
        int adminChoice; // Descriptive variable for menu choice
        cout << "Choice: "; // Prompt
        cin >> adminChoice; // Input choice

        if (adminChoice == 1) // Auto-Add functionality
        {
            ofstream autoQuizFile("quiz.txt"); // Reset file and open
            autoQuizFile << "1|Extension of C++ file?|.cpp" << endl; // Level 1
            autoQuizFile << "1|Is C++ case-sensitive?|yes" << endl; // Level 1
            autoQuizFile << "1|Standard output keyword?|cout" << endl; // Level 1
            autoQuizFile << "2|Data type for integers?|int" << endl; // Level 2
            autoQuizFile << "2|Size of char in bytes?|1" << endl; // Level 2
            autoQuizFile << "2|Data type for decimals?|float" << endl; // Level 2
            autoQuizFile << "3|Is int primitive?|yes" << endl; // Level 3
            autoQuizFile << "3|First index of array?|0" << endl; // Level 3
            autoQuizFile << "3|Symbol for address?|&" << endl; // Level 3
            autoQuizFile.close(); // Close file
            cout << "Default questions added automatically!" << endl;
        }
        else if (adminChoice == 2) // Manual add logic
        {
            int selectedLevel; // Level tracker
            int questionQty; // Question count
            cout << "Level (1, 2, or 3): "; cin >> selectedLevel;
            cout << "Quantity: "; cin >> questionQty;
            cin.ignore(); // Clear buffer
            ofstream manualQuizFile("quiz.txt", ios::app); // Open in append mode
            for (int i = 1; i <= questionQty; i++) // Loop to add
            {
                string questionText; // Text
                string answerText; // Text
                cout << "Question " << i << ": "; getline(cin, questionText);
                cout << "Answer: "; getline(cin, answerText);
                manualQuizFile << selectedLevel << "|" << questionText << "|" << answerText << endl;
            }
            manualQuizFile.close(); // Close
        }
        else if (adminChoice == 3) // Admin Statistics
        {
            ifstream progressFile("results.txt"); // Open file
            string studentName; // Name
            int quizLevel; // Level
            int studentScore; // Score
            float totalScore = 0; // Accumulator
            float totalCount = 0; // Counter
            while (progressFile >> studentName >> quizLevel >> studentScore)
            {
                totalScore += studentScore; // Accumulate
                totalCount++; // Increment
            }
            if (totalCount > 0) cout << "Class Average: " << (totalScore / totalCount) << "/3" << endl;
            progressFile.close(); // Close
        }
        else if (adminChoice == 4) { loggedIn = false; } // Logout
    }
};

// ============================================================
// ROLE: Child (Level Unlocking & Star Rating)
// ============================================================
class Child : public User
{
public:
    Child(string n) : User(n) {} // Constructor

    void displayMenu() override
    {
        cout << "\n[CHILD ZONE]" << endl; // Header
        cout << "1. Level 1\n2. Level 2\n3. Level 3\n4. Help Request\n5. Logout" << endl; // Options
    }

    void executeTask(SystemEngine& engine) override
    {
        int studentChoice; // Choice variable
        cout << "Choice: "; cin >> studentChoice;
        if (studentChoice >= 1 && studentChoice <= 3) // Quiz path
        {
            if (studentChoice > 1) // LEVEL UNLOCKING LOGIC
            {
                ifstream resultCheckFile("results.txt"); // Open results
                string recordName; // Variable
                int recordLevel; // Variable
                int recordScore; // Variable
                bool unlocked = false; // Flag
                while (resultCheckFile >> recordName >> recordLevel >> recordScore)
                {
                    if (recordName == name && recordScore >= 2) { unlocked = true; } // Check
                }
                resultCheckFile.close(); // Close
                if (!unlocked) { cout << "LOCKED! Pass previous level with 2+ stars." << endl; return; } // Exit
            }


            ifstream quizContentFile("quiz.txt"); // Open quiz bank
            int fileLevel; // Variable
            int pointsEarned = 0; // Tracker
            string questionStr; // Text
            string answerStr; // Text
            while (quizContentFile >> fileLevel) // EXTRACTION LOGIC
            {
                quizContentFile.ignore(); getline(quizContentFile, questionStr, '|'); getline(quizContentFile, answerStr);
                if (fileLevel == studentChoice) // Match check
                {
                    string inputAnswer; // Variable
                    cout << "\nQ: " << questionStr << "\nAns: "; cin >> inputAnswer;
                    if (inputAnswer == answerStr) { cout << "Correct!"; pointsEarned++; }
                    else cout << "Wrong!";
                }
            }
            quizContentFile.close(); // Close
            cout << "\nScore: " << pointsEarned << "/3 Stars: ";
            for (int i = 0; i < pointsEarned; i++) cout << "* "; // STAR RATING
            cout << endl; // New line

            ofstream resultSaveFile("results.txt", ios::app); // Save results
            resultSaveFile << name << " " << studentChoice << " " << pointsEarned << endl; // Save
            resultSaveFile.close(); // Close
        }
        else if (studentChoice == 4) // Help logic
        {
            string helpMessage; // Variable
            cout << "Question: "; cin.ignore(); getline(cin, helpMessage);
            ofstream helpRequestFile("help.txt", ios::app); helpRequestFile << name << ": " << helpMessage << endl; helpRequestFile.close();
        }
        else if (studentChoice == 5) { loggedIn = false; } // Logout
    }
};

// ==========================================
// ROLE: Teacher (Supports replying to students)
// ==========================================
class Teacher : public User
{
public:
    Teacher(string n) : User(n) {} // Constructor
    void displayMenu() override { cout << "\n[TEACHER PORTAL]\n1. View Help\n2. Reply\n3. Logout\nChoice: "; }
    void executeTask(SystemEngine& engine) override
    {
        int teacherAction; // Variable
        cin >> teacherAction; // Input
        if (teacherAction == 1) engine.showData("help.txt", "REQUESTS");
        else if (teacherAction == 2) // Response logic
        {
            string studentTarget; // Variable
            string teacherMsg; // Variable
            cout << "To Student: "; cin >> studentTarget;
            cout << "Msg: "; cin.ignore(); getline(cin, teacherMsg);
            ofstream teacherReplyFile("replies.txt", ios::app); teacherReplyFile << "To " << studentTarget << ": " << teacherMsg << endl; teacherReplyFile.close();
        }
        else if (teacherAction == 3) loggedIn = false;
    }
};

// ==========================================
// ROLE: Parent
// ==========================================
class Parent : public User
{
public:
    Parent(string n) : User(n) {} // Constructor
    void displayMenu() override { cout << "\n[PARENT MENU]\n1. Register Child\n2. View Results\n3. Logout\nChoice: "; }
    void executeTask(SystemEngine& engine) override
    {
        int parentAction; // Variable
        cin >> parentAction; // Input
        if (parentAction == 1) // Register
        {
            string childToAdd; // Variable
            cout << "Enter Child Name: "; 
            cin >> childToAdd;
            ofstream childDatabaseFile("children.txt", ios::app);
            childDatabaseFile << childToAdd << endl;
            childDatabaseFile.close();
        }
        else if (parentAction == 2) engine.showData("results.txt", "SCORES");
        else if (parentAction == 3) loggedIn = false;
    }
};

/**
 * ============================================================
 * MAIN CONTROLLER
 * ============================================================
 */
int main()
{
    SystemEngine portalLogic; // Create system engine object

    while (true) // Main Loop
    {
        cout << "\n==== MAIN PORTAL ====\n1. Admin\n2. Teacher\n3. Parent\n4. Child\n5. Exit\nRole: ";
        int userRole; // Variable
        cin >> userRole;
        if (userRole == 5) { 
            cout << "Goodbye!" << endl;
            break;
        } // Exit

        User* sessionPointer = nullptr; // Polymorphic pointer
        string loginName; // Variable
        string loginPass; // Variable

        // CHOICE-BASED SETUP AND LOGIN
        if (userRole == 1) // Admin
        {
            if (portalLogic.isSetupNeeded("admin_creds.txt")) { portalLogic.runRoleSetup("admin_creds.txt", "Admin"); }
            else
            {
                cout << "Username: "; cin >> loginName; cout << "Pass: ";
                cin >> loginPass;
                if (portalLogic.loginCheck("admin_creds.txt", loginName, loginPass)) 
                    sessionPointer = new Admin(loginName);
            }
        }
        else if (userRole == 2) // Teacher
        {
            if (portalLogic.isSetupNeeded("teacher_creds.txt")) { portalLogic.runRoleSetup("teacher_creds.txt", "Teacher"); }
            else
            {
                cout << "Username: "; cin >> loginName; cout << "Pass: "; cin >> loginPass;
                if (portalLogic.loginCheck("teacher_creds.txt", loginName, loginPass))
                    sessionPointer = new Teacher(loginName);
            }
        }
        else if (userRole == 3) // Parent
        {
            if (portalLogic.isSetupNeeded("parent_creds.txt")) { portalLogic.runRoleSetup("parent_creds.txt", "Parent"); }
            else
            {
                cout << "Username: "; cin >> loginName; cout << "Pass: ";
                cin >> loginPass;
                if (portalLogic.loginCheck("parent_creds.txt", loginName, loginPass))
                    sessionPointer = new Parent(loginName);
            }
        }
        else if (userRole == 4) // Child
        {
            cout << "Username: "; cin >> loginName;
            ifstream childAuthFile("children.txt"); string lineBuffer; bool registered = false;
            while (childAuthFile >> lineBuffer){ 
                if (lineBuffer == loginName) 
                    registered = true;
            }
            childAuthFile.close();
            if (registered) sessionPointer = new Child(loginName); else cout << "Not registered!" << endl;
        }

        if (sessionPointer) // Portal session execution
        {
            while (sessionPointer->isSessionActive()) { sessionPointer->displayMenu(); 
            sessionPointer->executeTask(portalLogic); }
            delete sessionPointer; // Cleanup
        }
    }
    return 0; // Terminate
}