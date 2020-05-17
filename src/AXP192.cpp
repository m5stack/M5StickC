#include "AXP192.h"

AXP192::AXP192()
{
  
}

void AXP192::begin(bool disableLDO2, bool disableLDO3, bool disableRTC, bool disableDCDC1, bool disableDCDC3)
{  
    Wire1.begin(21, 22);
    Wire1.setClock(400000);

    // Set LDO2 & LDO3(TFT_LED & TFT) 3.0V
    Write1Byte(0x28, 0xcc);

    // Set ADC sample rate to 200hz
    Write1Byte(0x84, 0b11110010);
    
    // Set ADC to All Enable
    Write1Byte(0x82, 0xff);

    // Bat charge voltage to 4.2, Current 100MA
    Write1Byte(0x33, 0xc0);

    // Depending on configuration enable LDO2, LDO3, DCDC1, DCDC3.
    byte buf = (Read8bit(0x12) & 0xef) | 0x4D;
    if(disableLDO3) buf &= ~(1<<3);
    if(disableLDO2) buf &= ~(1<<2);
    if(disableDCDC3) buf &= ~(1<<1);
    if(disableDCDC1) buf &= ~(1<<0);
    Write1Byte(0x12, buf);
    
    // 128ms power on, 4s power off
    Write1Byte(0x36, 0x0C);

    if(!disableRTC)
    {
        // Set RTC voltage to 3.3V
        Write1Byte(0x91, 0xF0);

        // Set GPIO0 to LDO
        Write1Byte(0x90, 0x02);
    }

    // Disable vbus hold limit
    Write1Byte(0x30, 0x80);

    // Set temperature protection
    Write1Byte(0x39, 0xfc);
    
    // Enable RTC BAT charge 
    Write1Byte(0x35, 0xa2 & (disableRTC ? 0x7F : 0xFF));
    
    // Enable bat detection
    Write1Byte(0x32, 0x46);

    // Set Power off voltage 3.0v
    Write1Byte(0x31 , (Read8bit(0x31) & 0xf8) | (1 << 2));
}

void AXP192::Write1Byte( uint8_t Addr ,  uint8_t Data )
{
    Wire1.beginTransmission(0x34);
    Wire1.write(Addr);
    Wire1.write(Data);
    Wire1.endTransmission();
}

uint8_t AXP192::Read8bit( uint8_t Addr )
{
    Wire1.beginTransmission(0x34);
    Wire1.write(Addr);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    return Wire1.read();
}

uint16_t AXP192::Read12Bit( uint8_t Addr)
{
    uint16_t Data = 0;
    uint8_t buf[2];
    ReadBuff(Addr,2,buf);
    Data = ((buf[0] << 4) + buf[1]);
    return Data;
}

uint16_t AXP192::Read13Bit( uint8_t Addr)
{
    uint16_t Data = 0;
    uint8_t buf[2];
    ReadBuff(Addr,2,buf);
    Data = ((buf[0] << 5) + buf[1]);
    return Data;
}

uint16_t AXP192::Read16bit( uint8_t Addr )
{
    uint16_t ReData = 0;
    Wire1.beginTransmission(0x34);
    Wire1.write(Addr);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 2);
    for( int i = 0 ; i < 2 ; i++ )
    {
        ReData <<= 8;
        ReData |= Wire1.read();
    }
    return ReData;
}

uint32_t AXP192::Read24bit( uint8_t Addr )
{
    uint32_t ReData = 0;
    Wire1.beginTransmission(0x34);
    Wire1.write(Addr);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 3);
    for( int i = 0 ; i < 3 ; i++ )
    {
        ReData <<= 8;
        ReData |= Wire1.read();
    }
    return ReData;
}

uint32_t AXP192::Read32bit( uint8_t Addr )
{
    uint32_t ReData = 0;
    Wire1.beginTransmission(0x34);
    Wire1.write(Addr);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 4);
    for( int i = 0 ; i < 4 ; i++ )
    {
        ReData <<= 8;
        ReData |= Wire1.read();
    }
    return ReData;
}

void AXP192::ReadBuff( uint8_t Addr , uint8_t Size , uint8_t *Buff )
{
    Wire1.beginTransmission(0x34);
    Wire1.write(Addr);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, (int)Size);
    for (int i = 0; i < Size; i++)
    {
        *( Buff + i )  = Wire1.read();
    }
}

