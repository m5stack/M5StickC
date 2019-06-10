#ifndef _BMM150_DEFS_H_
#define _BMM150_DEFS_H_

#include <Arduino.h>


/**\name Macro definitions */

/**\name API success code */
#define BMM150_OK	(0)

/**\name API error codes */
#define BMM150_E_ID_NOT_CONFORM		    (-1)
#define BMM150_E_INVALID_CONFIG         (-2)
// #define BMM150_E_ID_WRONG		    (-3)

/**\name API warning codes */
#define BMM150_W_NORMAL_SELF_TEST_YZ_FAIL	INT8_C(1)
#define BMM150_W_NORMAL_SELF_TEST_XZ_FAIL	INT8_C(2)
#define BMM150_W_NORMAL_SELF_TEST_Z_FAIL	INT8_C(3)
#define BMM150_W_NORMAL_SELF_TEST_XY_FAIL	INT8_C(4)
#define BMM150_W_NORMAL_SELF_TEST_Y_FAIL	INT8_C(5)
#define BMM150_W_NORMAL_SELF_TEST_X_FAIL	INT8_C(6)
#define BMM150_W_NORMAL_SELF_TEST_XYZ_FAIL	INT8_C(7)
#define BMM150_W_ADV_SELF_TEST_FAIL		INT8_C(8)

#define BMM150_I2C_Address  0x10

/**\name CHIP ID & SOFT RESET VALUES      */
#define BMM150_CHIP_ID              0x32
#define BMM150_SET_SOFT_RESET		0x82

/**\name POWER MODE DEFINTIONS      */
#define BMM150_NORMAL_MODE		0x00
#define BMM150_FORCED_MODE		0x01
#define BMM150_SLEEP_MODE		0x03
#define BMM150_SUSPEND_MODE		0x04

/**\name PRESET MODE DEFINITIONS  */
#define BMM150_PRESETMODE_LOWPOWER                 0x01
#define BMM150_PRESETMODE_REGULAR                  0x02
#define BMM150_PRESETMODE_HIGHACCURACY             0x03
#define BMM150_PRESETMODE_ENHANCED                 0x04

/**\name Power mode settings  */
#define	BMM150_POWER_CNTRL_DISABLE	0x00
#define	BMM150_POWER_CNTRL_ENABLE	0x01

/**\name Sensor delay time settings  */
#define BMM150_SOFT_RESET_DELAY		(1)
#define BMM150_NORMAL_SELF_TEST_DELAY	(2)
#define BMM150_START_UP_TIME		(3)
#define BMM150_ADV_SELF_TEST_DELAY	(4)

/**\name ENABLE/DISABLE DEFINITIONS  */
#define BMM150_XY_CHANNEL_ENABLE	0x00
#define BMM150_XY_CHANNEL_DISABLE	0x03

/**\name Register Address */
#define BMM150_CHIP_ID_ADDR		    0x40
#define BMM150_DATA_X_LSB		    0x42
#define BMM150_DATA_X_MSB		    0x43
#define BMM150_DATA_Y_LSB		    0x44
#define BMM150_DATA_Y_MSB		    0x45
#define BMM150_DATA_Z_LSB		    0x46
#define BMM150_DATA_Z_MSB		    0x47
#define BMM150_DATA_READY_STATUS	0x48
#define BMM150_INTERRUPT_STATUS		0x4A
#define BMM150_POWER_CONTROL_ADDR	0x4B
#define BMM150_OP_MODE_ADDR		    0x4C
#define BMM150_INT_CONFIG_ADDR		0x4D
#define BMM150_AXES_ENABLE_ADDR		0x4E
#define BMM150_LOW_THRESHOLD_ADDR	0x4F
#define BMM150_HIGH_THRESHOLD_ADDR	0x50
#define BMM150_REP_XY_ADDR		    0x51
#define BMM150_REP_Z_ADDR		    0x52

/**\name DATA RATE DEFINITIONS  */
#define BMM150_DATA_RATE_10HZ        (0x00)
#define BMM150_DATA_RATE_02HZ        (0x01)
#define BMM150_DATA_RATE_06HZ        (0x02)
#define BMM150_DATA_RATE_08HZ        (0x03)
#define BMM150_DATA_RATE_15HZ        (0x04)
#define BMM150_DATA_RATE_20HZ        (0x05)
#define BMM150_DATA_RATE_25HZ        (0x06)
#define BMM150_DATA_RATE_30HZ        (0x07)

