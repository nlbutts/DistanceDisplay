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

uint8_t spiWriteComplete()
{
    uint8_t rv;
    uint8_t status = SPIM_ReadTxStatus() & (SPIM_STS_SPI_DONE | SPIM_STS_TX_FIFO_EMPTY);
    rv = status == (SPIM_STS_SPI_DONE | SPIM_STS_TX_FIFO_EMPTY) ? 1 : 0;
    return rv;
}

int main(void)
{
    uint16_t distance = 100;
    char str[100];
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    SPIM_Start();
    LCD_EN_Write(1);

    SharpLCD_Config  cfg;
    cfg.LCD_EN      = LCD_EN_Write;
    cfg.LCD_CS      = CS_Write;
    cfg.WRITE       = SPIM_PutArray;
    cfg.TXCOMPLETE  = spiWriteComplete;
    cfg.DELAYUS     = CyDelayUs;

    SharpLS013B4DN04_DriverInit(&cfg);

    Graphics_Context ctx;
    Graphics_initContext(&ctx, &g_SharpLS013B4DN04_Driver);
    Graphics_setFont(&ctx, &g_sFontCmss24);
    Graphics_setForegroundColor(&ctx, 0xFFFFFF);
    Graphics_setBackgroundColor(&ctx, 0);

    for(;;)
    {
        strcpy(str, "Distance: ");
        Graphics_drawString(&ctx, str, strlen(str), 0, 0, 1);
        snprintf(str, 100, "%03d mm", distance);
        Graphics_drawString(&ctx, str, strlen(str), 0, 40, 1);
        distance++;
        SharpLS013B4DN04_FlushBufferToLCD();
        SharpLS013B4DN04_toggleVCOM();
        CyDelay(100);
    }
}

/* [] END OF FILE */
