


//#include "stm32f10x.h"
#include "LCD_Driver.h"
#include "LCD_Config.h"
//#include "delay.h"
 #include<inttypes.h>
#include <SPI.h>
 
//Һ��IO��ʼ������
void LCD_GPIO_Init(void)
{
  pinMode(TFT_MOSI, OUTPUT);
  pinMode(TFT_CLK, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  
  SPI.begin (TFT_CLK, -1, TFT_MOSI, -1);
  SPI.beginTransaction(SPISettings(70000000, MSBFIRST, SPI_MODE0));

}
void delay_ms(unsigned int x)
{
  delay(x);
}

//��SPI���ߴ���һ��8λ�
/*���
void  SPI_WriteData(u8 Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
	    LCD_SDA_SET; //�������
      else 
      LCD_SDA_CLR;
	   
      LCD_SCL_CLR;       
      LCD_SCL_SET;
      Data<<=1; 
	}
}
*/

void SPI_WriteData(u8 Data)
{
//  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(Data);
//  digitalWrite(slaveSelectPin, HIGH);
}
//��Һ����дһ��8λָ��
void Lcd_WriteIndex(u8 Index)
{
   //SPI д����ʱ��ʼ
   LCD_RS_CLR;
	 SPI_WriteData(Index);
}

//��Һ����дһ��8λ����
void Lcd_WriteData(u8 Data)
{
   LCD_RS_SET;
   SPI_WriteData(Data); 
}
//��Һ����дһ��16λ����
void LCD_WriteData_16Bit(u16 Data)
{

   LCD_RS_SET;
	 SPI_WriteData(Data>>8); 	//д���8λ����
	 SPI_WriteData(Data); 			//д���8λ����

}

void Lcd_WriteReg(u8 Index,u8 Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData(Data);
}

void Lcd_Reset(void)
{
	LCD_RST_CLR;
	delay_ms(100);
	LCD_RST_SET;
	delay_ms(50);
}

//LCD Init For 1.44Inch LCD Panel with ST7735R.
void Lcd_Init(void)
{	
	LCD_GPIO_Init();
	Lcd_Reset(); //Reset before LCD Init.

	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
  delay_ms (120);
  Lcd_WriteIndex(0x21); 
Lcd_WriteIndex(0x21); 

Lcd_WriteIndex(0xB1); 
Lcd_WriteData(0x05);
Lcd_WriteData(0x3A);
Lcd_WriteData(0x3A);

Lcd_WriteIndex(0xB2);
Lcd_WriteData(0x05);
Lcd_WriteData(0x3A);
Lcd_WriteData(0x3A);

Lcd_WriteIndex(0xB3); 
Lcd_WriteData(0x05);  
Lcd_WriteData(0x3A);
Lcd_WriteData(0x3A);
Lcd_WriteData(0x05);
Lcd_WriteData(0x3A);
Lcd_WriteData(0x3A);

Lcd_WriteIndex(0xB4);
Lcd_WriteData(0x03);

Lcd_WriteIndex(0xC0);
Lcd_WriteData(0x62);
Lcd_WriteData(0x02);
Lcd_WriteData(0x04);

Lcd_WriteIndex(0xC1);
Lcd_WriteData(0xC0);

Lcd_WriteIndex(0xC2);
Lcd_WriteData(0x0D);
Lcd_WriteData(0x00);

Lcd_WriteIndex(0xC3);
Lcd_WriteData(0x8D);
Lcd_WriteData(0x6A);   

Lcd_WriteIndex(0xC4);
Lcd_WriteData(0x8D); 
Lcd_WriteData(0xEE); 

Lcd_WriteIndex(0xC5);  /*VCOM*/
Lcd_WriteData(0x0E);    

Lcd_WriteIndex(0xE0);
Lcd_WriteData(0x10);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x02);
Lcd_WriteData(0x03);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x07);
Lcd_WriteData(0x02);
Lcd_WriteData(0x07);
Lcd_WriteData(0x0A);
Lcd_WriteData(0x12);
Lcd_WriteData(0x27);
Lcd_WriteData(0x37);
Lcd_WriteData(0x00);
Lcd_WriteData(0x0D);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x10);

Lcd_WriteIndex(0xE1);
Lcd_WriteData(0x10);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x03);
Lcd_WriteData(0x03);
Lcd_WriteData(0x0F);
Lcd_WriteData(0x06);
Lcd_WriteData(0x02);
Lcd_WriteData(0x08);
Lcd_WriteData(0x0A);
Lcd_WriteData(0x13);
Lcd_WriteData(0x26);
Lcd_WriteData(0x36);
Lcd_WriteData(0x00);
Lcd_WriteData(0x0D);
Lcd_WriteData(0x0E);
Lcd_WriteData(0x10);

