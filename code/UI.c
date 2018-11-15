#include "Pcar.h"
#include "font.h"

extern P_S p;         //The system setting struct

extern C_I C_info[3];

extern sqlite3 *mydata;

void * showtime()  /*this funtion use to show the system time int arm */
        {
			char str[80]={0};
			//time_t systime;
						
	         
			
			while(1)
			{
		       time(&p.systime);
               p.timenow=localtime(&p.systime);  /*get the system time*/
			    
			    /*shoe the time*/
			    Clean_Area(0,0,500,50,0x00ffff00);
			    sprintf(str,"Time now:  %d:%d:%d",p.timenow->tm_hour,p.timenow->tm_min,p.timenow->tm_sec);
			    Display_characterX(0,0,str,0x00000000,3); 
				
				/*show the  empty space */
				
				Clean_Area(100,50,500,50,0x00ffff00);                
                sprintf(str,"Empty space:   %d",p.num_park); 
				Display_characterX(100,50,str,0x00000000,3); 

				/*show the parking time*/
				
				Clean_Area(200,100,510,50,0x00ffff00);
                sprintf(str,"Parking time: %d hour",p.Parkingtime); 
				Display_characterX(200,100,str,0x00000000,3);

				/*show the cost*/
				
				Clean_Area(300,150,500,50,0x00ffff00);
                sprintf(str,"Cost:   %d   dollor",p.cost); 
				Display_characterX(300,150,str,0x00000000,3);
				
				sleep(1);
			}
		} 

void * listencar()
       {
		   while(1)
		   {
               int i; 
			   int ret;
			   int id=getid();
			   char str[100]={};
			   
			   if(id!=0)
			   {
				    printf("***********%x\n",id);
			   }
			  
			    if(id!=0)             //判断有没有读到卡号
			   { 
                  
		           for(i=0;i<3;i++)
				   {
					   if(C_info[i].carid==id)        
					   {
						  if(C_info[i].carstatus==1)  //判断出车
						  {
							  C_info[i].carstatus=0;      //停车暂停设为0
							  p.num_park++;
                             p.Parkingtime=(int)p.systime-C_info[i].intime;  //计算停车时间
							 p.cost=p.Parkingtime*5;      //计算停车费用
							 
							 if(i==0)                        //让小车图片消失
							 {
								 deletcar(465,315,"jpeg/car3.jpg");
							 }else if(i==1)
							 {
								deletcar(210,230,"jpeg/car2.jpg");
							 }else if(i==2)
							 {
                                deletcar(20,180,"jpeg/car1.jpg");
							 }
							 
							   sprintf(str,"delete from car_info where name=%d;",C_info[i].carid);
							   ret=sqlite3_exec(mydata,str,NULL,NULL,NULL);
							   if(ret!=SQLITE_OK)
							  {
								printf("删除数据失败!\n");    //删除数据库
								   break;
							   }

						  }
						  else if(C_info[i].carstatus==0)	//当有车进来						  
						  { 
					      p.Parkingtime=0;          //停车时间设为0
				          p.cost=0;                 //停车费用设为0

						  C_info[i].carstatus=1;                 //停车状态设为1
                          C_info[i].intime=(int)p.systime;     //记录停车时间
						  p.num_park--;                         //剩余位置-1
								/*show car*/
                         
						     if(i==0)                             //显示小车图片
							 {
								 showcar(465,315,"jpeg/car3.jpg");
							 }else if(i==1)
							 {
								showcar(210,230,"jpeg/car2.jpg");
							 }else if(i==2)
							 {
                                showcar(20,180,"jpeg/car1.jpg");
							 }
							 
							 
                            sprintf(str,"insert into car_info values(\"%d\",\"%s\",\"%s\",\"%s\");",C_info[i].carid,C_info[i].name,C_info[i].phone,C_info[i].carnumber);
							 
                   			 ret=sqlite3_exec(mydata,str,NULL,NULL,NULL);
	                           if(ret!=SQLITE_OK)
								{                                //将车主信息存入数据库
									printf("插入数据失败!\n");
									break;
								}
		 
						  
						  }
						  
					   }
				   }
			   } 
			   usleep(800000);
		   }
	   }		

int UI()
{
	

	
	 showjpeg(0,0,"jpeg/Parking.jpg");     //显示背景图

	
	
	pthread_create(&p.Pth_time,NULL,showtime,NULL);   //显示时间线程
	
	pthread_create(&p.Pth_rfid,NULL,listencar,NULL);   //监听刷卡线程
	
	
	pthread_join(p.Pth_time,NULL);           //线程阻塞
    pthread_join(p.Pth_rfid,NULL);
	
	return 0;
}