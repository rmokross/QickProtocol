// Qick.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>

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
    int stdCustID;
    string stdCust;
    string dbms;

    Database(int a, string b, string c)
    {
        stdCustID = a;
        stdCust = b;
        dbms = c;
    }

    void ShowErr(int rc,char* err)
    {
        // char* err;

        if (rc != SQLITE_OK)
        {
            cout << "Error: " << err << endl;
        }
    }

    void StartupDB()
    {
        char* err;
        sqlite3* db;
        int rc, standardCustID = 9999;
        string query;
        string standardCust = "No Customer";


        sqlite3_open("protocol_DB.db", &db); 

        // create tables
        rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS customer(customerID INT NOT NULL PRIMARY KEY, name varchar(100) NOT NULL, status BOOL DEFAULT 1);", NULL, NULL, &err);
        ShowErr(rc, err);
        rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS project(projectID NOT NULL PRIMARY KEY, customerID INT, name varchar(100) NOT NULL, description varchar(2000), start_date datetime default current_timestamp, end_date datetime, status BOOL DEFAULT 1, protocol_dir varchar(200), FOREIGN KEY (customerID) REFERENCES customer (customerID));", 
            NULL, NULL, &err); // FOREIGN KEY(trackartist) REFERENCES artist(artistid)
        ShowErr(rc, err);
        rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS protocol(protocolID INT NOT NULL PRIMARY KEY, protocol_type varchar(100) NOT NULL, create_dt datetime default current_timestamp, title varchar(100), summary varchar(10000), projectID INT, scratch_pad varchar(15000), FOREIGN KEY (projectID) REFERENCES project (projectID));", NULL, NULL, &err);
        ShowErr(rc, err);
        rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS todo(todoID INT NOT NULL PRIMARY KEY, protocolID INT NOT NULL, status BOOL DEFAULT 1, name varchar(100), FOREIGN KEY (protocolID) REFERENCES protocol (protocolID));", NULL, NULL, &err);
        ShowErr(rc, err);

        // customer for projects not linked to a real customer
        query = "INSERT INTO customer VALUES(" + to_string(standardCustID) + ", '" + standardCust + "', 1);";
        rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
        ShowErr(rc, err);

    }
    int MaxCustID()
    {
        sqlite3* db;
        int standardCustID = 9999;
        string query;
        sqlite3_open("protocol_DB.db", &db);
        // returns the customer with the highest ID

        query = "SELECT max(customerID) FROM customer where customerID != " + to_string(standardCustID) + ";";
        int maxID;
        sqlite3_stmt* stmt2;
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt2, 0);
        maxID = sqlite3_column_int(stmt2, 0);
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
        query = "INSERT INTO customer VALUIES(" + to_string(maxID) + " , " + custName.c_str() + ")";
        rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
        ShowErr(rc, err);
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
        string comp;
        int id;
        sqlite3_prepare_v2(db, "SELECT customerID, name FROM customer WHERE customerID != 0 AND status = 1;", -1, &stmt, 0);
        
        while (sqlite3_step(stmt) != SQLITE_DONE)
        {
            id = sqlite3_column_int(stmt, 0);
            custName = sqlite3_column_text(stmt, 1);
            cout << to_string(id) << " : " << custName << endl;
            
        }
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
};
int main()
{
    int customerID;

    Database MyDB;
    MyDB.StartupDB();
    MyDB.PrintCustNames();
    cout << "42 : Create a new Customer" << endl;
    cin >> customerID;
    
    // create new customer
    if (customerID == 42)
    {
        MyDB.CreateCust();
    }
    else // query data for customerID
    {
        // call method query customer 
    }


    return 0;
}
