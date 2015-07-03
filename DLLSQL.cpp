#include "DLLSQL.h"
#include <iostream>

using std::cout;
using std::cin;






// This function just display an error message that
  void dsperr(char**db_err)
{
    if( *db_err )
    {
        cout << *db_err << "\n";
        sqlite3_free(*db_err); // release (free) memory
        *db_err = 0; // reinitialize the caller's pointer
    }
}
// Parameters are:
//      argc -- the number of rows in the resultset
//      argv[] -- the data for each row
//      azColName -- the name of each column
 int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    cout << azColName[i]
          << " = ";
    if( argv[i] )
        cout << argv[i];
    else
        cout << "NULL";
     cout << "\n";
  }
  return 0;
}


   // Create a new or open an existing database.
// If the database does not exist it will be
// created.  Note that you can specif the full
// path and filename which may, or may not, contain spaces.
     int DLL_EXPORT CreateDatabase(sqlite3** db,string dbname)
{
    char hang[2];
    if (dbname==std::string())
        {
        throw std::invalid_argument("stoi: invalid argument dbanme name");
        }


    int n = sqlite3_open_v2(dbname.c_str(), db,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,NULL);

   if(n !=SQLITE_OK)
   {
       itoa(n,hang,10);
       throw string("sqlite3_open_v2 failure: Code "+ string(hang));

   }
return 0;
}




// Add a new table to the database.  If the table
// name already exists then an error message will be
// displayed.
 int DLL_EXPORT CreateTable(sqlite3* db, string tbname,char * db_err)
{
    if (tbname==std::string())
        {
         throw std::invalid_argument( "stoi: invalid argument table name");

        }
    string statement;

    statement = "CREATE TABLE " + tbname + " (id integer primary key asc,name char(10), ref char(30));";

    int n = sqlite3_exec(db, statement.c_str(), NULL, 0, &db_err);
    if( n != SQLITE_OK )
    throw string("sqlite3 failure"+ string(db_err));


    return 0;

}





// Just dump some data into the table.  You don't
// have a choice of the kind or quantity of data to be entered.
//
 int DLL_EXPORT fill(sqlite3* db, string tbname)
{
    char* db_err = 0;
        if (tbname==std::string())
            //tbname.length() == 0
        {
         cout << "Null tbname\n";
            return 0;

        }

    for(int i = 1; i < 100; i++)
    {
        char buf[80];
        sprintf(buf,"insert into %s values(%d);", tbname.c_str(), i);
        int n = sqlite3_exec(db, buf, NULL, 0, &db_err);
        dsperr(&db_err);
        if( n != SQLITE_OK )
        {
            cout << "Error inserting value " << i << "\n";
            break;
        }
    }
    return 1;
}


// Add one item to a table
// The column must be specify
//
 int DLL_EXPORT add_item(sqlite3* db, string tbname,string col,string item)
{
    char* db_err = 0;
    if (tbname==std::string()||col==std::string()||item==std::string())
        throw std::invalid_argument( "stoi: invalid argument table name");


        char buf[200];
        sprintf(buf,"insert into %s (%s) values ('%s');", tbname.c_str(), col.c_str(),item.c_str());
        int n = sqlite3_exec(db, buf, NULL, 0, &db_err);
        dsperr(&db_err);
        if( n != SQLITE_OK )
        {
        throw "sqlite3_exec failure: Code ";
        }

    return 0;
}


// Query the database for all the data in the table and
// display it in the callback function.
 void DLL_EXPORT display(sqlite3* db, string tbname)
{
    char* db_err = 0;
    string select;
    if (tbname==std::string())
            //tbname.length() == 0
        {
         cout << "Null tbname\n";
            return;

        }
    if( tbname.length() > 0)
    {
        select = "select * from " + tbname + ";";
        sqlite3_exec(db, select.c_str(), callback, 0, &db_err);
        dsperr(&db_err);
    }
}


// Query the database for one column data in the table and
// display it in the callback function.
 int DLL_EXPORT row(sqlite3* db, string tbname,string col,string id,int (*c_callback)(void*,int,char**,char**),void *answer, char **argv)
{   int result;
    string* hola = static_cast<string*>(answer);
   char* db_err = 0;
    string select;
    if (tbname==std::string()|| id==std::string()|| col==std::string())
            //tbname.length() == 0
        {
            throw std::invalid_argument( "stoi: invalid argument table name or column");

        }
    if( tbname.length() > 0)
    {
        select = "select " + col + " from " + tbname + " where id = "+ id +";";
        result =sqlite3_exec(db, select.c_str(),c_callback,hola , &db_err);
        dsperr(&db_err);
        return result;
    }
return -1;


}// Query the database for one column data in the table and
// display it in the callback function.
 int DLL_EXPORT n_row(sqlite3* db, string tbname,int (*c_callback)(void*,int,char**,char**),void *answer)
{   int result;
    string* cont = static_cast<string*>(answer);
    char* db_err = 0;
    string sql;
    if (tbname==std::string())
         throw std::invalid_argument( "stoi: invalid argument table name");
    sql ="SELECT COALESCE(MAX(id)+1, 0) FROM " +tbname;
    result =sqlite3_exec(db, sql.c_str(),c_callback,cont , &db_err);
    dsperr(&db_err);
    if(result != SQLITE_OK)
        throw "sqlite3_open_v2 failure: Code ";

    return 0;
}


string DLL_EXPORT readFromDB(sqlite3* db, int id)
{   string result;
    sqlite3_stmt *stmt;
     const char *sql="SELECT name FROM datos WHERE id = ?";
    int rc = sqlite3_prepare_v2(db, sql,-1 , &stmt, NULL);
    if (rc != SQLITE_OK)
        return string(sqlite3_errmsg(db));

    rc = sqlite3_bind_int(stmt, 1, id);    // Using parameters ("?") is not
    if (rc != SQLITE_OK) {                 // really necessary, but recommended
        string errmsg(sqlite3_errmsg(db)); // (especially for strings) to avoid
        sqlite3_finalize(stmt);            // formatting problems and SQL
        return errmsg;                      // injection attacks.
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        string errmsg(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return errmsg;
    }
    if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return string("customer not found");
    }

    result= string((char *)sqlite3_column_text(stmt, 0))+ " ";
   // *result=*result + string((char*)sqlite3_column_text(stmt, 1)) +" ";
    //*result=*result + string((char*)sqlite3_column_int(stmt, 2));

    sqlite3_finalize(stmt);
   return result;
}

// Query an SQL sentence, only for making proofs
 void DLL_EXPORT jokersql(sqlite3* db, string sql)
{
    char* db_err = 0;
    string select;
    if (sql==std::string())
            //tbname.length() == 0
        {
         cout << "Null sql\n";
            return;

        }
    if( sql.length() > 0)
    {
        select = sql ;
        sqlite3_exec(db, select.c_str(), callback, 0, &db_err);
        dsperr(&db_err);
    }
}




// This function just display an error message that
  int DLL_EXPORT ejemplo(char db_err)
{
    return 1;
}


/**
// a sample exported function
void DLL_EXPORT SomeFunction(const LPCSTR sometext)
{
    MessageBoxA(0, sometext, "DLL Message", MB_OK | MB_ICONINFORMATION);
}
*/
extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
