/*
 * Init.c
 *
 *  Created on: Sep 1, 2022
 *      Author: Yurij
 */

#include "Init.h"
#include "main.h"
//#include "HX8357D_Defines.h"
//#include "ILI9341_Defines.h"

#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC
#define    CS_set  LL_GPIO_ResetOutputPin(CS_LCD_GPIO_Port, CS_LCD_Pin)
#define    CS_reset  LL_GPIO_SetOutputPin(CS_LCD_GPIO_Port, CS_LCD_Pin)
#define    DC_cmd   LL_GPIO_ResetOutputPin(DC_LSD_GPIO_Port, DC_LSD_Pin)
#define    DC_data   LL_GPIO_SetOutputPin(DC_LSD_GPIO_Port, DC_LSD_Pin)

uint8_t cool_img = 0;

void shiftCooler(){//Крутим кулер
	cool_img = !cool_img;
	if (cool_img == 0){
	   drawImage(cooler_0, 200, (70 + hh), 50, 52);
	   } else {
	   drawImage(cooler_180, 200, (70 + hh), 50, 52);
	   }
}

void ILI9488_Reset()
{
	LL_GPIO_ResetOutputPin(RST_GPIO_Port, RST_Pin);
    HAL_Delay(5);
    LL_GPIO_SetOutputPin(RST_GPIO_Port, RST_Pin);
}



 void writecommand(uint8_t cmd){
	     while (LL_SPI_IsActiveFlag_BSY(SPI1)); // Ждем завершения предыдущих передач (если они были)
	     DC_cmd;//DC в LOW (режим команды)
	     CS_set;//CS в LOW (выбор чипа)
	     LL_SPI_TransmitData8(SPI1, cmd);//Кидаем байт в регистр
	     while (LL_SPI_IsActiveFlag_BSY(SPI1));//Ждем, пока байт уйдет, прежде чем менять DC обратно
	 }

 void writedata(uint8_t dt){
	 while (!LL_SPI_IsActiveFlag_TXE(SPI1));//Ждем, если буфер передачи полон
	 DC_data;//DC в HIGH (режим данных)
	 CS_set;//CS в LOW (выбор чипа)
	 LL_SPI_TransmitData8(SPI1, dt);//Кидаем байт в регистр
}

void init_9488()
{
        ILI9488_Reset();
        HAL_Delay(1000);
 	    writecommand(0xE0); // Positive Gamma Control
 	    writedata(0x00);
 	    writedata(0x03);
 	    writedata(0x09);
 	    writedata(0x08);
 	    writedata(0x16);
 	    writedata(0x0A);
 	    writedata(0x3F);
 	    writedata(0x78);
 	    writedata(0x4C);
 	    writedata(0x09);
 	    writedata(0x0A);
 	    writedata(0x08);
 	    writedata(0x16);
 	    writedata(0x1A);
 	    writedata(0x0F);

 	    writecommand(0XE1); // Negative Gamma Control
 	    writedata(0x00);
 	    writedata(0x16);
 	    writedata(0x19);
 	    writedata(0x03);
 	    writedata(0x0F);
 	    writedata(0x05);
 	    writedata(0x32);
 	    writedata(0x45);
 	    writedata(0x46);
 	    writedata(0x04);
 	    writedata(0x0E);
 	    writedata(0x0D);
 	    writedata(0x35);
 	    writedata(0x37);
 	    writedata(0x0F);

 	    writecommand(0XC0); // Power Control 1
 	    writedata(0x17);
 	    writedata(0x15);

 	    writecommand(0xC1); // Power Control 2
 	    writedata(0x41);

 	    writecommand(0xC5); // VCOM Control
 	    writedata(0x00);
 	    writedata(0x12);
 	    writedata(0x80);

 	    writecommand(ILI9488_MADCTL); // Memory Access Control
 	    writedata(0x48);          // MX, BGR

 	    writecommand(0x3A); // Pixel Interface Format

 	    writedata(0x66);  // 18 bit colour for SPI

 	    writecommand(0xB0); // Interface Mode Control
 	    writedata(0x00);

 	    writecommand(0xB1); // Frame Rate Control
 	    writedata(0xA0);

 	    writecommand(0xB4); // Display Inversion Control
 	    writedata(0x02);

 	    writecommand(0xB6); // Display Function Control
 	    writedata(0x02);
 	    writedata(0x02);
 	    writedata(0x3B);

 	    writecommand(0xB7); // Entry Mode Set
 	    writedata(0xC6);

 	    writecommand(0xF7); // Adjust Control 3
 	    writedata(0xA9);
 	    writedata(0x51);
 	    writedata(0x2C);
 	    writedata(0x82);
 	    writecommand(ILI9488_MADCTL);
 	    //writedata(0xE8);//Поворот на 180 град.
 	    writedata(0x28);

 	    writecommand(ILI9488_SLPOUT);  //Exit Sleep
 	    HAL_Delay(120);

 	    writecommand(ILI9488_DISPON);  //Display on
 	    HAL_Delay(25);
}


