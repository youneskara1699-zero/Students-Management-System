#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cstdlib>
using namespace std;

string FileName = "Students.txt";

struct stStudent 
{
    string ID;
    string FirstName;
    string LastName;
    float Grade;
    bool MarkForDelete = false;
};

enum enStudentMenuOptions {
    StudentList = 1, AddStudent = 2, DeleteStudent = 3, 
    UpdateStudentGrade = 4, SearchStudent = 5, 
    ShowStatistics = 6, SortStudents = 7, CountStudents = 8, Exit = 9
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    int pos = 0;
    string word = "";
    while ((pos = S1.find(Delim)) != string::npos)
    {
        word = S1.substr(0, pos);
        if (word != "")
        {
             vString.push_back(word);
        }
        
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }
    
    return vString;
}

stStudent ConvertLineToStudentRecord(string Line, string Separator = "#//#")
{
    vector<string> vStudentData = SplitString(Line, Separator);
    stStudent Student;
    if (vStudentData.size() >= 4)
    {
        Student.ID = vStudentData[0];
        Student.FirstName = vStudentData[1];
        Student.LastName = vStudentData[2];
        try {
            Student.Grade = stof(vStudentData[3]);
        } catch (...) {
            Student.Grade = 0.0f; 
        }
    }
    
    return Student;
}

string ConvertStudentRecordToLine(stStudent Student, string Separator = "#//#")
{
    string LineRecord;
    
    LineRecord += Student.ID + Separator; 
    LineRecord += Student.FirstName + Separator;
    LineRecord += Student.LastName + Separator;
    LineRecord += to_string(Student.Grade);
    return LineRecord;
}

vector<stStudent> LoadStudentsDataFromFile()
{
    vector<stStudent> vStudents;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
   
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            if (!Line.empty())
            {
                stStudent Student = ConvertLineToStudentRecord(Line);
                if (Student.MarkForDelete == false)
                {
                    vStudents.push_back(Student);
                }
            }
        }
        
        MyFile.close();
    }
   
    return vStudents;
}

void SaveStudentsDataToFile(vector<stStudent>& vStudents)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        string Line;
        for (stStudent& Student : vStudents)
        {
            if (Student.MarkForDelete == false)
            {
                Line = ConvertStudentRecordToLine(Student);  
                MyFile << Line << endl; 
            }
        }
        
        MyFile.close();
    }
}

void HeaderOfScreen(string Title, string SubTitle)
{
    cout << "==========================================" << endl;
    cout << Title << endl;
    if (!SubTitle.empty())
    {
        cout << SubTitle << endl;
    }
    cout << "==========================================" << endl;
}

void PrintStudentData(stStudent Student)
{
    cout << "|" << setw(10) << left << Student.ID;
    cout << "|" << setw(10) << left << Student.FirstName;
    cout << "|" << setw(10) << left << Student.LastName;
    cout << "|" << setw(7)  << left << Student.Grade << "|";
}

void ShowStudentsListScreen()
{
    vector<stStudent> vStudents = LoadStudentsDataFromFile();
    HeaderOfScreen("\t\tSTUDENTS LIST", "");
    
    cout << "|" << setw(10) << left << "Student ID";
    cout << "|" << setw(10) << left << "First Name";
    cout << "|" << setw(10) << left << "Last Name";
    cout << "|" << setw(7)  << left << "Grade" << "|" << endl;
    cout << "==========================================" << endl;

    if (vStudents.empty())
    {
        cout << "No students found." << endl;
    }
    else
    {
        for (stStudent& Student : vStudents)
        {
            PrintStudentData(Student);
            cout << endl;
        }       
    }
    
    cout << "==========================================\n\n";
}

string ReadID()
{
    string ID;
    
    do
    {
        cout << "Please, Enter ID (Just 4 Letters): ";
        cin >> ID;
        if (ID.length() != 4)
        {
            cout << "Invalid! ID must be exactly 4 characters." << endl; 
        }
    } while (ID.length() != 4);
    
    return ID;
}

