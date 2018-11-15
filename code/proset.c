#include "Pcar.h"
#include "font.h"

P_S p;
C_I C_info[10];

int pro_init()
{
	open_database();  //create or open database
	//.Carstatus=0;    //the car status express there was no car parking this place 
	
	
	p.Parkingtime=0;
	p.num_park=3;    //the number of parking
	p.cost=0;
	Init_Font();   //init the chinese 
	
	strcpy(C_info[0].name,"William");            //init the massage of car one
    strcpy(C_info[0].carnumber,"粤A8888");
	C_info[0].carid=0x3ea1c129;
	strcpy(C_info[0].phone,"12345678901");
	C_info[0].carstatus=0;
	
	strcpy(C_info[1].name,"San");                //init the massage of car two
    strcpy(C_info[1].carnumber,"京C8888");
	C_info[1].carid=0xfde482b6;
	strcpy(C_info[1].phone,"1895269825");
	C_info[1].carstatus=0;
	
	strcpy(C_info[2].name,"Mike");               //init the massage of car three
    strcpy(C_info[2].carnumber,"豫W8888");
	C_info[2].carid=0xa96e77f4;
	strcpy(C_info[2].phone,"29083579258");
	C_info[2].carstatus=0;
   
	
	return 0;
}


int showjpeg(int x,int y,char *jpgpath)
{
	
	printf("%s\n",jpgpath);
	
	int i,j;
	int lcdfd;
	int *lcdmem;
	//打开开发板液晶屏的驱动
	lcdfd=open("/dev/fb0",O_RDWR); //fb-->frame buffer
	if(lcdfd==-1)
	{
		perror("打开lcd失败！\n");
		return -1;
	}
	//通过内存映射得到液晶屏的首地址
	lcdmem=mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(lcdmem==NULL)
	{
		perror("映射lcd失败!\n");
		return -1;
	}
	//定义解压缩结构体变量和处理错误的结构体变量
	struct jpeg_decompress_struct mydem;
	struct jpeg_error_mgr myerr;
	mydem.err=jpeg_std_error(&myerr);
	jpeg_create_decompress(&mydem);
	FILE *jpgp;
	//打开你要显示的jpg
	jpgp=fopen(jpgpath,"r+"); 
	if(jpgp==NULL)
	{
		perror("打开jpg失败！\n");
		return -1;
	}
	//获取数据源
	jpeg_stdio_src(&mydem,jpgp);
	//读取jpg的头信息
	jpeg_read_header(&mydem,1);
	/* printf("图片的宽是：%d\n",mydem.image_width);
	printf("图片的高是：%d\n",mydem.image_height); */
	//开始解压缩
	jpeg_start_decompress(&mydem);
	//定义一个指针存放一行RGB数值
	char *rgbbuf=calloc(1,mydem.image_width*3);
	//定义一个指针存放转换得到的一行ARGB
	int *lcdbuf=calloc(1,mydem.image_width*4);
	//解压缩成功后得到的就是jpg图片的原始RGB数值--》读取并填充到lcd上
	for(i=0; i<mydem.image_height; i++) //比如：图片是800*480大小
	{
		//每次循环读取一行RGB
		jpeg_read_scanlines(&mydem,(JSAMPARRAY)&rgbbuf,1);
		//填充(写入)到开发板的液晶屏中
		//将一行RGB--》转化成ARGB
		for(j=0; j<mydem.image_width; j++)
		{
			lcdbuf[j]=0x00<<24|rgbbuf[3*j]<<16|rgbbuf[3*j+1]<<8|rgbbuf[3*j+2];
			           //00[0][1][2]
					   
					   
		}
		//在指定的位置(x,y)显示该图片
		memcpy(lcdmem+(y+i)*800+x,lcdbuf,mydem.image_width*4);
	}
	//收尾工作
	jpeg_finish_decompress(&mydem);
    jpeg_destroy_decompress(&mydem);
	close(lcdfd);
	fclose(jpgp);
	munmap(lcdmem,800*480*4);
	free(rgbbuf);
	free(lcdbuf);
	return 0;
}