/**\name TRIM REGISTERS      */
/* Trim Extended Registers */
#define BMM150_DIG_X1               UINT8_C(0x5D)
#define BMM150_DIG_Y1               UINT8_C(0x5E)
#define BMM150_DIG_Z4_LSB           UINT8_C(0x62)
#define BMM150_DIG_Z4_MSB           UINT8_C(0x63)
#define BMM150_DIG_X2               UINT8_C(0x64)
#define BMM150_DIG_Y2               UINT8_C(0x65)
#define BMM150_DIG_Z2_LSB           UINT8_C(0x68)
#define BMM150_DIG_Z2_MSB           UINT8_C(0x69)
#define BMM150_DIG_Z1_LSB           UINT8_C(0x6A)
#define BMM150_DIG_Z1_MSB           UINT8_C(0x6B)
#define BMM150_DIG_XYZ1_LSB         UINT8_C(0x6C)
#define BMM150_DIG_XYZ1_MSB         UINT8_C(0x6D)
#define BMM150_DIG_Z3_LSB           UINT8_C(0x6E)
#define BMM150_DIG_Z3_MSB           UINT8_C(0x6F)
#define BMM150_DIG_XY2              UINT8_C(0x70)
#define BMM150_DIG_XY1              UINT8_C(0x71)

/**\name PRESET MODES - REPETITIONS-XY RATES */
#define BMM150_LOWPOWER_REPXY                    (1)
#define BMM150_REGULAR_REPXY                     (4)
#define BMM150_ENHANCED_REPXY                    (7)
#define BMM150_HIGHACCURACY_REPXY                (23)

/**\name PRESET MODES - REPETITIONS-Z RATES */
#define BMM150_LOWPOWER_REPZ                     (2)
#define BMM150_REGULAR_REPZ                      (14)
#define BMM150_ENHANCED_REPZ                     (26)
#define BMM150_HIGHACCURACY_REPZ                 (82)

/**\name Macros for bit masking */
#define	BMM150_PWR_CNTRL_MSK		(0x01)

#define	BMM150_CONTROL_MEASURE_MSK	(0x38)
#define	BMM150_CONTROL_MEASURE_POS	(0x03)

#define BMM150_POWER_CONTROL_BIT_MSK	(0x01)
#define BMM150_POWER_CONTROL_BIT_POS	(0x00)

#define BMM150_OP_MODE_MSK		(0x06)
#define BMM150_OP_MODE_POS		(0x01)

#define BMM150_ODR_MSK			(0x38)
#define BMM150_ODR_POS			(0x03)

#define BMM150_DATA_X_MSK		(0xF8)
#define BMM150_DATA_X_POS		(0x03)

#define BMM150_DATA_Y_MSK		(0xF8)
#define BMM150_DATA_Y_POS		(0x03)

#define BMM150_DATA_Z_MSK		(0xFE)
#define BMM150_DATA_Z_POS		(0x01)

#define BMM150_DATA_RHALL_MSK		(0xFC)
#define BMM150_DATA_RHALL_POS		(0x02)

#define	BMM150_SELF_TEST_MSK		(0x01)

#define	BMM150_ADV_SELF_TEST_MSK	(0xC0)
#define	BMM150_ADV_SELF_TEST_POS	(0x06)

#define	BMM150_DRDY_EN_MSK		(0x80)
#define	BMM150_DRDY_EN_POS		(0x07)

#define	BMM150_INT_PIN_EN_MSK		(0x40)
#define	BMM150_INT_PIN_EN_POS		(0x06)

#define	BMM150_DRDY_POLARITY_MSK	(0x04)
#define	BMM150_DRDY_POLARITY_POS	(0x02)

#define	BMM150_INT_LATCH_MSK		(0x02)
#define	BMM150_INT_LATCH_POS		(0x01)

#define	BMM150_INT_POLARITY_MSK		(0x01)

#define	BMM150_DATA_OVERRUN_INT_MSK	(0x80)
#define	BMM150_DATA_OVERRUN_INT_POS	(0x07)

#define	BMM150_OVERFLOW_INT_MSK		(0x40)
#define	BMM150_OVERFLOW_INT_POS		(0x06)

#define	BMM150_HIGH_THRESHOLD_INT_MSK	(0x38)
#define	BMM150_HIGH_THRESHOLD_INT_POS	(0x03)

#define	BMM150_LOW_THRESHOLD_INT_MSK	(0x07)

