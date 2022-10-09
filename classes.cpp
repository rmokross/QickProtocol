/*
#include </Library/Developer/CommandLineTools/usr/include/c++/v1/iostream>
#include </Library/Developer/CommandLineTools/usr/include/c++/v1/string>
#include </Library/Developer/CommandLineTools/usr/include/c++/v1/fstream>
*/
#include <iostream>
#include <string>
#include <fstream>


using namespace std;
// Classes

class Customer
{
    public:
        int customerID;
        string name;
        bool status;

    Customer(int c, string n, bool x) // constructor
    {
        customerID = c;
        name = n;
        status = x;
    }
};

class Project
{
    public:
        int projectID;
        int customerID;
        string name;
        string description;
        int start_date;
        int exp_date; // official ending date
        int end_date; // real end of project
        bool status;
        string protocol_dir;

    Project(int a, int b, string c, string d, int g, int h, int i, bool j, string k)
    {
        projectID = a;
        customerID = b;
        name = c;
        description = d;
        start_date = g;
        exp_date = h; 
        end_date = i; 
        status = j;
        protocol_dir = k;
    }
};

class Protocol
{
    public:
        int protocolID;
        string protocolType;
        int creatDate;
        string title;
        string summary;
        int templ;
        string scratchPad;
    
    Protocol(int a, string b, int c, string d, string e, int f, string g)
    {
        protocolID = a;
        protocolType = b;
        creatDate = c;
        title = d;
        summary = e;
        templ = f;
        scratchPad = g;
    }
};

class ToDo
{
    public:
        int todoID;
        int protocolID;
        bool status;
        string todoText;

    ToDo(int a, int b, bool c, string d)
    {
        todoID = a;
        protocolID = b;
        status = c;
        todoText = d;
    }
};

int main()
{
    string fileLine;
    ifstream custFile("./test_files/customers.txt");
    int init = 0, customer;
    cout << "Choose a customer:" << endl; 
    while (getline (custFile, fileLine))
    {
        cout << init << " : "+fileLine << endl;
        init++;
    }
    cin >> customer;
    cout << "Customer:" << customer << endl;

    return 0;
    // cout <<< ""
}