void AXP192::ScreenBreath(uint8_t brightness)
{
    if (brightness > 12) 
    {
        brightness = 12;
    }
    uint8_t buf = Read8bit( 0x28 );
    Write1Byte( 0x28 , ((buf & 0x0f) | (brightness << 4)) );
}

// Return True = Battery Exist
bool AXP192::GetBatState()
{
    if( Read8bit(0x01) | 0x20 )
        return true;
    else
        return false;
}

// Input Power Status 
uint8_t AXP192::GetInputPowerStatus()
{
    return Read8bit(0x00);
}

// Battery Charging Status 
uint8_t AXP192::GetBatteryChargingStatus()
{
    return Read8bit(0x01);
}

//---------coulombcounter_from_here---------
//enable: void EnableCoulombcounter(void); 
//disable: void DisableCOulombcounter(void);
//stop: void StopCoulombcounter(void);
//clear: void ClearCoulombcounter(void);
//get charge data: uint32_t GetCoulombchargeData(void);
//get discharge data: uint32_t GetCoulombdischargeData(void);
//get coulomb val affter calculation: float GetCoulombData(void);
//------------------------------------------
void  AXP192::EnableCoulombcounter(void)
{
    Write1Byte( 0xB8 , 0x80 );
}

void  AXP192::DisableCoulombcounter(void)
{
    Write1Byte( 0xB8 , 0x00 );
}

void  AXP192::StopCoulombcounter(void)
{
    Write1Byte( 0xB8 , 0xC0 );
}

void  AXP192::ClearCoulombcounter(void)
{
    Write1Byte( 0xB8, Read8bit(0xB8) | 0x20);    // Only set the Clear Flag
}

uint32_t AXP192::GetCoulombchargeData(void)
{
    return Read32bit(0xB0);
}

uint32_t AXP192::GetCoulombdischargeData(void)
{
    return Read32bit(0xB4);
}

float AXP192::GetCoulombData(void)
{
    uint32_t coin = GetCoulombchargeData();
    uint32_t coout = GetCoulombdischargeData();
    uint32_t valueDifferent = 0;
    bool bIsNegative = false;

    if (coin > coout)
    {    // Expected, in always more then out
        valueDifferent = coin - coout;
    }
    else
    {    // Warning: Out is more than In, the battery is not started at 0% 
        // just Flip the output sign later
        bIsNegative = true;
        valueDifferent = coout - coin;
    }
    //c = 65536 * current_LSB * (coin - coout) / 3600 / ADC rate
    //Adc rate can be read from 84H, change this variable if you change the ADC reate
    float ccc = (65536 * 0.5 * valueDifferent) / 3600.0 / 200.0;  // Note the ADC has defaulted to be 200 Hz

    if( bIsNegative )
        ccc = 0.0 - ccc;    // Flip it back to negative
    return ccc;
}
//----------coulomb_end_at_here----------

uint16_t AXP192::GetVbatData(void){

    uint16_t vbat = 0;
    uint8_t buf[2];
    ReadBuff(0x78,2,buf);
    vbat = ((buf[0] << 4) + buf[1]); // V
    return vbat;
}

uint16_t AXP192::GetVinData(void)
{
    uint16_t vin = 0;
    uint8_t buf[2];
    ReadBuff(0x56,2,buf);
    vin = ((buf[0] << 4) + buf[1]); // V
    return vin;
}

uint16_t AXP192::GetIinData(void)
{
    uint16_t iin = 0;
    uint8_t buf[2];
    ReadBuff(0x58,2,buf);
    iin = ((buf[0] << 4) + buf[1]);
    return iin;
}

uint16_t AXP192::GetVusbinData(void)
{
    uint16_t vin = 0;
    uint8_t buf[2];
    ReadBuff(0x5a,2,buf);
    vin = ((buf[0] << 4) + buf[1]); // V
    return vin;
}

uint16_t AXP192::GetIusbinData(void)
{
    uint16_t iin = 0;
    uint8_t buf[2];
    ReadBuff(0x5C,2,buf);
    iin = ((buf[0] << 4) + buf[1]);
    return iin;
}

uint16_t AXP192::GetIchargeData(void)
{
    uint16_t icharge = 0;
    uint8_t buf[2];
    ReadBuff(0x7A,2,buf);
    icharge = ( buf[0] << 5 ) + buf[1] ;
    return icharge;
}

