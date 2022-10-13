// Qick.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
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
    string customerName;
    string name;
    string description;
    int start_date;
    int exp_date; // official ending date
    int end_date; // real end of project
    bool status;
    string protocol_dir;

    Project(int a, string b, string c, string d, int g, int h, int i, bool j, string k)
    {
        projectID = a;
        customerName = b;
        name = c;
        description = d;
        start_date = g;
        exp_date = h;
        end_date = i;
        status = j;
        protocol_dir = k;
    }
    Project(int a, string b, string c)
    {
        projectID = a;
        customerName = b;
        name = c;
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
        int rc;
        string query = "", line;
        string standardCust = "No Customer";
        
        sqlite3_open("protocol_DB.db", &db);
        // create tables
        rc = sqlite3_exec( // customer table
            db, 
            "CREATE TABLE IF NOT EXISTS "
            "customer("
            "cust_name varchar(100) PRIMARY KEY, "
            "status BOOL DEFAULT 1);",
            NULL, 
            NULL, 
            &err
        );
        ShowErr(rc, err, __LINE__, __FILE__);

        rc = sqlite3_exec( // project table
            db, 
            "CREATE TABLE IF NOT EXISTS "
            "project("
            "cust_name varchar(100) NOT NULL, "
            "proj_name varchar(100) PRIMARY KEY, "
            "description varchar(2000), "
            "start_date datetime default current_timestamp, "
            "end_date datetime, "
            "status BOOL DEFAULT 1, "
            "protocol_dir varchar(200), "
            "FOREIGN KEY (cust_name) REFERENCES customer (cust_name));", 
            NULL, 
            NULL, 
            &err
        ); // FOREIGN KEY(trackartist) REFERENCES artist(artistid)
        ShowErr(rc, err, __LINE__, __FILE__);

        rc = sqlite3_exec( // protocol table
            db, 
            "CREATE TABLE IF NOT EXISTS "
            "protocol("
            "protocol_type varchar(100) NOT NULL, "
            "create_dt datetime default current_timestamp, "
            "title varchar(100) PRIMARY KEY, "
            "summary varchar(10000), "
            "proj_name varchar(100) NOT NULL, "
            "scratch_pad varchar(15000), "
            "FOREIGN KEY (proj_name) REFERENCES project (proj_name));", 
            NULL, 
            NULL, 
            &err
        );
        ShowErr(rc, err, __LINE__, __FILE__);
        rc = sqlite3_exec( // todo table
            db, 
            "CREATE TABLE IF NOT EXISTS "
            "todo("
            "title varchar(100) NOT NULL, "
            "status BOOL DEFAULT 1, "
            "todo_name varchar(100), "
            "FOREIGN KEY(title) REFERENCES protocol(title)); ", 
            NULL, 
            NULL, 
            &err
        );
        ShowErr(rc, err, __LINE__, __FILE__);
        
        // check if standardCust already exists
        query = "SELECT count(cust_name) FROM customer WHERE cust_name = " + standardCust + "GROUP BY cust_name;";
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);
        if (sizeof(stmt) == 0)
        {
            // customer for projects not linked to a real customer
            query = "INSERT IF NOT EXISTS INTO customer (cust_name, status) VALUES('" + standardCust + "', 1);";
            rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
            ShowErr(rc, err, __LINE__, __FILE__);
        }
        sqlite3_close(db);
    }
    string QueryCustName(int custID)
    {
        sqlite3* db;
        sqlite3_stmt* stmt;
        string query, cust;
        const unsigned char* custNameDB;
        sqlite3_open("protocol_DB.db", &db);
        
        query = "SELECT cust_name FROM customer WHERE rowid = " + to_string(custID) + ";";
        
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

        while (sqlite3_step(stmt) != SQLITE_DONE)
        {
            custNameDB = sqlite3_column_text(stmt, 0);

            for (int i = 0; i < sizeof(custNameDB); i++)
            {
                if (custNameDB[i] == '\0')
                {
                    break;
                }
                cust += custNameDB[i];
                // cout << s << endl;
            }
            
        }
        return cust;
    }

    string QueryProjName(int projID)
    {
        sqlite3* db;
        sqlite3_stmt* stmt;
        string query, projName;
        const unsigned char* projNameDB;
        sqlite3_open("protocol_DB.db", &db);

        query = "SELECT proj_name FROM project WHERE rowid = " + to_string(projID) + ";";

        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

        while (sqlite3_step(stmt) != SQLITE_DONE)
        {
            projNameDB = sqlite3_column_text(stmt, 0);

            for (int i = 0; i < sizeof(projNameDB); i++)
            {
                if (projNameDB[i] == '\0')
                {
                    break;
                }
                projName += projNameDB[i];
                // cout << s << endl;
            }

        }
        return projName;
    }

    int QueryProjID(string projName)
    {
        sqlite3* db;
        sqlite3_stmt* stmt;
        string query;
        int projID;
        sqlite3_open("protocol_DB.db", &db);

        query = "SELECT rowid FROM project WHERE proj_name = '" + projName + "';";
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

        projID = sqlite3_column_int(stmt, 0);

        return projID;
    }

    int QueryCustID(string custName)
    {
        sqlite3* db;
        sqlite3_stmt* stmt;
        string query;
        int custID;
        sqlite3_open("protocol_DB.db", &db);

        query = "SELECT rowid FROM customer WHERE cust_name = '" + custName + "';";
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

        custID = sqlite3_column_int(stmt, 0);

        return custID;
    }

    string CreateCust()
    {
        char* err;
        sqlite3* db;
        int rc, standardCustID = 9999;
        string query, dbAttr, vals;
        string standardCust = "No Customer";
        sqlite3_open("protocol_DB.db", &db);
        // creates a new customer with the next highest ID

        string custName;
        cout << "Please enter customer name: " << endl;
        cin >> custName;

        dbAttr = "(cust_name)";
        vals = "('" + custName + "');";
        query = "INSERT INTO customer "+ dbAttr +" VALUES"+ vals;
        rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
        ShowErr(rc, err, __LINE__, __FILE__);
        sqlite3_close(db);

        return custName;
    }
    string CreateProj(string custName, string projPath)
    {
        char* err;
        sqlite3* db;
        int rc, standardCustID = 9999;
        string query, projName, dbAttr, vals;
        string standardCust = "No Customer";
        sqlite3_open("protocol_DB.db", &db);

        
        cout << "Please enter project name: " << endl;
        cin >> projName;


        dbAttr = "(proj_name, cust_name, protocol_dir)"; // description and other attr not set
        vals = "('" +projName+"' ,'"+ custName+ "','" + projPath + "/" + custName + "/" + projName + "');";
        query = "INSERT INTO project" + dbAttr + " VALUES" + vals; 
        cout << query << endl;
        rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
        ShowErr(rc, err, __LINE__, __FILE__);
        sqlite3_close(db);

        return projName;
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
        sqlite3_prepare_v2(db, "SELECT rowid, cust_name FROM customer WHERE status = 1;", -1, &stmt, 0);
        
        while (sqlite3_step(stmt) != SQLITE_DONE)
        {
            id = sqlite3_column_int(stmt, 0);
            custName = sqlite3_column_text(stmt, 1);
            cout << to_string(id) << " : " << custName << endl;
            
        }
        sqlite3_close(db);
    }

    void PrintProjNames(string cust_name)
    {
        sqlite3* db;
        sqlite3_stmt* stmt;
        int standardCustID = 9999;
        string query;
        string standardCust = "No Customer";
        sqlite3_open("protocol_DB.db", &db);

        const unsigned char* projName;
        int id;
        query = "SELECT rowid, proj_name FROM project WHERE cust_name ='"+ cust_name +"' ;";

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
    string customerName;


    Database MyDB;
    MyDB.StartupDB();
    MyDB.PrintCustNames();
    cout << "42 : Create a new Customer" << endl;
    cout << "Enter ID: " << endl;
    cin >> customerID;
    // create new customer
    if (customerID == 42)
    {
        customerName = MyDB.CreateCust();
        // customerID = MyDB.MaxCustID();
        customerID = MyDB.QueryCustID(customerName);
    }
    else // query data for customerID
    {
        // call method query customer 
        customerName = MyDB.QueryCustName(customerID);
    }

    Customer cu(customerID, customerName, 1);

    // continue with PROJECT
    int projectID;
    string projName, projectPath;

    cout << "Choose a project from your customer: " + cu.name << endl;
    MyDB.PrintProjNames(cu.name);
    cout << "42 : Create a new project" << endl;
    cout << "Enter ID: " << endl;
    cin >> projectID;
    if (projectID == 42)
    {
        projName = MyDB.CreateProj(cu.name, protocolDir);
        projectID = MyDB.QueryProjID(projName);

    }
    else
    {
        projName = MyDB.QueryProjName(projectID);
    }

    Project pr(projectID, cu.name, projName);

    for (const auto& entry : std::filesystem::directory_iterator(templDir))
    {
        cout << entry.path() << endl;
    }
    string templateType;
    cin >> templateType;
    

    //sqlite3* db;
    //sqlite3_stmt* stmt;
    //int standardCustID = 9999;
    //string query;
    //string standardCust = "No Customer";
    //sqlite3_open("protocol_DB.db", &db);

    //query = "SELECT * FROM customer;";


    //sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

    //while (sqlite3_step(stmt) != SQLITE_DONE)
    //{
    //    const unsigned char* a;
    //    a = sqlite3_column_text(stmt, 1);
    //    cout << a << endl;
    //}

    return 0;
}