float ReadGrade()
{
    float Grade;
    cout << "Please, Enter Grade: ";
    cin >> Grade;
    return Grade;
}

string ReadName(string Message)
{
    string Name;
    cout << Message;
    getline(cin >> ws, Name);
    return Name;  
}

bool IsStudentExist(string ID, const vector<stStudent>& vStudents)
{
    for (const stStudent& Student : vStudents)
    {
        if (Student.ID == ID)
        {
            return true;
        }       
    }
    
    return false;
}

stStudent ReadStudentInfo(string ID)
{
    stStudent Student;
    Student.ID = ID;
    Student.FirstName = ReadName("Please, Enter First Name: ");
    Student.LastName = ReadName("Please, Enter Last Name: ");
    Student.Grade = ReadGrade();

    return Student;
} 

void AddNewStudentScreen()
{
    HeaderOfScreen("\t\tADD NEW STUDENT", "");
    char Answer = 'Y';
   
    do
    {
        vector<stStudent> vStudents = LoadStudentsDataFromFile();
        string ID = ReadID();
        
        if (IsStudentExist(ID, vStudents))
        {
            cout << "Error! Student with this ID already exists." << endl;
            cout << "Try Again!" << endl;
            continue;
        }
    
        stStudent NewStudent = ReadStudentInfo(ID);
        vStudents.push_back(NewStudent);
        SaveStudentsDataToFile(vStudents);
        
        cout << "\nStudent Added Successfully!\n";
        cout << "\nDo you want to add more students? (Y/N): ";
        cin >> Answer;

    } while (toupper(Answer) == 'Y');
}

bool StudentMarkedForDelete(string ID, vector<stStudent>& vStudents)
{
    for (stStudent& Student : vStudents)
    {
        if (Student.ID == ID)
        {
            Student.MarkForDelete = true;
            return true;
        }        
    }
    
    return false;
}

void DeleteStudentScreen()
{
    HeaderOfScreen("\t\tDELETE STUDENT", "");
    
    vector<stStudent> vStudents = LoadStudentsDataFromFile();
    string ID = ReadID();
    
    if (!IsStudentExist(ID, vStudents))
    {
        cout << "Error! Student with this ID not found." << endl;
        return;
    }

    char confirm;
    cout << "Are you sure you want to delete this student? (Y/N): ";
    cin >> confirm;
    
    if (toupper(confirm) == 'Y')
    {
        if (StudentMarkedForDelete(ID, vStudents))
        {
            SaveStudentsDataToFile(vStudents);
            cout << "Student Deleted Successfully!" << endl;
        }
        else
        {
            cout << "Error deleting student." << endl;
        }
    }
    else
    {
        cout << "Deletion cancelled." << endl;
    }
}

void PrintStudentInfo(stStudent Student)
{
    cout << "Student Information: " << endl;
    cout << "==========================================" << endl;
    cout << "Student ID        : " << Student.ID << endl;
    cout << "Student FirstName : " << Student.FirstName << endl;
    cout << "Student LastName  : " << Student.LastName << endl;
    cout << "Student Grade     : " << Student.Grade << endl;
    cout << "==========================================" << endl;
}

void UpdateGradeScreen()
{
    HeaderOfScreen("\t\tUPDATE STUDENT GRADE", "");
    
    vector<stStudent> vStudents = LoadStudentsDataFromFile();
    string ID = ReadID();
    
    if (!IsStudentExist(ID, vStudents))
    {
        cout << "Error! Student with this ID not found." << endl;
        return;
    }

    bool found = false;
    
    for (stStudent& Student : vStudents)
    {
        if (Student.ID == ID)
        {
            PrintStudentInfo(Student);
            found = true;
            
            cout << "\nEnter new grade: ";
            float NewGrade;
            cin >> NewGrade;
            
            Student.Grade = NewGrade;
            SaveStudentsDataToFile(vStudents);
            cout << "\nGrade updated successfully!" << endl;
            break;
        }      
    }
    
    if (!found)
    {
        cout << "Error updating grade." << endl;
    }
}

