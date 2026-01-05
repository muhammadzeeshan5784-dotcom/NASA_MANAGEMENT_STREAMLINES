/**
 * @file nms.cpp
 * @brief NASA Management System (NMS) - Project Horizon
 * @author Muhammad Zeeshan (2025-cS-302)
 * @date January 2026
 * 
 * @details
 * The NASA Management System is a comprehensive console-based application designed to 
 * streamline administrative and operational workflows for space agency simulations. 
 * It provides a multi-tiered access control system (Admin, Staff, Visitor) to manage 
 * various agency modules including mission planning, inventory tracking, personnel 
 * management, and astronomical data exploration.
 * 
 * KEY MODULES:
 * - Flight Control: Mission manifest management, launch simulations, and docking protocols.
 * - Engineering: Inventory management for mission-critical equipment and rover construction.
 * - Science & Research: Database management for planetary and exoplanetary discoveries.
 * - HR & Personnel: Astronaut roster management and recruitment tracking.
 * - Admin Panel: System-wide oversight, funding approvals, and user authorization.
 * 
 * TECHNICAL SPECIFICATIONS:
 * - Language: C++
 * - Platform: Windows (utilizes Win32 API for console manipulation)
 * - Storage: Persistent CSV-based flat-file database system.
 * - UI: ANSI-colored console interface with custom coordinate-based rendering.
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <ctime>
#include <fstream>

using namespace std;

// Colouring Elements
const string RD = "\033[31m";
const string GRN = "\033[32m";
const string YLW = "\033[33m";
const string BL = "\033[34m";
const string MGD = "\033[35m";
const string CYN = "\033[36m";
const string WHT = "\033[37m";
const string GRA = "\033[90m";
const string RST = "\033[0m";

// GLOBAL CONSTANTS
const int MAX_USERS = 200;
const int MAX_HIRES = 200;
const int MAX_MISSIONS = 200;
const int MAX_INVENTORY = 500;
const int MAX_ASTRO = 100;
const int MAX_LOGS = 1000;
const int MAX_PLANETS = 100;

// Functions Prototypes
// Main Menu
bool signUp(string username, string password, string usernames[], string passwords[], string roles[], string departments[], int &count, string logActions[], int &logCount);
int signIn(string username, string password, string usernames[], string passwords[], int count, string logActions[], int &logCount);
void about();
void history();
void exit();
//   Visuals
void gotoxy(int x, int y);
void setCursor(bool visible);
void drawLogo(bool animate = false);
void drawBox(int x, int y, int width, int height);
void animations(string text, int speed);
void clearKeyboardBuffer();
void pause();
void message(string msg);

// Initialization
void init_Database(string usernames[], string passwords[], string roles[], string departments[], int &userCount,
                   string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount,
                   string missionNames[], string missionCodes[], string missionDates[], string missionVehicles[], string missionStatus[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount, double &agencyBudget,
                   string invNames[], string invCats[], double invQtys[], string invUnits[], double invCosts[], int &invCount,
                   string astroNames[], string astroRanks[], string astroStatus[], int &astroCount,
                   string planetNames[], string planetTypes[], double planetDists[], double planetGravs[], string planetAtms[], int &planetCount,
                   string exoNames[], double exoDists[], string exoTypes[], bool exoHabitable[], int &exoCount,
                   string logActions[], int &logCount);

void init_Users(string usernames[], string passwords[], string roles[], string departments[], int &count);
void init_Missions(string names[], string codes[], string vehicles[], string status[], double budgets[], string requesters[], double costs[], int &count, string dates[]);
void init_Inventory(string names[], string cats[], double qtys[], string units[], double costs[], int &count);
void init_Astronauts(string names[], string ranks[], string status[], int &count);
void init_Planets(string names[], string types[], double dists[], double gravs[], string atms[], int &count);
void init_Exoplanets(string names[], double dists[], string types[], bool habitable[], int &count);

// Dashboards
void dashboard_Main(string usernames[], string passwords[], string roles[], int userLimit, int &currentUserIdx,
                    string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount,
                    string missionNames[], string missionCodes[], string missionDates[], string missionVehicles[], string missionStatus[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount, double &agencyBudget,
                    string invNames[], string invCats[], double invQtys[], string invUnits[], double invCosts[], int &invCount,
                    string astroNames[], string astroRanks[], string astroStatus[], int &astroCount,
                    string planetNames[], string planetTypes[], double planetDists[], double planetGravs[], string planetAtms[], int &planetCount,
                    string exoNames[], double exoDists[], string exoTypes[], bool exoHabitable[], int &exoCount,
                    string logActions[], int &logCount);

void dashboard_Flight(string usernames[], string roles[], int currentUserIdx,
                      string missionNames[], string missionCodes[], string missionDates[], string missionVehicles[], string missionStatus[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount, double &agencyBudget,
                      string invNames[], double invCosts[], int invCount,
                      string logActions[], int &logCount);
void dashboard_Eng(string invNames[], string invCats[], double invQtys[], string invUnits[], double invCosts[], int &invCount, string logActions[], int &logCount);
void dashboard_Science(string planetNames[], string planetTypes[], double planetDists[], double planetGravs[], string planetAtms[], int &planetCount,
                       string exoNames[], double exoDists[], string exoTypes[], bool exoHabitable[], int &exoCount,
                       string logActions[], int &logCount);
void dashboard_HR(string astroNames[], string astroRanks[], string astroStatus[], int &astroCount);
void dashboard_Admin(string usernames[], string passwords[], string roles[], int &userCount,
                     string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount,
                     string missionNames[], string missionStatus[], string missionDates[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount, double &agencyBudget,
                     string astroNames[], string astroRanks[], string astroStatus[], int &astroCount,
                     string logActions[], int &logCount);

// Internal Features

void flight_Manifest(string names[], string codes[], string dates[], string vehicles[], string status[], string requesters[], int &count);
void flight_Request(string username, string missionNames[], string missionCodes[], string missionDates[], string missionVehicles[], string missionStatus[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount,
                    string invNames[], double invCosts[], int invCount, string logActions[], int &logCount);
void sim_Launch(string names[], string vehicles[], string status[], string requesters[], double costs[], int &count, double agencyBudget, string logActions[], int &logCount, string dates[]);
void sim_Docking();
void eng_Inventory(string names[], string cats[], double qtys[], double costs[], int &count);
void eng_RoverBuilder(string logActions[], int &logCount);
void sci_Planets(string names[], string types[], double dists[], double gravs[], string atms[], int &count);
void sci_Exoplanets(string names[], double dists[], string types[], bool habitable[], int &count);
void sci_AddPlanet(string names[], string types[], double dists[], double gravs[], string atms[], int &count);
void sci_AddExoplanet(string names[], double dists[], string types[], bool habitable[], int &count);
void sci_Decrypt(string logActions[], int &logCount);
void hr_Roster(string names[], string ranks[], string status[], int &count);
void hr_Training();
void career_Menu(string username, string userRole, string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount, string logActions[], int &logCount);
void admin_Hiring(string usernames[], string roles[], int userLimit, string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount,
                  string astroNames[], string astroRanks[], string astroStatus[], int &astroCount, string logActions[], int &logCount);
void admin_Personnel(string usernames[], string passwords[], string roles[], string departments[], int &userCount, string logActions[], int &logCount);
void admin_Missions(string names[], string status[], string dates[], double budgets[], string requesters[], int &count, double &agencyBudget, string logActions[], int &logCount);
void flight_DeleteMission(string names[], string status[], string dates[], string requesters[], double costs[], int &count);
void eng_AddInventory(string names[], string cats[], double qtys[], string units[], double costs[], int &count);
void eng_DeleteInventory(string names[], string cats[], double qtys[], string units[], double costs[], int &count);
void sci_DeletePlanet(string names[], string types[], double dists[], double gravs[], string atms[], int &count);
void sci_DeleteExoplanet(string names[], double dists[], string types[], bool habitable[], int &count);
void ops_RoverGame();

// Storage Modules
void saveUsers(string usernames[], string passwords[], string roles[], string departments[], int count);
void loadUsers(string usernames[], string passwords[], string roles[], string departments[], int &count);
void saveHires(string users[], string roles[], string exp[], string status[], string names[], string edu[], int count);
void loadHires(string users[], string roles[], string exp[], string status[], string names[], string edu[], int &count);
void saveMissions(int count, string names[], string status[], string requesters[], double costs[], double budget, string dates[]);
void loadMissions(int &count, string names[], string codes[], string vehicles[], string status[], double budgets[], string requesters[], double costs[], double &agencyBudget, string dates[]);
void saveInventory(int count, string names[], string cats[], double qtys[], string units[], double costs[]);
void loadInventory(int &count, string names[], string cats[], double qtys[], string units[], double costs[]);
void saveAstronauts(int count, string names[], string ranks[], string status[]);
void loadAstronauts(int &count, string names[], string ranks[], string status[]);
void savePlanets(int count, string names[], string types[], double dists[], double gravs[], string atms[]);
void loadPlanets(int &count, string names[], string types[], double dists[], double gravs[], string atms[]);
void saveExoplanets(int count, string names[], double dists[], string types[], bool habitable[]);
void loadExoplanets(int &count, string names[], double dists[], string types[], bool habitable[]);
void saveLogs(int count, string actions[]);
void loadLogs(int &count, string actions[]);
void addLog(string action, string logActions[], int &logCount);

// Input Processors
string getInput(string prompt);
int getInt(string prompt, int min, int max);
double getDouble(string prompt, double min, double max);
bool isValidUsername(string username);
bool isValidPassword(string password);
int safeStoi(string s);
double safeStod(string s);

// Main Function
main()
{
    // Variables Declaration and Initialization
    //  User Data
    string usernames[MAX_USERS], passwords[MAX_USERS], roles[MAX_USERS], departments[MAX_USERS];
    int userCount = 0, currentUserIdx = -1;

    // Hiring Data
    string hireUsers[MAX_HIRES], hireRoles[MAX_HIRES], hireExp[MAX_HIRES], hireStatus[MAX_HIRES];
    string hireNames[MAX_HIRES], hireEdu[MAX_HIRES];
    int hireCount = 0;

    // Mission Data
    string missionNames[MAX_MISSIONS], missionCodes[MAX_MISSIONS], missionDates[MAX_MISSIONS];
    string missionVehicles[MAX_MISSIONS], missionStatus[MAX_MISSIONS], missionRequesters[MAX_MISSIONS];
    double missionBudgets[MAX_MISSIONS], missionCosts[MAX_MISSIONS];
    int missionCount = 0;

    // Agency Finance
    double agencyBudget = 50.0; // Billions

    // Inventory Data
    string invNames[MAX_INVENTORY], invCats[MAX_INVENTORY], invUnits[MAX_INVENTORY];
    double invQtys[MAX_INVENTORY], invCosts[MAX_INVENTORY];
    int invCount = 0;

    // Astronaut Data
    string astroNames[MAX_ASTRO], astroRanks[MAX_ASTRO], astroStatus[MAX_ASTRO];
    int astroCount = 0;

    // Planet Data
    string planetNames[MAX_PLANETS], planetTypes[MAX_PLANETS];
    string planetAtms[MAX_PLANETS];
    double planetDists[MAX_PLANETS], planetGravs[MAX_PLANETS];
    int planetCount = 0;

    // Exoplanet Data
    string exoNames[MAX_PLANETS], exoTypes[MAX_PLANETS];
    double exoDists[MAX_PLANETS];
    bool exoHabitable[MAX_PLANETS];
    int exoCount = 0;

    // Log Data
    string logActions[MAX_LOGS];
    int logCount = 0;

    // --- INITIALIZATION ---
    init_Database(usernames, passwords, roles, departments, userCount,
                  hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount,
                  missionNames, missionCodes, missionDates, missionVehicles, missionStatus, missionBudgets, missionRequesters, missionCosts, missionCount, agencyBudget,
                  invNames, invCats, invQtys, invUnits, invCosts, invCount,
                  astroNames, astroRanks, astroStatus, astroCount,
                  planetNames, planetTypes, planetDists, planetGravs, planetAtms, planetCount,
                  exoNames, exoDists, exoTypes, exoHabitable, exoCount,
                  logActions, logCount);

    // Root Instructions
    SetConsoleTitle("NASA HORIZON - PROJECT TITAN");
    setCursor(false);
    srand(time(0)); // Seed RNG
    system("cls");
    cout << RD << "Please Wait! Initializing the Boot";
    Sleep(500);
    cout << ".";
    Sleep(700);
    cout << ".";
    Sleep(900);
    cout << ".";
    drawLogo(true);
    Sleep(500);

    // Main Menu
    while (true)
    {
        // Main menu interface
        drawLogo(false);
        clearKeyboardBuffer();
        int boxX = 25;
        int boxY = 12;

        gotoxy(boxX, boxY + 1);
        cout << WHT << "1. Sign In" << RST << endl;

        gotoxy(boxX, boxY + 2);
        cout << WHT << "2. Sign Up" << RST << endl;

        gotoxy(boxX, boxY + 3);
        cout << WHT << "3. History" << RST << endl;

        gotoxy(boxX, boxY + 4);
        cout << WHT << "4. About" << RST << endl;

        gotoxy(boxX, boxY + 5);
        cout << WHT << "5. Exit" << RST << endl;

        gotoxy(boxX - 3, boxY + 7);
        cout << YLW << "Select Option: " << RST;

        char choice = _getch();
        if (choice == '1')
        {
            int attempts = 0;
            bool loginSuccess = false;

            // Takes input and validates it
            while (attempts < 3 && !loginSuccess)
            {
                drawLogo(false);
                drawBox(8, 12, 80, 20);
                gotoxy(35, 13);
                cout << BL << "Sign IN" << RST;
                gotoxy(20, 14);
                cout << YLW << "Welcome back, Let's continue where you left" << RST;

                gotoxy(20, 16);
                string u = getInput("Username: ");
                gotoxy(20, 18);
                string p = getInput("Password: ");

                currentUserIdx = signIn(u, p, usernames, passwords, userCount, logActions, logCount);

                gotoxy(18, 21);
                // if successful  then give control to main dashboard
                if (currentUserIdx != -1)
                {
                    loginSuccess = true;
                    addLog("Login Success: " + u, logActions, logCount);
                    animations(GRN + "Login Successful! Welcome " + u + RST, 15);
                    gotoxy(18, 22);
                    animations(GRN + "Let's Embark on the journey to explore universe" + RST, 10);
                    Sleep(1500);

                    // Hand over Everything to Dashboard
                    dashboard_Main(usernames, passwords, roles, userCount, currentUserIdx,
                                   hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount,
                                   missionNames, missionCodes, missionDates, missionVehicles, missionStatus, missionBudgets, missionRequesters, missionCosts, missionCount, agencyBudget,
                                   invNames, invCats, invQtys, invUnits, invCosts, invCount,
                                   astroNames, astroRanks, astroStatus, astroCount,
                                   planetNames, planetTypes, planetDists, planetGravs, planetAtms, planetCount,
                                   exoNames, exoDists, exoTypes, exoHabitable, exoCount,
                                   logActions, logCount);
                }
                // if login fails due to invalid credentials
                else
                {
                    attempts++;
                    animations(RD + "Oops! Login failed. Seems Aliens destroyed your login data" + RST, 5);

                    if (attempts < 3)
                    {
                        gotoxy(18, 23);
                        cout << YLW << "Attempts remaining: " << (3 - attempts) << RST;
                        gotoxy(18, 24);
                        cout << CYN << "Press any key to try again..." << RST;
                        _getch();
                    }
                    else
                    {
                        gotoxy(18, 23);
                        cout << RD << "Maximum attempts exceeded!" << RST;
                        Sleep(1000);
                        system("cls");
                        drawLogo(false);
                        drawBox(15, 14, 70, 8);
                        gotoxy(28, 16);
                        cout << RD << "LOGIN FAILED!!! - MULTIPLE INVALID ATTEMPTS" << RST;
                        gotoxy(30, 18);
                        cout << YLW << "We are Suspicious you'r an Imposter. Returning to Main Menu..." << RST;
                        Sleep(2000);
                    }
                }
            }
        }
        // For Sign up
        else if (choice == '2')
        {
            drawLogo(false);
            drawBox(8, 12, 80, 20);
            gotoxy(25, 14);
            cout << GRN << "Welcome to NASA Management System" << RST;
            gotoxy(40, 13);
            cout << BL << "Sign UP" << RST;

            string u, p;

            // Takes username and validates it
            while (true)
            {
                gotoxy(17, 16);
                cout << RD << "Instructions--> " << YLW << "Username must be 4-10 characters" << RST;
                gotoxy(17, 17);
                cout << YLW << "NO Space allowed, Just Numerals and Alphabets" << RST;

                gotoxy(25, 18);
                cout << "Username:                     ";
                gotoxy(25, 18);
                u = getInput("Username: ");

                if (isValidUsername(u))
                {
                    break;
                }
                gotoxy(17, 20);
                cout << RD << "Invalid Username! Follow the Instructions" << RST;
                Sleep(900);
                gotoxy(17, 20);
                cout << "                                          ";
            }

            // Takes passsword and validates it
            while (true)
            {
                gotoxy(17, 22);
                cout << RD << "Instructions-->" << YLW << "Password must be min 6 characters" << RST;
                gotoxy(17, 23);
                cout << YLW << "Min 1 num, 1 small and 1 capital letter and 1 Special Char" << RST;

                gotoxy(25, 24);
                cout << "Password:                    ";
                gotoxy(25, 24);
                p = getInput("Password: ");

                if (isValidPassword(p))
                {
                    break;
                }
                gotoxy(17, 25);
                cout << RD << " Invalid Password! Follow the instructions" << RST;
                Sleep(900);
                gotoxy(17, 25);
                cout << "                                                       ";
            }
            // If the credentials meets the requirement then sign up successful
            if (signUp(u, p, usernames, passwords, roles, departments, userCount, logActions, logCount))
            {
                gotoxy(18, 27);
                animations(GRN + "Account Created Successfully!" + RST, 20);
                gotoxy(18, 28);
                animations(GRN + "Now you are part of this cosmic Family" + RST, 20);
                Sleep(1500);
            }
            // Other error and loop runs again
            else
            {
                // registerUser handles checks but loops above handle format.
                // This else catches DB full or Duplicate if registerUser handles it.
                // We should ensure registerUser doesn't double print if we call it here.
                // Based on previous code, registerUser prints "USERNAME TAKEN".
                gotoxy(18, 27);
                cout << RD << "Registration failed (Duplicate or Database is Full)!" << RST;
                Sleep(1500);
            }
        }
        // Modules for other options
        else if (choice == '3')
            history();
        else if (choice == '4')
            about();
        else if (choice == '5')
            exit();
    }

    return 0;
}

// Functions definitions
bool signUp(string username, string password, string usernames[], string passwords[], string roles[], string departments[], int &count, string logActions[], int &logCount)
{
    // Checks credentials format
    if (!isValidUsername(username) || !isValidPassword(password))
    {
        return false;
    }
    // Checks if database is full
    if (count >= MAX_USERS)
    {
        cout << "\n   " << RD << "Database is Full." << RST;
        return false;
    }

    // Checks for  already registered usernames
    for (int i = 0; i < count; i++)
    {
        if (usernames[i] == username)
        {
            cout << "\n   " << RD << "USERNAME TAKEN." << RST;
            return false;
        }
    }

    // If all checks all passed then passes the entered credentials to credential arrays to save
    usernames[count] = username;
    passwords[count] = password;
    roles[count] = "visitor"; // New users will be visitors until apply for job and get hired
    departments[count] = "GEN";
    count++;
    addLog("New Visitor Registered: " + username, logActions, logCount);
    saveUsers(usernames, passwords, roles, departments, count);
    return true;
}

int signIn(string username, string password, string usernames[], string passwords[], int count, string logActions[], int &logCount)
{
    // Default value is -1 in case if credentials are not found
    int idx = -1;
    for (int i = 0; i < count; i++)
    {
        // Checks if the entered credentials are present in the database
        if (usernames[i] == username && passwords[i] == password)
        {
            idx = i;
            break;
        }
    }
    return idx;
}

// Main dashboard after successfully logining in
void dashboard_Main(string usernames[], string passwords[], string roles[], int userLimit, int &currentUserIdx,
                    string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount,
                    string missionNames[], string missionCodes[], string missionDates[], string missionVehicles[], string missionStatus[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount, double &agencyBudget,
                    string invNames[], string invCats[], double invQtys[], string invUnits[], double invCosts[], int &invCount,
                    string astroNames[], string astroRanks[], string astroStatus[], int &astroCount,
                    string planetNames[], string planetTypes[], double planetDists[], double planetGravs[], string planetAtms[], int &planetCount,
                    string exoNames[], double exoDists[], string exoTypes[], bool exoHabitable[], int &exoCount,
                    string logActions[], int &logCount)
{
    bool stay = true;
    while (stay)
    {
        // Dashboard Interface
        drawLogo(false);
        drawBox(10, 10, 80, 18);
        gotoxy(38, 11);
        cout << BL << "DASHBOARD" << RST;
        int x = 25, y = 13;

        // Displays user's information
        gotoxy(60, 11);
        cout << "User: " << usernames[currentUserIdx];
        gotoxy(60, 12);
        cout << "Role: " << roles[currentUserIdx];

        gotoxy(x, y);
        cout << CYN << "[1] FLIGHT OPS" << RST;
        gotoxy(x, y + 1);
        cout << GRN << "[2] ENGINEERING" << RST;
        gotoxy(x, y + 2);
        cout << MGD << "[3] SCIENCE" << RST;
        gotoxy(x, y + 3);
        cout << BL << "[4] HR" << RST;
        gotoxy(x, y + 4);
        cout << YLW << "[5] ROVER OPS" << RST;
        gotoxy(x, y + 5);
        cout << WHT << "[6] CAREER CENTER" << RST;

        // In case user is admin, gives access to admin features
        if (roles[currentUserIdx] == "admin")
        {
            gotoxy(x, y + 7);
            cout << RD << "[9] ADMIN PANEL" << RST;
        }
        // Otherwise it is locked for standard users
        else
        {
            gotoxy(x, y + 7);
            cout << GRA << "[Locked] Admin Only" << RST;
        }

        gotoxy(x, y + 9);
        cout << "[0] LOGOUT";
        // Actions in case of choosing any option
        char c = _getch();
        if (c == '1')
        {
            if (roles[currentUserIdx] == "visitor")
            {
                message("Restricted Area. Employees Only.");
            }
            else
                dashboard_Flight(usernames, roles, currentUserIdx, missionNames, missionCodes, missionDates, missionVehicles, missionStatus, missionBudgets, missionRequesters, missionCosts, missionCount, agencyBudget, invNames, invCosts, invCount, logActions, logCount);
        }
        else if (c == '2')
        {
            if (roles[currentUserIdx] == "visitor")
            {
                message("Restricted Area. Engineering Access Required.");
            }
            else
                dashboard_Eng(invNames, invCats, invQtys, invUnits, invCosts, invCount, logActions, logCount);
        }
        else if (c == '3')
        {
            dashboard_Science(planetNames, planetTypes, planetDists, planetGravs, planetAtms, planetCount, exoNames, exoDists, exoTypes, exoHabitable, exoCount, logActions, logCount);
        }
        else if (c == '4')
        {
            if (roles[currentUserIdx] == "visitor")
            {
                message("Restricted Area. Personnel Only.");
            }
            else
                dashboard_HR(astroNames, astroRanks, astroStatus, astroCount);
        }
        else if (c == '5')
            ops_RoverGame();
        // Gives chance to standard to visitor to apply for job
        else if (c == '6')
            career_Menu(usernames[currentUserIdx], roles[currentUserIdx], hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount, logActions, logCount);
        // In case user is admin and presses admin option then passes it to admin module
        else if (c == '9' && roles[currentUserIdx] == "admin")
            dashboard_Admin(usernames, passwords, roles, userLimit, hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount, missionNames, missionStatus, missionDates, missionBudgets, missionRequesters, missionCosts, missionCount, agencyBudget, astroNames, astroRanks, astroStatus, astroCount, logActions, logCount);
        // In case if user wants to logout and goes back to main menu
        else if (c == '0')
        {
            currentUserIdx = -1;
            stay = false;
        }
    }
}

// Internal Features

void dashboard_Flight(string usernames[], string roles[], int currentUserIdx,
                      string missionNames[], string missionCodes[], string missionDates[], string missionVehicles[], string missionStatus[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount, double &agencyBudget,
                      string invNames[], double invCosts[], int invCount,
                      string logActions[], int &logCount)
{
    // Interface
    while (true)
    {
        drawLogo(false);
        drawBox(10, 11, 80, 13);
        gotoxy(35, 12);
        cout << CYN << "FLIGHT CONTROL" << RST;
        gotoxy(20, 14);
        cout << "[1] Manifest";
        gotoxy(20, 15);
        cout << "[2] Launch Sim";
        gotoxy(20, 16);
        cout << "[3] Docking";
        gotoxy(20, 17);
        cout << "[4] Request New Mission (Staff Only)";
        gotoxy(20, 18);
        cout << "[5] Delete Mission";
        gotoxy(20, 19);
        cout << "[6] Back";
        char c = _getch();
        // Options Conditions
        if (c == '1')
            flight_Manifest(missionNames, missionCodes, missionDates, missionVehicles, missionStatus, missionRequesters, missionCount);
        if (c == '2')
            sim_Launch(missionNames, missionVehicles, missionStatus, missionRequesters, missionCosts, missionCount, agencyBudget, logActions, logCount, missionDates);
        if (c == '3')
            sim_Docking();
        if (c == '4')
        {
            if (roles[currentUserIdx] == "guest")
            {
                system("cls");
                cout << RD << "Access Denied. Guests cannot plan missions." << RST;
                pause();
            }
            else
                flight_Request(usernames[currentUserIdx], missionNames, missionCodes, missionDates, missionVehicles, missionStatus, missionBudgets, missionRequesters, missionCosts, missionCount, invNames, invCosts, invCount, logActions, logCount);
        }
        if (c == '5')
            flight_DeleteMission(missionNames, missionStatus, missionDates, missionRequesters, missionCosts, missionCount);
        if (c == '6')
            break;
    }
}

// Additional Flight Functions
// For removing an mission
void flight_DeleteMission(string names[], string status[], string dates[], string requesters[], double costs[], int &count)
{
    system("cls");
    cout << "DELETE MISSION. Mission IDs(1-" << count << "): ";
    int id = getInt("", 1, count);
    int i = id - 1;
    cout << "Deleting " << names[i] << "... ";
    for (int k = i; k < count - 1; k++)
    {
        names[k] = names[k + 1];
        status[k] = status[k + 1];
        dates[k] = dates[k + 1];
        requesters[k] = requesters[k + 1];
        costs[k] = costs[k + 1];
    }
    count--;
    cout << GRN << "Eliminated." << RST;
    pause();
}
// For displaying the missions information
void flight_Manifest(string names[], string codes[], string dates[], string vehicles[], string status[], string requesters[], int &count)
{
    system("cls");
    cout << YLW << "   NASA MISSION MANIFEST DATABASE" << RST << endl;
    cout << left << setw(4) << "ID" << setw(10) << "CODE" << setw(15) << "DATE" << setw(20) << "NAME" << setw(15) << "STATUS" << "REQUESTER\n";
    cout << "------------------------------------------------------------------------------------\n";
    for (int i = 0; i < count; i++)
    {
        if (names[i].empty())
            continue;
        string color = (status[i] == "Success" ? GRN : (status[i] == "Failure" ? RD : (status[i] == "Planned" ? CYN : YLW)));
        cout << setw(4) << i + 1 << setw(10) << codes[i] << setw(15) << (dates ? dates[i] : "N/A") << setw(20) << names[i] << color << setw(15) << status[i] << RST << requesters[i] << endl;
    }
    pause();
}
// For adding a new mission
void flight_Request(string username, string missionNames[], string missionCodes[], string missionDates[], string missionVehicles[], string missionStatus[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount,
                    string invNames[], double invCosts[], int invCount, string logActions[], int &logCount)
{
    system("cls");
    cout << GRN << "   MISSION PLANNING PROTOCOL" << RST << endl;
    if (missionCount >= MAX_MISSIONS)
    {
        cout << "Manifest Full.";
        pause();
        return;
    }

    string name = getInput("   Mission Name (0 to Cancel): ");
    if (name == "0")
        return;
    string vehicle = getInput("   Vehicle Class: ");

    cout << "\n   -- EQUIPMENT REQUISITION --\n";
    cout << "   Select items to load onto the mission. Costs will accumulate.\n";
    double totalCost = 0.5; // Base ops cost

    while (true)
    {
        cout << "\n   Current Cost: $" << totalCost << "B\n";
        cout << "   1. Add Fuel (Liquid H2) - $0.1B\n";
        cout << "   2. Add Rover Upgrade - $0.5B\n";
        cout << "   3. Add Advanced Comms - $0.2B\n";
        cout << "   4. Done / Submit\n";
        int op = getInt("   Choice: ", 1, 4);
        if (op == 1)
            totalCost += 0.1;
        else if (op == 2)
            totalCost += 0.5;
        else if (op == 3)
            totalCost += 0.2;
        else if (op == 4)
            break;
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);
    char datStr[20];
    strftime(datStr, 20, "%Y-%m-%d", ltm);

    missionNames[missionCount] = name;
    missionCodes[missionCount] = "REQ-" + to_string(missionCount + 100);
    missionDates[missionCount] = string(datStr);
    missionVehicles[missionCount] = vehicle;
    missionStatus[missionCount] = "Pending";
    missionBudgets[missionCount] = totalCost;
    missionCosts[missionCount] = totalCost;
    missionRequesters[missionCount] = username;
    missionCount++;

    addLog("Mission Requested: " + name, logActions, logCount);

    cout << "\n   " << GRN << "MISSION REQUEST SUBMITTED." << RST << " Waiting for Admin Funding Approval.";
    pause();
}
// Launching Simulation Prototype
void sim_Launch(string names[], string vehicles[], string status[], string requesters[], double costs[], int &count, double agencyBudget, string logActions[], int &logCount, string dates[])
{
    system("cls");
    // If no mission created
    if (count == 0)
    {
        cout << RD << "No missions available to launch. Create one first." << RST;
        pause();
        return;
    }
    cout << "LAUNCH. ID(1-" << count << "): ";
    int id = getInt("", 1, count);
    int idx = id - 1;

    if (status[idx] == "Pending")
    {
        cout << RD << "\n   ERROR: Mission not approved/funded by Admin yet." << RST;
        pause();
        return;
    }

    cout << "Launching " << names[idx] << "...\n";
    Sleep(1000);
    string sys[] = {"Fuel", "Guidance", "Comms", "Telemetry"};
    for (string s : sys)
    {
        cout << "   " << s << "... ";
        Sleep(800);
        if (rand() % 10 == 0) // 10% Failure Chance
        {
            cout << RD << "FAIL" << RST << endl;
            status[idx] = "Failure";
            addLog("Launch Failure: " + names[idx], logActions, logCount);
            saveMissions(count, names, status, requesters, costs, agencyBudget, dates);

            clearKeyboardBuffer();
            cout << "\n   " << RD << "MISSION ABORTED." << RST << endl;
            cout << "   " << YLW << "Press any key to return to Flight Control..." << RST;
            _getch();
            return;
        }
        cout << GRN << "GO" << RST << endl;
    }
    cout << "\n   " << GRN << "LIFTOFF! SUCCESSFUL ORBITAL INSERTION." << RST << endl;
    status[idx] = "Success";
    addLog("Launch Success: " + names[idx], logActions, logCount);
    saveMissions(count, names, status, requesters, costs, agencyBudget, dates);

    clearKeyboardBuffer();
    cout << "\n   " << YLW << "Press any key to return..." << RST;
    _getch();
}
// Docking Simulation inspired by Interstellar
void sim_Docking()
{
    system("cls");
    cout << "DOCKING SIM (WASD). Goal: [+] to (O)\n";
    int px = 0, py = 0, tx = 10, ty = 5, f = 20;
    while (f > 0)
    {
        system("cls");
        cout << "Fuel: " << f << endl;
        for (int y = 0; y < 10; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                if (x == px && y == py)
                    cout << "[+]";
                else if (x == tx && y == ty)
                    cout << "(O)";
                else
                    cout << " . ";
            }
            cout << endl;
        }
        if (px == tx && py == ty)
        {
            cout << GRN << "SUCCESS" << RST;
            pause();
            return;
        }
        // For calibrating the docking controls
        char c = _getch();
        if (c == 'w' && py > 0)
            py--;
        if (c == 's' && py < 9)
            py++;
        if (c == 'a' && px > 0)
            px--;
        if (c == 'd' && px < 19)
            px++;
        f--;
    }
    cout << RD << "Failed" << RST;
    pause();
}

// Module to apply for a job

void career_Menu(string username, string userRole, string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount, string logActions[], int &logCount)
{
    while (true)
    {
        system("cls");
        cout << YLW << "   NASA CAREER CENTER" << RST << endl;
        cout << "   Current User: " << username << " (" << userRole << ")" << endl;
        cout << "\n   [1] Apply for Position";
        cout << "\n   [2] View Application Status";
        cout << "\n   [3] Back";

        char c = _getch();
        if (c == '3')
            return;
        // In Case Admin tries to apply for a job. meri billi muji ko meow
        if (c == '1')
        {
            if (userRole == "admin")
            {
                cout << "\n   " << RD << "Admins cannot apply for lower positions." << RST;
                pause();
                continue;
            }
            if (hireCount >= MAX_HIRES)
            {
                cout << "\n   Sorry, NASA is running low on resources so we cannot hire you as we can't pay you.";
                pause();
                continue;
            }
            // Ckecks if user already applied
            bool already = false;
            for (int i = 0; i < hireCount; i++)
                if (hireUsers[i] == username && hireStatus[i] == "Pending")
                    already = true;
            if (already)
            {
                cout << "\n   You have a pending application.";
                pause();
                continue;
            }
            // If new candidate, prompts to fill the details
            cout << "\n\n   -- NEW APPLICATION --\n";
            string full = getInput("   Full Legal Name: ");
            string edu = getInput("   Highest Education/Degree: ");
            string exp = getInput("   Experience (Place & Years): ");

            cout << "   Position:\n   1. Astronaut Candidate\n   2. Systems Engineer\n   3. Data Scientist\n   Select: ";
            int r = getInt("", 1, 3);
            string role = (r == 1 ? "astronaut" : (r == 2 ? "engineer" : "scientist"));
            // Stores the info
            hireUsers[hireCount] = username;
            hireRoles[hireCount] = role;
            hireExp[hireCount] = exp;
            hireNames[hireCount] = full;
            hireEdu[hireCount] = edu;
            hireStatus[hireCount] = "Pending";
            hireCount++;
            saveHires(hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount);
            addLog("Applied: " + role, logActions, logCount);
            cout << GRN << "\n   Application Received." << RST;
            pause();
        }
        // If the user already applied and wants to check the status
        if (c == '2')
        {
            cout << "\n\n   -- STATUS --\n";
            bool found = false;
            for (int i = 0; i < hireCount; i++)
            // Searches through stored data application submitted and what's the status
            {
                if (hireUsers[i] == username)
                {
                    cout << "   Role: " << hireRoles[i] << " | Status: " << (hireStatus[i] == "Approved" ? GRN : YLW) << hireStatus[i] << RST << endl;
                    found = true;
                }
            }
            if (!found)
                cout << "   No applications found.";
            pause();
        }
    }
}
// Module if user is the admin
void dashboard_Admin(string usernames[], string passwords[], string roles[], int &userCount,
                     string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount,
                     string missionNames[], string missionStatus[], string missionDates[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount, double &agencyBudget,
                     string astroNames[], string astroRanks[], string astroStatus[], int &astroCount,
                     string logActions[], int &logCount)
{
    while (true)
    {
        // Admin Dashboard Interface
        drawLogo(false);
        drawBox(10, 11, 80, 13);
        gotoxy(35, 12);
        cout << RD << "ADMINISTRATION" << RST;
        gotoxy(20, 14);
        cout << "[1] System Logs";
        gotoxy(20, 15);
        cout << "[2] Hiring Requests";
        gotoxy(20, 16);
        cout << "[3] Mission Funding Approvals";
        gotoxy(20, 17);
        cout << "[4] Personnel";
        gotoxy(20, 18);
        cout << "[5] Back";
        // Action on different choices
        char c = _getch();
        if (c == '5')
            break;
        if (c == '2')
            admin_Hiring(usernames, roles, userCount, hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount, astroNames, astroRanks, astroStatus, astroCount, logActions, logCount);
        if (c == '4')
            admin_Personnel(usernames, passwords, roles, nullptr, userCount, logActions, logCount);
        if (c == '3')
            admin_Missions(missionNames, missionStatus, missionDates, missionBudgets, missionRequesters, missionCount, agencyBudget, logActions, logCount);
        // Displays the activities that the users have done in this app
        if (c == '1')
        {
            system("cls");
            cout << "SYSTEM LOGS\n";
            for (int i = 0; i < logCount; i++)
                cout << logActions[i] << endl;
            pause();
        }
    }
}
// Module to give admin access to all the personnels and users available in the agency
void admin_Personnel(string usernames[], string passwords[], string roles[], string departments[], int &userCount, string logActions[], int &logCount)
{
    system("cls");
    cout << "PERSONNEL DIRECTORY\n";
    // Creates a table to display the users and their roles going upto usercount
    cout << left << setw(5) << "ID" << setw(15) << "USER" << "ROLE\n";
    for (int i = 0; i < userCount; i++)
        cout << setw(5) << i + 1 << setw(15) << usernames[i] << roles[i] << endl;
    // Editing options
    cout << "\n[E] Edit Role  [D] Delete User  [B] Back: ";
    char c = _getch();
    if (c == 'b' || c == 'B')
        return;
    int id = getInt("\nID: ", 1, userCount);
    int i = id - 1;
    // For editing the role of users
    if (c == 'e' || c == 'E')
    {
        cout << "New Role: ";
        roles[i] = getInput("");
        addLog("Updated Role: " + usernames[i], logActions, logCount);
        saveUsers(usernames, passwords, roles, departments, userCount);
    }
    // For deleting users
    if (c == 'd' || c == 'D')
    {
        if (usernames[i] == "themystery")
        {
            cout << RD << "Cannot delete SuperAdmin." << RST;
            pause();
            return;
        }
        addLog("Deleted User: " + usernames[i], logActions, logCount);
        for (int k = i; k < userCount - 1; k++)
        {
            usernames[k] = usernames[k + 1];
            passwords[k] = passwords[k + 1];
            roles[k] = roles[k + 1];
            if (departments)
                departments[k] = departments[k + 1];
        }
        userCount--;
        saveUsers(usernames, passwords, roles, departments, userCount);
    }
    pause();
}
// Module for approving missions and releasing funds
void admin_Missions(string names[], string status[], string dates[], double budgets[], string requesters[], int &count, double &agencyBudget, string logActions[], int &logCount)
{
    system("cls");
    cout << "MISSION FUNDING | Agency Budget: $" << agencyBudget << "B\n";
    cout << left << setw(5) << "ID" << setw(20) << "NAME" << setw(10) << "COST" << "STATUS\n";
    // Displays those missions which are pending to be approved
    for (int i = 0; i < count; i++)
    {
        if (status[i] == "Pending")
        {
            cout << setw(5) << i + 1 << setw(20) << names[i] << "$" << setw(9) << budgets[i] << status[i] << endl;
        }
    }
    int id = getInt("\nApprove ID (0 to cancel): ", 0, count);
    // To return back
    if (id == 0)
        return;
    // Takes input for which mission to be approved
    int i = id - 1;
    if (agencyBudget >= budgets[i])
    {
        agencyBudget -= budgets[i];
        status[i] = "Planned";
        addLog("Funded Mission: " + names[i], logActions, logCount);
        saveMissions(count, names, status, requesters, budgets, agencyBudget, dates);
    }
    else
        cout << RD << "Insufficient Funds." << RST;
    pause();
}
// For approving job applications of candidates
void admin_Hiring(string usernames[], string roles[], int userLimit, string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount,
                  string astroNames[], string astroRanks[], string astroStatus[], int &astroCount, string logActions[], int &logCount)
{
    system("cls");
    cout << YLW << "   HIRING REQUESTS" << RST << endl;
    cout << left << setw(3) << "ID" << setw(10) << "USER" << setw(15) << "NAME" << setw(10) << "EDU" << setw(10) << "ROLE" << "EXP\n";
    // Displays those applications which are pending to be approved
    for (int i = 0; i < hireCount; i++)
    {
        if (hireStatus[i] == "Pending")
            cout << setw(3) << i + 1 << setw(10) << hireUsers[i] << setw(15) << hireNames[i] << setw(10) << hireEdu[i] << setw(10) << hireRoles[i] << hireExp[i] << endl;
    }
    // Approve or reject
    cout << "\n   [A] Approve  [R] Reject  [B] Back\n";
    char c = _getch();
    if (c == 'b' || c == 'B')
        return;

    if (c == 'a' || c == 'A')
    {
        int id = getInt("   Enter ID: ", 1, hireCount);
        int idx = id - 1;
        // Checks if the applicant is present in the user database
        bool found = false;
        for (int k = 0; k < userLimit; k++)
        {
            if (usernames[k] == hireUsers[idx])
            {
                roles[k] = hireRoles[idx];
                found = true;
                break;
            }
        }

        if (found)
        {
            hireStatus[idx] = "Approved";
            addLog("Hired " + hireUsers[idx], logActions, logCount);
            // If applicant register as an astronaut he is saved in that category
            if (hireRoles[idx] == "astronaut")
            {
                if (astroCount < MAX_ASTRO)
                {
                    astroNames[astroCount] = hireNames[idx];
                    astroRanks[astroCount] = "Recruit";
                    astroStatus[astroCount] = "Active";
                    astroCount++;
                    saveAstronauts(astroCount, astroNames, astroRanks, astroStatus);
                    cout << GRN << "   [!] Added to Astronaut Roster." << RST;
                }
                else
                {
                    cout << RD << "   [!] Roster Full. Not added." << RST;
                }
            }
            saveHires(hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount);
            cout << GRN << "   Promoted." << RST;
        }
    }
    // If the admin wants to reject the application
    if (c == 'r' || c == 'R')
    {
        int id = getInt("   Enter ID to REJECT: ", 1, hireCount);
        int idx = id - 1;
        hireStatus[idx] = "Rejected";
        saveHires(hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount);
        cout << RD << "   Application Rejected." << RST;
    }
    pause();
}
// Module for engineers to check available inventory and build rovers
void dashboard_Eng(string invNames[], string invCats[], double invQtys[], string invUnits[], double invCosts[], int &invCount, string logActions[], int &logCount)
{
    while (true)
    {
        drawLogo(false);
        drawBox(10, 11, 80, 12);
        gotoxy(35, 12);
        cout << GRN << "ENG" << RST;
        gotoxy(20, 14);
        cout << "[1] Inventory";
        gotoxy(20, 15);
        cout << "[2] Rover Builder";
        gotoxy(20, 16);
        cout << "[3] Add Item";
        gotoxy(20, 17);
        cout << "[4] Delete Item";
        gotoxy(20, 18);
        cout << "[5] Back";
        char c = _getch();

        if (c == '1')
            eng_Inventory(invNames, invCats, invQtys, invCosts, invCount);
        if (c == '2')
            eng_RoverBuilder(logActions, logCount);
        if (c == '3')
            eng_AddInventory(invNames, invCats, invQtys, invUnits, invCosts, invCount);
        if (c == '4')
            eng_DeleteInventory(invNames, invCats, invQtys, invUnits, invCosts, invCount);
        if (c == '5')
            break;
    }
}
// Module to add newly designed invent
void eng_AddInventory(string names[], string cats[], double qtys[], string units[], double costs[], int &count)
{
    if (count >= MAX_INVENTORY)
    {
        cout << "Database is Full. Press any key...";
        _getch();
        return;
    }
    system("cls");
    cout << "ADD COMPONENT\n";
    // Adds component information to inventory
    names[count] = getInput("Name: ");
    cats[count] = getInput("Category (Propulsion/Structure/Power): ");
    qtys[count] = getDouble("Quantity: ", 1, 10000);
    units[count] = getInput("Unit (kg/box/pcs): ");
    costs[count] = getDouble("Unit Cost ($M): ", 0.001, 100.0);
    count++;
    saveInventory(count, names, cats, qtys, units, costs);
    cout << GRN << "Item Added. Press any key to return..." << RST;
    _getch();
}
// Module for deleting any outdated component in inventory
void eng_DeleteInventory(string names[], string cats[], double qtys[], string units[], double costs[], int &count)
{
    system("cls");
    cout << "DELETE COMPONENT. IDs(1-" << count << "): ";
    // Gets the id of the component to delete
    int id = getInt("", 1, count);
    int i = id - 1;
    cout << "Removing " << names[i] << "... ";
    // Replaces the id of the component with the next one
    for (int k = i; k < count - 1; k++)
    {
        names[k] = names[k + 1];
        cats[k] = cats[k + 1];
        qtys[k] = qtys[k + 1];
        units[k] = units[k + 1];
        costs[k] = costs[k + 1];
    }
    count--;
    saveInventory(count, names, cats, qtys, units, costs);
    cout << GRN << "Updated. Press any key..." << RST;
    _getch();
}
// Module for Cosmic Science knowledge
void dashboard_Science(string planetNames[], string planetTypes[], double planetDists[], double planetGravs[], string planetAtms[], int &planetCount,
                       string exoNames[], double exoDists[], string exoTypes[], bool exoHabitable[], int &exoCount,
                       string logActions[], int &logCount)
{
    while (true)
    {
        // Dashboard Interface
        drawLogo(false);
        drawBox(10, 11, 80, 13);
        gotoxy(35, 12);
        cout << MGD << "COSMIC SCIENCE" << RST;
        gotoxy(20, 14);
        cout << "[1] Planets";
        gotoxy(20, 15);
        cout << "[2] Exoplanets";
        gotoxy(20, 16);
        cout << "[3] Decrypt";
        gotoxy(20, 17);
        cout << "[4] Discover Planet";
        gotoxy(20, 18);
        cout << "[5] Discover Exoplanet";
        gotoxy(20, 19);
        cout << "[6] Delete Planet";
        gotoxy(20, 20);
        cout << "[7] Delete Exoplanet";
        gotoxy(20, 21);
        cout << "[8] Back";
        char c = _getch();
        // Option choices to go to different parts

        if (c == '1')
            sci_Planets(planetNames, planetTypes, planetDists, planetGravs, planetAtms, planetCount);
        if (c == '2')
            sci_Exoplanets(exoNames, exoDists, exoTypes, exoHabitable, exoCount);
        if (c == '3')
            sci_Decrypt(logActions, logCount);
        if (c == '4')
            sci_AddPlanet(planetNames, planetTypes, planetDists, planetGravs, planetAtms, planetCount);
        if (c == '5')
            sci_AddExoplanet(exoNames, exoDists, exoTypes, exoHabitable, exoCount);
        if (c == '6')
            sci_DeletePlanet(planetNames, planetTypes, planetDists, planetGravs, planetAtms, planetCount);
        if (c == '7')
            sci_DeleteExoplanet(exoNames, exoDists, exoTypes, exoHabitable, exoCount);
        if (c == '8')
            break;
    }
}
// Module for adding a new planet
void sci_AddPlanet(string names[], string types[], double dists[], double gravs[], string atms[], int &count)
{
    if (count >= MAX_PLANETS)
    {
        cout << "Database is Full.";
        pause();
        return;
    }
    system("cls");
    cout << "DISCOVER NEW PLANET (0 to Cancel)\n";
    names[count] = getInput("   What shall we name it?: ");
    if (names[count] == "0")
        return;
    types[count] = getInput("   Planet Type (Rocky/Gas/Ice): ");
    dists[count] = getDouble("   Distance from Sun (AU): ", 0.1, 100.0);
    gravs[count] = getDouble("   Gravity (m/s2): ", 0.1, 100.0);
    atms[count] = getInput("   Atmosphere Composition: ");
    count++;
    savePlanets(count, names, types, dists, gravs, atms);
    cout << GRN << "Planet Cataloged." << RST;
    pause();
}
// Module for adding any exoplanet
void sci_AddExoplanet(string names[], double dists[], string types[], bool habitable[], int &count)
{
    if (count >= MAX_PLANETS)
    {
        cout << "Database is Full.";
        pause();
        return;
    }
    system("cls");
    cout << "DISCOVER EXOPLANET\n";
    names[count] = getInput("Name: ");
    dists[count] = getDouble("Dist (Light Years): ", 1.0, 10000.0);
    types[count] = getInput("Type: ");
    cout << "Habitable? (1=Yes, 0=No): ";
    habitable[count] = (getInt("", 0, 1) == 1);
    count++;
    saveExoplanets(count, names, dists, types, habitable);
    cout << GRN << "Discovery Logged." << RST;
    pause();
}
// Mofule for deleting a Planet
void sci_DeletePlanet(string names[], string types[], double dists[], double gravs[], string atms[], int &count)
{
    system("cls");
    cout << "DELETE PLANET. ID(1-" << count << "): ";
    int id = getInt("", 1, count);
    int i = id - 1;
    cout << "Deleting " << names[i] << "... ";
    for (int k = i; k < count - 1; k++)
    {
        names[k] = names[k + 1];
        types[k] = types[k + 1];
        dists[k] = dists[k + 1];
        gravs[k] = gravs[k + 1];
        atms[k] = atms[k + 1];
    }
    count--;
    cout << GRN << "Deleted." << RST;
    pause();
}
// Module for deleting a exoplanet
void sci_DeleteExoplanet(string names[], double dists[], string types[], bool habitable[], int &count)
{
    system("cls");
    cout << "DELETE NOVELTY. ID(1-" << count << "): ";
    int id = getInt("", 1, count);
    int i = id - 1;
    cout << "Deleting " << names[i] << "... ";
    for (int k = i; k < count - 1; k++)
    {
        names[k] = names[k + 1];
        types[k] = types[k + 1];
        dists[k] = dists[k + 1];
        habitable[k] = habitable[k + 1];
    }
    count--;
    cout << GRN << "Deleted." << RST;
    pause();
}
// Just a little fun activity for decrypting aliens message
void sci_Decrypt(string logActions[], int &logCount)
{
    system("cls");
    cout << "DECRYPT: 1, 1, 2, 3, 5... ";
    if (getInt("", 0, 100) == 8)
    {
        cout << GRN << "MATCH" << RST;
        addLog("Decrypted", logActions, logCount);
    }
    else
        cout << RD << "FAIL" << RST;
    pause();
}
// Moudle for HR to display the available staff for different missions
void dashboard_HR(string astroNames[], string astroRanks[], string astroStatus[], int &astroCount)
{
    while (true)
    {
        drawLogo(false);
        drawBox(10, 11, 80, 12);
        gotoxy(35, 12);
        cout << BL << "HR" << RST;
        gotoxy(20, 14);
        cout << "[1] Roster";
        gotoxy(20, 15);
        cout << "[2] Training";
        gotoxy(20, 16);
        cout << "[3] Back";
        char c = _getch();
        if (c == '3')
            break;
        if (c == '1')
            hr_Roster(astroNames, astroRanks, astroStatus, astroCount);
        if (c == '2')
            hr_Training();
    }
}
// For displaying the inventory of the agency
void eng_Inventory(string names[], string cats[], double qtys[], double costs[], int &count)
{
    system("cls");
    cout << "INVENTORY\n";
    cout << left << setw(5) << "ID" << setw(30) << "ITEM" << setw(12) << "CAT" << setw(8) << "QTY" << "COST\n";
    cout << "--------------------------------------------------------------------\n";
    for (int i = 0; i < 30 && i < count; i++)
    {
        cout << setw(5) << i + 1 << setw(30) << names[i] << setw(12) << cats[i] << setw(8) << qtys[i] << "$" << costs[i] << "\n";
    }
    pause();
}
// Just for fun to build a rover that explores mars
void eng_RoverBuilder(string logActions[], int &logCount)
{
    system("cls");
    cout << "ROVER BUILDER. Name: ";
    string n = getInput("");
    cout << "   [O-O]\n  /_____\\\n  O-----O\n";
    addLog("Built Rover: " + n, logActions, logCount);
    pause();
}
// For displaying the planets with their details
void sci_Planets(string names[], string types[], double dists[], double gravs[], string atms[], int &count)
{
    system("cls");
    cout << "PLANETS\n";
    cout << left << setw(20) << "NAME" << setw(15) << "TYPE" << setw(10) << "DISTANCE" << setw(10) << "GRAVITY" << "ATMOSPHERE\n";

    for (int i = 0; i < count; i++)
    {
        cout << setw(20) << names[i] << setw(15) << types[i] << setw(10) << dists[i] << " AU" << setw(10) << gravs[i] << atms[i] << endl;
    }
    pause();
}
// For displaying the exoplanets with their details
void sci_Exoplanets(string names[], double dists[], string types[], bool habitable[], int &count)
{
    system("cls");
    cout << "EXOPLANETS\n";
    cout << left << setw(20) << "NAME" << setw(15) << "TYPE" << setw(10) << "DIST" << "HABITABLE\n";
    for (int i = 0; i < count; i++)
    {
        cout << setw(20) << names[i] << setw(15) << types[i] << setw(10) << dists[i] << (habitable[i] ? (GRN + "YES" + RST) : (RD + "NO" + RST)) << endl;
    }
    pause();
}
// For displaying the personnel or rosters
void hr_Roster(string names[], string ranks[], string status[], int &count)
{
    system("cls");
    cout << "PERSONNEL\n";
    cout << left << setw(20) << "NAME" << setw(10) << "RANK" << "STATUS\n";
    for (int i = 0; i < count; i++)
        cout << setw(20) << names[i] << setw(10) << ranks[i] << status[i] << endl;
    pause();
}

void hr_Training()
{
    system("cls");

    cout << "Q1: Escape velocity of Earth (km/s)?\n";
    cout << "(1) 9.8   (2) 11.2   (3) 15.0 : ";
    if (getInt("", 1, 3) == 2)
        cout << GRN << " PASS\n"
             << RST;
    else
        cout << RD << " FAIL\n"
             << RST;

    cout << "\nQ2: Closest planet to the Sun?\n";
    cout << "(1) Mercury   (2) Venus   (3) Mars : ";
    if (getInt("", 1, 3) == 1)
        cout << GRN << " PASS\n"
             << RST;
    else
        cout << RD << " FAIL\n"
             << RST;

    cout << "\nQ3: First human to walk on the Moon?\n";
    cout << "(1) Buzz Aldrin   (2) Yuri Gagarin  (3) Neil Armstrong : ";
    if (getInt("", 1, 3) == 3)
        cout << GRN << " PASS\n"
             << RST;
    else
        cout << RD << " FAIL\n"
             << RST;

    cout << "\nQ4: Largest planet in the solar system?\n";
    cout << "(1) Earth   (2) Saturn   (3) Jupiter : ";
    if (getInt("", 1, 3) == 3)
        cout << GRN << " PASS\n"
             << RST;
    else
        cout << RD << " FAIL\n"
             << RST;

    cout << "\nQ5: Mars is commonly known as the?\n";
    cout << "(1) Red Planet   (2)  Ice Planet   (3) Gas Giant : ";
    if (getInt("", 1, 3) == 1)
        cout << GRN << " PASS\n"
             << RST;
    else
        cout << RD << " FAIL\n"
             << RST;

    cout << "\nQ6: SI unit of force?\n";
    cout << "(1) Joule   (2) Pascal   (3) Newton : ";
    if (getInt("", 1, 3) == 3)
        cout << GRN << " PASS\n"
             << RST;
    else
        cout << RD << " FAIL\n"
             << RST;

    cout << "\nHR Training Complete!";
    pause();
}

// Rover Game for searching samples
void ops_RoverGame()
{
    int rx = 2, ry = 2, sx = rand() % 20, sy = rand() % 15, score = 0;
    int c1x = rand() % 18, c1y = rand() % 13;
    int c2x = rand() % 20, c2y = rand() % 15; // Crater
    while (true)
    {
        system("cls");
        cout << "ROVER OPS | Science: " << score << " | Q to Exit | WASD to Move\n";
        cout << GRN << "S = Science Sample  " << RD << "X = Crater\n"
             << RST;
        for (int y = 0; y < 15; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                if (x == rx && y == ry)
                    cout << "R ";
                else if (x == sx && y == sy)
                    cout << "S ";
                else if (x == c1x && y == c1y)
                    cout << "X ";
                else if (x == c2x && y == c2y)
                    cout << "X ";

                else
                    cout << ". ";
            }
            cout << endl;
        }
        if (rx == sx && ry == sy)
        {
            score++;
            sx = rand() % 20;
            sy = rand() % 15;
        }
        if (rx == c1x && ry == c1y || rx == c2x && ry == c2y)
        {
            cout << RD << "\nCRASHED INTO CRATER! MISSION TERMINATED." << RST;
            pause();
            return;
        }
        char c = _getch();
        if (c == 'q')
            return;
        if (c == 'w' && ry > 0)
            ry--;
        if (c == 's' && ry < 14)
            ry++;
        if (c == 'a' && rx > 0)
            rx--;
        if (c == 'd' && rx < 19)
            rx++;
    }
}

// initialize the core database of the agency
void init_Database(string usernames[], string passwords[], string roles[], string departments[], int &userCount,
                   string hireUsers[], string hireRoles[], string hireExp[], string hireStatus[], string hireNames[], string hireEdu[], int &hireCount,
                   string missionNames[], string missionCodes[], string missionDates[], string missionVehicles[], string missionStatus[], double missionBudgets[], string missionRequesters[], double missionCosts[], int &missionCount, double &agencyBudget,
                   string invNames[], string invCats[], double invQtys[], string invUnits[], double invCosts[], int &invCount,
                   string astroNames[], string astroRanks[], string astroStatus[], int &astroCount,
                   string planetNames[], string planetTypes[], double planetDists[], double planetGravs[], string planetAtms[], int &planetCount,
                   string exoNames[], double exoDists[], string exoTypes[], bool exoHabitable[], int &exoCount,
                   string logActions[], int &logCount)
{
    init_Users(usernames, passwords, roles, departments, userCount);
    loadHires(hireUsers, hireRoles, hireExp, hireStatus, hireNames, hireEdu, hireCount);

    init_Missions(missionNames, missionCodes, missionVehicles, missionStatus, missionBudgets, missionRequesters, missionCosts, missionCount, missionDates);
    loadMissions(missionCount, missionNames, missionCodes, missionVehicles, missionStatus, missionBudgets, missionRequesters, missionCosts, agencyBudget, missionDates);

    init_Inventory(invNames, invCats, invQtys, invUnits, invCosts, invCount);
    loadInventory(invCount, invNames, invCats, invQtys, invUnits, invCosts);
    init_Astronauts(astroNames, astroRanks, astroStatus, astroCount);
    loadAstronauts(astroCount, astroNames, astroRanks, astroStatus);
    init_Planets(planetNames, planetTypes, planetDists, planetGravs, planetAtms, planetCount);
    loadPlanets(planetCount, planetNames, planetTypes, planetDists, planetGravs, planetAtms);
    init_Exoplanets(exoNames, exoDists, exoTypes, exoHabitable, exoCount);
    loadExoplanets(exoCount, exoNames, exoDists, exoTypes, exoHabitable);
    loadLogs(logCount, logActions);
}
// Default users module
void init_Users(string usernames[], string passwords[], string roles[], string departments[], int &count)
{
    loadUsers(usernames, passwords, roles, departments, count);
    if (count == 0)
    {
        usernames[0] = "themystery";
        passwords[0] = "29July1958";
        roles[0] = "admin";
        departments[0] = "HQ";
        usernames[1] = "flight";
        passwords[1] = "flight123";
        roles[1] = "staff";
        departments[1] = "FLT";
        usernames[2] = "eng";
        passwords[2] = "engineer1";
        roles[2] = "staff";
        departments[2] = "ENG";
        count = 3;
        saveUsers(usernames, passwords, roles, departments, count);
    }
}
// Default missions module
void init_Missions(string names[], string codes[], string vehicles[], string status[], double budgets[], string requesters[], double costs[], int &count, string dates[])
{
    if (count > 0)
        return; // Don't re-init
    // Mercury
    names[0] = "Freedom 7";
    codes[0] = "MR-3";
    vehicles[0] = "Redstone";
    status[0] = "Success";
    budgets[0] = 0.1;
    requesters[0] = "History";
    costs[0] = 0.1;
    dates[0] = "1961-05-05";
    names[1] = "Liberty Bell 7";
    codes[1] = "MR-4";
    vehicles[1] = "Redstone";
    status[1] = "Success";
    budgets[1] = 0.1;
    requesters[1] = "History";
    costs[1] = 0.1;
    dates[1] = "1961-07-21";
    names[2] = "Friendship 7";
    codes[2] = "MA-6";
    vehicles[2] = "Atlas";
    status[2] = "Success";
    budgets[2] = 0.2;
    requesters[2] = "History";
    costs[2] = 0.2;
    dates[2] = "1962-02-20";
    count = 3;
}
// Default inventory module
void init_Inventory(string names[], string cats[], double qtys[], string units[], double costs[], int &count)
{
    if (count > 0)
        return;
    names[0] = "Hydrazine Fuel";
    cats[0] = "Propulsion";
    qtys[0] = 5000;
    costs[0] = 0.5;
    names[1] = "LOX Tank";
    cats[1] = "Propulsion";
    qtys[1] = 200;
    costs[1] = 1.2;
    names[2] = "Heat Shield Tile";
    cats[2] = "Structure";
    qtys[2] = 1500;
    costs[2] = 0.05;
    names[3] = "Solar Array";
    cats[3] = "Power";
    qtys[3] = 50;
    costs[3] = 2.5;
    names[4] = "RTG Fuel Cell";
    cats[4] = "Power";
    qtys[4] = 10;
    costs[4] = 15.0;
    names[5] = "Comm Antenna";
    cats[5] = "Electronics";
    qtys[5] = 25;
    costs[5] = 3.0;
    names[6] = "Nav Computer";
    cats[6] = "Electronics";
    qtys[6] = 15;
    costs[6] = 5.5;
    names[7] = "Life Support Module";
    cats[7] = "Habitation";
    qtys[7] = 8;
    costs[7] = 8.0;
    names[8] = "Space Suit (EVA)";
    cats[8] = "Equipment";
    qtys[8] = 12;
    costs[8] = 10.0;
    names[9] = "Rover Wheels";
    cats[9] = "Robotics";
    qtys[9] = 40;
    costs[9] = 0.2;
    names[10] = "Camera Lens";
    cats[10] = "Optics";
    qtys[10] = 30;
    costs[10] = 1.5;
    names[11] = "Thermal Blanket";
    cats[11] = "Structure";
    qtys[11] = 500;
    costs[11] = 0.01;
    names[12] = "Docking Port";
    cats[12] = "Structure";
    qtys[12] = 5;
    costs[12] = 4.5;
    names[13] = "Drill Bit (Diamond)";
    cats[13] = "Robotics";
    qtys[13] = 100;
    costs[13] = 0.3;
    names[14] = "Sample Container";
    cats[14] = "Science";
    qtys[14] = 200;
    costs[14] = 0.05;
    count = 15;
}
// Defualt Astronauts
void init_Astronauts(string names[], string ranks[], string status[], int &count)
{
    names[0] = "Neil Armstrong";
    ranks[0] = "Commander";
    status[0] = "Retired";
    names[1] = "Buzz Aldrin";
    ranks[1] = "Pilot";
    status[1] = "Retired";
    names[2] = "Michael Collins";
    ranks[2] = "Pilot";
    status[2] = "Retired";
    names[3] = "Yuri Gagarin";
    ranks[3] = "Cosmonaut";
    status[3] = "Retired";
    names[4] = "Alan Shepard";
    ranks[4] = "Commander";
    status[4] = "Retired";
    names[5] = "John Glenn";
    ranks[5] = "Pilot";
    status[5] = "Retired";
    names[6] = "Victor Glover";
    ranks[6] = "Commander";
    status[6] = "Active";
    names[7] = "Reid Wiseman";
    ranks[7] = "Commander";
    status[7] = "Active";
    names[8] = "Christina Koch";
    ranks[8] = "Specialist";
    status[8] = "Active";

    count = 9; // Only real astronauts
}
// Solar system Planets
void init_Planets(string names[], string types[], double dists[], double gravs[], string atms[], int &count)
{
    names[0] = "Mercury";
    types[0] = "Rocky";
    dists[0] = 0.39;
    gravs[0] = 3.7;
    atms[0] = "None";
    names[1] = "Venus";
    types[1] = "Rocky";
    dists[1] = 0.72;
    gravs[1] = 8.87;
    atms[1] = "Thick CO2";
    names[2] = "Earth";
    types[2] = "Rocky";
    dists[2] = 1.0;
    gravs[2] = 9.81;
    atms[2] = "N2/O2";
    names[3] = "Mars";
    types[3] = "Rocky";
    dists[3] = 1.52;
    gravs[3] = 3.71;
    atms[3] = "Thin CO2";
    names[4] = "Jupiter";
    types[4] = "Gas";
    dists[4] = 5.2;
    gravs[4] = 24.79;
    atms[4] = "H/He";
    names[5] = "Saturn";
    types[5] = "Gas";
    dists[5] = 9.5;
    gravs[5] = 10.44;
    atms[5] = "H/He";
    names[6] = "Uranus";
    types[6] = "Ice";
    dists[6] = 19.2;
    gravs[6] = 8.69;
    atms[6] = "H/He/CH4";
    names[7] = "Neptune";
    types[7] = "Ice";
    dists[7] = 30.0;
    gravs[7] = 11.15;
    atms[7] = "H/He/CH4";
    count = 8;
}
// outer terestial palanets
void init_Exoplanets(string names[], double dists[], string types[], bool habitable[], int &count)
{
    names[0] = "Proxima Centauri b";
    dists[0] = 4.2;
    habitable[0] = true;
    types[0] = "Super Earth";
    names[1] = "TRAPPIST-1e";
    dists[1] = 39.0;
    habitable[1] = true;
    types[1] = "Earth-size";
    names[2] = "Kepler-186f";
    dists[2] = 500.0;
    habitable[2] = true;
    types[2] = "Earth-size";
    names[3] = "Kepler-22b";
    dists[3] = 600.0;
    habitable[3] = true;
    types[3] = "Super Earth";
    names[4] = "HD 209458 b";
    dists[4] = 150.0;
    habitable[4] = false;
    types[4] = "Hot Jupiter";
    count = 5;
}

// Module for saving users
void saveUsers(string usernames[], string passwords[], string roles[], string departments[], int count)
{
    ofstream f("nasa_users.csv");
    if (f.is_open())
    {
        f << count << endl;
        for (int i = 0; i < count; i++)
            f << usernames[i] << "," << passwords[i] << "," << roles[i] << "," << departments[i] << endl;
    }
}
// Module for loading the users in the file
void loadUsers(string usernames[], string passwords[], string roles[], string departments[], int &count)
{
    ifstream f("nasa_users.csv");
    if (f.is_open())
    {
        int cnt;
        f >> cnt;
        if (cnt > MAX_USERS)
            cnt = MAX_USERS;
        count = 0;
        string line;
        f.ignore(1000, '\n');
        for (int i = 0; i < cnt && i < MAX_USERS; i++)
        {
            if (getline(f, line))
            {
                size_t p1 = line.find(',');
                size_t p2 = line.find(',', p1 + 1);
                size_t p3 = line.find(',', p2 + 1);
                if (p3 != string::npos)
                {
                    usernames[i] = line.substr(0, p1);
                    passwords[i] = line.substr(p1 + 1, p2 - p1 - 1);
                    roles[i] = line.substr(p2 + 1, p3 - p2 - 1);
                    departments[i] = line.substr(p3 + 1);
                    count++;
                }
            }
        }
    }
}
// For saving the hire applicants
void saveHires(string users[], string roles[], string exp[], string status[], string names[], string edu[], int count)
{
    ofstream f("nasa_hires.csv");
    if (f.is_open())
    {
        f << count << endl;
        for (int i = 0; i < count; i++)
            f << users[i] << "," << roles[i] << "," << exp[i] << "," << status[i] << "," << names[i] << "," << edu[i] << endl;
    }
}
// For loading the applicants
void loadHires(string users[], string roles[], string exp[], string status[], string names[], string edu[], int &count)
{
    ifstream f("nasa_hires.csv");
    if (f.is_open())
    {
        int cnt;
        f >> cnt;
        f.ignore(1000, '\n');
        count = 0;
        string line;
        for (int i = 0; i < cnt; i++)
        {
            if (getline(f, line))
            {
                size_t p1 = line.find(',');
                size_t p2 = line.find(',', p1 + 1);
                size_t p3 = line.find(',', p2 + 1);
                size_t p4 = line.find(',', p3 + 1);
                size_t p5 = line.find(',', p4 + 1);
                if (p5 != string::npos)
                {
                    users[i] = line.substr(0, p1);
                    roles[i] = line.substr(p1 + 1, p2 - p1 - 1);
                    exp[i] = line.substr(p2 + 1, p3 - p2 - 1);
                    status[i] = line.substr(p3 + 1, p4 - p3 - 1);
                    names[i] = line.substr(p4 + 1, p5 - p4 - 1);
                    edu[i] = line.substr(p5 + 1);
                    count++;
                }
            }
        }
    }
}
// For saving the missions
void saveMissions(int count, string names[], string status[], string requesters[], double costs[], double budget, string dates[])
{
    ofstream f("nasa_missions.csv");
    if (f.is_open())
    {
        f << count << "," << budget << endl;
        for (int i = 0; i < count; i++)
            f << names[i] << "," << status[i] << "," << requesters[i] << "," << costs[i] << "," << dates[i] << endl;
    }
}
// For loading the missions
void loadMissions(int &count, string names[], string codes[], string vehicles[], string status[], double budgets[], string requesters[], double costs[], double &agencyBudget, string dates[])
{
    ifstream f("nasa_missions.csv");
    if (f.is_open())
    {
        int cnt;
        double bud;
        char c;
        f >> cnt >> c >> bud; // Read int, comma, double
        f.ignore(1000, '\n');
        agencyBudget = bud;
        count = 0;
        string line;
        for (int i = 0; i < cnt && i < MAX_MISSIONS; i++)
        {
            if (getline(f, line))
            {
                size_t p1 = line.find(',');
                size_t p2 = line.find(',', p1 + 1);
                size_t p3 = line.find(',', p2 + 1);
                size_t p4 = line.find(',', p3 + 1);
                if (p4 != string::npos)
                {
                    names[i] = line.substr(0, p1);
                    status[i] = line.substr(p1 + 1, p2 - p1 - 1);
                    requesters[i] = line.substr(p2 + 1, p3 - p2 - 1);
                    costs[i] = safeStod(line.substr(p3 + 1, p4 - p3 - 1));
                    dates[i] = line.substr(p4 + 1);
                    codes[i] = "MSN-" + to_string(i + 101);
                    vehicles[i] = "TBD";
                    budgets[i] = costs[i];
                    count++;
                }
            }
        }
    }
}
// For Saving Astronauts
void saveAstronauts(int count, string names[], string ranks[], string status[])
{
    ofstream f("nasa_astro.csv");
    if (f.is_open())
    {
        f << count << endl;
        for (int i = 0; i < count; i++)
            f << names[i] << "," << ranks[i] << "," << status[i] << endl;
    }
}
// For loading Astronauts
void loadAstronauts(int &count, string names[], string ranks[], string status[])
{
    ifstream f("nasa_astro.csv");
    if (f.is_open())
    {
        int cnt;
        f >> cnt;
        f.ignore(1000, '\n');
        if (cnt > 0)
            count = 0;
        string line;
        for (int i = 0; i < cnt; i++)
        {
            if (getline(f, line))
            {
                size_t p1 = line.find(',');
                size_t p2 = line.find(',', p1 + 1);
                if (p2 != string::npos)
                {
                    names[i] = line.substr(0, p1);
                    ranks[i] = line.substr(p1 + 1, p2 - p1 - 1);
                    status[i] = line.substr(p2 + 1);
                    count++;
                }
            }
        }
    }
}
// For saving inventory
void saveInventory(int count, string names[], string cats[], double qtys[], string units[], double costs[])
{
    ofstream f("nasa_inv.csv");
    if (f.is_open())
    {
        f << count << endl;
        for (int i = 0; i < count; i++)
            f << names[i] << "," << cats[i] << "," << qtys[i] << "," << units[i] << "," << costs[i] << endl;
    }
}
// For loading inventory
void loadInventory(int &count, string names[], string cats[], double qtys[], string units[], double costs[])
{
    ifstream f("nasa_inv.csv");
    if (f.is_open())
    {
        int cnt;
        f >> cnt;
        f.ignore(1000, '\n');
        if (cnt > 0)
            count = 0;
        string line;
        for (int i = 0; i < cnt; i++)
        {
            if (getline(f, line))
            {
                size_t p1 = line.find(',');
                size_t p2 = line.find(',', p1 + 1);
                size_t p3 = line.find(',', p2 + 1);
                size_t p4 = line.find(',', p3 + 1);
                if (p4 != string::npos)
                {
                    names[i] = line.substr(0, p1);
                    cats[i] = line.substr(p1 + 1, p2 - p1 - 1);
                    qtys[i] = safeStod(line.substr(p2 + 1, p3 - p2 - 1));
                    units[i] = line.substr(p3 + 1, p4 - p3 - 1);
                    costs[i] = safeStod(line.substr(p4 + 1));
                    count++;
                }
            }
        }
    }
}
// For saving planets
void savePlanets(int count, string names[], string types[], double dists[], double gravs[], string atms[])
{
    ofstream f("nasa_planets.csv");
    if (f.is_open())
    {
        f << count << endl;
        for (int i = 0; i < count; i++)
            f << names[i] << "," << types[i] << "," << dists[i] << "," << gravs[i] << "," << atms[i] << endl;
    }
}
// for loading planets
void loadPlanets(int &count, string names[], string types[], double dists[], double gravs[], string atms[])
{
    ifstream f("nasa_planets.csv");
    if (f.is_open())
    {
        int cnt;
        f >> cnt;
        f.ignore(1000, '\n');
        if (cnt > 0)
            count = 0;
        string line;
        for (int i = 0; i < cnt; i++)
        {
            if (getline(f, line))
            {
                size_t p1 = line.find(',');
                size_t p2 = line.find(',', p1 + 1);
                size_t p3 = line.find(',', p2 + 1);
                size_t p4 = line.find(',', p3 + 1);
                if (p4 != string::npos)
                {
                    names[i] = line.substr(0, p1);
                    types[i] = line.substr(p1 + 1, p2 - p1 - 1);
                    dists[i] = safeStod(line.substr(p2 + 1, p3 - p2 - 1));
                    gravs[i] = safeStod(line.substr(p3 + 1, p4 - p3 - 1));
                    atms[i] = line.substr(p4 + 1);
                    count++;
                }
            }
        }
    }
}
// For saving exoplanets
void saveExoplanets(int count, string names[], double dists[], string types[], bool habitable[])
{
    ofstream f("nasa_exo.csv");
    if (f.is_open())
    {
        f << count << endl;
        for (int i = 0; i < count; i++)
            f << names[i] << "," << dists[i] << "," << types[i] << "," << habitable[i] << endl;
    }
}
// For loading exoplanets
void loadExoplanets(int &count, string names[], double dists[], string types[], bool habitable[])
{
    ifstream f("nasa_exo.csv");
    if (f.is_open())
    {
        int cnt;
        f >> cnt;
        f.ignore(1000, '\n');
        if (cnt > 0)
            count = 0;
        string line;
        for (int i = 0; i < cnt && i < MAX_PLANETS; i++)
        {
            if (getline(f, line))
            {
                size_t p1 = line.find(',');
                size_t p2 = line.find(',', p1 + 1);
                size_t p3 = line.find(',', p2 + 1);
                if (p3 != string::npos)
                {
                    names[i] = line.substr(0, p1);
                    dists[i] = safeStod(line.substr(p1 + 1, p2 - p1 - 1));
                    types[i] = line.substr(p2 + 1, p3 - p2 - 1);
                    habitable[i] = (line.substr(p3 + 1) == "1");
                    count++;
                }
            }
        }
    }
}
// For saving the activities logs
void saveLogs(int count, string actions[])
{
    ofstream f("nasa_logs.csv");
    if (f.is_open())
    {
        f << count << endl;
        for (int i = 0; i < count; i++)
            f << actions[i] << endl;
    }
}
// For loading the activities logs
void loadLogs(int &count, string actions[])
{
    ifstream f("nasa_logs.csv");
    if (f.is_open())
    {
        f >> count;
        f.ignore(1000, '\n');
        int max = count;
        count = 0;
        string l;
        for (int i = 0; i < max; i++)
        {
            if (getline(f, l) && count < MAX_LOGS)
                actions[count++] = l;
        }
    }
}
// for adding something into the log
void addLog(string action, string logActions[], int &logCount)
{
    if (logCount < MAX_LOGS)
    {
        logActions[logCount++] = action;
        saveLogs(logCount, logActions);
    }
}

// Utilitiies for different general actions
void about()
{
    drawLogo();
    drawBox(10, 12, 80, 18);

    gotoxy(42, 13);
    animations(YLW + "ABOUT THIS APP" + RST, 20);

    gotoxy(18, 16);
    cout << MGD << "Welcome to the NASA(National Aeronautics and Space Administration)";
    gotoxy(18, 17);
    cout << MGD << "Management Streamlines";

    gotoxy(18, 19);
    cout << BL << "This app will take you through the histories of this planet and ";
    gotoxy(38, 20);
    cout << BL << "mysteries of the universe ";

    gotoxy(38, 25);
    cout << RD << "Version 2.7.1958 by Zeeshan\n";

    gotoxy(40, 24);
    cout << GRN << "App in Beta Testing" << RST;
    gotoxy(18, 22);
    cout << RD << "Be careful! you can hit by an asteroid. Play safe, stay safe" << RST;

    gotoxy(18, 27);
    cout << GRN << "Press any key..." << RST;
    _getch();
}
void history()
{
    drawLogo();
    drawBox(5, 12, 90, 22);

    gotoxy(35, 13);
    animations(MGD + "NASA HISTORY" + RST, 20);

    gotoxy(10, 15);
    cout << "National Aeronautics and Space Administration is US Agency established in 1958";
    gotoxy(10, 16);
    cout << "It is responsible for doing research for science and Technology in space and air.";

    gotoxy(15, 18);
    cout << CYN << "1. Apollo 11(1969)" << RST;
    gotoxy(12, 19);
    cout << "First human landed on the Moon.";

    gotoxy(15, 21);
    cout << CYN << "2. STS-1(1981)" << RST;
    gotoxy(12, 22);
    cout << "First orbital flight of Space Shuttle.";

    gotoxy(15, 24);
    cout << CYN << "3. Mars Pathfinder (1997)" << RST;
    gotoxy(12, 25);
    cout << "Landed a base station on Mars.";

    gotoxy(17, 27);
    cout << MGD << "Now it's time to carve our own history, to go beyond the horizon" << RST;

    gotoxy(10, 29);
    cout << GRN << "Press any key..." << RST;
    _getch();
}
void exit()
{
    system("cls");
    gotoxy(5, 5);
    cout << RD << "SHUTTING DOWN SYSTEM..." << RST << endl;
    Sleep(1000);
    exit(0);
}
// For coordination and positioning
void gotoxy(int x, int y)
{
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}
// For hidding the cursor while loading the app
void setCursor(bool visible)
{
    cout << (visible ? "\033[?25h" : "\033[?25l");
}
// Displaying the logo
void drawLogo(bool animate)
{
    system("cls");
    int X = 20;
    int Y = 3;
    string logo[] = {
        RD + "  _   _      _    ____      _    " + RST,
        RD + " | \\ | |    / \\  / ___|    / \\   " + RST,
        RD + " |  \\| |   / _ \\ \\___ \\   / _ \\  " + RST,
        RD + " | |\\  |  / ___ \\ ___) | / ___ \\ " + RST,
        RD + " |_| \\_| /_/   \\_\\____/ /_/   \\_\\" + RST,
        YLW + "              MANAGEMENT SYSTEM" + RST};
    string ufo[] = {
        CYN + "       _.---._    " + RST,
        CYN + "     .'       '.  " + RST,
        CYN + " _.-~___________~-._" + RST,
        CYN + "(___________________)" + RST,
        CYN + "     I  I  I  I   " + RST};
    for (int i = 0; i < 6; i++)
    {
        gotoxy(X, Y + i);
        if (animate)
        {
            animations(logo[i], 5);
        }
        else
        {
            cout << logo[i];
        }
        if (i < 5)
        {
            cout << ufo[i];
        }
        cout << endl;
    }

    gotoxy(X + 15, Y + 7);
    if (animate)
    {
        animations(CYN + "Taking you beyond the horizon... " + RST, 20);
    }
    else
    {
        cout << CYN + "Taking you beyond the horizon..." + RST;
    }
}
// For drawing box around different sections
void drawBox(int x, int y, int width, int height)
{

    gotoxy(x, y);
    cout << "+";
    for (int i = 1; i < width - 1; i++)
        cout << "=";
    cout << "+" << endl;
    for (int i = 1; i < height - 1; i++)
    {
        gotoxy(x, y + i);
        cout << "||";
        gotoxy(x + width - 1, y + i);
        cout << "||" << endl;
    }
    gotoxy(x, y + height - 1);
    cout << "+";
    for (int i = 0; i < width - 1; i++)
        cout << "=";
    cout << "+" << endl;
}
// For animating the text as design
void animations(string t, int s)
{
    for (char c : t)
    {
        cout << c;
        Sleep(s);
    }
}
// For clearing the buffer after any input so that it doesn't cause any issues
void clearKeyboardBuffer()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hIn);
}
// For taking valid input and omitting out any error causing thing in input
string getInput(string prompt)
{
    cout << prompt;
    string input;
    getline(cin, input);
    for (size_t i = 0; i < input.length(); i++)
    {
        if (input[i] == ',' || input[i] == '|')
            input[i] = ';';
    }
    return input;
}

// Safe conversions for CSV Loading
int safeStoi(string s)
{
    try
    {
        return stoi(s);
    }
    catch (...)
    {
        return 0;
    }
}
double safeStod(string s)
{
    try
    {
        return stod(s);
    }
    catch (...)
    {
        return 0.0;
    }
}
// For taking input within some range in integer
int getInt(string p, int min, int max)
{
    int v;
    if (min > max)
    {
        return min;
    }
    while (true)
    {
        if (p != "")
            cout << p;
        if (cin >> v && v >= min && v <= max)
        {
            cin.ignore(1000, '\n');
            return v;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << RD << "   Invalid Input. Range (" << min << "-" << max << "): " << RST;
    }
}
// For takin input within some range in double
double getDouble(string p, double min, double max)
{
    double v;
    while (true)
    {
        if (p != "")
            cout << p;
        if (cin >> v && v >= min && v <= max)
        {
            cin.ignore(1000, '\n');
            return v;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        cout << RD << "   Invalid Input. Range (" << min << "-" << max << "): " << RST;
    }
}
// For pausing and wait for user interaction
void pause()
{
    cout << "\n[Press Key]";
    _getch();
}
// For checking username validity
bool isValidUsername(string u)
{
    if (u.length() < 4 || u.length() > 10)
    {
        return false;
    }
    for (int i = 0; i < u.length(); i++)
    {
        if (!((u[i] >= 'a' && u[i] <= 'z') || (u[i] >= '0' && u[i] <= '9')))
        {
            return false;
        }
    }
    return true;
}

bool isValidPassword(string p)
{
    if (p.length() < 6)
        return false;
    bool small = false;
    bool large = false;
    bool digits = false;
    bool specialChar = false;

    for (int i = 0; i < p.length(); i++)
    {
        if (p[i] >= 'a' && p[i] <= 'z')
            small = true;
        else if (p[i] >= 'A' && p[i] <= 'Z')
            large = true;
        else if (p[i] >= '0' && p[i] <= '9')
            digits = true;
        else
            specialChar = true;
    }
    return (small && large && digits && specialChar);
}
// To display a specific message
void message(string msg)
{
    system("cls");
    drawLogo();
    drawBox(15, 12, 70, 5);
    gotoxy(20, 14);
    cout << RD << msg << RST;
    pause();
}
