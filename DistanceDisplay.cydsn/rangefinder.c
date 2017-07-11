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
 * File: rangefinder.c
 * Creator: Nick Butts
 * Date: Sept 27, 2016
 *
 * Copyright Version 1.0
 */

#include <project.h>
#include "rangefinder.h"
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "debug.h"

void print_pal_error(VL53L0X_Error Status){
    char buf[VL53L0X_MAX_STRING_LENGTH];
    VL53L0X_GetPalErrorString(Status, buf);
    DBG_PRINTF("API Status: %i : %s\n", Status, buf);
}

int32_t rangeFinder_setAddress(VL53L0X_Dev_t * pDev, uint8_t newAddress)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    // Initialize Comms
    pDev->I2cDevAddr      = 0x29;
    pDev->comms_type      =  1;
    pDev->comms_speed_khz =  400;

    DBG_PRINTF("Setting LRF address to %02X\r\n", newAddress);
    Status = VL53L0X_SetDeviceAddress(pDev, newAddress);
    print_pal_error(Status);
    if (Status == VL53L0X_ERROR_NONE)
    {
        pDev->I2cDevAddr = newAddress / 2;
    }

    return Status;
}

void rangeFinder_initConfig(VL53L0X_Config_t * config)
{
    // These values are based on calibration values
//     config->RefSpadCount        = 3;
//     config->IsApertureSpads     = 1;
//     config->VhvSettings         = 25;
//     config->PhaseCal            = 1;
    // These values are based on trial and error with the plastic material we are using.
    config->XTalkCompensation   = 0;
    config->OffsetMicroMeter    = 0;
    config->SignalRate          = 0.1*65536;
    config->SigmaRate           = 60*65536;
    config->TimingBudget        = 33000;
}

int32_t rangeFinder_init(VL53L0X_Dev_t * pDev, VL53L0X_Config_t * config)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    if(Status == VL53L0X_ERROR_NONE)
    {
        //printf ("Call of VL53L0X_DataInit\n");
        Status = VL53L0X_DataInit(pDev); // Data initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        //printf ("Call of VL53L0X_StaticInit\n");
        Status = VL53L0X_StaticInit(pDev); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE) // needed if a coverglass is used and no calibration has been performed
    {
        //printf ("Call of VL53L0X_PerformRefSpadManagement\n");
        Status = VL53L0X_PerformRefSpadManagement(pDev,
                &config->RefSpadCount, &config->IsApertureSpads); // Device Initialization
        print_pal_error(Status);
        printf("RefSpaceCount: %d  - IsApertureSpads: %d\n", (int)config->RefSpadCount, (int)config->IsApertureSpads);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        //printf ("Call of VL53L0X_PerformRefCalibration\n");
        Status = VL53L0X_PerformRefCalibration(pDev,
                &config->VhvSettings, &config->PhaseCal); // Device Initialization
        print_pal_error(Status);
        printf ("VhvSettings: %d  = PhaseCal: %d\n", (int)config->VhvSettings, (int)config->PhaseCal);
    }


    // Set offset calibration
    // if(Status == VL53L0X_ERROR_NONE)
    // {
    //     //printf ("Call of VL53L0X_PerformRefCalibration\n");
    //     Status = VL53L0X_SetOffsetCalibrationDataMicroMeter(pDev,
    //             config->OffsetMicroMeter);
    //     print_pal_error(Status);
    // }

    // // Set xtalk calibration
    // if(Status == VL53L0X_ERROR_NONE)
    // {
    //     //printf ("Call of VL53L0X_PerformRefCalibration\n");
    //     Status = VL53L0X_SetXTalkCompensationRateMegaCps(pDev,
    //             config->XTalkCompensation);
    //     print_pal_error(Status);
    // }


    // if(Status == VL53L0X_ERROR_NONE)
    // {
    //     Status = VL53L0X_SetXTalkCompensationEnable(pDev, 1);
    //     print_pal_error(Status);
    // }

    if(Status == VL53L0X_ERROR_NONE)
    {

        // no need to do this when we use VL53L0X_PerformSingleRangingMeasurement
        //printf ("Call of VL53L0X_SetDeviceMode\n");
        Status = VL53L0X_SetDeviceMode(pDev, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
        print_pal_error(Status);
    }

#if 0
    // Enable/Disable Sigma and Signal check
    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_SetLimitCheckEnable(pDev,
                VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    }
    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_SetLimitCheckEnable(pDev,
                VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    }

    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_SetLimitCheckValue(pDev,
                VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
                config->SignalRate);
    }
    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_SetLimitCheckValue(pDev,
                VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
                config->SigmaRate);
    }
    if (Status == VL53L0X_ERROR_NONE) {
        Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pDev,
                config->TimingBudget);
    }
   if (Status == VL53L0X_ERROR_NONE) {
       Status = VL53L0X_SetVcselPulsePeriod(pDev,
               VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
   }
   if (Status == VL53L0X_ERROR_NONE) {
       Status = VL53L0X_SetVcselPulsePeriod(pDev,
               VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
   }
#endif

    return Status;
}

int32_t rangeFinder_calibrateXTalk(VL53L0X_Dev_t * pDev, VL53L0X_Config_t * config, uint32_t distance)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    if(Status == VL53L0X_ERROR_NONE)
    {
        //printf ("Call of VL53L0X_DataInit\n");
        Status = VL53L0X_DataInit(pDev); // Data initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        //printf ("Call of VL53L0X_StaticInit\n");
        Status = VL53L0X_StaticInit(pDev); // Device Initialization
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        //printf ("Call of VL53L0X_PerformRefCalibration\n");
        Status = VL53L0X_PerformRefCalibration(pDev,
                &config->VhvSettings, &config->PhaseCal); // Device Initialization
        printf ("VhvSettings = %d, PhaseCal = %d\n",
                (int)config->VhvSettings,
                (int)config->PhaseCal);
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE) // needed if a coverglass is used and no calibration has been performed
    {
        //printf ("Call of VL53L0X_PerformRefSpadManagement\n");
        Status = VL53L0X_PerformRefSpadManagement(pDev,
                &config->RefSpadCount, &config->IsApertureSpads); // Device Initialization
        printf ("refSpadCount = %d, isApertureSpads = %d\n", (int)config->RefSpadCount, (int)config->IsApertureSpads);
        print_pal_error(Status);
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        FixPoint1616_t calDistance = distance << 16;
        Status = VL53L0X_PerformXTalkCalibration(pDev, calDistance, &config->XTalkCompensation);
        printf("XTalk Cal = %d\n", (int) config->XTalkCompensation);
        print_pal_error(Status);
    }

    return Status;
}

