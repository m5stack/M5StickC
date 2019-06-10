#include "bmm150.h"
#include <Arduino.h>
#include <Wire.h>


BMM150::BMM150()
{
}

int8_t BMM150::initialize(void)
{ 
  //Wire.begin();

	/* Power up the sensor from suspend to sleep mode */
  set_op_mode(BMM150_SLEEP_MODE);
	delay(BMM150_START_UP_TIME);

	/* Check chip ID */
	uint8_t id = i2c_read(BMM150_CHIP_ID_ADDR);	
	Serial.printf("id = %d\r\n",id);
	if(id != BMM150_CHIP_ID) {
			return BMM150_E_ID_NOT_CONFORM;
	}

	/* Function to update trim values */
	read_trim_registers();

  /* Setting the power mode as normal */
  set_op_mode(BMM150_NORMAL_MODE);

	/* Setting the preset mode as Low power mode 
	i.e. data rate = 10Hz XY-rep = 1 Z-rep = 2*/
	set_presetmode(BMM150_PRESETMODE_LOWPOWER);
	// set_presetmode(BMM150_HIGHACCURACY_REPZ);
	
  return BMM150_OK;
}

void BMM150::read_mag_data()
{
    int16_t msb_data;
    int8_t reg_data[BMM150_XYZR_DATA_LEN] = {0};
    
    i2c_read(BMM150_DATA_X_LSB, reg_data, BMM150_XYZR_DATA_LEN);

    /* Mag X axis data */
    reg_data[0] = BMM150_GET_BITS(reg_data[0], BMM150_DATA_X);
    /* Shift the MSB data to left by 5 bits */
    /* Multiply by 32 to get the shift left by 5 value */
    msb_data = ((int16_t)((int8_t)reg_data[1])) * 32;
    /* Raw mag X axis data */
    raw_mag_data.raw_datax = (int16_t)(msb_data | reg_data[0]);
    /* Mag Y axis data */
    reg_data[2] = BMM150_GET_BITS(reg_data[2], BMM150_DATA_Y);
    /* Shift the MSB data to left by 5 bits */
    /* Multiply by 32 to get the shift left by 5 value */
    msb_data = ((int16_t)((int8_t)reg_data[3])) * 32;
    /* Raw mag Y axis data */
    raw_mag_data.raw_datay = (int16_t)(msb_data | reg_data[2]);
    /* Mag Z axis data */
    reg_data[4] = BMM150_GET_BITS(reg_data[4], BMM150_DATA_Z);
    /* Shift the MSB data to left by 7 bits */
    /* Multiply by 128 to get the shift left by 7 value */
    msb_data = ((int16_t)((int8_t)reg_data[5])) * 128;
    /* Raw mag Z axis data */
    raw_mag_data.raw_dataz = (int16_t)(msb_data | reg_data[4]);
    /* Mag R-HALL data */
    reg_data[6] = BMM150_GET_BITS(reg_data[6], BMM150_DATA_RHALL);
    raw_mag_data.raw_data_r = (uint16_t)(((uint16_t)reg_data[7] << 6) | reg_data[6]);

    /* Compensated Mag X data in int16_t format */
    mag_data.x = compensate_x(raw_mag_data.raw_datax, raw_mag_data.raw_data_r);
    /* Compensated Mag Y data in int16_t format */
    mag_data.y = compensate_y(raw_mag_data.raw_datay, raw_mag_data.raw_data_r);
    /* Compensated Mag Z data in int16_t format */
    mag_data.z = compensate_z(raw_mag_data.raw_dataz, raw_mag_data.raw_data_r);
}

/*
 * @brief This internal API is used to obtain the compensated
 * magnetometer X axis data(micro-tesla) in int16_t.
 */
