#include "Pcar.h"
#include <sqlite3.h>

sqlite3 *mydata;

int open_database()    //create or open a database
{
	char strsql[80]={};
	int ret=sqlite3_open("./cardata.db",&mydata);
	if(ret!=SQLITE_OK)
	{
		printf("create a database failed\n");
		return -1;
	}
	
	//往数据库文件中新建表格
	ret=sqlite3_exec(mydata,"create table if not exists car_info (id int,name text,phone text,carnumber text);",NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("新建表格失败!\n");
		return -1;
	}

	//sprintf(strsql,"insert into stu values(\"William\",\"12345678901\",\"粤A8888\",0,\"0\",\"0\");",name,age);
	
	return 0;
}