#define	BMM150_DRDY_STATUS_MSK		(0x01)

/**\name OVERFLOW DEFINITIONS  */
#define BMM150_XYAXES_FLIP_OVERFLOW_ADCVAL	(-4096)
#define BMM150_ZAXIS_HALL_OVERFLOW_ADCVAL	  (-16384)
#define BMM150_OVERFLOW_OUTPUT			        (-32768)
#define BMM150_NEGATIVE_SATURATION_Z        (-32767)
#define BMM150_POSITIVE_SATURATION_Z        (32767)
#ifdef BMM150_USE_FLOATING_POINT
#define BMM150_OVERFLOW_OUTPUT_FLOAT		0.0f
#endif

/**\name Register read lengths	*/
#define BMM150_SELF_TEST_LEN			(5)
#define BMM150_SETTING_DATA_LEN			(8)
#define BMM150_XYZR_DATA_LEN			(8)

/**\name Self test selection macros */
#define BMM150_NORMAL_SELF_TEST			(0)
#define BMM150_ADVANCED_SELF_TEST		(1)

/**\name Self test settings */
#define BMM150_DISABLE_XY_AXIS			(0x03)
#define BMM150_SELF_TEST_REP_Z			(0x04)

/**\name Advanced self-test current settings */
#define BMM150_DISABLE_SELF_TEST_CURRENT	(0x00)
#define BMM150_ENABLE_NEGATIVE_CURRENT		(0x02)
#define BMM150_ENABLE_POSITIVE_CURRENT		(0x03)

/**\name Normal self-test status */
#define BMM150_SELF_TEST_STATUS_XYZ_FAIL	(0x00)
#define BMM150_SELF_TEST_STATUS_SUCCESS		(0x07)

/**\name Macro to SET and GET BITS of a register*/
#define BMM150_SET_BITS(reg_data, bitname, data) \
				((reg_data & ~(bitname##_MSK)) | \
				((data << bitname##_POS) & bitname##_MSK))

#define BMM150_GET_BITS(reg_data, bitname)  ((reg_data & (bitname##_MSK)) >> \
							(bitname##_POS))

#define BMM150_SET_BITS_POS_0(reg_data, bitname, data) \
				((reg_data & ~(bitname##_MSK)) | \
				(data & bitname##_MSK))

#define BMM150_GET_BITS_POS_0(reg_data, bitname)  (reg_data & (bitname##_MSK))


struct bmm150_mag_data
{
    int16_t x;
    int16_t y;
    int16_t z;
};

/*
 * @brief bmm150 un-compensated (raw) magnetometer data
 */
struct bmm150_raw_mag_data {
	/*! Raw mag X data */
	int16_t raw_datax;
	/*! Raw mag Y data */
	int16_t raw_datay;
	/*! Raw mag Z data */
	int16_t raw_dataz;
	/*! Raw mag resistance value */
	uint16_t raw_data_r;
};

/*!
 * @brief bmm150 trim data structure
 */
struct bmm150_trim_registers {
	/*! trim x1 data */
	int8_t dig_x1;
	/*! trim y1 data */
	int8_t dig_y1;
	/*! trim x2 data */
	int8_t dig_x2;
	/*! trim y2 data */
	int8_t dig_y2;
	/*! trim z1 data */
	uint16_t dig_z1;
	/*! trim z2 data */
	int16_t dig_z2;
	/*! trim z3 data */
	int16_t dig_z3;
	/*! trim z4 data */
	int16_t dig_z4;
	/*! trim xy1 data */
	uint8_t dig_xy1;
	/*! trim xy2 data */
	int8_t dig_xy2;
	/*! trim xyz1 data */
	uint16_t dig_xyz1;
};

/**
 * @brief bmm150 sensor settings
 */
struct bmm150_settings {
	/*! Control measurement of XYZ axes */
	uint8_t xyz_axes_control;
	/*! Power control bit value */
	uint8_t pwr_cntrl_bit;
	/*! Power control bit value */
	uint8_t pwr_mode;
	/*! Data rate value (ODR) */
	uint8_t data_rate;
	/*! XY Repetitions */
	uint8_t xy_rep;
	/*! Z Repetitions */
	uint8_t z_rep;
	/*! Preset mode of sensor */
	uint8_t preset_mode;
	/*! Interrupt configuration settings */
	// struct bmm150_int_ctrl_settings int_settings;
};

#endif