int16_t BMM150::compensate_x(int16_t mag_data_x, uint16_t data_rhall)
{
	int16_t retval;
	uint16_t process_comp_x0 = 0;
	int32_t process_comp_x1;
	uint16_t process_comp_x2;
	int32_t process_comp_x3;
	int32_t process_comp_x4;
	int32_t process_comp_x5;
	int32_t process_comp_x6;
	int32_t process_comp_x7;
	int32_t process_comp_x8;
	int32_t process_comp_x9;
	int32_t process_comp_x10;

	/* Overflow condition check */
	if (mag_data_x != BMM150_XYAXES_FLIP_OVERFLOW_ADCVAL) {
		if (data_rhall != 0) {
			/* Availability of valid data*/
			process_comp_x0 = data_rhall;
		} else if (trim_data.dig_xyz1 != 0) {
			process_comp_x0 = trim_data.dig_xyz1;
		} else {
			process_comp_x0 = 0;
		}
		if (process_comp_x0 != 0) {
			/* Processing compensation equations*/
			process_comp_x1 = ((int32_t)trim_data.dig_xyz1) * 16384;
			process_comp_x2 = ((uint16_t)(process_comp_x1 / process_comp_x0)) - ((uint16_t)0x4000);
			retval = ((int16_t)process_comp_x2);
			process_comp_x3 = (((int32_t)retval) * ((int32_t)retval));
			process_comp_x4 = (((int32_t)trim_data.dig_xy2) * (process_comp_x3 / 128));
			process_comp_x5 = (int32_t)(((int16_t)trim_data.dig_xy1) * 128);
			process_comp_x6 = ((int32_t)retval) * process_comp_x5;
			process_comp_x7 = (((process_comp_x4 + process_comp_x6) / 512) + ((int32_t)0x100000));
			process_comp_x8 = ((int32_t)(((int16_t)trim_data.dig_x2) + ((int16_t)0xA0)));
			process_comp_x9 = ((process_comp_x7 * process_comp_x8) / 4096);
			process_comp_x10 = ((int32_t)mag_data_x) * process_comp_x9;
			retval = ((int16_t)(process_comp_x10 / 8192));
			retval = (retval + (((int16_t)trim_data.dig_x1) * 8)) / 16;
		} else {
			retval = BMM150_OVERFLOW_OUTPUT;
		}
	} else {
		/* Overflow condition */
		retval = BMM150_OVERFLOW_OUTPUT;
	}

	return retval;
}

/*
 * @brief This internal API is used to obtain the compensated
 * magnetometer Y axis data(micro-tesla) in int16_t.
 */
int16_t BMM150::compensate_y(int16_t mag_data_y, uint16_t data_rhall)
{
	int16_t retval;
	uint16_t process_comp_y0 = 0;
	int32_t process_comp_y1;
	uint16_t process_comp_y2;
	int32_t process_comp_y3;
	int32_t process_comp_y4;
	int32_t process_comp_y5;
	int32_t process_comp_y6;
	int32_t process_comp_y7;
	int32_t process_comp_y8;
	int32_t process_comp_y9;

	/* Overflow condition check */
	if (mag_data_y != BMM150_XYAXES_FLIP_OVERFLOW_ADCVAL) {
		if (data_rhall != 0) {
			/* Availability of valid data*/
			process_comp_y0 = data_rhall;
		} else if (trim_data.dig_xyz1 != 0) {
			process_comp_y0 = trim_data.dig_xyz1;
		} else {
			process_comp_y0 = 0;
		}
		if (process_comp_y0 != 0) {
			/*Processing compensation equations*/
			process_comp_y1 = (((int32_t)trim_data.dig_xyz1) * 16384) / process_comp_y0;
			process_comp_y2 = ((uint16_t)process_comp_y1) - ((uint16_t)0x4000);
			retval = ((int16_t)process_comp_y2);
			process_comp_y3 = ((int32_t) retval) * ((int32_t)retval);
			process_comp_y4 = ((int32_t)trim_data.dig_xy2) * (process_comp_y3 / 128);
			process_comp_y5 = ((int32_t)(((int16_t)trim_data.dig_xy1) * 128));
			process_comp_y6 = ((process_comp_y4 + (((int32_t)retval) * process_comp_y5)) / 512);
			process_comp_y7 = ((int32_t)(((int16_t)trim_data.dig_y2) + ((int16_t)0xA0)));
			process_comp_y8 = (((process_comp_y6 + ((int32_t)0x100000)) * process_comp_y7) / 4096);
			process_comp_y9 = (((int32_t)mag_data_y) * process_comp_y8);
			retval = (int16_t)(process_comp_y9 / 8192);
			retval = (retval + (((int16_t)trim_data.dig_y1) * 8)) / 16;
		} else {
			retval = BMM150_OVERFLOW_OUTPUT;
		}
	} else {
		/* Overflow condition*/
		retval = BMM150_OVERFLOW_OUTPUT;
	}

	return retval;
}

