#ifndef __DLLSQL_H__
#define __DLLSQL_H__

#include <iostream>
#include <windows.h>
#include "sqlite3.h"
#include <stdio.h>
#include <string>
#include <stdexcept>

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





       int  DLL_EXPORT CreateDatabase(sqlite3** db,const char* dbname); //being used
      // int DLL_EXPORT ejemplo(char db_err);
       int  DLL_EXPORT CreateTable(sqlite3* db,const char* tbname); //being used
       int DLL_EXPORT add_item(sqlite3* db,const char* tbname,const char* col,const char* item); //being used
       int DLL_EXPORT del_item(sqlite3* db,const char* tbname,const char* item,const char* id); //being used
       int DLL_EXPORT del_item_v2(sqlite3* db, const char* tbname,const char* item,const char* id,const char* item2,const char* id2); //being used
      // int DLL_EXPORT fill(sqlite3* db, string tbname);
      // void DLL_EXPORT display(sqlite3* db, string tbname);
      // void DLL_EXPORT jokersql(sqlite3* db, string sql);
       int DLL_EXPORT add_params(sqlite3* db, const char* tbname,const char* col,const char* id,const char* item,const char* item2,const char* item3,const char* item4);//being used
       int DLL_EXPORT row(sqlite3* db,const char* tbname,const char* col,const char* id,int (*callback)(void*,int,char**,char**),void *answer); //being used
       int DLL_EXPORT id_row(sqlite3* db, const char* tbname,const char* col,const char* sel,int (*c_callback)(void*,int,char**,char**),void *answer); // being used
       int DLL_EXPORT n_row(sqlite3* db,const char* tbname,int (*callback)(void*,int,char**,char**),void *answer); //being used
       int DLL_EXPORT add_text(sqlite3* db,const char* tbname,const char* col,const char* item,const char* idname,const char* id);//being used
       int DLL_EXPORT add_text_v2(sqlite3* db,const char* tbname,const char* col,const char* item,const char* idname,const char* id,const char* idname2,const char* id2);//being used
       // string DLL_EXPORT readFromDB(sqlite3* db, int id);
        int DLL_EXPORT get_item(sqlite3* db,const char* tbname,const char* sel,const char* col,const char* id,const char* id2,int (*c_callback)(void*,int,char**,char**),void *answer);//being used
        int DLL_EXPORT max_parms(sqlite3* db,const char* tbname,const char* col,int (*c_callback)(void*,int,char**,char**),void *answer);//being used

#ifdef __cplusplus
}
#endif
#endif // _DLLSQL_H__