int32_t rangeFinder_calibrateOffset(VL53L0X_Dev_t * pDev, VL53L0X_Config_t * config, uint32_t distance)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    FixPoint1616_t calDistance = distance << 16;

    Status = VL53L0X_PerformOffsetCalibration(pDev, calDistance, &config->OffsetMicroMeter);
    printf("Offset Cal = %d\n", (int)config->OffsetMicroMeter);
    print_pal_error(Status);
    return Status;
}


int32_t rangeFinder_getRange(VL53L0X_Dev_t * pDev)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;

    /*
     *  Get the version of the VL53L0X API running in the firmware
     */

    Status = VL53L0X_PerformSingleRangingMeasurement(pDev, &RangingMeasurementData);
    status_int = -1;
    if (VL53L0X_ERROR_NONE == Status)
    {
        status_int = RangingMeasurementData.RangeMilliMeter;

        DBG_PRINTF("*********\n");
        DBG_PRINTF("Range Status sensor    = %02X\n", (int)pDev->I2cDevAddr);
        CyDelay(10);
        DBG_PRINTF("SignalRateRtnMegaCps   = %d\n", (int)RangingMeasurementData.SignalRateRtnMegaCps);
        CyDelay(10);
        DBG_PRINTF("AmbientRateRtnMegaCps  = %d\n", (int)RangingMeasurementData.AmbientRateRtnMegaCps);
        CyDelay(10);
        DBG_PRINTF("EffectiveSpadRtnCount  = %d\n", (int)RangingMeasurementData.EffectiveSpadRtnCount);
        CyDelay(10);
        DBG_PRINTF("RangeStatus            = %d\n", (int)RangingMeasurementData.RangeStatus);
        CyDelay(10);
        DBG_PRINTF("Range                  = %d\n", (int)RangingMeasurementData.RangeMilliMeter);
        CyDelay(10);
        DBG_PRINTF("MaxRange               = %d\n", (int)RangingMeasurementData.RangeDMaxMilliMeter);
        CyDelay(10);
    }
    else
    {
        //rangeFinder_init(pDev);
    }

    return status_int;
}