int showcar(int x,int y,char *carpath)
{
	int i,j;
	int lcdfd;
	int *lcdmem;
	//打开开发板液晶屏的驱动
	lcdfd=open("/dev/fb0",O_RDWR); //fb-->frame buffer
	if(lcdfd==-1)
	{
		perror("打开lcd失败！\n");
		return -1;
	}
	//通过内存映射得到液晶屏的首地址
	lcdmem=mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(lcdmem==NULL)
	{
		perror("映射lcd失败!\n");
		return -1;
	}
	//定义解压缩结构体变量和处理错误的结构体变量
	struct jpeg_decompress_struct mydem;
	struct jpeg_error_mgr myerr;
	mydem.err=jpeg_std_error(&myerr);
	jpeg_create_decompress(&mydem);
	FILE *jpgp;
	//打开你要显示的jpg
	jpgp=fopen(carpath,"r+"); 
	if(jpgp==NULL)
	{
		perror("打开jpg失败！\n");
		return -1;
	}
	//获取数据源
	jpeg_stdio_src(&mydem,jpgp);
	//读取jpg的头信息
	jpeg_read_header(&mydem,1);
	/* printf("图片的宽是：%d\n",mydem.image_width);
	printf("图片的高是：%d\n",mydem.image_height); */
	//开始解压缩
	jpeg_start_decompress(&mydem);
	//定义一个指针存放一行RGB数值
	char *rgbbuf=calloc(1,mydem.image_width*3);
	//定义一个指针存放转换得到的一行ARGB
	int *lcdbuf=calloc(1,mydem.image_height*mydem.image_width*4);
	//解压缩成功后得到的就是jpg图片的原始RGB数值--》读取并填充到lcd上
	for(i=0; i<mydem.image_height; i++) //比如：图片是800*480大小
	{
		//每次循环读取一行RGB
		jpeg_read_scanlines(&mydem,(JSAMPARRAY)&rgbbuf,1);
		//填充(写入)到开发板的液晶屏中
		//将一行RGB--》转化成ARGB
		for(j=0; j<mydem.image_width; j++)
		{
			*(lcdbuf+i*mydem.image_width+j)=0x00<<24|rgbbuf[3*j]<<16|rgbbuf[3*j+1]<<8|rgbbuf[3*j+2];
			           //00[0][1][2]
			if(*(lcdbuf+i*mydem.image_width+j)!=0x00ffffff)		   
			{ 
		        *(lcdmem+(y+i)*800+x+j)=*(lcdbuf+i*mydem.image_width+j);
			}		   
					   
		}
		//在指定的位置(x,y)显示该图片
		//memcpy(lcdmem+(y+i)*800+x,lcdbuf,mydem.image_width*4);
	}
	//收尾工作
	jpeg_finish_decompress(&mydem);
    jpeg_destroy_decompress(&mydem);
	close(lcdfd);
	fclose(jpgp);
	munmap(lcdmem,800*480*4);
	free(rgbbuf);
	free(lcdbuf);
	return 0;
}



int deletcar(int x,int y,char *carpath)
     {
int i,j;
	int lcdfd;
	int *lcdmem;
	//打开开发板液晶屏的驱动
	lcdfd=open("/dev/fb0",O_RDWR); //fb-->frame buffer
	if(lcdfd==-1)
	{
		perror("打开lcd失败！\n");
		return -1;
	}
	//通过内存映射得到液晶屏的首地址
	lcdmem=mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(lcdmem==NULL)
	{
		perror("映射lcd失败!\n");
		return -1;
	}
	//定义解压缩结构体变量和处理错误的结构体变量
	struct jpeg_decompress_struct mydem;
	struct jpeg_error_mgr myerr;
	mydem.err=jpeg_std_error(&myerr);
	jpeg_create_decompress(&mydem);
	FILE *jpgp;
	//打开你要显示的jpg
	jpgp=fopen(carpath,"r+"); 
	if(jpgp==NULL)
	{
		perror("打开jpg失败！\n");
		return -1;
	}
	//获取数据源
	jpeg_stdio_src(&mydem,jpgp);
	//读取jpg的头信息
	jpeg_read_header(&mydem,1);
	/* printf("图片的宽是：%d\n",mydem.image_width);
	printf("图片的高是：%d\n",mydem.image_height); */
	//开始解压缩
	jpeg_start_decompress(&mydem);
	//定义一个指针存放一行RGB数值
	char *rgbbuf=calloc(1,mydem.image_width*3);
	//定义一个指针存放转换得到的一行ARGB
	int *lcdbuf=calloc(1,mydem.image_height*mydem.image_width*4);
	//解压缩成功后得到的就是jpg图片的原始RGB数值--》读取并填充到lcd上
	for(i=0; i<mydem.image_height; i++) //比如：图片是800*480大小
	{
		//每次循环读取一行RGB
		jpeg_read_scanlines(&mydem,(JSAMPARRAY)&rgbbuf,1);
		//填充(写入)到开发板的液晶屏中
		//将一行RGB--》转化成ARGB
		for(j=0; j<mydem.image_width; j++)
		{
			*(lcdbuf+i*mydem.image_width+j)=0x00<<24|rgbbuf[3*j]<<16|rgbbuf[3*j+1]<<8|rgbbuf[3*j+2];
			           //00[0][1][2]
			if(*(lcdbuf+i*mydem.image_width+j)!=0x00ffffff)		   
			{ 
		        *(lcdmem+(y+i)*800+x+j)=0x00ffffff;
			}		   
					   
		}
		//在指定的位置(x,y)显示该图片
		//memcpy(lcdmem+(y+i)*800+x,lcdbuf,mydem.image_width*4);
	}
	//收尾工作
	jpeg_finish_decompress(&mydem);
    jpeg_destroy_decompress(&mydem);
	close(lcdfd);
	fclose(jpgp);
	munmap(lcdmem,800*480*4);
	free(rgbbuf);
	free(lcdbuf);
	return 0;
		 
	 }

int pro_end()
{
	UnInit_Font();
	return 0;
}