uint16_t AXP192::GetIdischargeData(void)
{
    uint16_t idischarge = 0;
    uint8_t buf[2];
    ReadBuff(0x7C,2,buf);
    idischarge = ( buf[0] << 5 ) + buf[1] ;
    return idischarge;
}

uint16_t AXP192::GetTempData(void)
{
    uint16_t temp = 0;
    uint8_t buf[2];
    ReadBuff(0x5e,2,buf);
    temp = ((buf[0] << 4) + buf[1]);
    return temp;
}

uint32_t AXP192::GetPowerbatData(void)
{
    uint32_t power = 0;
    uint8_t buf[3];
    ReadBuff(0x70,2,buf);
    power = (buf[0] << 16) + (buf[1] << 8) + buf[2];
    return power;
}

uint16_t AXP192::GetVapsData(void)
{
    uint16_t vaps = 0;
    uint8_t buf[2];
    ReadBuff(0x7e,2,buf);
    vaps = ((buf[0] << 4) + buf[1]);
    return vaps;
}

void AXP192::SetSleep(void)
{
    Write1Byte(0x31 , Read8bit(0x31) | ( 1 << 3)); // Turn on short press to wake up
    Write1Byte(0x90 , Read8bit(0x90) | 0x07); // GPIO1 floating
    Write1Byte(0x82, 0x00); // Disable ADCs
    Write1Byte(0x12, Read8bit(0x12) & 0xA1); // Disable all outputs but DCDC1
}

uint8_t AXP192::GetWarningLeve(void)
{
    Wire1.beginTransmission(0x34);
    Wire1.write(0x47);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 1);
    uint8_t buf = Wire1.read();
    return (buf & 0x01);
}

// -- sleep
void AXP192::DeepSleep(uint64_t time_in_us)
{ 
    SetSleep();
    esp_sleep_enable_ext0_wakeup((gpio_num_t)37, LOW);
    if (time_in_us > 0)
    {
        esp_sleep_enable_timer_wakeup(time_in_us);
    }
    else
    {
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
    }
    (time_in_us == 0) ? esp_deep_sleep_start() : esp_deep_sleep(time_in_us);
}

void AXP192::LightSleep(uint64_t time_in_us)
{
    if (time_in_us > 0)
    {
        esp_sleep_enable_timer_wakeup(time_in_us);
    }
    else
    {
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
    }
    esp_light_sleep_start();
}

// Return 0 = not press, 0x01 = long press(1.5s), 0x02 = short press
uint8_t AXP192::GetBtnPress()
{
    uint8_t state = Read8bit(0x46);  // IRQ 3 status.  
    if(state) 
    {
        Write1Byte( 0x46 , 0x03 );   // Write 1 back to clear IRQ
    }
    return state;
}

// Low Volt Level 1, when APS Volt Output < 3.4496 V
// Low Volt Level 2, when APS Volt Output < 3.3992 V, then this flag is SET (0x01)
// Flag will reset once battery volt is charged above Low Volt Level 1
// Note: now AXP192 have the Shutdown Voltage of 3.0V (B100) Def in REG 31H
uint8_t AXP192::GetWarningLevel(void)
{
    return Read8bit(0x47) & 0x01;
}

float AXP192::GetBatVoltage()
{
    float ADCLSB = 1.1 / 1000.0;
    uint16_t ReData = Read12Bit( 0x78 );
    return ReData * ADCLSB;
}

float AXP192::GetBatCurrent()
{
    float ADCLSB = 0.5;
    uint16_t CurrentIn = Read13Bit( 0x7A );
    uint16_t CurrentOut = Read13Bit( 0x7C );
    return ( CurrentIn - CurrentOut ) * ADCLSB;
}

float AXP192::GetVinVoltage()
{
    float ADCLSB = 1.7 / 1000.0;
    uint16_t ReData = Read12Bit( 0x56 );
    return ReData * ADCLSB;
}

float AXP192::GetVinCurrent()
{
    float ADCLSB = 0.625;
    uint16_t ReData = Read12Bit( 0x58 );
    return ReData * ADCLSB;
}

float AXP192::GetVBusVoltage()
{
    float ADCLSB = 1.7 / 1000.0;
    uint16_t ReData = Read12Bit( 0x5A );
    return ReData * ADCLSB;
}

float AXP192::GetVBusCurrent()
{
    float ADCLSB = 0.375;
    uint16_t ReData = Read12Bit( 0x5C );
    return ReData * ADCLSB;
}

