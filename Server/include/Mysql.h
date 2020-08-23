#ifndef _MYSQL_H
#define _MYSQL_H
#include "packdef.h"
#include <mysql/mysql.h>

MYSQL *conn;
MYSQL_RES *results;
MYSQL_ROW record;

int ConnectMysql(char*,char*,char*,char*);
int SelectMysql(char*,int,Queue*);
int UpdataMysql(char*);
void DisConnect();


#endif
