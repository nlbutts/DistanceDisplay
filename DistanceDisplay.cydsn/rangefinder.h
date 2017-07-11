/**
 * Copyright (c) 2016 Appareo Systems, LLC.
 * 1810 NDSU Research Park Circle North
 * Fargo ND, 58102
 * All rights reserved.
 *
 * This is the confidential and proprietary information of Appareo Systems, LLC. You shall not
 * disclose such confidential information and shall use it only in accordance with the terms

 * of the license agreement you entered into with Appareo.
 *
 * File: rangefinder.h
 * Creator: Nick Butts
 * Date: Sept 27, 2016
 *
 * Copyright Version 1.0
 */

#ifndef RANGEFINDER_H
#define RANGEFINDER_H

#include "vl53l0x_api.h"

typedef struct
{
    uint32_t        RefSpadCount;
    uint8_t         IsApertureSpads;
    uint8_t         VhvSettings;
    uint8_t         PhaseCal;
    FixPoint1616_t  XTalkCompensation;
    int32_t         OffsetMicroMeter;
    FixPoint1616_t  SignalRate;
    FixPoint1616_t  SigmaRate;
    FixPoint1616_t  TimingBudget;

} VL53L0X_Config_t;

/**
* This function sets the laser rangefinders I2C address to a new address.
*
* @param pDev a pointer to the structure that contains information on the sensor
* @param newAddress the new I2C address
* @return int32_t the error code from the VL53L0x API
*/
int32_t rangeFinder_setAddress(VL53L0X_Dev_t * pDev, uint8_t newAddress);
/**
* Initializes the config structure with same values or from EEPROM
*
* @param config the configuration structure
*/
void rangeFinder_initConfig(VL53L0X_Config_t * config);
/**
* Initialize the range finders
*
* @param pDev pointer to a dev structure
* @param config the configuration structure
* @return int32_t the error code from the VL53L0x API
*/
int32_t rangeFinder_init(VL53L0X_Dev_t * pDev, VL53L0X_Config_t * config);
/**
* Calibrate cross talk
*
* @param pDev pointer to a dev structure
* @param config the configuration structure
* @param distance the distance to the calibration target
* @return int32_t the error code from the VL53L0x API
*/
int32_t rangeFinder_calibrateXTalk(VL53L0X_Dev_t * pDev, VL53L0X_Config_t * config, uint32_t distance);
/**
* Calibrate the offset to a target, the XTalk calibration MUST be called before this function
*
* @param pDev pointer to a dev structure
* @param config the configuration structure
* @param distance the distance to a target
* @return int32_t the error code from the VL53L0x API
*/
int32_t rangeFinder_calibrateOffset(VL53L0X_Dev_t * pDev, VL53L0X_Config_t * config, uint32_t distance);
/**
* This function performs a ranging and returns the range in the bottom 16-bits
* and the status in the next 8-bits. If there is an error it returns -1
*
* @param pDev a pointer to a VL53L0X_Dev_t structure
* @return int32_t the resulting measurement, see above
*/
int32_t rangeFinder_getRange(VL53L0X_Dev_t * pDev);

#endif /* RANGEFINDER_H */