/*
 * @brief This internal API is used to obtain the compensated
 * magnetometer Z axis data(micro-tesla) in int16_t.
 */
int16_t BMM150::compensate_z(int16_t mag_data_z, uint16_t data_rhall)
{
	int32_t retval;
	int16_t process_comp_z0;
	int32_t process_comp_z1;
	int32_t process_comp_z2;
	int32_t process_comp_z3;
	int16_t process_comp_z4;

	if (mag_data_z != BMM150_ZAXIS_HALL_OVERFLOW_ADCVAL) {
		if ((trim_data.dig_z2 != 0) && (trim_data.dig_z1 != 0)
		&& (data_rhall != 0) && (trim_data.dig_xyz1 != 0)) {
			/*Processing compensation equations*/
			process_comp_z0 = ((int16_t)data_rhall) - ((int16_t) trim_data.dig_xyz1);
			process_comp_z1 = (((int32_t)trim_data.dig_z3) * ((int32_t)(process_comp_z0))) / 4;
			process_comp_z2 = (((int32_t)(mag_data_z - trim_data.dig_z4)) * 32768);
			process_comp_z3 = ((int32_t)trim_data.dig_z1) * (((int16_t)data_rhall) * 2);
			process_comp_z4 = (int16_t)((process_comp_z3 + (32768)) / 65536);
			retval = ((process_comp_z2 - process_comp_z1) / (trim_data.dig_z2 + process_comp_z4));

			/* saturate result to +/- 2 micro-tesla */
			if (retval > BMM150_POSITIVE_SATURATION_Z) {
				retval =  BMM150_POSITIVE_SATURATION_Z;
			} else {
				if (retval < BMM150_NEGATIVE_SATURATION_Z)
					retval = BMM150_NEGATIVE_SATURATION_Z;
			}
			/* Conversion of LSB to micro-tesla*/
			retval = retval / 16;
		} else {
			retval = BMM150_OVERFLOW_OUTPUT;

		}
	} else {
		/* Overflow condition*/
		retval = BMM150_OVERFLOW_OUTPUT;
	}

	return (int16_t)retval;
}

void BMM150::set_presetmode(uint8_t preset_mode)
{
    switch (preset_mode) {
		case BMM150_PRESETMODE_LOWPOWER:
			/* Set the data rate x,y,z repetition
			for Low Power mode */
			settings.data_rate = BMM150_DATA_RATE_10HZ;
			settings.xy_rep = BMM150_LOWPOWER_REPXY;
			settings.z_rep = BMM150_LOWPOWER_REPZ;
			set_odr_xyz_rep(settings);
			break;
		case BMM150_PRESETMODE_REGULAR:
			/* Set the data rate x,y,z repetition
			for Regular mode */
			settings.data_rate = BMM150_DATA_RATE_10HZ;
			settings.xy_rep = BMM150_REGULAR_REPXY;
			settings.z_rep = BMM150_REGULAR_REPZ;
			set_odr_xyz_rep(settings);
			break;
		case BMM150_PRESETMODE_HIGHACCURACY:
			/* Set the data rate x,y,z repetition
			for High Accuracy mode */
			settings.data_rate = BMM150_DATA_RATE_20HZ;
			settings.xy_rep = BMM150_HIGHACCURACY_REPXY;
			settings.z_rep = BMM150_HIGHACCURACY_REPZ;
			set_odr_xyz_rep(settings);
			break;
		case BMM150_PRESETMODE_ENHANCED:
			/* Set the data rate x,y,z repetition
			for Enhanced Accuracy mode */
			settings.data_rate = BMM150_DATA_RATE_10HZ;
			settings.xy_rep = BMM150_ENHANCED_REPXY;
			settings.z_rep = BMM150_ENHANCED_REPZ;
			set_odr_xyz_rep(settings);
			break;
		default:
			break;
		}
}

void BMM150::set_odr_xyz_rep(struct bmm150_settings settings)
{
    /* Set the ODR */
    set_odr(settings);
    /* Set the XY-repetitions number */
    set_xy_rep(settings);
    /* Set the Z-repetitions number */
    set_z_rep(settings);
}

void BMM150::set_xy_rep(struct bmm150_settings settings)
{   
    uint8_t rep_xy;
    rep_xy = settings.xy_rep;
    i2c_write(BMM150_REP_XY_ADDR, rep_xy);

}