void SearchStudentScreen()
{
    HeaderOfScreen("\t\tSEARCH STUDENT", "");
    
    vector<stStudent> vStudents = LoadStudentsDataFromFile();
    string ID = ReadID();
    
    if (!IsStudentExist(ID, vStudents))
    {
        cout << "Error! Student with this ID not found." << endl;
        return;
    }
    
    for (stStudent& Student : vStudents)
    {
        if (Student.ID == ID)
        {
            PrintStudentInfo(Student);
            break;
        }      
    }  
}

void ShowStatisticsScreen()
{
    HeaderOfScreen("\t\tSTATISTICS", "");
    
    vector<stStudent> vStudents = LoadStudentsDataFromFile();
    
    if (vStudents.empty())
    {
        cout << "No students found." << endl;
        return;
    }
    
    int totalStudents = vStudents.size();
    float totalGrade = 0.0f;
    float highestGrade = vStudents[0].Grade;
    float lowestGrade = vStudents[0].Grade;
    
    for (const stStudent& Student : vStudents)
    {
        totalGrade += Student.Grade;
        if (Student.Grade > highestGrade) highestGrade = Student.Grade;
        if (Student.Grade < lowestGrade) lowestGrade = Student.Grade;
    }
    
    float averageGrade = totalGrade / totalStudents;
    
    cout << "Total Students    : " << totalStudents << endl;
    cout << "Average Grade     : " << fixed << setprecision(2) << averageGrade << endl;
    cout << "Highest Grade     : " << highestGrade << endl;
    cout << "Lowest Grade      : " << lowestGrade << endl;
    cout << "==========================================\n\n";
}

void SortStudentsScreen()
{
    HeaderOfScreen("\t\tSORT STUDENTS", "");
    
    vector<stStudent> vStudents = LoadStudentsDataFromFile();
    
    if (vStudents.empty())
    {
        cout << "No students found to sort." << endl;
        return;
    }
    
    cout << "Sort by:\n";
    cout << "[1] ID (Ascending)\n";
    cout << "[2] Grade (Highest First)\n";
    cout << "[3] Name (Alphabetical)\n";
    cout << "Enter choice (1-3): ";
    
    int choice;
    cin >> choice;
    
    switch (choice)
    {
        case 1: // Sort by ID
            sort(vStudents.begin(), vStudents.end(), 
                [](const stStudent& a, const stStudent& b) {
                    return a.ID < b.ID;
                });
            break;
            
        case 2: // Sort by Grade (descending)
            sort(vStudents.begin(), vStudents.end(), 
                [](const stStudent& a, const stStudent& b) {
                    return a.Grade > b.Grade;
                });
            break;
            
        case 3: // Sort by Last Name then First Name
            sort(vStudents.begin(), vStudents.end(), 
                [](const stStudent& a, const stStudent& b) {
                    if (a.LastName == b.LastName)
                        return a.FirstName < b.FirstName;
                    return a.LastName < b.LastName;
                });
            break;
            
        default:
            cout << "Invalid choice! Returning to menu.\n";
            return;
    }
    
    // Display sorted list
    cout << "\nSorted Students List:\n";
    cout << "==========================================\n";
    cout << "|" << setw(10) << left << "Student ID";
    cout << "|" << setw(10) << left << "First Name";
    cout << "|" << setw(10) << left << "Last Name";
    cout << "|" << setw(7)  << left << "Grade" << "|\n";
    cout << "==========================================\n";
    
    for (const stStudent& Student : vStudents)
    {
        PrintStudentData(Student);
        cout << endl;
    }
    cout << "==========================================\n\n";
}

