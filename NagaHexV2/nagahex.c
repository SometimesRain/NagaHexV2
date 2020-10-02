#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <hidapi.h>

//Run program without cmd window
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")

//########################################### MACROS ###########################################
#define SHIFT(value, bits) (value >> bits) & 0xff
#define POINT(value) SHIFT(value, 8), SHIFT(value, 0)
#define RGB(value) SHIFT(value, 16), SHIFT(value, 8), SHIFT(value, 0)
#define ALL(macro, value) macro(LED_SCROLL, value) | macro(LED_LOGO, value) | macro(LED_THUMB, value)

#define LED_BRIGHTNESS(led, value) set(0x3, 0x3, 3, 0x1, led, value)
#define LED_COLOR(led, value) set(0x3, 0xd, 6, 0x1, led, EFF_COLOR, RGB(value))
#define LED_SPECTRUM(led, value) set(0x3, 0xd, 3, 0x1, led, EFF_SPECTRUM)
#define LED_REACTIVE(led, value) set(0x3, 0xd, 7, 0x1, led, EFF_REACTIVE, 0, RGB(value))
#define LED_BREATH(led, value) set(0x3, 0xd, 7, 0x1, led, EFF_BREATH, 1, RGB(value))

#define SET_SENSITIVITY(value) set(0x4, 0x5, 5, 0x1, POINT(value), POINT(value))
#define SET_POLLRATE(value) set(0x5, 0x1, 2, 0x1, value)
#define SET_BRIGHTNESS(value) ALL(LED_BRIGHTNESS, value)
#define SET_COLOR(value) ALL(LED_COLOR, value)
#define SET_SPECTRUM(value) ALL(LED_SPECTRUM, value)
#define SET_REACTIVE(value) ALL(LED_REACTIVE, value)
#define SET_BREATH(value) ALL(LED_BREATH, value)

//########################################## CONSTANTS #########################################
enum
{
	LED_SCROLL = 0x01,
	LED_LOGO = 0x04,
	LED_THUMB = 0x05,
};

enum
{
	EFF_REACTIVE = 0x02,
	EFF_BREATH = 0x03,
	EFF_SPECTRUM = 0x04,
	EFF_COLOR = 0x06,
};

//##################################### GLOBAL DEFINITIONS #####################################
typedef struct {
	char _0; //unused
	char _1; //unused
	char transaction;
	char _3; //unused
	char _4; //unused
	char _5; //unused
	char size;
	char category;
	char type;
	char data[80];
	char crc;
	char _90; //unused
} BUFFER;

hid_device* device = NULL;

//########################################## FUNCTIONS #########################################
int set(char category, char type, char size, ...)
{
	//===================== Create buffer and assign values =====================
	BUFFER b = { 0 };
	b.transaction = 0x3f;
	b.size = size;
	b.category = category;
	b.type = type;

	//======================= Handle variadic parameters ========================
	va_list valist;
	va_start(valist, size);
	for (size_t i = 0; i < size; i++)
	{
		b.data[i] = va_arg(valist, char);
		b.crc ^= b.data[i];
	}
	va_end(valist);

	//======================= Update CRC and send packet ========================
	b.crc ^= size ^ category ^ type;
	hid_send_feature_report(device, &b._0, sizeof(BUFFER));
	return 0;
}

int main(int argc, char** argv)
{
	//=============================== Find device ===============================
	hid_init();
	struct hid_device_info* info = hid_enumerate(0x1532, 0x0050);

	for (struct hid_device_info* iter = info; iter != NULL; iter = iter->next)
	{
		if (iter->usage_page == 1 && (iter->usage == 2 || iter->usage == 3))
		{
			if (device = hid_open_path(iter->path))
				break;
		}
	}
	hid_free_enumeration(info);

	//============================== Change options =============================
	SET_SENSITIVITY(1600);
	SET_BRIGHTNESS(64);
	SET_COLOR(0xff0000);

	//================================ Clean up =================================
	hid_exit();
	return 0;
}