float AXP192::GetTempInAXP192()
{
    float ADCLSB = 0.1;
    const float OFFSET_DEG_C = -144.7;
    uint16_t ReData = Read12Bit( 0x5E );
    return OFFSET_DEG_C + ReData * ADCLSB;
}

float AXP192::GetBatPower()
{
    float VoltageLSB = 1.1;
    float CurrentLCS = 0.5;
    uint32_t ReData = Read24bit( 0x70 );
    return  VoltageLSB * CurrentLCS * ReData/ 1000.0;
}

float AXP192::GetBatChargeCurrent()
{
    float ADCLSB = 0.5;
    uint16_t ReData = Read13Bit( 0x7A );
    return ReData * ADCLSB;
}

float AXP192::GetAPSVoltage()
{
    float ADCLSB = 1.4  / 1000.0;
    uint16_t ReData = Read12Bit( 0x7E );
    return ReData * ADCLSB;
}

float AXP192::GetBatCoulombInput()
{
    uint32_t ReData = Read32bit( 0xB0 );
    return ReData * 65536 * 0.5 / 3600 /25.0;
}

float AXP192::GetBatCoulombOut()
{
    uint32_t ReData = Read32bit( 0xB4 );
    return ReData * 65536 * 0.5 / 3600 /25.0;
}

void AXP192::SetCoulombClear()
{
    Write1Byte(0xB8,0x20);
}

// Can turn LCD Backlight OFF for power saving
void AXP192::SetLDO2( bool State )
{
    uint8_t buf = Read8bit(0x12);
    if( State == true )
    {
        buf = (1<<2) | buf;
    }
    else
    {
        buf = ~(1<<2) & buf;
    }
    Write1Byte( 0x12 , buf );
}

void AXP192::SetLDO3(bool State)
{
    uint8_t buf = Read8bit(0x12);
    if( State == true )
    {
        buf = (1<<3) | buf;
    }
    else
    {
        buf = ~(1<<3) & buf;
    }
    Write1Byte( 0x12 , buf );
}

void AXP192::SetGPIO0(bool State)
{
    uint8_t buf = Read8bit(0x90);
    if ( State == true )
    {
        buf &= ~(0x07); // clear last 3 bits
        buf |= 0x02;    // set as LDO
    }
    else
    {
        buf |= 0x07;    // set as floating
    }
    Write1Byte( 0x90 , buf );
}

// Not recommend to set charge current > 100mA, since Battery is only 80mAh.
// more then 1C charge-rate may shorten battery life-span.
void AXP192::SetChargeCurrent(uint8_t current)
{
    uint8_t buf = Read8bit(0x33);
    buf = (buf & 0xf0) | (current & 0x07);
    Write1Byte(0x33, buf);
}

// Cut all power, except for LDO1 (RTC)
void AXP192::PowerOff()
{
    Write1Byte(0x32, Read8bit(0x32) | 0x80);     // MSB for Power Off
}

void AXP192::SetAdcState(bool state)
{
    Write1Byte(0x82, state ? 0xff : 0x00);  // Enable / Disable all ADCs
}

// AXP192 have a 6 byte storage, when the power is still valid, the data will not be lost
void AXP192::Read6BytesStorage( uint8_t *bufPtr )
{
    // Address from 0x06 - 0x0B
    Wire1.beginTransmission(0x34);
    Wire1.write(0x06);
    Wire1.endTransmission();
    Wire1.requestFrom(0x34, 6);
    for( int i = 0; i < 6; ++i )
    {
        bufPtr[i] = Wire1.read();
    }
}

// AXP192 have a 6 byte storage, when the power is still valid, the data will not be lost
void AXP192::Write6BytesStorage( uint8_t *bufPtr )
{
    // Address from 0x06 - 0x0B
    Wire1.beginTransmission(0x34);
    Wire1.write(0x06);
    Wire1.write(bufPtr[0]);
    Wire1.write(0x07);
    Wire1.write(bufPtr[1]);
    Wire1.write(0x08);
    Wire1.write(bufPtr[2]);
    Wire1.write(0x09);
    Wire1.write(bufPtr[3]);
    Wire1.write(0x0A);
    Wire1.write(bufPtr[4]);
    Wire1.write(0x0B);
    Wire1.write(bufPtr[5]);
    Wire1.endTransmission();
}
