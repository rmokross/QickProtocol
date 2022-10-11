// Qick.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "sqlite/sqlite3.h"
using namespace std;


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

class Database
{
public:

 /*   Database(int a, string b, string c)
    {
        stdCustID = a;
        stdCust = b;
        dbms = c;
    }*/
    Database(){};

    void ShowErr(int rc,char* err, int cur_line, string cur_file)
    {
        // char* err;

        if (rc != SQLITE_OK)
        {
            cout << "Error: " << err << "\nLine: " + to_string(cur_line) + "\nFile: " + cur_file << endl;
        }
    }
    void StartupDB()
    {
        char* err;
        sqlite3* db;
        sqlite3_stmt* stmt;
        int rc, standardCustID = 9999;
        string query = "", line;
        string standardCust = "No Customer";
        
        sqlite3_open("protocol_DB.db", &db);
        // create tables
        rc = sqlite3_exec( // customer table
            db, 
            "CREATE TABLE IF NOT EXISTS "
            "customer("
            "customerID INT NOT NULL PRIMARY KEY, "
            "name varchar(100) NOT NULL, "
            "status BOOL DEFAULT 1); ",
            NULL, 
            NULL, 
            &err
        );
        ShowErr(rc, err, __LINE__, __FILE__);

        rc = sqlite3_exec( // project table
            db, 
            "CREATE TABLE IF NOT EXISTS "
            "project("
            "projectID NOT NULL PRIMARY KEY, "
            "customerID INT, "
            "name varchar(100) NOT NULL, "
            "description varchar(2000), "
            "start_date datetime default current_timestamp, "
            "end_date datetime, "
            "status BOOL DEFAULT 1, "
            "protocol_dir varchar(200), "
            "FOREIGN KEY (customerID) REFERENCES customer (customerID));", 
            NULL, 
            NULL, 
            &err
        ); // FOREIGN KEY(trackartist) REFERENCES artist(artistid)
        ShowErr(rc, err, __LINE__, __FILE__);

        rc = sqlite3_exec( // protocol table
            db, 
            "CREATE TABLE IF NOT EXISTS "
            "protocol("
            "protocolID INT NOT NULL PRIMARY KEY, "
            "protocol_type varchar(100) NOT NULL, "
            "create_dt datetime default current_timestamp, "
            "title varchar(100), "
            "summary varchar(10000), "
            "projectID INT, "
            "scratch_pad varchar(15000), "
            "FOREIGN KEY (projectID) REFERENCES project (projectID));", 
            NULL, 
            NULL, 
            &err
        );
        ShowErr(rc, err, __LINE__, __FILE__);
        rc = sqlite3_exec( // todo table
            db, 
            "CREATE TABLE IF NOT EXISTS "
            "todo("
            "todoID INT NOT NULL PRIMARY KEY, "
            "protocolID INT NOT NULL, "
            "status BOOL DEFAULT 1, "
            "name varchar(100), "
            "FOREIGN KEY(protocolID) REFERENCES protocol(protocolID)); ", 
            NULL, 
            NULL, 
            &err
        );
        ShowErr(rc, err, __LINE__, __FILE__);
        
        // check if standardCust already exists
        query = "SELECT count(name) FROM customer WHERE customerID = " + to_string(standardCustID) + "GROUP BY customerID;";
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        if (sizeof(stmt) == 0)
        {
            // customer for projects not linked to a real customer
            query = "INSERT IF NOT EXISTS INTO customer VALUES(" + to_string(standardCustID) + ", '" + standardCust + "', 1);";
            rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
            ShowErr(rc, err, __LINE__, __FILE__);
        }
        sqlite3_close(db);
    }
    string QuyerCustName(int custID)
    {
        char* err;
        sqlite3* db;
        sqlite3_stmt* stmt;
        string query, custNameDB;
        sqlite3_open("protocol_DB.db", &db);

        query = "SELECT name FROM customer WHERE customerID = " + to_string(custID) + ";";
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        custNameDB = std::string(reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, 0)
            ));
        sqlite3_close(db);
        return custNameDB;
    }
    int MaxCustID()
    {
        sqlite3* db;
        int standardCustID = 9999;
        string query;
        sqlite3_open("protocol_DB.db", &db);
        // returns the customer with the highest ID

        query = "SELECT max(customerID) FROM customer where customerID != " + to_string(standardCustID) + " GROUP BY name;";
        int maxID;
        sqlite3_stmt* stmt2;
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt2, 0);
        maxID = sqlite3_column_int(stmt2, 0);
        cout << to_string(maxID) << endl;
        sqlite3_close(db);
        return maxID;
    }
    void CreateCust()
    {
        char* err;
        sqlite3* db;
        int rc, standardCustID = 9999;
        string query;
        string standardCust = "No Customer";
        sqlite3_open("protocol_DB.db", &db);
        // creates a new customer with the next highest ID

        string custName;
        int maxID = MaxCustID() + 1;
        cout << "Please enter customer name: " << endl;
        cin >> custName;
        query = "INSERT INTO customer VALUES(" + to_string(maxID) + " , '" + custName.c_str() + "', 1)";
        rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
        ShowErr(rc, err, __LINE__, __FILE__);
        sqlite3_close(db);
    }
    void PrintCustNames()
    {
        sqlite3* db;
        sqlite3_stmt* stmt;
        int standardCustID = 9999;
        string query;
        string standardCust = "No Customer";
        sqlite3_open("protocol_DB.db", &db);

        const unsigned char* custName;
        int id;
        sqlite3_prepare_v2(db, "SELECT customerID, name FROM customer WHERE customerID != 0 AND status = 1;", -1, &stmt, 0);
        
        while (sqlite3_step(stmt) != SQLITE_DONE)
        {
            id = sqlite3_column_int(stmt, 0);
            custName = sqlite3_column_text(stmt, 1);
            cout << to_string(id) << " : " << custName << endl;
            
        }
        sqlite3_close(db);
        /*
        else
        {
            char answere;
            cout << "No custoimers in Database. Do you want to create a Customer? (Y/N)" << endl;
            cin >> answere;
            while (true)
            {
                if (answere == 'Y' || answere == 'y')
                {
                    string customer;
                    customer = CreateCust();
                    return customer;
                }
                else if (answere == 'N' || answere == 'n')
                {
                    cout << "Do you want to"
                }
            }
        }
        */
    }

    void PrintProjNames(int customerID)
    {
        sqlite3* db;
        sqlite3_stmt* stmt;
        int standardCustID = 9999;
        string query;
        string standardCust = "No Customer";
        sqlite3_open("protocol_DB.db", &db);

        const unsigned char* projName;
        int id;
        query = "SELECT projectID, name FROM project WHERE customerID =" + to_string(customerID) + ";";

        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE)
        {
            id = sqlite3_column_int(stmt, 0);
            projName = sqlite3_column_text(stmt, 1);
            cout << to_string(id) << " : " << projName << endl;
        }
    }
};
int main()
{
    // read configurations

    ifstream setupConfig("setup.config");
    string varName, varVal, line, delimiter = "=";
    vector<string> params;
    while (getline(setupConfig, line))
    {
        params.push_back(line);
    }

    string vaultDir;
    for (string param : params)
    {
        if (param.substr(0, param.find(delimiter)) == "vault_dir") // think of something nicer here
        {
            vaultDir = param.erase(0, param.find(delimiter) + delimiter.length());
        }
    }
    cout << "Looking at vault: " << vaultDir << endl;

    string templDir = vaultDir + "/Tempaltes";
    string protocolDir = vaultDir + "/protocols";

    // Handle user input


    int customerID;
    string custName;

    Database MyDB;
    MyDB.StartupDB();
    MyDB.PrintCustNames();
    cout << "42 : Create a new Customer" << endl;
    cout << "Enter ID: " << endl;
    cin >> customerID;
    
    // create new customer
    if (customerID == 42)
    {
        MyDB.CreateCust();
        customerID = MyDB.MaxCustID();
        custName = MyDB.QuyerCustName(customerID);
    }
    else // query data for customerID
    {
        // call method query customer 
        custName = MyDB.QuyerCustName(customerID);
    }
    Customer cu(customerID, custName, 1);

    // continue with PROJECT
    int projectID;
    string projName;

    cout << "Choose a project from your customer: " + cu.name << endl;
    MyDB.PrintProjNames(cu.customerID);
    cout << "42 : Create a new Customer" << endl;
    cout << "Enter ID: " << endl;
    cin >> projectID;


    return 0;
}

