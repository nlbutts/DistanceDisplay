//*****************************************************************************
//
// Template_Driver.h - Display driver for any LCD Controller. This file serves as
//						a template for creating new LCD driver files
//
//*****************************************************************************

#ifndef SHARP_LS013B4DN04_H_
#define SHARP_LS013B4DN04_H_

//*****************************************************************************
//
// Include Files
//
//*****************************************************************************
#include "grlib.h"
/*Put any necessary include files here*/

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************
// Number of pixels on LCD X-axis
// TemplateDisplayFix
#define LCD_X_SIZE      96

// Number of pixels on LCD Y-axis
// TemplateDisplayFix
#define LCD_Y_SIZE      96

// Number of bits required to draw one pixel on the LCD screen
// TemplateDisplayFix
#define BPP     1

// Define LCD Screen Orientation Here
#define LANDSCAPE
//#define LANDSCAPE_FLIP
//#define PORTRAIT
//#define PORTRAIT_FLIP

//*****************************************************************************
//
// Defines for the pins that are used to communicate with the LCD Driver
//
//*****************************************************************************

/*Define Pin Names here i.e. #define LCD_RESET_OUT	P6OUT*/

//*****************************************************************************
//
// Defines for LCD driver configuration
//
//*****************************************************************************

/* Defines for pixels, colors, masks, etc. Anything Template_Driver.c needs*/

//*****************************************************************************
//
// This driver operates in four different screen orientations.  They are:
//
// * Portrait - The screen is taller than it is wide. This is selected by defining
//              PORTRAIT.
//
// * Landscape - The screen is wider than it is tall. This is selected by defining
//               LANDSCAPE.
//
// * Portrait flip - The screen is taller than it is wide. This is
//                   selected by defining PORTRAIT_FLIP.
//
// * Landscape flip - The screen is wider than it is tall. This is
//                    selected by defining LANDSCAPE_FLIP.
//
// These can also be imagined in terms of screen rotation; if landscape mode is
// 0 degrees of screen rotation, portrait flip is 90 degrees of clockwise
// rotation, landscape flip is 180 degrees of rotation, and portrait is
// 270 degress of clockwise rotation.
//
// If no screen orientation is selected, "landscape" mode will be used.
//
//*****************************************************************************
#if !defined(PORTRAIT) && !defined(PORTRAIT_FLIP) && \
    !defined(LANDSCAPE) && !defined(LANDSCAPE_FLIP)
#define LANDSCAPE
#endif

//*****************************************************************************
//
// Various definitions controlling coordinate space mapping and drawing
// direction in the four supported orientations.
//
//*****************************************************************************
#ifdef PORTRAIT
#define MAPPED_X(x, y) (LCD_X_SIZE - (y) - 1)
#define MAPPED_Y(x, y) (x)
#endif
#ifdef LANDSCAPE
#define MAPPED_X(x, y) (x)
#define MAPPED_Y(x, y) (y)
#endif
#ifdef PORTRAIT_FLIP
#define MAPPED_X(x, y)  (y)
#define MAPPED_Y(x, y)  (LCD_Y_SIZE - (x) - 1)
#endif
#ifdef LANDSCAPE_FLIP
#define MAPPED_X(x, y)  (LCD_X_SIZE - (x) - 1)
#define MAPPED_Y(x, y)  (LCD_Y_SIZE - (y) - 1)
#endif

//*****************************************************************************
//
// Various LCD Controller command name labels and associated control bits
//
//*****************************************************************************

//*****************************************************************************
//
// Macros for the Display Driver
//
//*****************************************************************************

/* All macros can go here. This is typically the color translation function (example below)
   and could also include Set_Address(), Write_Data(), etc. */

//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color. This particular example
// requires the 8-8-8 24 bit RGB color to convert into 5-6-5 16 bit RGB Color
// Your conversion should be made to fit your LCD settings.
//
// \return Returns the display-driver specific color
//
// TemplateDisplayFix

#define DPYCOLORTRANSLATE(c)    ((((c) & 0x00f80000) >> 8) |               \
                                 (((c) & 0x0000fc00) >> 5) |               \
                                 (((c) & 0x000000f8) >> 3))

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
typedef struct
{
    /**
    * This function is used to enable/disable the LCD. This is typically connected to an external
    * GPIO pin
    *
    * @param uint8_t 1 to enable, 0 to display
    */
    void (*LCD_EN)(uint8_t);
    /**
    * This function is used to control the chip select. The chip select is an active high
    * style CS
    *
    * @param uint8_t 1 CS enabled, 0 disabled
    */
    void (*LCD_CS)(uint8_t);
    /**
    * This function is used to write data to the LCD. It is up to the implementor of this
    * function to handle the max buffer write length. Generally a buffer of 32 bytes
    * should be sufficent. This function can block if necessary

    * @param data a pointer to an array of bytes
    * @param len the number of bytes to write to the LCD
    */
    void (*WRITE)(const uint8_t* data, uint8_t len);
    /**
    * Called to determine if the transfer is complete
    *
    * @return 1 the TX is complete, 0 if not
    */
    uint8_t (*TXCOMPLETE)();

    /**
    * This function is used to delay to meet LCD timing
    *
    * @param delayInUs how long to delay in microseconds
    */
    void (*DELAYUS)(uint16_t delayInUs);
} SharpLCD_Config;

extern void SharpLS013B4DN04_DriverInit(SharpLCD_Config * config);
extern void SharpLS013B4DN04_FlushBufferToLCD();
extern void SharpLS013B4DN04_toggleVCOM();
void SharpLS013B4DN04_DriverPixelDraw(int16_t lX,
                                      int16_t lY,
                                      uint16_t ulValue);

extern const Graphics_Display g_SharpLS013B4DN04_Driver;
extern uint8_t SharpLS013B4DN04_Memory[];


#endif // SHARP_LS013B4DN04_H_
