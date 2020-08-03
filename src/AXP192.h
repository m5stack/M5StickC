#ifndef __AXP192_H__
#define __AXP192_H__

#include <Wire.h>
#include <Arduino.h>

#define SLEEP_MSEC(us) (((uint64_t)us) * 1000L)
#define SLEEP_SEC(us)  (((uint64_t)us) * 1000000L)
#define SLEEP_MIN(us)  (((uint64_t)us) * 60L * 1000000L)
#define SLEEP_HR(us)   (((uint64_t)us) * 60L * 60L * 1000000L)

#define ADC_RATE_025HZ (0b00 << 6)
#define ADC_RATE_050HZ (0b01 << 6)
#define ADC_RATE_100HZ (0b10 << 6)
#define ADC_RATE_200HZ (0b11 << 6)

#define CURRENT_100MA  (0b0000)
#define CURRENT_190MA  (0b0001)
#define CURRENT_280MA  (0b0010)
#define CURRENT_360MA  (0b0011)
#define CURRENT_450MA  (0b0100)
#define CURRENT_550MA  (0b0101)
#define CURRENT_630MA  (0b0110)
#define CURRENT_700MA  (0b0111)

#define VOLTAGE_4100MV (0b00 << 5)
#define VOLTAGE_4150MV (0b01 << 5)
#define VOLTAGE_4200MV (0b10 << 5)
#define VOLTAGE_4360MV (0b11 << 5)

#define VOLTAGE_OFF_2600MV (0b000)
#define VOLTAGE_OFF_2700MV (0b001)
#define VOLTAGE_OFF_2800MV (0b010)
#define VOLTAGE_OFF_2900MV (0b011)
#define VOLTAGE_OFF_3000MV (0b100)
#define VOLTAGE_OFF_3100MV (0b101)
#define VOLTAGE_OFF_3200MV (0b110)
#define VOLTAGE_OFF_3300MV (0b111)

class AXP192 {
public:
    AXP192();
    /**
     * LDO2: Display backlight
     * LDO3: Display Control
     * RTC: Always ON, Switch RTC charging.
     * DCDC1: Main rail. When not set the controller shuts down.
     * DCDC3: Use unknown
     * LDO0: MIC
     */
    void  begin(bool disableLDO2 = false, bool disableLDO3 = false, bool disableRTC = false, bool disableDCDC1 = false, bool disableDCDC3 = false, bool disableLDO0 = false);
    void  ScreenBreath(uint8_t brightness);
    bool  GetBatState();
  
    uint8_t GetInputPowerStatus();
    uint8_t GetBatteryChargingStatus();

    void  DisableAllIRQ(void);
    void  ClearAllIRQ(void);
    void  EnablePressIRQ(bool short_press, bool long_press);
    void  GetPressIRQ(bool *short_press, bool* long_press);
    void  ClearPressIRQ(bool short_press, bool long_press);

    void  EnableCoulombcounter(void);
    void  DisableCoulombcounter(void);
    void  StopCoulombcounter(void);
    void  ClearCoulombcounter(void);
    uint32_t GetCoulombchargeData(void);        // Raw Data for Charge
    uint32_t GetCoulombdischargeData(void);     // Raw Data for Discharge
    float GetCoulombData(void);                 // total in - total out and calc

    uint16_t GetVbatData(void) __attribute__((deprecated));
    uint16_t GetIchargeData(void) __attribute__((deprecated));
    uint16_t GetIdischargeData(void) __attribute__((deprecated));
    uint16_t GetTempData(void) __attribute__((deprecated));
    uint32_t GetPowerbatData(void) __attribute__((deprecated));
    uint16_t GetVinData(void) __attribute__((deprecated));
    uint16_t GetIinData(void) __attribute__((deprecated));
    uint16_t GetVusbinData(void) __attribute__((deprecated));
    uint16_t GetIusbinData(void) __attribute__((deprecated));
    uint16_t GetVapsData(void) __attribute__((deprecated));
    uint8_t GetBtnPress(void);

      // -- sleep
    void SetSleep(void);
    void DeepSleep(uint64_t time_in_us = 0);
    void LightSleep(uint64_t time_in_us = 0);
    uint8_t GetWarningLeve(void) __attribute__((deprecated));

public:
    void  SetChargeVoltage( uint8_t );
    void  SetChargeCurrent( uint8_t );
    void  SetVOff( uint8_t voltage );
    float GetBatVoltage();
    float GetBatCurrent();
    float GetVinVoltage();
    float GetVinCurrent();
    float GetVBusVoltage();
    float GetVBusCurrent();
    float GetTempInAXP192();
    float GetBatPower();
    float GetBatChargeCurrent();
    float GetAPSVoltage();
    float GetBatCoulombInput();
    float GetBatCoulombOut();
    uint8_t GetWarningLevel(void);    
    void SetCoulombClear()  __attribute__((deprecated)); // use ClearCoulombcounter instead
    void SetLDO2( bool State );     // Can turn LCD Backlight OFF for power saving
    void SetLDO3( bool State );
    void SetGPIO0( bool State );
    void SetAdcState(bool State);
    void SetAdcRate( uint8_t rate );
    
    // -- Power Off
    void PowerOff();

    // Power Maintained Storage
    void Read6BytesStorage( uint8_t *bufPtr );
    void Write6BytesStorage( uint8_t *bufPtr );

    
private:
    void Write1Byte( uint8_t Addr ,  uint8_t Data );
    uint8_t Read8bit( uint8_t Addr );
    uint16_t Read12Bit( uint8_t Addr);
    uint16_t Read13Bit( uint8_t Addr);
    uint16_t Read16bit( uint8_t Addr );
    uint32_t Read24bit( uint8_t Addr );
    uint32_t Read32bit( uint8_t Addr );
    void ReadBuff( uint8_t Addr , uint8_t Size , uint8_t *Buff );
}; 

#endif
