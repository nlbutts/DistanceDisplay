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

    const char str[] = "Blah";
    //Graphics_drawString(&g_SharpLS013B4DN04_Driver, str, strlen(str), 10, 10, 1);
    g_SharpLS013B4DN04_Driver.callLineDrawH(NULL, 0, 50, 0, 1);
    g_SharpLS013B4DN04_Driver.callLineDrawH(NULL, 0, 50, 1, 1);
    g_SharpLS013B4DN04_Driver.callLineDrawH(NULL, 0, 50, 2, 1);
    g_SharpLS013B4DN04_Driver.callLineDrawH(NULL, 0, 50, 3, 1);
    g_SharpLS013B4DN04_Driver.callPixelDraw(NULL, 0, 0, 1);
    g_SharpLS013B4DN04_Driver.callPixelDraw(NULL, 1, 0, 1);
    g_SharpLS013B4DN04_Driver.callPixelDraw(NULL, 2, 0, 1);
    g_SharpLS013B4DN04_Driver.callPixelDraw(NULL, 3, 0, 1);

    SharpLS013B4DN04_FlushBufferToLCD();

    for(;;)
    {
        SharpLS013B4DN04_toggleVCOM();
        CyDelay(500);
    }
}

/* [] END OF FILE */
