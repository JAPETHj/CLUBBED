#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>

using namespace std;

class User {
protected:
    string username;
    string password;

public:
    User(const string& username, const string& password) : username(username), password(password) {}

    virtual bool login() = 0;

    string getUsername() const { return username; }
};

class Admin : public User {
public:
    Admin(const string& username, const string& password) : User(username, password) {}

    bool login() override {
        string enteredPassword;
        cout << "Enter password: ";
        getline(cin, enteredPassword);
        return enteredPassword == password;
    }

  void viewAllocatedFunds(const map<string, double>& allocatedFunds, double fundLimit) const {
    cout << "\n----- Allocated Funds -----" << endl;
    for (const auto& pair : allocatedFunds) {
        double remainingBalance = fundLimit - pair.second;
        cout << "Club: " << pair.first << ", Allocated Funds: Rs" << pair.second << ", Remaining Balance: Rs" << remainingBalance << endl;
    }
}
 
};

class ClubMember : public User {
private:
    string clubName;
    double requestedFunds;

public:
    ClubMember(const string& username, const string& password, const string& clubName) : User(username, password), clubName(clubName), requestedFunds(0.0) {}

    bool login() override {
        string enteredPassword;
        cout << "Enter password: ";
        getline(cin, enteredPassword);
        return enteredPassword == password;
    }

    string getClubName() const { return clubName; }

    // Functionalities for requesting funds, writing blogs (implemented later)
    void requestFunds(double amount) {
        requestedFunds += amount;
    }

    double getRequestedFunds() const {
        return requestedFunds;
    }

    void resetRequestedFunds() {
        requestedFunds = 0.0;
    }

    void notifyAllocation(bool allocated) {
        if (allocated) {
            cout << "Funds allocated successfully." << endl;
        } else {
            cout << "Failed to allocate funds. Please try again later." << endl;
        }
    }
};

class Student : public User {
public:
    Student(const string& username, const string& password) : User(username, password) {}

    bool login() override {
        string enteredPassword;
        cout << "Enter password: ";
        getline(cin, enteredPassword);
        return enteredPassword == password;
    }

   
};

class ClubManagementSystem {
private:
    vector<User*> users;
    vector<string> blogs;
    double fundLimit;
    map<string, double> allocatedFunds; // store allocated funds for each club

public:
    ClubManagementSystem(double fundLimit) : fundLimit(fundLimit) {}

    ~ClubManagementSystem() {
        // Clean up dynamically allocated users
        for (User* user : users) {
            delete user;
        }
    }

    void registerUser(const string& username, const string& password, const string& userType) {
        if (userType == "Admin") {
            users.push_back(new Admin(username, password));
        } else if (userType == "ClubMember") {
            string clubName;
            cout << "Enter club name: ";
            getline(cin, clubName);
            users.push_back(new ClubMember(username, password, clubName));
        } else if (userType == "Student") {
            users.push_back(new Student(username, password));
        } else {
            cout << "Invalid user type." << endl;
        }
    }

    User* loginUser() {
        string username, password;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        for (User* user : users) {
            if (user->getUsername() == username && user->login()) {
                return user;
            }
        }

        cout << "Login failed." << endl;
        return nullptr;
    }

   
   void manageClubs() {
    
    cout << "----- Manage Clubs -----" << endl;
    cout << "1. Increase Budget for a Club" << endl;
    cout << "2. Decrease Budget for a Club" << endl;
    cout << "3. Create New Club" << endl; // Added option to create a new club
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    switch (choice) {
        case 1: {
            // Increase budget for a club
            string clubName;
            double amount;
            cout << "Enter club name: ";
            getline(cin, clubName);
            cout << "Enter amount to increase: Rs";
            cin >> amount;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
           
            if (allocatedFunds.find(clubName) != allocatedFunds.end()) {
                allocatedFunds[clubName] += amount;
                cout << "Budget increased successfully for club '" << clubName << "' by Rs" << amount << endl;
            } else {
                cout << "Club '" << clubName << "' not found." << endl;
            }
            break;
        }
        case 2: {
         
            string clubName;
            double amount;
            cout << "Enter club name: ";
            getline(cin, clubName);
            cout << "Enter amount to decrease: Rs";
            cin >> amount;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
          
            if (allocatedFunds.find(clubName) != allocatedFunds.end()) {
                if (allocatedFunds[clubName] >= amount) {
                    allocatedFunds[clubName] -= amount;
                    cout << "Budget decreased successfully for club '" << clubName << "' by Rs" << amount << endl;
                } else {
                    cout << "Error: Amount exceeds allocated funds for club '" << clubName << "'." << endl;
                }
            } else {
                cout << "Club '" << clubName << "' not found." << endl;
            }
            break;
        }
        case 3: {
            string newClubName;
            double amount;
            cout << "Enter name of the new club: ";
            getline(cin, newClubName);
            cout << "Enter initial funds for the new club: Rs";
            cin >> amount;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (allocatedFunds.find(newClubName) == allocatedFunds.end()) {
                allocatedFunds[newClubName] = amount;
                cout << "New club '" << newClubName << "' created and funds allocated successfully." << endl;
            } else {
                cout << "Club '" << newClubName << "' already exists." << endl;
            }
            break;
        }
        default:
            cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
    }
}