void BMM150::set_z_rep(struct bmm150_settings settings)
{
    uint8_t rep_z;
    rep_z = settings.z_rep;
    i2c_write(BMM150_REP_Z_ADDR, rep_z);
}


void BMM150::soft_reset()
{
	uint8_t reg_data;

	reg_data = i2c_read(BMM150_POWER_CONTROL_ADDR);
	reg_data = reg_data | BMM150_SET_SOFT_RESET;
	i2c_write(BMM150_POWER_CONTROL_ADDR, reg_data);
	delay(BMM150_SOFT_RESET_DELAY);
}


void BMM150::set_odr(struct bmm150_settings settings)
{
    uint8_t reg_data;

    reg_data = i2c_read(BMM150_OP_MODE_ADDR);
    /*Set the ODR value */
		reg_data = BMM150_SET_BITS(reg_data, BMM150_ODR, settings.data_rate);
    i2c_write(BMM150_OP_MODE_ADDR, reg_data);
}

void BMM150::i2c_write(short address, short data)
{
    Wire.beginTransmission(BMM150_I2C_Address);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

void BMM150::i2c_read(short address, uint8_t *buffer, short length)
{
    Wire.beginTransmission(BMM150_I2C_Address);
    Wire.write(address);
    Wire.endTransmission();

    Wire.beginTransmission(BMM150_I2C_Address);
    Wire.requestFrom(BMM150_I2C_Address, length);
    
    if(Wire.available() == length)
    {
        for(uint8_t i = 0; i < length; i++)
        {
            buffer[i] = Wire.read();
        }
    }
    
    Wire.endTransmission();
}


void BMM150::i2c_read(short address, int8_t *buffer, short length)
{
    Wire.beginTransmission(BMM150_I2C_Address);
    Wire.write(address);
    Wire.endTransmission();

    Wire.beginTransmission(BMM150_I2C_Address);
    Wire.requestFrom(BMM150_I2C_Address, length);
    
    if(Wire.available() == length)
    {
        for(uint8_t i = 0; i < length; i++)
        {
            buffer[i] = Wire.read();
        }
    }
    
    Wire.endTransmission();
}

uint8_t BMM150::i2c_read(short address)
{
    uint8_t byte;

    Wire.beginTransmission(BMM150_I2C_Address);
    Wire.write(address);
    Wire.endTransmission();

    Wire.beginTransmission(BMM150_I2C_Address);
    Wire.requestFrom(BMM150_I2C_Address, 1);
    byte = Wire.read();
    
    Wire.endTransmission();
    return byte;
}


// char* BMM150::getErrorText(short errorCode);
// {
//     if(ERRORCODE_1_NUM == 1)
//     return ERRORCODE_1;

//     return "Error not defined.";
// }

void BMM150::set_op_mode(uint8_t pwr_mode)
{	
    /* Select the power mode to set */
    switch (pwr_mode) {
        case BMM150_NORMAL_MODE:
            /* If the sensor is in suspend mode
            put the device to sleep mode */
            suspend_to_sleep_mode();
            /* write the op mode */
            write_op_mode(pwr_mode);
            break;
        case BMM150_FORCED_MODE:
            /* If the sensor is in suspend mode
            put the device to sleep mode */
            suspend_to_sleep_mode();
            /* write the op mode */
            write_op_mode(pwr_mode);
            break;
        case BMM150_SLEEP_MODE:
            /* If the sensor is in suspend mode
            put the device to sleep mode */
            suspend_to_sleep_mode();
            /* write the op mode */
            write_op_mode(pwr_mode);
            break;
        case BMM150_SUSPEND_MODE:
            /* Set the power control bit to zero */
            set_power_control_bit(BMM150_POWER_CNTRL_DISABLE);
            break;
        default:
            break;
    }
}

void BMM150::suspend_to_sleep_mode(void)
{
    set_power_control_bit(BMM150_POWER_CNTRL_ENABLE);
    /* Start-up time delay of 3ms*/
    delay(3);
}


void BMM150::read_trim_registers()
{
	uint8_t trim_x1y1[2] = {0};
	uint8_t trim_xyz_data[4] = {0};
	uint8_t trim_xy1xy2[10] = {0};
	uint16_t temp_msb = 0;

	/* Trim register value is read */
	i2c_read(BMM150_DIG_X1, trim_x1y1, 2);
	i2c_read(BMM150_DIG_Z4_LSB, trim_xyz_data, 4);
	i2c_read(BMM150_DIG_Z2_LSB, trim_xy1xy2, 10);
	/* Trim data which is read is updated
	in the device structure */
	trim_data.dig_x1 = (int8_t)trim_x1y1[0];
	trim_data.dig_y1 = (int8_t)trim_x1y1[1];
	trim_data.dig_x2 = (int8_t)trim_xyz_data[2];
	trim_data.dig_y2 = (int8_t)trim_xyz_data[3];
	temp_msb = ((uint16_t)trim_xy1xy2[3]) << 8;
	trim_data.dig_z1 = (uint16_t)(temp_msb | trim_xy1xy2[2]);
	temp_msb = ((uint16_t)trim_xy1xy2[1]) << 8;
	trim_data.dig_z2 = (int16_t)(temp_msb | trim_xy1xy2[0]);
	temp_msb = ((uint16_t)trim_xy1xy2[7]) << 8;
	trim_data.dig_z3 = (int16_t)(temp_msb | trim_xy1xy2[6]);
	temp_msb = ((uint16_t)trim_xyz_data[1]) << 8;
	trim_data.dig_z4 = (int16_t)(temp_msb | trim_xyz_data[0]);
	trim_data.dig_xy1 = trim_xy1xy2[9];
	trim_data.dig_xy2 = (int8_t)trim_xy1xy2[8];
	temp_msb = ((uint16_t)(trim_xy1xy2[5] & 0x7F)) << 8;
	trim_data.dig_xyz1 = (uint16_t)(temp_msb | trim_xy1xy2[4]);

}

void BMM150::write_op_mode(uint8_t op_mode)
{
    uint8_t reg_data = 0;
    reg_data = i2c_read(BMM150_OP_MODE_ADDR);
    /* Set the op_mode value in Opmode bits of 0x4C */
	  reg_data = BMM150_SET_BITS(reg_data, BMM150_OP_MODE, op_mode);
    i2c_write(BMM150_OP_MODE_ADDR, reg_data);
}

void BMM150::set_power_control_bit(uint8_t pwrcntrl_bit)
{
	uint8_t reg_data = 0;
    /* Power control register 0x4B is read */
    reg_data = i2c_read(BMM150_POWER_CONTROL_ADDR);
    /* Sets the value of power control bit */
    reg_data = BMM150_SET_BITS_POS_0(reg_data, BMM150_PWR_CNTRL, pwrcntrl_bit);
    i2c_write(BMM150_POWER_CONTROL_ADDR, reg_data);
}


// /*!
//  * @brief This API is used to perform the complete self test
//  * (both normal and advanced) for the BMM150 sensor
//  */
// int8_t BMM150::perform_self_test(uint8_t self_test_mode)
// {
// 	int8_t rslt;
// 	int8_t self_test_rslt = 0;

// 	switch (self_test_mode) {
// 	case BMM150_NORMAL_SELF_TEST:
// 		/* Set the sensor in sleep mode */
// 		settings.pwr_mode = BMM150_SLEEP_MODE;
// 		set_op_mode(BMM150_SLEEP_MODE);
// 		/* Perform the normal self test */
// 		rslt = perform_normal_self_test();
// 		break;

// 	case BMM150_ADVANCED_SELF_TEST:
// 		/* Perform the advanced self test */
// 		rslt = perform_adv_self_test();
// 		/* Check to ensure bus error does not occur */
// 		if (rslt >=  BMM150_OK) {
// 			/* Store the status of self test result */
// 			self_test_rslt = rslt;
// 			/* Perform soft reset */
// 			soft_reset();
// 		}
// 		rslt = self_test_rslt;
// 		break;
// 	default:
// 		rslt = BMM150_E_INVALID_CONFIG;
// 		break;
// 	}

// 	return rslt;
// }

// /*
//  * @brief This internal API is used to perform the normal self test
//  * of the sensor and return the self test result as return value
//  */
// int8_t BMM150::perform_normal_self_test()
// {
// 	int8_t rslt;
// 	uint8_t self_test_bit;

// 	/* Triggers the start of normal self test */
// 	enable_normal_self_test(&self_test_bit);
// 		/* Check for self test completion status */
// 		if (self_test_bit == 0) {
// 			/* Validates the self test results for all 3 axes */
// 			rslt = validate_normal_self_test();
// 		}

// 	return rslt;
// }

// /*!
//  * @brief This internal API is used to enable the normal self test by setting
//  * the Self Test bit (bit0) of the 0x4C register,
//  * which triggers the start of self test
//  */
// void BMM150::enable_normal_self_test(uint8_t *self_test_enable)
// {
// 	uint8_t reg_data;
// 	uint8_t self_test_val;

// 	/* Read the data from register 0x4C */
// 	reg_data = i2c_read(BMM150_OP_MODE_ADDR);
// 	/* Set the Self Test bit(bit0) of the 0x4C register */
// 	self_test_val = 1;
// 	reg_data = BMM150_SET_BITS_POS_0(reg_data, BMM150_SELF_TEST, self_test_val);
// 	/* Write the data to 0x4C register to trigger self test */
// 	i2c_write(BMM150_OP_MODE_ADDR, reg_data);
// 	delay(BMM150_NORMAL_SELF_TEST_DELAY);
// 	/* Read the data from register 0x4C */
// 	reg_data = i2c_read(BMM150_OP_MODE_ADDR);
// 	/* Self Test bit(bit0) is stored in self_test_enable,
// 	It will be reset to zero after the self test is over */
// 	*self_test_enable = BMM150_GET_BITS_POS_0(reg_data, BMM150_SELF_TEST);
// }

// /*!
//  * @brief This internal API is used to validate the results of normal self test
//  * by using the self test status available in the bit0 of registers 0x42,0x44
//  * and 0x46.
//  */
// int8_t BMM150::validate_normal_self_test()
// {
// 	int8_t rslt;
// 	uint8_t status;
// 	uint8_t self_test_rslt[5];

// 	/* Read the data from register 0x42 to 0x46 */
// 	i2c_read(BMM150_DATA_X_LSB, self_test_rslt, BMM150_SELF_TEST_LEN);
// 	/* Parse and get the self test status bits */
// 	/* X-Self-Test (bit0) of 0x42 register is stored*/
// 	self_test_rslt[0] = BMM150_GET_BITS_POS_0(self_test_rslt[0], BMM150_SELF_TEST);
// 	/* Y-Self-Test (bit0) of 0x44 register is stored */
// 	self_test_rslt[2] = BMM150_GET_BITS_POS_0(self_test_rslt[2], BMM150_SELF_TEST);
// 	/* Z-Self-Test (bit0) of 0x46 register is stored */
// 	self_test_rslt[4] = BMM150_GET_BITS_POS_0(self_test_rslt[4], BMM150_SELF_TEST);
// 	/* Combine the self test status and store it in the first
// 	3 bits of the status variable for processing*/
// 	status = (uint8_t)((self_test_rslt[4] << 2) | (self_test_rslt[2] << 1) | self_test_rslt[0]);
// 	/* Validate status and store Self test result in "rslt" */
// 	if (status == BMM150_SELF_TEST_STATUS_SUCCESS) {
// 		/* Self test is success when all status bits are set */
// 		rslt = BMM150_OK;
// 	} else {
// 		if (status == BMM150_SELF_TEST_STATUS_XYZ_FAIL) {
// 			/* Self test - all axis fail condition */
// 			rslt = BMM150_W_NORMAL_SELF_TEST_XYZ_FAIL;
// 		} else {
// 			/* Self test - some axis fail condition */
// 			rslt = (int8_t)status;
// 		}
// 	}

// 	return rslt;
// }

// /*!
//  * @brief This internal API is used to perform advanced self test for Z axis
//  */
// int8_t BMM150::perform_adv_self_test()
// {
// 	uint8_t self_test_current;
// 	int16_t positive_data_z;
// 	int16_t negative_data_z;
// 	int8_t rslt;

// 	/* Set the desired power mode ,axes control and repetition settings */
// 	adv_self_test_settings();
// 	/* Measure the Z axes data with positive self-test current */
// 	self_test_current = BMM150_ENABLE_POSITIVE_CURRENT;
// 	adv_self_test_measurement(self_test_current, &positive_data_z);
// 	/* Measure the Z axes data with
// 	negative self-test current */
// 	self_test_current = BMM150_ENABLE_NEGATIVE_CURRENT;
// 	adv_self_test_measurement(self_test_current, &negative_data_z);
// 	/* Disable self-test current */
// 	self_test_current = BMM150_DISABLE_SELF_TEST_CURRENT;
// 	set_adv_self_test_current(self_test_current);
// 	/* Validate the advanced self test */
// 	rslt = validate_adv_self_test(positive_data_z, negative_data_z);

// 	return rslt;
// }

// /*!
//  * @brief This internal API is used to set the desired power mode ,
//  * axes control and repetition settings for advanced self test
//  */
// void BMM150::adv_self_test_settings()
// {
// 	/* Set the power mode as sleep mode */
// 	settings.pwr_mode = BMM150_SLEEP_MODE;
// 	set_op_mode(BMM150_SLEEP_MODE);
// 	/* Disable XY-axis measurement */
// 	settings.xyz_axes_control = BMM150_DISABLE_XY_AXIS;
// 	set_control_measurement_xyz(settings);
// 	/* Repetition value is set as 0x04 */
// 	settings.z_rep = BMM150_SELF_TEST_REP_Z;
// 	set_z_rep(settings);
// }

// /*!
//  * @brief This internal API is used to set the positive or negative value of
//  * self-test current and obtain the corresponding magnetometer z axis data
//  */
// void BMM150::adv_self_test_measurement(uint8_t self_test_current, int16_t *data_z)
// {
// 	/* Set the advanced self test current as positive or
// 	negative based on the value of parameter "self_test_current" */
// 	set_adv_self_test_current(self_test_current);
// 	/* Set the device in forced mode*/
// 	settings.pwr_mode = BMM150_FORCED_MODE;
// 	set_op_mode(BMM150_FORCED_MODE);
// 	/* Delay to ensure measurement is complete */
// 	delay(4);
// 	/* Read Mag data and store the value of Z axis data */
// 	read_mag_data();
// 	/* Mag Z axis data is stored */
// 	*data_z = mag_data.z;
// }

// /*!
//  * @brief This internal API is used to get the difference between the
//  * Z axis mag data obtained by positive and negative self-test current
//  * and validate whether the advanced self test is done successfully or not.
//  */
// int8_t BMM150::validate_adv_self_test(int16_t positive_data_z, int16_t negative_data_z)
// {
// 	int32_t adv_self_test_rslt;
// 	int8_t rslt;

// 	/* Advanced self test difference between the Z axis mag data
// 	   obtained by the positive and negative self-test current */
// 	adv_self_test_rslt = positive_data_z - negative_data_z;
// 	/* Advanced self test validation */
// 	/*Value of adv_self_test_rslt should be in between 180-240 micro-tesla*/
// 	if ((adv_self_test_rslt > 180) && (adv_self_test_rslt < 240)) {
// 		/* Advanced self test success */
// 		rslt = BMM150_OK;
// 	} else {
// 		/* Advanced self test fail */
// 		rslt = BMM150_W_ADV_SELF_TEST_FAIL;
// 	}

// 	return rslt;
// }

// /*
//  * @brief This internal API is used to set the self test current value in
//  * the Adv. ST bits (bit6 and bit7) of 0x4C register
//  */
// void BMM150::set_adv_self_test_current(uint8_t self_test_current)
// {
// 	uint8_t reg_data;

// 	/* Read the 0x4C register */
// 	reg_data = i2c_read(BMM150_OP_MODE_ADDR);
// 	/* Set the self test current value in the Adv. ST bits
// 	(bit6 and bit7) of 0x4c register */
// 	reg_data = BMM150_SET_BITS(reg_data, BMM150_ADV_SELF_TEST, self_test_current);
// 	i2c_write(BMM150_OP_MODE_ADDR, reg_data);

// }

// /*
//  * @brief This internal API is used to enable or disable the magnetic
//  * measurement of x,y,z axes based on the value of xyz_axes_control.
//  */
// void BMM150::set_control_measurement_xyz(struct bmm150_settings settings)
// {	
// 	uint8_t reg_data;

// 	reg_data = i2c_read(BMM150_AXES_ENABLE_ADDR);
// 	/* Set the axes to be enabled/disabled*/
// 	reg_data = BMM150_SET_BITS(reg_data, BMM150_CONTROL_MEASURE, settings.xyz_axes_control);
// 	i2c_write(BMM150_AXES_ENABLE_ADDR, reg_data);
// }