void CountStudentsScreen()
{
    HeaderOfScreen("\t\tCOUNT STUDENTS", "");
    
    vector<stStudent> vStudents = LoadStudentsDataFromFile();
    
    cout << "Total number of students: " << vStudents.size() << endl;
    
    if (!vStudents.empty())
    {
        int excellent = 0, good = 0, average = 0, fail = 0;
        
        for (const stStudent& Student : vStudents)
        {
            if (Student.Grade >= 90) excellent++;
            else if (Student.Grade >= 75) good++;
            else if (Student.Grade >= 50) average++;
            else fail++;
        }
        
        cout << "\nGrade Distribution:\n";
        cout << "Excellent (90-100): " << excellent << " students\n";
        cout << "Good (75-89): " << good << " students\n";
        cout << "Average (50-74): " << average << " students\n";
        cout << "Fail (0-49): " << fail << " students\n";
    }
    cout << "==========================================\n\n";
}

void GoBackToMainMenu()
{
    cout << "\nPress any key to go back to Main Menu..." << endl;
    system("pause > nul");
    system("cls");
}

int ReadChoice()
{
    int Choice;
    do
    {
        cout << "Please, Enter what do you want to do (1 To 9): ";
        cin >> Choice;
        if (Choice > 9 || Choice < 1)
        {
            cout << "Invalid Choice! Please enter a number between 1 and 9." << endl;
        }
        
    } while (Choice > 9 || Choice < 1);
    
    return Choice;
}

void PerformMainMenuOption(enStudentMenuOptions Choice)
{
    system("cls");
    
    switch (Choice)
    {
    case enStudentMenuOptions::StudentList:
    {
        ShowStudentsListScreen();
        GoBackToMainMenu();
        break; 
    }
    
    case enStudentMenuOptions::AddStudent:
    {
        AddNewStudentScreen();
        GoBackToMainMenu();
        break; 
    }
    
    case enStudentMenuOptions::DeleteStudent:
    {
        DeleteStudentScreen();
        GoBackToMainMenu();
        break; 
    }

    case enStudentMenuOptions::UpdateStudentGrade:
    {
        UpdateGradeScreen();
        GoBackToMainMenu();
        break; 
    }

    case enStudentMenuOptions::SearchStudent:
    {
        SearchStudentScreen();
        GoBackToMainMenu();
        break; 
    }

    case enStudentMenuOptions::ShowStatistics:
    {
        ShowStatisticsScreen();
        GoBackToMainMenu();
        break; 
    }

    case enStudentMenuOptions::SortStudents:
    {
        SortStudentsScreen();
        GoBackToMainMenu();
        break; 
    }

    case enStudentMenuOptions::CountStudents:
    {
        CountStudentsScreen();
        GoBackToMainMenu();
        break; 
    }

    case enStudentMenuOptions::Exit:
    {   
        system("cls");
        HeaderOfScreen("\t\tSTUDENT MANAGEMENT SYSTEM", "");
        cout << "\nThank you for using Student Management System. Goodbye!\n" << endl;
        exit(0);
        break; 
    }
     
    default:
        break;
    }
}

void ShowMainMenuScreen()
{
    system("cls");
    HeaderOfScreen("\t\tMain Menu Screen", "");
    cout << "[1] Show Students List" << endl;
    cout << "[2] Add New Student" << endl;
    cout << "[3] Delete Student" << endl;
    cout << "[4] Update Grade" << endl;
    cout << "[5] Search Student" << endl;
    cout << "[6] Show Statistics" << endl;
    cout << "[7] Sort Students" << endl;
    cout << "[8] Count Students" << endl;
    cout << "[9] Exit" << endl;
    cout << "==============================================\n\n";

    PerformMainMenuOption(enStudentMenuOptions(ReadChoice()));
}

int main()
{
    while (true)
    {
        ShowMainMenuScreen();
    }
    
    return 0;
}