    void allocateFunds(Admin* admin, const string& clubName, double amount) {
      
        if (!admin) {
            cout << "Error: Admin not found." << endl;
            return;
        }

      
        if (amount > fundLimit) {
            cout << "Error: Allocated amount exceeds fund limit." << endl;
            return;
        }

        
        if (allocatedFunds.find(clubName) == allocatedFunds.end()) {
            cout << "Error: Club '" << clubName << "' not found." << endl;
            return;
        }

        
        double remainingBalance = fundLimit - allocatedFunds[clubName];
        if (amount > remainingBalance) {
            cout << "Error: Insufficient funds. Allocation failed." << endl;
            return;
        }

        
        allocatedFunds[clubName] += amount;
        cout << "Funds allocated successfully. Remaining balance for club '" << clubName << "': Rs" << remainingBalance << endl;
    }

    // Functionalities for ClubMember (request funds, write blogs)
    void requestFunds(ClubMember* clubMember, double amount) {
        // Check if the club member exists
        if (!clubMember) {
            cout << "Error: Club member not found." << endl;
            return;
        }

        // Check if the requested amount exceeds the fund limit
        if (clubMember->getRequestedFunds() + amount > fundLimit) {
            cout << "Request exceeds fund limit. Please request a smaller amount." << endl;
            return;
        }

        // Otherwise, proceed with the request
        clubMember->requestFunds(amount);
        cout << "Funds requested by club member '" << clubMember->getUsername() << "': Rs" << amount << endl;
    }

    void writeBlog(const string& title, const string& content) {
        // Implement functionalities for writing blogs (improve with files later)
        blogs.push_back("*" + title + "*\n" + content + "\n");
    }

    // Functionalities for Student (read blogs, react to them)
    void readBlogs() {
        if (blogs.empty()) {
            cout << "No blogs available yet." << endl;
            return;
        }

        cout << "\n----- Blogs -----" << endl;
        for (size_t i = 0; i < blogs.size(); ++i) {
            cout << "Blog #" << (i + 1) << ":\n" << blogs[i] << endl;
        }
    }

    const map<string, double>& getAllocatedFunds() const {
        return allocatedFunds;
    }
     void viewAllocatedFunds(const map<string, double>& allocatedFunds, double fundLimit) const {
    cout << "\n----- Allocated Funds -----" << endl;
    for (const auto& pair : allocatedFunds) {
        double remainingBalance = fundLimit - pair.second;
        cout << "Club: " << pair.first << ", Allocated Funds: Rs" << pair.second << ", Remaining Balance: Rs" << remainingBalance << endl;
    }
}
};

int main() {
    // Set a fund limit
    double fundLimit = 1000.0;

    ClubManagementSystem cms(fundLimit);

    int choice;
    do {
        cout << "\n----- Clubbed NITT systemdesign-----\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Write Blog (Club Member)\n";
        cout << "4. Read Blogs (Student)\n";
        cout << "5. Manage clubs(Admin)\n";
        cout << "6. Request Funds (Club Member)\n";
        cout << "7. View Allocated Funds (Admin)\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (choice) {
            case 1: {
                string username, password, userType;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                cout << "Enter user type (Admin/ClubMember/Student): ";
                getline(cin, userType);
                cms.registerUser(username, password, userType);
                break;
            }
            case 2: {
                User* currentUser = cms.loginUser();
                if (currentUser) {
                    if (dynamic_cast<Admin*>(currentUser)) {
                        cout << "Admin logged in.\n";
                        // Implement admin functionalities here
                    } else if (dynamic_cast<ClubMember*>(currentUser)) {
                        cout << "Club Member logged in.\n";
                        // Implement club member functionalities here
                    } else if (dynamic_cast<Student*>(currentUser)) {
                        cout << "Student logged in.\n";
                        // Implement student functionalities here
                    }
                }
                break;
            }
            case 3: {
                string title, content;
                cout << "Enter blog title: ";
                getline(cin, title);
                cout << "Enter blog content: ";
                getline(cin, content);
                cms.writeBlog(title, content);
                break;
            }
            case 4: {
                cout << "\n----- Blogs -----\n";
                cms.readBlogs();
                break;
            }
            
            case 5: {
    User* currentUser = cms.loginUser();
    Admin* admin = dynamic_cast<Admin*>(currentUser);
    if (admin) {
        cms.manageClubs();
    } else {
        cout << "Only Admin can manage clubs." << endl;
    }
    break;
}
            case 6: {
                // Request funds (Club Member)
                double amount;
                cout << "Enter amount to request: Rs";
                cin >> amount;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
                // Get the current user
                User* currentUser = cms.loginUser();
                ClubMember* clubMember = dynamic_cast<ClubMember*>(currentUser);
                if (clubMember) {
                    // Request funds only if the amount is valid
                    if (amount > 0 && amount <= fundLimit) {
                        // Request funds
                        cms.requestFunds(clubMember, amount);
                    } else {
                        cout << "Invalid amount. Request failed." << endl;
                    }
                }
                break;
            }
            case 7: {
                User* currentUser = cms.loginUser();
                Admin* admin = dynamic_cast<Admin*>(currentUser);
                if (admin) {
                    cms.viewAllocatedFunds(cms.getAllocatedFunds(), fundLimit);
                }
                break;
            }
            case 8:
                cout << "Exiting... Goodbye!\n";
                break;
    
            default:
                cout << "Invalid choice. Please enter a number from 1 to 8.\n";
        }
    } while (choice != 8);

    return 0;
}
