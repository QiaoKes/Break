#include "Mysql.h"

int ConnectMysql(char* server,char* user,char* password,char* database)
{
	conn = NULL;
	conn = mysql_init(NULL);
	mysql_set_character_set(conn,"gb2312");
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		return FALSE;
	}
	return TRUE;
}

int SelectMysql(char* szSql,int nColumn,Queue* pQueue)
{
	results = NULL;
	if(mysql_query(conn,szSql))return FALSE;
	results = mysql_store_result(conn);
	if(NULL == results)return FALSE;
	while((record = mysql_fetch_row(results)))
	{

		for(int i=0;i<nColumn;i++)
		{
			q_Push(pQueue,(void*)record[i]);
		}
	}
	return TRUE;
}


int UpdataMysql(char *szsql)
{
	if(!szsql)return FALSE;
	if(mysql_query(conn,szsql))return FALSE;

	return TRUE;
}

void DisConnect()
{
	mysql_close(conn);
}