//===============================================================================================================

void fillScreen(uint16_t color)
	      {
	        fillRect(0, 0,  ILI9488_TFTWIDTH, ILI9488_TFTHEIGHT, color);
	      }

void setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea)
	      {
	        //if (hwSPI) spi_begin();
	        writecommand(0x33); // Vertical scroll definition
	        writedata(topFixedArea >> 8);
	        writedata(topFixedArea);
	        writedata((ILI9488_TFTHEIGHT - topFixedArea - bottomFixedArea) >> 8);
	        writedata(ILI9488_TFTHEIGHT - topFixedArea - bottomFixedArea);
	        writedata(bottomFixedArea >> 8);
	        writedata(bottomFixedArea);
	       // if (hwSPI) spi_end();
	      }
	      void scroll(uint16_t pixels)
	      {
	        //if (hwSPI) spi_begin();
	        writecommand(0x37); // Vertical scrolling start address
	        writedata(pixels >> 8);
	        writedata(pixels);
	        //if (hwSPI) spi_end();
	      }

	      void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
	      {
	        writecommand(ILI9488_CASET); // Column addr set
	        writedata(x0 >> 8);
	        writedata(x0 & 0xFF);     // XSTART
	        writedata(x1 >> 8);
	        writedata(x1 & 0xFF);     // XEND

	        writecommand(ILI9488_PASET); // Row addr set
	        writedata(y0>>8);
	        writedata(y0 &0xff);     // YSTART
	        writedata(y1>>8);
	        writedata(y1 &0xff);     // YEND

	        writecommand(ILI9488_RAMWR); // write to RAM

	      }

 void drawImage(const uint16_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
// rudimentary clipping (drawChar w/big text requires this)
if((x >= ILI9488_TFTWIDTH) || (y >= ILI9488_TFTHEIGHT)) return;
if((x + w - 1) >= ILI9488_TFTWIDTH)  w = ILI9488_TFTWIDTH  - x;
if((y + h - 1) >= ILI9488_TFTHEIGHT) h = ILI9488_TFTHEIGHT - y;

setAddrWindow(x, y, x+w-1, y+h-1);

uint8_t linebuff[w*3+1];
//uint16_t pixels = w*h;
uint32_t count = 0;
for (uint16_t i = 0; i < h; i++)
{
uint16_t pixcount = 0;
for (uint16_t o = 0; o <  w; o++)
{
uint16_t b1 = img[count];
count++;
uint16_t color = b1;

linebuff[pixcount] = (((color & 0xF800) >> 11)* 255) / 31;
pixcount++;
linebuff[pixcount] = (((color & 0x07E0) >> 5) * 255) / 63;
pixcount++;
linebuff[pixcount] = ((color & 0x001F)* 255) / 31;
pixcount++;
} // for row
for(uint16_t b = 0; b < w*3; b++)
{
writedata(linebuff[b]);
}
}
}



  void pushColor(uint16_t color)
  {
	write16BitColor(color);
  }

#if 0
  void pushColors(uint16_t *data, uint8_t len, boolean first){
	uint16_t color;
	uint8_t  buff[len*3+1];
	uint16_t count = 0;
	uint8_t lencount = len;

	while(lencount--)
	{
	  color = *data++;
	  buff[count] = (((color & 0xF800) >> 11)* 255) / 31;
	  count++;
	  buff[count] = (((color & 0x07E0) >> 5) * 255) / 63;
	  count++;
	  buff[count] = ((color & 0x001F)* 255) / 31;
	  count++;
	}
  }
