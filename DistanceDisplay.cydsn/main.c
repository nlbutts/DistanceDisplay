/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <stdint.h>
#include <stdio.h>
#include "project.h"
#include "SharpLS013B4DN04.h"
#include "rangefinder.h"
#include "debug.h"

static VL53L0X_Dev_t laserRF;
static VL53L0X_Config_t config;

uint8_t spiWriteComplete()
{
    return !SPI_SpiIsBusBusy();
}

int main(void)
{
    int range = 0;
    char str[100];
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    SPI_Start();
    UART_Start();
    LCD_EN_Write(1);

    VL53L0X_i2c_init(0, 0);

    memset(&laserRF, 0, sizeof(VL53L0X_Dev_t));
    memset(&config, 0, sizeof(VL53L0X_Config_t));

    //UART_DEBUG_PutString("Distance Display\r");
    //printf("Blah\n");
    DBG_PRINTF("Blah blah\n");

    SharpLCD_Config  cfg;
    cfg.LCD_EN      = LCD_EN_Write;
    cfg.LCD_CS      = CS_Write;
    cfg.WRITE       = SPI_SpiUartPutArray;
    cfg.TXCOMPLETE  = spiWriteComplete;
    cfg.DELAYUS     = CyDelayUs;
 
    VL_EN_Write(0);
    CyDelay(100);
    VL_EN_Write(1);
    rangeFinder_setAddress(&laserRF, 0x29 * 2);
    rangeFinder_initConfig(&config);
    rangeFinder_init(&laserRF, &config);
    range = rangeFinder_getRange(&laserRF);

    SharpLS013B4DN04_DriverInit(&cfg);

    Graphics_Context ctx;
    Graphics_initContext(&ctx, &g_SharpLS013B4DN04_Driver);
    Graphics_setFont(&ctx, &g_sFontCmss24);
    Graphics_setForegroundColor(&ctx, 0);
    Graphics_setBackgroundColor(&ctx, 0xFFFFFF);
    Graphics_clearDisplay(&ctx);

    for(;;)
    {
        range = rangeFinder_getRange(&laserRF);
        //printf("%d\n", (int)range);
        strcpy(str, "Distance: ");
        Graphics_drawString(&ctx, str, strlen(str), 0, 0, 1);
        snprintf(str, 100, "%03d mm", range);
        Graphics_drawString(&ctx, str, strlen(str), 0, 40, 1);
        SharpLS013B4DN04_FlushBufferToLCD();
        SharpLS013B4DN04_toggleVCOM();
        CyDelay(100);
    }
}

/* [] END OF FILE */
