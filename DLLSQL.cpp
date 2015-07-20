#include "DLLSQL.h"

/**
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
*/

// Create a new or open an existing database.
// If the database does not exist it will be
// created.  Note that you can specif the full
// path and filename which may, or may not, contain spaces.
     int DLL_EXPORT CreateDatabase(sqlite3** db,const char * dbname)
{
    char hang[2]; //Not more than 2 numbers for error codes
    if (dbname=='\0')
        {
        throw std::invalid_argument("stoi: invalid argument dbanme name");
        }
    int n = sqlite3_open_v2(dbname, db,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,NULL);
    if(n !=SQLITE_OK)
        {
            itoa(n,hang,10);
            throw std::runtime_error("sqlite3_open_v2 failure: Code "+ string(hang));
        }
    return 0;
}
// Add a new table to the database.  If the table
// name already exists then an error message will be
// displayed.
 int DLL_EXPORT CreateTable(sqlite3* db,const char * tbname)
{
    char* db_err = 0;
    if (tbname=='\0')
        {
            throw std::invalid_argument( "Invalid argument table name");
        }
    char* zSQL = sqlite3_mprintf("CREATE TABLE %q (id integer primary key asc,name char(10), ref TEXT DEFAULT 'New', parms TEXT DEFAULT '', libs TEXT DEFAULT '', ret TEXT DEFAULT 'void')",tbname);
    int n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    sqlite3_free(zSQL);
    if( n != SQLITE_OK )
        throw std::runtime_error("Sqlite3 failure "+ string(db_err));
    return 0;
}

/**
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
        //dsperr(&db_err);
        if( n != SQLITE_OK )
        {
            cout << "Error inserting value " << i << "\n";
            break;
        }
    }
    return 1;
}

*/
// Add one item to a table
// The column must be specify
//
 int DLL_EXPORT add_item(sqlite3* db, const char* tbname,const char* col,const char* item)
{
    char* db_err = 0;
    if (tbname=='\0'||col=='\0'||item=='\0')
        throw std::invalid_argument( "stoi: invalid argument table name");
    char *zSQL = sqlite3_mprintf("INSERT INTO %q (%Q) VALUES(%Q)", tbname,col,item);
    int n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    sqlite3_free(zSQL);
    if( n != SQLITE_OK )
        {
        throw std::runtime_error("sqlite3_exec failure:"+string(db_err));
        }
    return 0;
}

// Add one text to a table using UPDATE query
// The column must be specify and the id
//
int DLL_EXPORT add_text(sqlite3* db,const char* tbname,const char* col,const char* id,const char* item)
{
    char* db_err = 0;
    if (tbname=='\0'||col=='\0'||item=='\0')
        throw std::invalid_argument( "stoi: invalid argument table name");

    char *zSQL = sqlite3_mprintf("UPDATE %q SET %Q=(%Q) WHERE id=%q", tbname,col,item,id);
    int n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    sqlite3_free(zSQL);
    if( n != SQLITE_OK )
        {
        throw std::runtime_error("sqlite3_exec failure: "+string(db_err));
        }
    return 0;
}
// Delete one item to a table
// The column must be specify
//
 int DLL_EXPORT del_item(sqlite3* db, const char* tbname,const char* item)
{
    char* db_err = 0;
    if (tbname=='\0'||item=='\0')
        throw std::invalid_argument( "stoi: invalid argument table name");

    char *zSQL = sqlite3_mprintf("delete from %q where id = %q", tbname,item);
        //sprintf(buf,"delete from %s where id = %s;", tbname.c_str(),item.c_str());
    int n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    //dsperr(&db_err);
    sqlite3_free(zSQL);
    if( n != SQLITE_OK )
        {
        throw std::runtime_error("sqlite3_exec failure: "+string(db_err));
        }

    return 0;
}

/**
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

*/
// Query the database for one column data in the table and
// display it in the callback function.
 int DLL_EXPORT row(sqlite3* db,const char* tbname,const char* col,const char* id,int (*c_callback)(void*,int,char**,char**),void *answer)
{   int result;
    string* cont = static_cast<string*>(answer);
    char* db_err = 0;
    string select;
    if (tbname=='\0'|| id=='\0'|| col=='\0')
        {
            throw std::invalid_argument( "stoi: invalid argument table name or column");

        }
        char *zSQL = sqlite3_mprintf("select %q from %q where id = %q",col,tbname,id);
        result =sqlite3_exec(db, zSQL,c_callback,cont , &db_err);
        sqlite3_free(zSQL);
        if(result != SQLITE_OK)
            throw std::runtime_error("sqlite3_open_v2 failure: "+string(db_err));

        return 0;





}
// count the number of row in the database
 int DLL_EXPORT n_row(sqlite3* db,const char* tbname,int (*c_callback)(void*,int,char**,char**),void *answer)
{   int result;
    string* cont = static_cast<string*>(answer);
    char* db_err = 0;
    if (tbname=='\0')
         throw std::invalid_argument( "stoi: invalid argument table name");
    char *zSQL = sqlite3_mprintf("SELECT COALESCE(MAX(id)+1, 0) FROM %q",tbname);
    result =sqlite3_exec(db, zSQL,c_callback,cont , &db_err);
    sqlite3_free(zSQL);
    if(result != SQLITE_OK)
        throw std::runtime_error("sqlite3_open_v2 failure: "+string(db_err));
    return 0;
}
// count the number of row in the database
 int DLL_EXPORT id_row(sqlite3* db,const char* tbname,const char* col,const char* sel,int (*c_callback)(void*,int,char**,char**),void *answer)
{   int result;
    string* cont = static_cast<string*>(answer);
    char* db_err = 0;
    if (tbname=='\0')
         throw std::invalid_argument( "stoi: invalid argument table name");
    char *zSQL = sqlite3_mprintf("select id from %q where %q = %Q", tbname,col,sel);
    result =sqlite3_exec(db, zSQL,c_callback,cont , &db_err);
    sqlite3_free(zSQL);
    if(result != SQLITE_OK)
        throw std::runtime_error("sqlite3_open_v2 failure: "+string(db_err));
    return 0;
}

/**
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

    *result= string((char *)sqlite3_column_text(stmt, 0))+ " ";
    *result=*result + string((char*)sqlite3_column_text(stmt, 1)) +" ";
     *result=*result + string((char*)sqlite3_column_int(stmt, 2));

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
*/


/**
// This function just display an error message that
  int DLL_EXPORT ejemplo(char db_err)
{
    return 1;
}
*/

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
