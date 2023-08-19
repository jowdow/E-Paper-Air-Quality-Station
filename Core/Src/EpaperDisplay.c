#include "EpaperDisplay.h"
#include "EPD_4in2.h"
#include <string.h>



void love(uint8_t dataToDisplay_u8){
    if(DEV_Module_Init()!=0){ // JD - Setting certain pins high or low

    }

    EPD_4IN2_Init(); // JD - Sending commands to setup display
    EPD_4IN2_Clear(); // JD - Sending commands to clear the display
    DEV_Delay_ms(2000);

	//Create a new image cache
    uint8_t *BlackImage;
	//you have to edit the startup_stm32fxxx.s file and set a big enough heap size
	uint16_t Imagesize = ((EPD_4IN2_WIDTH % 8 == 0)? (EPD_4IN2_WIDTH / 8 ): (EPD_4IN2_WIDTH / 8 + 1)) * EPD_4IN2_HEIGHT;
	if((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
		//printf("Failed to apply for black memory...\r\n");

	}
	//printf("Paint_NewImage\r\n");
	Paint_NewImage(BlackImage, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, ROTATE_270, WHITE);

	// show image for array
	//printf("show image for array\r\n");
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);















	Paint_DrawString_EN(0, 0, "Temperature", &Font20, WHITE, BLACK);
	Paint_DrawString_EN(150, 0, ":", &Font20,  WHITE, BLACK);
	Paint_DrawString_EN(162, 0, "70F", &Font20,  WHITE, BLACK); // TODO Make into real data

	Paint_DrawString_EN(42, 30, "Humidity", &Font20, WHITE, BLACK);
	Paint_DrawString_EN(150, 30, ":", &Font20,  WHITE, BLACK);
	Paint_DrawString_EN(162, 30, "80%", &Font20,  WHITE, BLACK); // TODO Make into real data

	Paint_DrawString_EN(42, 60, "Pressure", &Font20, WHITE, BLACK);
	Paint_DrawString_EN(150, 60, ":", &Font20,  WHITE, BLACK);
	Paint_DrawString_EN(162, 60, "1000 hPa", &Font20,  WHITE, BLACK); // TODO Make into real data

	Paint_DrawString_EN(113, 90, "Co2", &Font20,  WHITE, BLACK);
	Paint_DrawString_EN(150, 90, ":", &Font20,  WHITE, BLACK);
	Paint_DrawString_EN(162, 90, "680 ppm", &Font20,  WHITE, BLACK); // TODO Make into real data




	// JD - Main Graph Lines
	Paint_DrawLine(30, 370, 30, 200, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);// X
	Paint_DrawLine(30, 370, 290, 370, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);// Y

	// JD - X Axis lines
	Paint_DrawLine(30, 200, 35, 200, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(30, 230, 35, 230, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(30, 260, 35, 260, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(30, 290, 35, 290, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(30, 320, 35, 320, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(30, 350, 35, 350, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);

	// JD - X Axis Values
//	Paint_DrawString_EN(0, 60, ":", &Font20,  WHITE, BLACK);
//	Paint_DrawString_EN(0, 60, ":", &Font20,  WHITE, BLACK);
//	Paint_DrawString_EN(0, 60, ":", &Font20,  WHITE, BLACK);
//	Paint_DrawString_EN(0, 60, ":", &Font20,  WHITE, BLACK);
//	Paint_DrawString_EN(0, 60, ":", &Font20,  WHITE, BLACK);
//	Paint_DrawString_EN(0, 60, ":", &Font20,  WHITE, BLACK);





	// JD - Y Axis lines
	Paint_DrawLine(50, 370, 50, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(80, 370, 80, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(110, 370, 110, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(140, 370, 140, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(170, 370, 170, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(200, 370, 200, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(230, 370, 230, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(260, 370, 260, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
	Paint_DrawLine(290, 370, 290, 365, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);

	// JD - Y Axis Values



	// JD - Graph Title
	switch(dataToDisplay_u8){
	case 0:
		Paint_DrawString_EN(60, 170, "Temperature", &Font24,  BLACK, WHITE);
		Paint_DrawString_EN(10, 195, "40", &Font12,  WHITE, BLACK);
		Paint_DrawString_EN(10, 225, "35", &Font12,  WHITE, BLACK);
		Paint_DrawString_EN(10, 255, "30", &Font12,  WHITE, BLACK);
		Paint_DrawString_EN(10, 285, "25", &Font12,  WHITE, BLACK);
		Paint_DrawString_EN(10, 315, "20", &Font12,  WHITE, BLACK);
		Paint_DrawString_EN(10, 345, "15", &Font12,  WHITE, BLACK);
		break;
	case 1:
		Paint_DrawString_EN(85, 170, "Humidity", &Font24,  BLACK, WHITE);
		break;
	case 2:
		Paint_DrawString_EN(85, 170, "Pressure", &Font24,  BLACK, WHITE);
		break;
	case 3:
		Paint_DrawString_EN(125, 170, "C02", &Font24,  BLACK, WHITE);
		break;
	default:
		Paint_DrawString_EN(40, 170, "Error display #", &Font24,  BLACK, WHITE);
		break;
	}
























	EPD_4IN2_Display(BlackImage); // JD - Sending the image and commands to display the image
	DEV_Delay_ms(5000);

	//EPD_4IN2_Clear(); // JD - Sending commands to clear the display
	free(BlackImage); // JD - "Freeing" the memory used for the image. Look into just keeping it in the heap
	EPD_4IN2_Sleep(); // JD - Sending the commands to turn off the display
	DEV_Module_Exit(); // JD - Setting the pin to low and shutting off 5v to the module
}

void displayData(uint8_t dataArray){
    if(DEV_Module_Init()!=0){ // JD - Setting certain pins high or low

    }

    EPD_4IN2_Init(); // JD - Sending commands to setup display
    EPD_4IN2_Clear(); // JD - Sending commands to clear the display
    DEV_Delay_ms(500);

	//Create a new image cache
    uint8_t *BlackImage;
	//you have to edit the startup_stm32fxxx.s file and set a big enough heap size
	uint16_t Imagesize = ((EPD_4IN2_WIDTH % 8 == 0)? (EPD_4IN2_WIDTH / 8 ): (EPD_4IN2_WIDTH / 8 + 1)) * EPD_4IN2_HEIGHT;
	if((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
		//printf("Failed to apply for black memory...\r\n");

	}
	//printf("Paint_NewImage\r\n");
	Paint_NewImage(BlackImage, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, ROTATE_270, WHITE);

	// show image for array
	//printf("show image for array\r\n");
	Paint_SelectImage(BlackImage);
	Paint_Clear(WHITE);

	Paint_DrawNum(0, 0, dataArray, &Font20, WHITE, BLACK);
	Paint_DrawNum(0, 100, 55, &Font20, WHITE, BLACK);

	EPD_4IN2_Display(BlackImage); // JD - Sending the image and commands to display the image
	//DEV_Delay_ms(2000);

	//EPD_4IN2_Clear(); // JD - Sending commands to clear the display
	free(BlackImage); // JD - "Freeing" the memory used for the image. Look into just keeping it in the heap
	EPD_4IN2_Sleep(); // JD - Sending the commands to turn off the display
	DEV_Module_Exit(); // JD - Setting the pin to low and shutting off 5v to the module

}
