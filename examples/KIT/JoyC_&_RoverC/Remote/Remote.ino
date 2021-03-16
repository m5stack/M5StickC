/*
    Description: JoyC will scan the mac address name of RoverC, 
    press and hold the Home button of M5StickC on JoyC for 3 seconds to start scanning the hotspot of the car, 
    and the pairing will be successful. After successful pairing, 
    the link icon will be highlighted in the upper left corner of the screen, 
    and the joystick value will be displayed on the screen. The left joystick controls the front and rear up and down, 
    the left and right controls the pan, and the right joystick controls the left and right steering.
*/

#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "EEPROM.h"

#define EEPROM_SIZE 64

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);
extern const unsigned char connect_on[800];
extern const unsigned char connect_off[800];

#define SYSNUM 		3

uint64_t realTime[4], time_count = 0;
bool k_ready = false;
uint32_t key_count = 0;

IPAddress local_IP(192, 168, 4, 100 + SYSNUM );
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char *ssid = "M5AP";
const char *password = "77777777";

WiFiUDP Udp;  
uint32_t send_count = 0;
uint8_t system_state = 0;


void I2CWrite1Byte( uint8_t Addr ,  uint8_t Data )
{
	Wire.beginTransmission(0x38);
	Wire.write(Addr);
	Wire.write(Data);
	Wire.endTransmission();
}

void I2CWritebuff( uint8_t Addr,  uint8_t* Data, uint16_t Length )
{
	Wire.beginTransmission(0x38);
	Wire.write(Addr);
	for (int i = 0; i < Length; i++)
	{
		Wire.write(Data[i]);
	}
	Wire.endTransmission();
}

uint8_t I2CRead8bit( uint8_t Addr )
{
	Wire.beginTransmission(0x38);
	Wire.write(Addr);
	Wire.endTransmission();
	Wire.requestFrom(0x38, 1);
	return Wire.read();
}

uint16_t I2CRead16bit( uint8_t Addr )
{
	uint16_t ReData = 0;
	Wire.beginTransmission(0x38);
	Wire.write(Addr);
	Wire.endTransmission();
	Wire.requestFrom(0x38, 2);
	for( int i = 0 ; i < 2 ; i++ )
	{
		ReData <<= 8;
		ReData |= Wire.read();
	}
	return ReData;
}

bool joys_l = false ;
uint8_t color[3] = {0,100,0};



uint8_t SendBuff[8] = 	{ 0xAA, 0x55, 
						 SYSNUM,
						 0x00, 0x00,
						 0x00, 0x00,
						 0xee};

void SendUDP()
{
	if( WiFi.status() == WL_CONNECTED )
	{
		Udp.beginPacket(IPAddress(192,168,4,1),1000 + SYSNUM );
		Udp.write(SendBuff,8);
		Udp.endPacket();
	}
}

