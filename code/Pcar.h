#ifndef _PCAR_H_
#define _PCAR_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include "jpeglib.h"

#include <termios.h> 
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <sqlite3.h>



typedef struct pro_set
{
	time_t systime;        //get 
	pthread_t Pth_time;     //use to showtime funtion
	pthread_t Pth_rfid;     //use to rfid
	struct tm *timenow;
	int num_park;
	int Parkingtime;
    int cost;
	//int Carstatus;   
	
	
}P_S,*P;

typedef struct car_info
{
	char name[30];            //carname
	char carnumber[20];       //the car number
	int intime;               //time that car come in
	int carid;                 //the carid
   	char phone[12];           // phone number
	int carstatus;             //car status
}C_I,*C;


int pro_init();   //init this system

int pro_end();  //do some work to end this system

int UI();  //parking system main funtion

void * showtime(); //The funtion of show the system time

void * listencar();  //use to listen the rfid 

int showjpeg(int x,int y,char *jpgpath);   //show the picture for background

int showcar(int x,int y,char *carpath);    //show the picture of car 

int PiccAnticoll(int fd);

unsigned char CalBCC(unsigned char *buf, int n);

void init_tty(int fd);

int PiccRequest(int fd);

int deletcar(int x,int y,char *carpath);

int open_database();  //create or open a database

int getid();   //get the rfid id

#endif