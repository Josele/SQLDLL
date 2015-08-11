#include "DLLSQL.h"


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
    char* zSQL = sqlite3_mprintf("CREATE TABLE %q (id integer primary key asc,name char(10), ref TEXT DEFAULT '', parms TEXT DEFAULT 'result', ret TEXT DEFAULT 'void')",tbname);
    int n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    if( n != SQLITE_OK )
        throw std::runtime_error("Sqlite3 failure "+ string(db_err));
    zSQL = sqlite3_mprintf("CREATE TABLE params (id integer,name char(15) DEFAULT '',type char(10) DEFAULT '',array int(1) DEFAULT '0', value char(30) DEFAULT '', FOREIGN KEY (id)  REFERENCES datos(id) ON DELETE CASCADE)");
    n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    if( n != SQLITE_OK )
        throw std::runtime_error("Sqlite3 failure "+ string(db_err));
    zSQL = sqlite3_mprintf("CREATE TABLE config (id integer primary key,name char(30) DEFAULT '',libs TEXT DEFAULT '-1',CC TEXT DEFAULT 'none')");
    n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    if( n != SQLITE_OK )
        throw std::runtime_error("Sqlite3 failure "+ string(db_err));
    zSQL = sqlite3_mprintf("INSERT INTO config (id) VALUES(1)");
    n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    sqlite3_free(zSQL);
    if( n != SQLITE_OK )
        throw std::runtime_error("Sqlite3 failure "+ string(db_err));

    return 0;



}

// Add params item to a table
// The column must be specify
//
 int DLL_EXPORT add_params(sqlite3* db, const char* tbname,const char* col,const char* id,const char* item,const char* item2,const char* item3,const char* item4)
{
    char* db_err = 0;
    if (tbname=='\0'||col=='\0'||item=='\0'||item2=='\0'||item3=='\0'||item4=='\0')
        throw std::invalid_argument( "stoi: invalid argument table name");
    char *zSQL = sqlite3_mprintf("INSERT INTO %q (%s) VALUES(%Q,%Q,%Q,%Q,%Q)", tbname,col,id,item,item2,item3,item4);
    int n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    sqlite3_free(zSQL);
    if( n != SQLITE_OK )
        {
        throw std::runtime_error("sqlite3_exec failure:"+string(db_err));
        }
    return 0;
}
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
int DLL_EXPORT add_text(sqlite3* db,const char* tbname,const char* col,const char* item,const char* idname,const char* id)
{
    char* db_err = 0;
    if (tbname=='\0'||col=='\0'||item=='\0'||idname=='\0')
        throw std::invalid_argument( "stoi: invalid argument table name");

    char *zSQL = sqlite3_mprintf("UPDATE %s SET %s=(%Q) WHERE %s='%q'", tbname,col,item,idname,id);
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
 int DLL_EXPORT del_item(sqlite3* db, const char* tbname,const char* item,const char* id)
{
    char* db_err = 0;
    if (tbname=='\0'||item=='\0'||id=='\0')
        throw std::invalid_argument( "stoi: invalid argument table name");

    char *zSQL = sqlite3_mprintf("PRAGMA foreign_keys = ON");
        //sprintf(buf,"delete from %s where id = %s;", tbname.c_str(),iid,tem.c_str());
    int n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    //dsperr(&db_err);
    if( n != SQLITE_OK )
        {
        throw std::runtime_error("sqlite3_exec failure: "+string(db_err));
        }
    zSQL = sqlite3_mprintf("delete from %q where %q = '%q'", tbname,id,item);
        //sprintf(buf,"delete from %s where id = %s;", tbname.c_str(),iid,tem.c_str());
     n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    //dsperr(&db_err);
    if( n != SQLITE_OK )
        {
        throw std::runtime_error("sqlite3_exec failure: "+string(db_err));
        }
        zSQL = sqlite3_mprintf("PRAGMA foreign_keys = OFF;");
        //sprintf(buf,"delete from %s where id = %s;", tbname.c_str(),iid,tem.c_str());
     n = sqlite3_exec(db, zSQL, NULL, 0, &db_err);
    //dsperr(&db_err);
    sqlite3_free(zSQL);
    if( n != SQLITE_OK )
        {
        throw std::runtime_error("sqlite3_exec failure: "+string(db_err));
        }

    return 0;
}


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
// Query the database for one column data in the table and
// display it in the callback function.
 int DLL_EXPORT max_parms(sqlite3* db,const char* tbname,const char* col,int (*c_callback)(void*,int,char**,char**),void *answer)
{   int result;
    string* cont = static_cast<string*>(answer);
    char* db_err = 0;
    string select;
    if (tbname=='\0'||  col=='\0')
        {
            throw std::invalid_argument( "stoi: invalid argument table name or column");

        }
        char *zSQL = sqlite3_mprintf("SELECT MAX(count) FROM( SELECT COUNT(*) AS count FROM %q GROUP BY %q ORDER BY count DESC) AS count;",tbname,col);
        result =sqlite3_exec(db, zSQL,c_callback,cont , &db_err);
        sqlite3_free(zSQL);
        if(result != SQLITE_OK)
            throw std::runtime_error("sqlite3_open_v2 failure: "+string(db_err));

        return 0;





}
// Query the database for one column data in the table and
// display it in the callback function.
 int DLL_EXPORT get_item(sqlite3* db,const char* tbname,const char* sel,const char* col,const char* id,const char* id2,int (*c_callback)(void*,int,char**,char**),void *answer)
{   int result;
    string* cont = static_cast<string*>(answer);
    char* db_err = 0;
    string select;
    if (tbname=='\0'|| id=='\0'|| col=='\0'||sel=='\0'||id2=='\0')
        {
            throw std::invalid_argument( "stoi: invalid argument table name or column");

        }
        char *zSQL = sqlite3_mprintf("select %q from %q where %q = '%q' and id= '%q'",sel,tbname,col,id,id2);
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