char APName[20];
String WfifAPBuff[16];
uint32_t count_bn_a = 0, choose = 0;
String ssidname;
void setup() {
  // put your setup code here, to run once:
    M5.begin();
    Wire.begin(0,26,10000);
	EEPROM.begin(EEPROM_SIZE);

	M5.Lcd.setRotation(4);
    M5.Lcd.setSwapBytes(false);
    Disbuff.createSprite(80, 160);
    Disbuff.setSwapBytes(true);

	Disbuff.fillRect(0,0,80,20,Disbuff.color565(50,50,50));
	Disbuff.setTextSize(2);
	Disbuff.setTextColor(GREEN);
	Disbuff.setCursor(55,6);
	//Disbuff.printf("%03d",SYSNUM);

	Disbuff.pushImage(0,0,20,20,(uint16_t *)connect_off);
	Disbuff.pushSprite(0,0);

	uint8_t res = I2CRead8bit(0x32);
	Serial.printf("Res0 = %02X \r\n",res );
	/*
	if( res & 0x02 )
	{
		I2CWrite1Byte(0x10,0x02);
	}
	else
	{
		Disbuff.setTextSize(1);
		Disbuff.setTextColor(GREEN);
		Disbuff.fillRect(0,0,80,20,Disbuff.color565(50,50,50));
		Disbuff.setCursor(5,20);
		Disbuff.printf("calibration");
		Disbuff.pushSprite(0,0);
		I2CWrite1Byte( 0x32 , 0x01 );
	}
	*/
	M5.update();
	if(( EEPROM.read(0) != 0x56 )||( M5.BtnA.read() == 1 ))
	{
		WiFi.mode(WIFI_STA);
		int n = WiFi.scanNetworks();
		Disbuff.setTextSize(1);
		Disbuff.setTextColor(GREEN);
		Disbuff.fillRect(0,0,80,20,Disbuff.color565(50,50,50));
		
		if (n == 0) 
		{
			Disbuff.setCursor(5,20);
			Disbuff.printf("no networks");

		} 
		else {
			int count = 0;
			for (int i = 0; i < n; ++i) {
				if( WiFi.SSID(i).indexOf("M5AP") != -1 )
				{
					if( count == 0 )
					{
						Disbuff.setTextColor(GREEN);
					}
					else
					{
						Disbuff.setTextColor(WHITE);
					}
					Disbuff.setCursor(5,25 + count *10 );
					String str = WiFi.SSID(i);
					Disbuff.printf(str.c_str());
					WfifAPBuff[count] = WiFi.SSID(i);
					count++;
				}
			}
			Disbuff.pushSprite(0,0);
			while( 1 )
			{
				if( M5.BtnA.read() == 1 )
				{	
					if( count_bn_a >= 200 )
					{
						count_bn_a = 201;
						EEPROM.writeUChar(0,0x56);
						EEPROM.writeString(1,WfifAPBuff[choose]);
						ssidname = WfifAPBuff[choose];
						break;
					}
					count_bn_a ++;
					Serial.printf("count_bn_a %d \n", count_bn_a );
				}
				else if(( M5.BtnA.isReleased())&&( count_bn_a != 0 ))
				{
					Serial.printf("count_bn_a %d",count_bn_a);
					if( count_bn_a > 200 )
					{
						
					}
					else
					{
						choose ++;
						if( choose >= count )
						{
							choose = 0;
						}
						Disbuff.fillRect(0,0,80,20,Disbuff.color565(50,50,50));
						for (int i = 0; i < count; i++)
						{
							Disbuff.setCursor(5,25 + i *10 );
							if( choose == i )
							{
								Disbuff.setTextColor(GREEN);
							}
							else
							{
								Disbuff.setTextColor(WHITE);
							}
							Disbuff.printf(WfifAPBuff[i].c_str());
						}
						Disbuff.pushSprite(0,0);
					}
					count_bn_a = 0;
				}
				delay(10);
				M5.update();
			}
			//EEPROM.writeString(1,WfifAPBuff[0]);
    	}
	}
	else if( EEPROM.read(0) == 0x56 )
	{
		ssidname = EEPROM.readString(1);
		EEPROM.readString(1,APName,16);
	}

	Disbuff.fillRect(0,20,80,140,BLACK);

	if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) 
	{
    	Serial.println("STA Failed to configure");
  	}

	WiFi.begin(ssidname.c_str(), password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
  	}
	

	Udp.begin(2000);
	
	Disbuff.pushImage(0,0,20,20,(uint16_t *)connect_on);
	Disbuff.pushSprite(0,0);
}


uint8_t adc_value[5] = {0};
uint16_t AngleBuff[4];
uint32_t count = 0;
void loop() {


	for (int i = 0; i < 5; i++)
	{
		adc_value[i] =  I2CRead8bit( 0x60 + i );
	}

	for (int i = 0; i < 4; i++)
	{
		AngleBuff[i] = 	I2CRead16bit( 0x50 + i * 2 );
	}

	delay(10);

	
	if( WiFi.status() != WL_CONNECTED )
	{
		Disbuff.pushImage(0,0,20,20,(uint16_t *)connect_off);
		Disbuff.pushSprite(0,0);

		count ++;
		if( count > 500 )
		{
			WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
			count = 0;
		}
	}
	else
	{
		SendBuff[3] = map(AngleBuff[0],0,4000,0,200);
		SendBuff[4] = map(AngleBuff[1],0,4000,0,200);
		SendBuff[5] = map(AngleBuff[2],0,4000,0,200);
		/*
		Disbuff.pushImage(0,0,20,20,(uint16_t *)connect_on);
		Disbuff.pushSprite(0,0);
		count = 0;
		*/
		if( ( SendBuff[3] > 110  )||( SendBuff[3] < 90 )||\
			( SendBuff[4] > 110  )||( SendBuff[4] < 90 )||\
			( SendBuff[5] > 110  )||( SendBuff[5] < 90 ))
		{
			SendBuff[6] = 0x01;
		}
		else
		{
			SendBuff[6] = 0x00;
		}
		SendUDP();
	}
	

	Disbuff.fillRect( 0,30,80,130,BLACK);
	Disbuff.setCursor(10,30);
	Disbuff.printf("%04d",SendBuff[3]);
	Disbuff.setCursor(10,45);
	Disbuff.printf("%04d",SendBuff[4]);
	Disbuff.setCursor(10,60);
	Disbuff.printf("%04d",SendBuff[5]);

	Disbuff.setCursor(10,100);
	Disbuff.printf("%04X",adc_value[4]);
	Disbuff.pushSprite(0,0);
}