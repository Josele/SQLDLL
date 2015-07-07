#ifndef __DLLSQL_H__
#define __DLLSQL_H__

#include <windows.h>
#include "sqlite3.h"
#include <stdio.h>
#include <string>
#include <stdexcept>      // std::invalid_argument

/*  To use this exported function of dll, include this header
 *  in your project.
 */


#define DLL_EXPORT __declspec(dllexport)
using std::string;
using std::exception;

#ifdef __cplusplus
extern "C"
{
#endif


//void DLL_EXPORT SomeFunction(const LPCSTR sometext);





       int DLL_EXPORT CreateDatabase(sqlite3** db,string dbname); //being used
       int DLL_EXPORT ejemplo(char db_err);
       int DLL_EXPORT CreateTable(sqlite3* db, string tbname,char * db_err); //being used
       int DLL_EXPORT add_item(sqlite3* db, string tbname,string col,string item); //being used
       int DLL_EXPORT del_item(sqlite3* db, string tbname,string item); //being used
       int DLL_EXPORT fill(sqlite3* db, string tbname);
       void DLL_EXPORT display(sqlite3* db, string tbname);
       void DLL_EXPORT jokersql(sqlite3* db, string sql);
       int DLL_EXPORT row(sqlite3* db, string tbname,string col,string id,int (*callback)(void*,int,char**,char**),void *answer);
       int DLL_EXPORT id_row(sqlite3* db, string tbname,string col,string sel,int (*c_callback)(void*,int,char**,char**),void *answer); // being used
       int DLL_EXPORT n_row(sqlite3* db, string tbname,int (*callback)(void*,int,char**,char**),void *answer); //being used
       int DLL_EXPORT add_text(sqlite3* db, string tbname,string col,string id,string item);//being used
        string DLL_EXPORT readFromDB(sqlite3* db, int id);


#ifdef __cplusplus
}
#endif
#endif // _DLLSQL_H__