#endif
	     	      void write16BitColor(uint16_t color)
	     	      {
	     	        uint8_t r = (color & 0xF800) >> 11;
	     	        uint8_t g = (color & 0x07E0) >> 5;
	     	        uint8_t b = color & 0x001F;

	     	        r = (r * 255) / 31;
	     	        g = (g * 255) / 63;
	     	        b = (b * 255) / 31;

	     	        writedata(r);
	     	        writedata(g);
	     	        writedata(b);
	     	      }

	     	      void drawPixel(int16_t x, int16_t y, uint16_t color)
	     	      {
	     	        if((x < 0) ||(x >= ILI9488_TFTWIDTH) || (y < 0) || (y >= ILI9488_TFTHEIGHT)) return;
	     	        setAddrWindow(x,y,x+1,y+1);
	     	        write16BitColor(color);
	     	      }


	     	      void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
	     	      {
	     	        // Rudimentary clipping
	     	        if((x >= ILI9488_TFTWIDTH) || (y >= ILI9488_TFTHEIGHT)) return;

	     	        if((y+h-1) >= ILI9488_TFTHEIGHT)
	     	          h = ILI9488_TFTHEIGHT-y;


	     	        setAddrWindow(x, y, x, y+h-1);
	     	        while (h--)
	     	        {
	     	          write16BitColor(color);
	     	        }
	     	      }


	     	      void drawFastHLine(int16_t x, int16_t y, int16_t w,uint16_t color)
	     	      {
	     	        // Rudimentary clipping
	     	        if((x >= ILI9488_TFTWIDTH) || (y >= ILI9488_TFTHEIGHT)) return;
	     	        if((x+w-1) >= ILI9488_TFTWIDTH)  w = ILI9488_TFTWIDTH-x;
	     	        setAddrWindow(x, y, x+w-1, y);
	     	        while (w--)
	     	        {

	     	          write16BitColor(color);
	     	        }

	     	      }

	     	      void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	     	        if((x + w - 1) >= ILI9488_TFTWIDTH)  w = ILI9488_TFTWIDTH  - x;
	     	        if((y + h - 1) >= ILI9488_TFTHEIGHT) h = ILI9488_TFTHEIGHT - y;


	     	        setAddrWindow(x, y, x+w-1, y+h-1);
	     	        for(y=h; y>0; y--)
	     	        {
	     	          for(x=w; x>0; x--)
	     	          {
	     	            write16BitColor(color);
	     	          }
	     	        }
	     	      }


	     	      // Pass 8-bit (each) R,G,B, get back 16-bit packed color
	     	      uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
	     	      {
	     	        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	     	      }

	     	      #define MADCTL_MY  0x80
	     	      #define MADCTL_MX  0x40
	     	      #define MADCTL_MV  0x20
	     	      #define MADCTL_ML  0x10
	     	      #define MADCTL_RGB 0x00
	     	      #define MADCTL_BGR 0x08
	     	      #define MADCTL_MH  0x04

	     	      void setRotation(uint8_t m)
	     	      {
	     	        writecommand(ILI9488_MADCTL);
	     	        uint8_t rotation = m % 4; // can't be higher than 3
	     	        switch (rotation)
	     	        {
	     	         case 0:
	     	           writedata(MADCTL_MX | MADCTL_BGR);
	     	      //     ILI9488_TFTWIDTH  = ILI9488_TFTWIDTH;
	     	      //     ILI9488_TFTHEIGHT = ILI9488_TFTHEIGHT;
	     	           break;
	     	         case 1:
	     	           writedata(MADCTL_MV | MADCTL_BGR);
	     	      //     ILI9488_TFTWIDTH  = ILI9488_TFTHEIGHT;
	     	      //     ILI9488_TFTHEIGHT = ILI9488_TFTWIDTH;
	     	           break;
	     	        case 2:
	     	          writedata(MADCTL_MY | MADCTL_BGR);
	     	      //     ILI9488_TFTWIDTH  = ILI9488_TFTWIDTH;
	     	      //     ILI9488_TFTHEIGHT = ILI9488_TFTHEIGHT;
	     	          break;
	     	         case 3:
	     	           writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
	     //	           ILI9488_TFTWIDTH  = ILI9488_TFTHEIGHT;
	     //	           ILI9488_TFTHEIGHT = ILI9488_TFTWIDTH;
	     	           break;
	     	        }

	     	      }


	     	      void invertDisplay(boolean i)
	     	      {
	     	        writecommand(i);
	     	      }


	     	     // void init_9341()
	     	     //  {
	     	     //
	     	     //	 // This is the command sequence that initialises the ILI9341 driver
	     	     //	 //
	     	     //	 // This setup information uses simple 8 bit SPI writecommand() and writedata() functions
	     	     //	 //
	     	     //	 // See ST7735_Setup.h file for an alternative format
	     	     //
	     	     //	 {
	     	     //	   writecommand(0xEF);
	     	     //	   writedata(0x03);
	     	     //	   writedata(0x80);
	     	     //	   writedata(0x02);
	     	     //
	     	     //	   writecommand(0xCF);
	     	     //	   writedata(0x00);
	     	     //	   writedata(0XC1);
	     	     //	   writedata(0X30);
	     	     //
	     	     //	   writecommand(0xED);
	     	     //	   writedata(0x64);
	     	     //	   writedata(0x03);
	     	     //	   writedata(0X12);
	     	     //	   writedata(0X81);
	     	     //
	     	     //	   writecommand(0xE8);
	     	     //	   writedata(0x85);
	     	     //	   writedata(0x00);
	     	     //	   writedata(0x78);
	     	     //
	     	     //	   writecommand(0xCB);
	     	     //	   writedata(0x39);
	     	     //	   writedata(0x2C);
	     	     //	   writedata(0x00);
	     	     //	   writedata(0x34);
	     	     //	   writedata(0x02);
	     	     //
	     	     //	   writecommand(0xF7);
	     	     //	   writedata(0x20);
	     	     //
	     	     //	   writecommand(0xEA);
	     	     //	   writedata(0x00);
	     	     //	   writedata(0x00);
	     	     //
	     	     //	   writecommand(ILI9341_PWCTR1);    //Power control
	     	     //	   writedata(0x23);   //VRH[5:0]
	     	     //
	     	     //	   writecommand(ILI9341_PWCTR2);    //Power control
	     	     //	   writedata(0x10);   //SAP[2:0];BT[3:0]
	     	     //
	     	     //	   writecommand(ILI9341_VMCTR1);    //VCM control
	     	     //	   writedata(0x3e);
	     	     //	   writedata(0x28);
	     	     //
	     	     //	   writecommand(ILI9341_VMCTR2);    //VCM control2
	     	     //	   writedata(0x86);  //--
	     	     //
	     	     //	   writecommand(ILI9341_MADCTL);    // Memory Access Control
	     	     ////	 #ifdef M5STACK
	     	     ////	   writedata(0xA8); // Rotation 0 (portrait mode)
	     	     ////	 #else
	     	     ////	   writedata(0x48); // Rotation 0 (portrait mode)
	     	     ////	 #endif
	     	     //
	     	     //	   writecommand(ILI9341_PIXFMT);
	     	     //	   writedata(0x55);
	     	     //
	     	     //	   writecommand(ILI9341_FRMCTR1);
	     	     //	   writedata(0x00);
	     	     //	   writedata(0x13); // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
	     	     //
	     	     //	   writecommand(ILI9341_DFUNCTR);    // Display Function Control
	     	     //	   writedata(0x08);
	     	     //	   writedata(0x82);
	     	     //	   writedata(0x27);
	     	     //
	     	     //	   writecommand(0xF2);    // 3Gamma Function Disable
	     	     //	   writedata(0x00);
	     	     //
	     	     //	   writecommand(ILI9341_GAMMASET);    //Gamma curve selected
	     	     //	   writedata(0x01);
	     	     //
	     	     //	   writecommand(ILI9341_GMCTRP1);    //Set Gamma
	     	     //	   writedata(0x0F);
	     	     //	   writedata(0x31);
	     	     //	   writedata(0x2B);
	     	     //	   writedata(0x0C);
	     	     //	   writedata(0x0E);
	     	     //	   writedata(0x08);
	     	     //	   writedata(0x4E);
	     	     //	   writedata(0xF1);
	     	     //	   writedata(0x37);
	     	     //	   writedata(0x07);
	     	     //	   writedata(0x10);
	     	     //	   writedata(0x03);
	     	     //	   writedata(0x0E);
	     	     //	   writedata(0x09);
	     	     //	   writedata(0x00);
	     	     //
	     	     //	   writecommand(ILI9341_GMCTRN1);    //Set Gamma
	     	     //	   writedata(0x00);
	     	     //	   writedata(0x0E);
	     	     //	   writedata(0x14);
	     	     //	   writedata(0x03);
	     	     //	   writedata(0x11);
	     	     //	   writedata(0x07);
	     	     //	   writedata(0x31);
	     	     //	   writedata(0xC1);
	     	     //	   writedata(0x48);
	     	     //	   writedata(0x08);
	     	     //	   writedata(0x0F);
	     	     //	   writedata(0x0C);
	     	     //	   writedata(0x31);
	     	     //	   writedata(0x36);
	     	     //	   writedata(0x0F);
	     	     //
	     	     //	   writecommand(ILI9341_SLPOUT);    //Exit Sleep
	     	     //
	     	     //
	     	     //	   HAL_Delay(120);
	     	     //
	     	     //
	     	     //	   writecommand(ILI9341_DISPON);    //Display on
	     	     //
	     	     //	 }
	     	     //  }
	     	     // void init_9486()
	     	     // {
	     	     //	 writecommand(0x01);
	     	     //	     writedata(0x00);
	     	     //	     HAL_Delay(50);
	     	     //
	     	     //	     writecommand(0x28);
	     	     //	     writedata(0x00);
	     	     //
	     	     //	     writecommand(0xC0);        // Power Control 1
	     	     //	     writedata(0x0d);
	     	     //	     writedata(0x0d);
	     	     //
	     	     //	     writecommand(0xC1);        // Power Control 2
	     	     //	     writedata(0x43);
	     	     //	     writedata(0x00);
	     	     //
	     	     //	     writecommand(0xC2);        // Power Control 3
	     	     //	     writedata(0x00);
	     	     //
	     	     //	     writecommand(0xC5);        // VCOM Control
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x48);
	     	     //
	     	     //	     writecommand(0xB6);        // Display Function Control
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x22);           // 0x42 = Rotate display 180 deg.
	     	     //	     writedata(0x3B);
	     	     //
	     	     //	     writecommand(0xE0);        // PGAMCTRL (Positive Gamma Control)
	     	     //	     writedata(0x0f);
	     	     //	     writedata(0x24);
	     	     //	     writedata(0x1c);
	     	     //	     writedata(0x0a);
	     	     //	     writedata(0x0f);
	     	     //	     writedata(0x08);
	     	     //	     writedata(0x43);
	     	     //	     writedata(0x88);
	     	     //	     writedata(0x32);
	     	     //	     writedata(0x0f);
	     	     //	     writedata(0x10);
	     	     //	     writedata(0x06);
	     	     //	     writedata(0x0f);
	     	     //	     writedata(0x07);
	     	     //	     writedata(0x00);
	     	     //
	     	     //	     writecommand(0xE1);        // NGAMCTRL (Negative Gamma Control)
	     	     //	     writedata(0x0F);
	     	     //	     writedata(0x38);
	     	     //	     writedata(0x30);
	     	     //	     writedata(0x09);
	     	     //	     writedata(0x0f);
	     	     //	     writedata(0x0f);
	     	     //	     writedata(0x4e);
	     	     //	     writedata(0x77);
	     	     //	     writedata(0x3c);
	     	     //	     writedata(0x07);
	     	     //	     writedata(0x10);
	     	     //	     writedata(0x05);
	     	     //	     writedata(0x23);
	     	     //	     writedata(0x1b);
	     	     //	     writedata(0x00);
	     	     //
	     	     //	     writecommand(0x20);        // Display Inversion OFF, 0x21 = ON
	     	     //
	     	     //	     writecommand(0x36);        // Memory Access Control
	     	     //	     writedata(0x0A);
	     	     //
	     	     //	     writecommand(0x3A);        // Interface Pixel Format
	     	     //	     writedata(0x55);
	     	     //
	     	     //	     writecommand(0x11);
	     	     //
	     	     //	     HAL_Delay(150);
	     	     //
	     	     //	     writecommand(0x29);
	     	     //	     HAL_Delay(25);
	     	     // }
	     	     //
	     	     // void init_HX8357C()
	     	     // {
	     	     //	 writecommand(0xB9); // Enable extension command
	     	     //	     writedata(0xFF);
	     	     //	     writedata(0x83);
	     	     //	     writedata(0x57);
	     	     //	     HAL_Delay(50);
	     	     //
	     	     //	     writecommand(0xB6); //Set VCOM voltage
	     	     //	     writedata(0x2C);    //0x52 for HSD 3.0"
	     	     //
	     	     //	     writecommand(0x11); // Sleep off
	     	     //	     HAL_Delay(200);
	     	     //
	     	     //	     writecommand(0x35); // Tearing effect on
	     	     //	     writedata(0x00);    // Added parameter
	     	     //
	     	     //	     writecommand(0x3A); // Interface pixel format
	     	     //	     writedata(0x55);    // 16 bits per pixel
	     	     //
	     	     //	     //writecommand(0xCC); // Set panel characteristic
	     	     //	     //writedata(0x09);    // S960>S1, G1>G480, R-G-B, normally black
	     	     //
	     	     //	     //writecommand(0xB3); // RGB interface
	     	     //	     //writedata(0x43);
	     	     //	     //writedata(0x00);
	     	     //	     //writedata(0x06);
	     	     //	     //writedata(0x06);
	     	     //
	     	     //	     writecommand(0xB1); // Power control
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x15);
	     	     //	     writedata(0x0D);
	     	     //	     writedata(0x0D);
	     	     //	     writedata(0x83);
	     	     //	     writedata(0x48);
	     	     //
	     	     //
	     	     //	     writecommand(0xC0); // Does this do anything?
	     	     //	     writedata(0x24);
	     	     //	     writedata(0x24);
	     	     //	     writedata(0x01);
	     	     //	     writedata(0x3C);
	     	     //	     writedata(0xC8);
	     	     //	     writedata(0x08);
	     	     //
	     	     //	     writecommand(0xB4); // Display cycle
	     	     //	     writedata(0x02);
	     	     //	     writedata(0x40);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x2A);
	     	     //	     writedata(0x2A);
	     	     //	     writedata(0x0D);
	     	     //	     writedata(0x4F);
	     	     //
	     	     //	     writecommand(0xE0); // Gamma curve
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x15);
	     	     //	     writedata(0x1D);
	     	     //	     writedata(0x2A);
	     	     //	     writedata(0x31);
	     	     //	     writedata(0x42);
	     	     //	     writedata(0x4C);
	     	     //	     writedata(0x53);
	     	     //	     writedata(0x45);
	     	     //	     writedata(0x40);
	     	     //	     writedata(0x3B);
	     	     //	     writedata(0x32);
	     	     //	     writedata(0x2E);
	     	     //	     writedata(0x28);
	     	     //
	     	     //	     writedata(0x24);
	     	     //	     writedata(0x03);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x15);
	     	     //	     writedata(0x1D);
	     	     //	     writedata(0x2A);
	     	     //	     writedata(0x31);
	     	     //	     writedata(0x42);
	     	     //	     writedata(0x4C);
	     	     //	     writedata(0x53);
	     	     //	     writedata(0x45);
	     	     //	     writedata(0x40);
	     	     //	     writedata(0x3B);
	     	     //	     writedata(0x32);
	     	     //
	     	     //	     writedata(0x2E);
	     	     //	     writedata(0x28);
	     	     //	     writedata(0x24);
	     	     //	     writedata(0x03);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x01);
	     	     //
	     	     //	     writecommand(0x36); // MADCTL Memory access control
	     	     //	     writedata(0x48);
	     	     //	     HAL_Delay(20);
	     	     //
	     	     //	     writecommand(0x21); //Display inversion on
	     	     //	     HAL_Delay(20);
	     	     //
	     	     //	     writecommand(0x29); // Display on
	     	     //
	     	     //	     HAL_Delay(120);
	     	     // }
	     	     //
	     	     //
	     	     // void init_HX8357B()
	     	     // {
	     	     //	 writecommand(0x11);
	     	     //	 HAL_Delay(20);
	     	     //	     writecommand(0xD0);
	     	     //	     writedata(0x07);
	     	     //	     writedata(0x42);
	     	     //	     writedata(0x18);
	     	     //
	     	     //	     writecommand(0xD1);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x07);
	     	     //	     writedata(0x10);
	     	     //
	     	     //	     writecommand(0xD2);
	     	     //	     writedata(0x01);
	     	     //	     writedata(0x02);
	     	     //
	     	     //	     writecommand(0xC0);
	     	     //	     writedata(0x10);
	     	     //	     writedata(0x3B);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x02);
	     	     //	     writedata(0x11);
	     	     //
	     	     //	     writecommand(0xC5);
	     	     //	     writedata(0x08);
	     	     //
	     	     //	     writecommand(0xC8);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x32);
	     	     //	     writedata(0x36);
	     	     //	     writedata(0x45);
	     	     //	     writedata(0x06);
	     	     //	     writedata(0x16);
	     	     //	     writedata(0x37);
	     	     //	     writedata(0x75);
	     	     //	     writedata(0x77);
	     	     //	     writedata(0x54);
	     	     //	     writedata(0x0C);
	     	     //	     writedata(0x00);
	     	     //
	     	     //	     writecommand(0x36);
	     	     //	     writedata(0x0a);
	     	     //
	     	     //	     writecommand(0x3A);
	     	     //	     writedata(0x55);
	     	     //
	     	     //	     writecommand(0x2A);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x01);
	     	     //	     writedata(0x3F);
	     	     //
	     	     //	     writecommand(0x2B);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x01);
	     	     //	     writedata(0xDF);
	     	     //
	     	     //	     HAL_Delay(120);
	     	     //	     writecommand(0x29);
	     	     //
	     	     //	     HAL_Delay(25);
	     	     // }
	     	     //
	     	     // void init_9481()
	     	     // {
	     	     //
	     	     //	 // This is the command sequence that initialises the ILI9481 driver
	     	     //	 //
	     	     //	 // This setup information uses simple 8 bit SPI writecommand() and writedata() functions
	     	     //	 //
	     	     //	 // See ST7735_Setup.h file for an alternative format
	     	     //
	     	     //
	     	     //	 // Configure ILI9481 display
	     	     //
	     	     //	     writecommand(TFT_SLPOUT);
	     	     //	     HAL_Delay(20);
	     	     //
	     	     //	     writecommand(0xD0);
	     	     //	     writedata(0x07);
	     	     //	     writedata(0x42);
	     	     //	     writedata(0x18);
	     	     //
	     	     //	     writecommand(0xD1);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x07);
	     	     //	     writedata(0x10);
	     	     //
	     	     //	     writecommand(0xD2);
	     	     //	     writedata(0x01);
	     	     //	     writedata(0x02);
	     	     //
	     	     //	     writecommand(0xC0);
	     	     //	     writedata(0x10);
	     	     //	     writedata(0x3B);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x02);
	     	     //	     writedata(0x11);
	     	     //
	     	     //	     writecommand(0xC5);
	     	     //	     writedata(0x03);
	     	     //
	     	     //	     writecommand(0xC8);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x32);
	     	     //	     writedata(0x36);
	     	     //	     writedata(0x45);
	     	     //	     writedata(0x06);
	     	     //	     writedata(0x16);
	     	     //	     writedata(0x37);
	     	     //	     writedata(0x75);
	     	     //	     writedata(0x77);
	     	     //	     writedata(0x54);
	     	     //	     writedata(0x0C);
	     	     //	     writedata(0x00);
	     	     //
	     	     //	     writecommand(TFT_MADCTL);
	     	     //	     writedata(0x0A);
	     	     //
	     	     //	     writecommand(0x3A);
	     	     //	     writedata(0x55);
	     	     //
	     	     //	     writecommand(TFT_CASET);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x01);
	     	     //	     writedata(0x3F);
	     	     //
	     	     //	     writecommand(TFT_PASET);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x00);
	     	     //	     writedata(0x01);
	     	     //	     writedata(0xDF);
	     	     //
	     	     //	     HAL_Delay(120);
	     	     //	     writecommand(TFT_DISPON);
	     	     //
	     	     //	     HAL_Delay(25);
	     	     //	 // End of ILI9481 display configuration
	     	     //
	     	     //
	     	     //
	     	     //
	     	     // }
	     	     //
	     	     //
	     	     //
	     	     //void init_HX8357D()
	     	     //{
	     	     //
	     	     //	// This is the command sequence that initialises the HX8357D driver
	     	     //	//
	     	     //	// This setup information uses simple 8 bit SPI writecommand() and writedata() functions
	     	     //	//
	     	     //	// See ST7735_Setup.h file for an alternative format
	     	     //
	     	     //
	     	     //	// Configure HX8357D display
	     	     //
	     	     //	    // setextc
	     	     //	    writecommand(HX8357D_SETC);
	     	     //	    writedata(0xFF);
	     	     //	    writedata(0x83);
	     	     //	    writedata(0x57);
	     	     //	    HAL_Delay(300);
	     	     //
	     	     //	    // setRGB which also enables SDO
	     	     //	    writecommand(HX8357_SETRGB);
	     	     //	    writedata(0x80);  //enable SDO pin!
	     	     //	//  writedata(0x00);  //disable SDO pin!
	     	     //	    writedata(0x0);
	     	     //	    writedata(0x06);
	     	     //	    writedata(0x06);
	     	     //
	     	     //	    writecommand(HX8357D_SETCOM);
	     	     //	    writedata(0x25);  // -1.52V
	     	     //
	     	     //	    writecommand(HX8357_SETOSC);
	     	     //	    writedata(0x68);  // Normal mode 70Hz, Idle mode 55 Hz
	     	     //
	     	     //	    writecommand(HX8357_SETPANEL); //Set Panel
	     	     //	    writedata(0x05);  // BGR, Gate direction swapped
	     	     //
	     	     //	    writecommand(HX8357_SETPWR1);
	     	     //	    writedata(0x00);  // Not deep standby
	     	     //	    writedata(0x15);  //BT
	     	     //	    writedata(0x1C);  //VSPR
	     	     //	    writedata(0x1C);  //VSNR
	     	     //	    writedata(0x83);  //AP
	     	     //	    writedata(0xAA);  //FS
	     	     //
	     	     //	    writecommand(HX8357D_SETSTBA);
	     	     //	    writedata(0x50);  //OPON normal
	     	     //	    writedata(0x50);  //OPON idle
	     	     //	    writedata(0x01);  //STBA
	     	     //	    writedata(0x3C);  //STBA
	     	     //	    writedata(0x1E);  //STBA
	     	     //	    writedata(0x08);  //GEN
	     	     //
	     	     //	    writecommand(HX8357D_SETCYC);
	     	     //	    writedata(0x02);  //NW 0x02
	     	     //	    writedata(0x40);  //RTN
	     	     //	    writedata(0x00);  //DIV
	     	     //	    writedata(0x2A);  //DUM
	     	     //	    writedata(0x2A);  //DUM
	     	     //	    writedata(0x0D);  //GDON
	     	     //	    writedata(0x78);  //GDOFF
	     	     //
	     	     //	    writecommand(HX8357D_SETGAMMA);
	     	     //	    writedata(0x02);
	     	     //	    writedata(0x0A);
	     	     //	    writedata(0x11);
	     	     //	    writedata(0x1d);
	     	     //	    writedata(0x23);
	     	     //	    writedata(0x35);
	     	     //	    writedata(0x41);
	     	     //	    writedata(0x4b);
	     	     //	    writedata(0x4b);
	     	     //	    writedata(0x42);
	     	     //	    writedata(0x3A);
	     	     //	    writedata(0x27);
	     	     //	    writedata(0x1B);
	     	     //	    writedata(0x08);
	     	     //	    writedata(0x09);
	     	     //	    writedata(0x03);
	     	     //	    writedata(0x02);
	     	     //	    writedata(0x0A);
	     	     //	    writedata(0x11);
	     	     //	    writedata(0x1d);
	     	     //	    writedata(0x23);
	     	     //	    writedata(0x35);
	     	     //	    writedata(0x41);
	     	     //	    writedata(0x4b);
	     	     //	    writedata(0x4b);
	     	     //	    writedata(0x42);
	     	     //	    writedata(0x3A);
	     	     //	    writedata(0x27);
	     	     //	    writedata(0x1B);
	     	     //	    writedata(0x08);
	     	     //	    writedata(0x09);
	     	     //	    writedata(0x03);
	     	     //	    writedata(0x00);
	     	     //	    writedata(0x01);
	     	     //
	     	     //	    writecommand(HX8357_COLMOD);
	     	     //	    writedata(0x55);  // 16 bit
	     	     //
	     	     //	    writecommand(HX8357_MADCTL);
	     	     //	    writedata(0xC0);
	     	     //
	     	     //	    writecommand(HX8357_TEON);  // TE off
	     	     //	    writedata(0x00);
	     	     //
	     	     //	    writecommand(HX8357_TEARLINE);  // tear line
	     	     //	    writedata(0x00);
	     	     //	    writedata(0x02);
	     	     //
	     	     //	    writecommand(HX8357_SLPOUT);  //Exit Sleep
	     	     //	    HAL_Delay(150);
	     	     //
	     	     //	    writecommand(HX8357_DISPON);  // display on
	     	     //	    HAL_Delay(50);
	     	     //
	     	     //	// End of HX8357D display configuration
	     	     //
	     	     //}
	     	     //void DWT_Init(void)
	     	     //{
	     	     //        //разрешаем использовать счётчик
	     	     //        SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	     	     //         //обнуляем значение счётного регистра
	     	     //	DWT_CYCCNT  = 0;
	     	     //         //запускаем счётчик
	     	     //	DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;
	     	     //}
	     	     //
	     	     //static __inline uint32_t delta(uint32_t t0, uint32_t t1)
	     	     //{
	     	     //    return (t1 - t0);
	     	     //}
	     	     //void delay_us(uint32_t us)
	     	     //{
	     	     //      uint32_t t0 =  DWT->CYCCNT;
	     	     //      uint32_t us_count_tic =  us * (SystemCoreClock/1000000);
	     	     //      while (delta(t0, DWT->CYCCNT) < us_count_tic) ;
	     	     //}