Lcd_WriteIndex(0x3A); 
Lcd_WriteData(0x05);

Lcd_WriteIndex(0x36);
Lcd_WriteData(0xC8);

Lcd_WriteIndex(0x29); 

	 
}


/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{		
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+0x1A);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+0x1A);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start+1);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end+1);	
	Lcd_WriteIndex(0x2c);

}

/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd_SetXY(u16 x,u16 y)
{
  	Lcd_SetRegion(x,y,x,y);
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	Lcd_SetRegion(x,y,x+1,y+1);
	LCD_WriteData_16Bit(Data);

}    

/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
unsigned int Lcd_ReadPoint(u16 x,u16 y)
{
  unsigned int Data;
  Lcd_SetXY(x,y);

  //Lcd_ReadData();//���������ֽ�
  //Data=Lcd_ReadData();
  Lcd_WriteData(Data);
  return Data;
}
/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd_Clear(u16 Color)               
{	
   unsigned int i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   Lcd_WriteIndex(0x2C);
   
   LCD_RS_SET;
   
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  //	LCD_WriteData_16Bit(Color);
     SPI.write16(Color);
    }   
    delay(100);
}

void Lcd_ClearRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  
  for(int i=x;i< w;i++)
    for(int m=y;m < h;m++)
    {	
	  //	LCD_WriteData_16Bit(Color);
     Gui_DrawPoint(i,m,color);
    } 

}

void Lcd_pic(const unsigned char *array)               
{  
   unsigned int i,m,z=0;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   Lcd_WriteIndex(0x2C);
   
   LCD_RS_SET;
   //SPI.writeBytes((uint8_t*)array, 25600);
    SPI.writePixels(array, 25600);
   
   /*
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {  
    //  SPI_WriteData(array[(z * 2) + 1]);  //д���8λ����
   //   SPI_WriteData(array[(z * 2)]);       //д���8λ��
      
      z++;
    }   */
}

void Lcd_Asc(uint16_t cursor_x,uint16_t cursor_y,char *c){
  
  uint8_t * pAscCharMatrix = (uint8_t *)&ASC16[0];
  uint32_t offset;
  uint8_t mask;
  uint16_t posX = cursor_x, posY = cursor_y;
  uint8_t charMatrix[16];

  uint8_t *pCharMatrix;

  int n = 0;
  while(*c != '\0'){
      offset = (uint32_t)(*c) * 16;
      pCharMatrix = pAscCharMatrix + offset;
      for (uint8_t row = 0; row < 16; row++) {
        mask = 0x80;
        posX = cursor_x + n*8;
        for (uint8_t col = 0; col < 8; col++) {
          if ((*pCharMatrix & mask) != 0) {
            Gui_DrawPoint(posX,posY,BLACK);
          }
          else{
            Gui_DrawPoint(posX,posY,WHITE);
          }
          posX ++;
          mask >>= 1;
        }
        posY += 1;
        pCharMatrix++;
      }
      //posX += 8;
      posY = cursor_y;
      c += 1;
      n++;
   }
  
}


void Lcd_PrintValueI(char x, char y, int dat)
{ 
  #if 0
	char i,j,k,m;  

	Lcd_SetXY(x,y);										   //设定x，y坐标
	if(dat < 0)											   //判断数据是否大于0
	{
		Lcd_Asc(x,y,"-");
		dat = - dat;	
	}
	else
	{
		Lcd_Asc(x,y,"+");
	}
	
	//m=  dat/1000;										   //取出数据的各个位
	i = (dat%1000)/100 + 48;
	j = (dat%100)/10 + 48;
	k = dat%10 + 48;

	//LcdWriteData(m+48);									   //依次显示出来
	//LcdWriteData(i+48);
	//LcdWriteData(j+48);
	//LcdWriteData(k+48);		

  Lcd_Asc(x+8,y,&i);
  Lcd_Asc(x+16,y,&j);
  Lcd_Asc(x+24,y,&k);
  
  #else 
    char buf[32];
    sprintf(buf,"%d",dat);
    //strlen(buf); 
    Lcd_Asc(x,y,buf);
  #endif
}


void Lcd_PrintValueF(char x, char y, float data, char num){

  char buf[32];
  snprintf(buf,num,"%f",data);
  Lcd_Asc(x,y,buf);


}
