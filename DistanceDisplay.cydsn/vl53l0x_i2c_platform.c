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
 * File: vl53l0x_i2c_platform.c
 * Creator: Nick Butts
 * Date: Jan 5, 2015
 *
 * Copyright Version 1.0
 */

#include <project.h>
#include <stdint.h>
#include "vl53l0x_i2c_platform.h"
#include "debug.h"


int VL53L0X_i2c_init(char *comPortStr, unsigned int baudRate)
{
    I2C_Start();
    return 0;
}

int32_t VL53L0X_comms_close(void)
{
    return 0;
}

int32_t VL53L0X_write_multi(uint8_t address, uint8_t index, uint8_t  *pdata, int32_t count)
{
    uint32_t status;
    uint8_t buf[32];
    buf[0] = index;
    memcpy(buf+1, pdata, count);
    I2C_I2CMasterClearStatus();
    status = I2C_I2CMasterWriteBuf(address, buf, count+1, I2C_I2C_MODE_COMPLETE_XFER);
    if (status != I2C_I2C_MSTR_NO_ERROR)
    {
        //SCB_I2CMasterSendStop();
        DBG_PRINTF("VL53L0X_write_multi write error\r\n");
        return 1;
    }

    do
    {
        status = I2C_I2CMasterStatus();
    } while (status & I2C_I2C_MSTAT_XFER_INP);

    if (status & I2C_I2C_MSTAT_ERR_MASK)
    {
        DBG_PRINTF("VL53L0X_write_multi write error2\r\n");
        return 1;
    }

    return(0);
}

int32_t VL53L0X_read_multi(uint8_t address,  uint8_t index, uint8_t  *pdata, int32_t count)
{
    uint32_t status;
    I2C_I2CMasterClearStatus();
    status = I2C_I2CMasterWriteBuf(address, &index, sizeof(index), I2C_I2C_MODE_COMPLETE_XFER);
    if (status != I2C_I2C_MSTR_NO_ERROR)
    {
        //SCB_I2CMasterSendStop();
        DBG_PRINTF("VL53L0X_read_multi error\r\n");
        return 1;
    }

    do
    {
        status = I2C_I2CMasterStatus();
    } while (status & I2C_I2C_MSTAT_XFER_INP);

    if (status & I2C_I2C_MSTAT_ERR_MASK)
    {
        DBG_PRINTF("VL53L0X_read_multi error2\r\n");
        return 1;
    }

    status = I2C_I2CMasterReadBuf(address, pdata, count, I2C_I2C_MODE_COMPLETE_XFER);
    if (status != I2C_I2C_MSTR_NO_ERROR)
    {
        //SCB_I2CMasterSendStop();
        DBG_PRINTF("VL53L0X_read_multi error3\r\n");
        return 1;
    }

    do
    {
        status = I2C_I2CMasterStatus();
    } while (status & I2C_I2C_MSTAT_XFER_INP);

    if (status & I2C_I2C_MSTAT_ERR_MASK)
    {
        DBG_PRINTF("VL53L0X_read_multi error4\r\n");
        return 1;
    }

    return(0);
}

int32_t VL53L0X_write_byte(uint8_t address,  uint8_t index, uint8_t   data)
{
    return VL53L0X_write_multi(address, index, &data, 1);
}

int32_t VL53L0X_write_word(uint8_t address,  uint8_t index, uint16_t  data)
{
    return VL53L0X_write_multi(address, index, (uint8_t*)&data, 2);
}

int32_t VL53L0X_write_dword(uint8_t address, uint8_t index, uint32_t  data)
{
    return VL53L0X_write_multi(address, index, (uint8_t*)&data, 4);
}

int32_t VL53L0X_read_byte(uint8_t address,  uint8_t index, uint8_t  *pdata)
{
    return VL53L0X_read_multi(address, index, pdata, 1);
}

int32_t VL53L0X_read_word(uint8_t address,  uint8_t index, uint16_t *pdata)
{
    return VL53L0X_read_multi(address, index, (uint8_t*)pdata, 2);
}

int32_t VL53L0X_read_dword(uint8_t address, uint8_t index, uint32_t *pdata)
{
    return VL53L0X_read_multi(address, index, (uint8_t*)pdata, 4);
}
