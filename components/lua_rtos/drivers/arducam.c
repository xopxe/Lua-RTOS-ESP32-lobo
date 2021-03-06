/*
 * Lua-RTOS-ESP32 ArducamMini camera driver
 *
 *  Created on: 2017.02.24
 *      Author: LoBo (loboris@gmail.com, https://github.com/loboris)
 */

#include "luartos.h"

#if CONFIG_LUA_RTOS_LUA_USE_CAM

#include "arducam.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <drivers/i2c.h>

static struct CAM myCAM;

static const sensor_reg_t OV2640_QVGA[] =
{
	{0xff, 0x0},
	{0x2c, 0xff},
	{0x2e, 0xdf},
	{0xff, 0x1},
	{0x3c, 0x32},
	{0x11, 0x0},
	{0x9, 0x2},
	{0x4, 0xa8},
	{0x13, 0xe5},
	{0x14, 0x48},
	{0x2c, 0xc},
	{0x33, 0x78},
	{0x3a, 0x33},
	{0x3b, 0xfb},
	{0x3e, 0x0},
	{0x43, 0x11},
	{0x16, 0x10},
	{0x39, 0x2},
	{0x35, 0x88},

	{0x22, 0xa},
	{0x37, 0x40},
	{0x23, 0x0},
	{0x34, 0xa0},
	{0x6, 0x2},
	{0x6, 0x88},
	{0x7, 0xc0},
	{0xd, 0xb7},
	{0xe, 0x1},
	{0x4c, 0x0},
	{0x4a, 0x81},
	{0x21, 0x99},
	{0x24, 0x40},
	{0x25, 0x38},
	{0x26, 0x82},
	{0x5c, 0x0},
	{0x63, 0x0},
	{0x46, 0x22},
	{0xc, 0x3a},
	{0x5d, 0x55},
	{0x5e, 0x7d},
	{0x5f, 0x7d},
	{0x60, 0x55},
	{0x61, 0x70},
	{0x62, 0x80},
	{0x7c, 0x5},
	{0x20, 0x80},
	{0x28, 0x30},
	{0x6c, 0x0},
	{0x6d, 0x80},
	{0x6e, 0x0},
	{0x70, 0x2},
	{0x71, 0x94},
	{0x73, 0xc1},
	{0x3d, 0x34},
	{0x12, 0x4},
	{0x5a, 0x57},
	{0x4f, 0xbb},
	{0x50, 0x9c},
	{0xff, 0x0},
	{0xe5, 0x7f},
	{0xf9, 0xc0},
	{0x41, 0x24},
	{0xe0, 0x14},
	{0x76, 0xff},
	{0x33, 0xa0},
	{0x42, 0x20},
	{0x43, 0x18},
	{0x4c, 0x0},
	{0x87, 0xd0},
	{0x88, 0x3f},
	{0xd7, 0x3},
	{0xd9, 0x10},
	{0xd3, 0x82},
	{0xc8, 0x8},
	{0xc9, 0x80},
	{0x7c, 0x0},
	{0x7d, 0x0},
	{0x7c, 0x3},
	{0x7d, 0x48},
	{0x7d, 0x48},
	{0x7c, 0x8},
	{0x7d, 0x20},
	{0x7d, 0x10},
	{0x7d, 0xe},
	{0x90, 0x0},
	{0x91, 0xe},
	{0x91, 0x1a},
	{0x91, 0x31},
	{0x91, 0x5a},
	{0x91, 0x69},
	{0x91, 0x75},
	{0x91, 0x7e},
	{0x91, 0x88},
	{0x91, 0x8f},
	{0x91, 0x96},
	{0x91, 0xa3},
	{0x91, 0xaf},
	{0x91, 0xc4},
	{0x91, 0xd7},
	{0x91, 0xe8},
	{0x91, 0x20},
	{0x92, 0x0},

	{0x93, 0x6},
	{0x93, 0xe3},
	{0x93, 0x3},
	{0x93, 0x3},
	{0x93, 0x0},
	{0x93, 0x2},
	{0x93, 0x0},
	{0x93, 0x0},
	{0x93, 0x0},
	{0x93, 0x0},
	{0x93, 0x0},
	{0x93, 0x0},
	{0x93, 0x0},
	{0x96, 0x0},
	{0x97, 0x8},
	{0x97, 0x19},
	{0x97, 0x2},
	{0x97, 0xc},
	{0x97, 0x24},
	{0x97, 0x30},
	{0x97, 0x28},
	{0x97, 0x26},
	{0x97, 0x2},
	{0x97, 0x98},
	{0x97, 0x80},
	{0x97, 0x0},
	{0x97, 0x0},
	{0xa4, 0x0},
	{0xa8, 0x0},
	{0xc5, 0x11},
	{0xc6, 0x51},
	{0xbf, 0x80},
	{0xc7, 0x10},
	{0xb6, 0x66},
	{0xb8, 0xa5},
	{0xb7, 0x64},
	{0xb9, 0x7c},
	{0xb3, 0xaf},
	{0xb4, 0x97},
	{0xb5, 0xff},
	{0xb0, 0xc5},
	{0xb1, 0x94},
	{0xb2, 0xf},
	{0xc4, 0x5c},
	{0xa6, 0x0},
	{0xa7, 0x20},
	{0xa7, 0xd8},
	{0xa7, 0x1b},
	{0xa7, 0x31},
	{0xa7, 0x0},
	{0xa7, 0x18},
	{0xa7, 0x20},
	{0xa7, 0xd8},
	{0xa7, 0x19},
	{0xa7, 0x31},
	{0xa7, 0x0},
	{0xa7, 0x18},
	{0xa7, 0x20},
	{0xa7, 0xd8},
	{0xa7, 0x19},
	{0xa7, 0x31},
	{0xa7, 0x0},
	{0xa7, 0x18},
	{0x7f, 0x0},
	{0xe5, 0x1f},
	{0xe1, 0x77},
	{0xdd, 0x7f},
	{0xc2, 0xe},

	{0xff, 0x0},
	{0xe0, 0x4},
	{0xc0, 0xc8},
	{0xc1, 0x96},
	{0x86, 0x3d},
	{0x51, 0x90},
	{0x52, 0x2c},
	{0x53, 0x0},
	{0x54, 0x0},
	{0x55, 0x88},
	{0x57, 0x0},

	{0x50, 0x92},
	{0x5a, 0x50},
	{0x5b, 0x3c},
	{0x5c, 0x0},
	{0xd3, 0x4},
	{0xe0, 0x0},

	{0xff, 0x0},
	{0x5, 0x0},

	{0xda, 0x8},
	{0xd7, 0x3},
	{0xe0, 0x0},

	{0x5, 0x0},


	{0xff,0xff},
};

static const sensor_reg_t OV2640_JPEG_INIT[] =
{
  { 0xff, 0x00 },
  { 0x2c, 0xff },
  { 0x2e, 0xdf },
  { 0xff, 0x01 },
  { 0x3c, 0x32 },
  { 0x11, 0x04 },
  { 0x09, 0x02 },
  { 0x04, 0x28 },
  { 0x13, 0xe5 },
  { 0x14, 0x48 },
  { 0x2c, 0x0c },
  { 0x33, 0x78 },
  { 0x3a, 0x33 },
  { 0x3b, 0xfB },
  { 0x3e, 0x00 },
  { 0x43, 0x11 },
  { 0x16, 0x10 },
  { 0x39, 0x92 },
  { 0x35, 0xda },
  { 0x22, 0x1a },
  { 0x37, 0xc3 },
  { 0x23, 0x00 },
  { 0x34, 0xc0 },
  { 0x36, 0x1a },
  { 0x06, 0x88 },
  { 0x07, 0xc0 },
  { 0x0d, 0x87 },
  { 0x0e, 0x41 },
  { 0x4c, 0x00 },
  { 0x48, 0x00 },
  { 0x5B, 0x00 },
  { 0x42, 0x03 },
  { 0x4a, 0x81 },
  { 0x21, 0x99 },
  { 0x24, 0x40 },
  { 0x25, 0x38 },
  { 0x26, 0x82 },
  { 0x5c, 0x00 },
  { 0x63, 0x00 },
  { 0x61, 0x70 },
  { 0x62, 0x80 },
  { 0x7c, 0x05 },
  { 0x20, 0x80 },
  { 0x28, 0x30 },
  { 0x6c, 0x00 },
  { 0x6d, 0x80 },
  { 0x6e, 0x00 },
  { 0x70, 0x02 },
  { 0x71, 0x94 },
  { 0x73, 0xc1 },
  { 0x12, 0x40 },
  { 0x17, 0x11 },
  { 0x18, 0x43 },
  { 0x19, 0x00 },
  { 0x1a, 0x4b },
  { 0x32, 0x09 },
  { 0x37, 0xc0 },
  { 0x4f, 0x60 },
  { 0x50, 0xa8 },
  { 0x6d, 0x00 },
  { 0x3d, 0x38 },
  { 0x46, 0x3f },
  { 0x4f, 0x60 },
  { 0x0c, 0x3c },
  { 0xff, 0x00 },
  { 0xe5, 0x7f },
  { 0xf9, 0xc0 },
  { 0x41, 0x24 },
  { 0xe0, 0x14 },
  { 0x76, 0xff },
  { 0x33, 0xa0 },
  { 0x42, 0x20 },
  { 0x43, 0x18 },
  { 0x4c, 0x00 },
  { 0x87, 0xd5 },
  { 0x88, 0x3f },
  { 0xd7, 0x03 },
  { 0xd9, 0x10 },
  { 0xd3, 0x82 },
  { 0xc8, 0x08 },
  { 0xc9, 0x80 },
  { 0x7c, 0x00 },
  { 0x7d, 0x00 },
  { 0x7c, 0x03 },
  { 0x7d, 0x48 },
  { 0x7d, 0x48 },
  { 0x7c, 0x08 },
  { 0x7d, 0x20 },
  { 0x7d, 0x10 },
  { 0x7d, 0x0e },
  { 0x90, 0x00 },
  { 0x91, 0x0e },
  { 0x91, 0x1a },
  { 0x91, 0x31 },
  { 0x91, 0x5a },
  { 0x91, 0x69 },
  { 0x91, 0x75 },
  { 0x91, 0x7e },
  { 0x91, 0x88 },
  { 0x91, 0x8f },
  { 0x91, 0x96 },
  { 0x91, 0xa3 },
  { 0x91, 0xaf },
  { 0x91, 0xc4 },
  { 0x91, 0xd7 },
  { 0x91, 0xe8 },
  { 0x91, 0x20 },
  { 0x92, 0x00 },
  { 0x93, 0x06 },
  { 0x93, 0xe3 },
  { 0x93, 0x05 },
  { 0x93, 0x05 },
  { 0x93, 0x00 },
  { 0x93, 0x04 },
  { 0x93, 0x00 },
  { 0x93, 0x00 },
  { 0x93, 0x00 },
  { 0x93, 0x00 },
  { 0x93, 0x00 },
  { 0x93, 0x00 },
  { 0x93, 0x00 },
  { 0x96, 0x00 },
  { 0x97, 0x08 },
  { 0x97, 0x19 },
  { 0x97, 0x02 },
  { 0x97, 0x0c },
  { 0x97, 0x24 },
  { 0x97, 0x30 },
  { 0x97, 0x28 },
  { 0x97, 0x26 },
  { 0x97, 0x02 },
  { 0x97, 0x98 },
  { 0x97, 0x80 },
  { 0x97, 0x00 },
  { 0x97, 0x00 },
  { 0xc3, 0xed },
  { 0xa4, 0x00 },
  { 0xa8, 0x00 },
  { 0xc5, 0x11 },
  { 0xc6, 0x51 },
  { 0xbf, 0x80 },
  { 0xc7, 0x10 },
  { 0xb6, 0x66 },
  { 0xb8, 0xA5 },
  { 0xb7, 0x64 },
  { 0xb9, 0x7C },
  { 0xb3, 0xaf },
  { 0xb4, 0x97 },
  { 0xb5, 0xFF },
  { 0xb0, 0xC5 },
  { 0xb1, 0x94 },
  { 0xb2, 0x0f },
  { 0xc4, 0x5c },
  { 0xc0, 0x64 },
  { 0xc1, 0x4B },
  { 0x8c, 0x00 },
  { 0x86, 0x3D },
  { 0x50, 0x00 },
  { 0x51, 0xC8 },
  { 0x52, 0x96 },
  { 0x53, 0x00 },
  { 0x54, 0x00 },
  { 0x55, 0x00 },
  { 0x5a, 0xC8 },
  { 0x5b, 0x96 },
  { 0x5c, 0x00 },
  { 0xd3, 0x00 },	//{ 0xd3, 0x7f },
  { 0xc3, 0xed },
  { 0x7f, 0x00 },
  { 0xda, 0x00 },
  { 0xe5, 0x1f },
  { 0xe1, 0x67 },
  { 0xe0, 0x00 },
  { 0xdd, 0x7f },
  { 0x05, 0x00 },

  { 0x12, 0x40 },
  { 0xd3, 0x04 },	//{ 0xd3, 0x7f },
  { 0xc0, 0x16 },
  { 0xC1, 0x12 },
  { 0x8c, 0x00 },
  { 0x86, 0x3d },
  { 0x50, 0x00 },
  { 0x51, 0x2C },
  { 0x52, 0x24 },
  { 0x53, 0x00 },
  { 0x54, 0x00 },
  { 0x55, 0x00 },
  { 0x5A, 0x2c },
  { 0x5b, 0x24 },
  { 0x5c, 0x00 },
  { 0xff, 0xff },
};

static const sensor_reg_t OV2640_YUV422[] =
{
  { 0xFF, 0x00 },
  { 0x05, 0x00 },
  { 0xDA, 0x10 },
  { 0xD7, 0x03 },
  { 0xDF, 0x00 },
  { 0x33, 0x80 },
  { 0x3C, 0x40 },
  { 0xe1, 0x77 },
  { 0x00, 0x00 },
  { 0xff, 0xff },
};

static const sensor_reg_t OV2640_JPEG[] =
{
  { 0xe0, 0x14 },
  { 0xe1, 0x77 },
  { 0xe5, 0x1f },
  { 0xd7, 0x03 },
  { 0xda, 0x10 },
  { 0xe0, 0x00 },
  { 0xFF, 0x01 },
  { 0x04, 0x08 },
  { 0xff, 0xff },
};

/* JPG 160x120 */
static const sensor_reg_t OV2640_160x120_JPEG[] =
{
  { 0xff, 0x01 },
  { 0x12, 0x40 },
  { 0x17, 0x11 },
  { 0x18, 0x43 },
  { 0x19, 0x00 },
  { 0x1a, 0x4b },
  { 0x32, 0x09 },
  { 0x4f, 0xca },
  { 0x50, 0xa8 },
  { 0x5a, 0x23 },
  { 0x6d, 0x00 },
  { 0x39, 0x12 },
  { 0x35, 0xda },
  { 0x22, 0x1a },
  { 0x37, 0xc3 },
  { 0x23, 0x00 },
  { 0x34, 0xc0 },
  { 0x36, 0x1a },
  { 0x06, 0x88 },
  { 0x07, 0xc0 },
  { 0x0d, 0x87 },
  { 0x0e, 0x41 },
  { 0x4c, 0x00 },
  { 0xff, 0x00 },
  { 0xe0, 0x04 },
  { 0xc0, 0x64 },
  { 0xc1, 0x4b },
  { 0x86, 0x35 },
  { 0x50, 0x92 },
  { 0x51, 0xc8 },
  { 0x52, 0x96 },
  { 0x53, 0x00 },
  { 0x54, 0x00 },
  { 0x55, 0x00 },
  { 0x57, 0x00 },
  { 0x5a, 0x28 },
  { 0x5b, 0x1e },
  { 0x5c, 0x00 },
  { 0xe0, 0x00 },
  { 0xff, 0xff },
};

/* JPG, 0x176x144 */

static const sensor_reg_t OV2640_176x144_JPEG[] =
{
  { 0xff, 0x01 },
  { 0x12, 0x40 },
  { 0x17, 0x11 },
  { 0x18, 0x43 },
  { 0x19, 0x00 },
  { 0x1a, 0x4b },
  { 0x32, 0x09 },
  { 0x4f, 0xca },
  { 0x50, 0xa8 },
  { 0x5a, 0x23 },
  { 0x6d, 0x00 },
  { 0x39, 0x12 },
  { 0x35, 0xda },
  { 0x22, 0x1a },
  { 0x37, 0xc3 },
  { 0x23, 0x00 },
  { 0x34, 0xc0 },
  { 0x36, 0x1a },
  { 0x06, 0x88 },
  { 0x07, 0xc0 },
  { 0x0d, 0x87 },
  { 0x0e, 0x41 },
  { 0x4c, 0x00 },
  { 0xff, 0x00 },
  { 0xe0, 0x04 },
  { 0xc0, 0x64 },
  { 0xc1, 0x4b },
  { 0x86, 0x35 },
  { 0x50, 0x92 },
  { 0x51, 0xc8 },
  { 0x52, 0x96 },
  { 0x53, 0x00 },
  { 0x54, 0x00 },
  { 0x55, 0x00 },
  { 0x57, 0x00 },
  { 0x5a, 0x2c },
  { 0x5b, 0x24 },
  { 0x5c, 0x00 },
  { 0xe0, 0x00 },
  { 0xff, 0xff },
};

/* JPG 320x240 */

static const sensor_reg_t OV2640_320x240_JPEG[] =
{
  { 0xff, 0x01 },
  { 0x12, 0x40 },
  { 0x17, 0x11 },
  { 0x18, 0x43 },
  { 0x19, 0x00 },
  { 0x1a, 0x4b },
  { 0x32, 0x09 },
  { 0x4f, 0xca },
  { 0x50, 0xa8 },
  { 0x5a, 0x23 },
  { 0x6d, 0x00 },
  { 0x39, 0x12 },
  { 0x35, 0xda },
  { 0x22, 0x1a },
  { 0x37, 0xc3 },
  { 0x23, 0x00 },
  { 0x34, 0xc0 },
  { 0x36, 0x1a },
  { 0x06, 0x88 },
  { 0x07, 0xc0 },
  { 0x0d, 0x87 },
  { 0x0e, 0x41 },
  { 0x4c, 0x00 },
  { 0xff, 0x00 },
  { 0xe0, 0x04 },
  { 0xc0, 0x64 },
  { 0xc1, 0x4b },
  { 0x86, 0x35 },
  { 0x50, 0x89 },
  { 0x51, 0xc8 },
  { 0x52, 0x96 },
  { 0x53, 0x00 },
  { 0x54, 0x00 },
  { 0x55, 0x00 },
  { 0x57, 0x00 },
  { 0x5a, 0x50 },
  { 0x5b, 0x3c },
  { 0x5c, 0x00 },
  { 0xe0, 0x00 },
  { 0xff, 0xff },
};

/* JPG 352x288 */

static const sensor_reg_t OV2640_352x288_JPEG[] =

{
  { 0xff, 0x01 },
  { 0x12, 0x40 },
  { 0x17, 0x11 },
  { 0x18, 0x43 },
  { 0x19, 0x00 },
  { 0x1a, 0x4b },
  { 0x32, 0x09 },
  { 0x4f, 0xca },
  { 0x50, 0xa8 },
  { 0x5a, 0x23 },
  { 0x6d, 0x00 },
  { 0x39, 0x12 },
  { 0x35, 0xda },
  { 0x22, 0x1a },
  { 0x37, 0xc3 },
  { 0x23, 0x00 },
  { 0x34, 0xc0 },
  { 0x36, 0x1a },
  { 0x06, 0x88 },
  { 0x07, 0xc0 },
  { 0x0d, 0x87 },
  { 0x0e, 0x41 },
  { 0x4c, 0x00 },
  { 0xff, 0x00 },
  { 0xe0, 0x04 },
  { 0xc0, 0x64 },
  { 0xc1, 0x4b },
  { 0x86, 0x35 },
  { 0x50, 0x89 },
  { 0x51, 0xc8 },
  { 0x52, 0x96 },
  { 0x53, 0x00 },
  { 0x54, 0x00 },
  { 0x55, 0x00 },
  { 0x57, 0x00 },
  { 0x5a, 0x58 },
  { 0x5b, 0x48 },
  { 0x5c, 0x00 },
  { 0xe0, 0x00 },
  { 0xff, 0xff },
};

/* JPG 640x480 */
static const sensor_reg_t OV2640_640x480_JPEG[] =
{
	{0xff, 0x01},
	{0x11, 0x01},
	{0x12, 0x00}, // Bit[6:4]: Resolution selection//0x02Ϊ����
	{0x17, 0x11}, // HREFST[10:3]
	{0x18, 0x75}, // HREFEND[10:3]
	{0x32, 0x36}, // Bit[5:3]: HREFEND[2:0]; Bit[2:0]: HREFST[2:0]
	{0x19, 0x01}, // VSTRT[9:2]
	{0x1a, 0x97}, // VEND[9:2]
	{0x03, 0x0f}, // Bit[3:2]: VEND[1:0]; Bit[1:0]: VSTRT[1:0]
	{0x37, 0x40},
	{0x4f, 0xbb},
	{0x50, 0x9c},
	{0x5a, 0x57},
	{0x6d, 0x80},
	{0x3d, 0x34},
	{0x39, 0x02},
	{0x35, 0x88},
	{0x22, 0x0a},
	{0x37, 0x40},
	{0x34, 0xa0},
	{0x06, 0x02},
	{0x0d, 0xb7},
	{0x0e, 0x01},

	{0xff, 0x00},
	{0xe0, 0x04},
	{0xc0, 0xc8},
	{0xc1, 0x96},
	{0x86, 0x3d},
	{0x50, 0x89},
	{0x51, 0x90},
	{0x52, 0x2c},
	{0x53, 0x00},
	{0x54, 0x00},
	{0x55, 0x88},
	{0x57, 0x00},
	{0x5a, 0xa0},
	{0x5b, 0x78},
	{0x5c, 0x00},
	{0xd3, 0x04},
	{0xe0, 0x00},

  	{0xff, 0xff},
};

/* JPG 800x600 */
static const sensor_reg_t OV2640_800x600_JPEG[] =
{
	{0xff, 0x01},
	{0x11, 0x01},
	{0x12, 0x00}, // Bit[6:4]: Resolution selection//0x02Ϊ����
	{0x17, 0x11}, // HREFST[10:3]
	{0x18, 0x75}, // HREFEND[10:3]
	{0x32, 0x36}, // Bit[5:3]: HREFEND[2:0]; Bit[2:0]: HREFST[2:0]
	{0x19, 0x01}, // VSTRT[9:2]
	{0x1a, 0x97}, // VEND[9:2]
	{0x03, 0x0f}, // Bit[3:2]: VEND[1:0]; Bit[1:0]: VSTRT[1:0]
	{0x37, 0x40},
	{0x4f, 0xbb},
	{0x50, 0x9c},
	{0x5a, 0x57},
	{0x6d, 0x80},
	{0x3d, 0x34},
	{0x39, 0x02},
	{0x35, 0x88},
	{0x22, 0x0a},
	{0x37, 0x40},
	{0x34, 0xa0},
	{0x06, 0x02},
	{0x0d, 0xb7},
	{0x0e, 0x01},

	{0xff, 0x00},
	{0xe0, 0x04},
	{0xc0, 0xc8},
	{0xc1, 0x96},
	{0x86, 0x35},
	{0x50, 0x89},
	{0x51, 0x90},
	{0x52, 0x2c},
	{0x53, 0x00},
	{0x54, 0x00},
	{0x55, 0x88},
	{0x57, 0x00},
	{0x5a, 0xc8},
	{0x5b, 0x96},
	{0x5c, 0x00},
	{0xd3, 0x02},
	{0xe0, 0x00},

  	{0xff, 0xff},
};

/* JPG 1024x768 */
static const sensor_reg_t OV2640_1024x768_JPEG[] =
{
	{0xff, 0x01},
	{0x11, 0x01},
	{0x12, 0x00}, // Bit[6:4]: Resolution selection//0x02Ϊ����
	{0x17, 0x11}, // HREFST[10:3]
	{0x18, 0x75}, // HREFEND[10:3]
	{0x32, 0x36}, // Bit[5:3]: HREFEND[2:0]; Bit[2:0]: HREFST[2:0]
	{0x19, 0x01}, // VSTRT[9:2]
	{0x1a, 0x97}, // VEND[9:2]
	{0x03, 0x0f}, // Bit[3:2]: VEND[1:0]; Bit[1:0]: VSTRT[1:0]
	{0x37, 0x40},
	{0x4f, 0xbb},
	{0x50, 0x9c},
	{0x5a, 0x57},
	{0x6d, 0x80},
	{0x3d, 0x34},
	{0x39, 0x02},
	{0x35, 0x88},
	{0x22, 0x0a},
	{0x37, 0x40},
	{0x34, 0xa0},
	{0x06, 0x02},
	{0x0d, 0xb7},
	{0x0e, 0x01},

	{0xff, 0x00},
	{0xc0, 0xC8},
	{0xc1, 0x96},
	{0x8c, 0x00},
	{0x86, 0x3D},
	{0x50, 0x00},
	{0x51, 0x90},
	{0x52, 0x2C},
	{0x53, 0x00},
	{0x54, 0x00},
	{0x55, 0x88},
	{0x5a, 0x00},
	{0x5b, 0xC0},
	{0x5c, 0x01},
	{0xd3, 0x02},


  {0xff, 0xff},
};

   /* JPG 1280x1024 */
static const sensor_reg_t OV2640_1280x1024_JPEG[] =
{
	{0xff, 0x01},
	{0x11, 0x01},
	{0x12, 0x00}, // Bit[6:4]: Resolution selection//0x02Ϊ����
	{0x17, 0x11}, // HREFST[10:3]
	{0x18, 0x75}, // HREFEND[10:3]
	{0x32, 0x36}, // Bit[5:3]: HREFEND[2:0]; Bit[2:0]: HREFST[2:0]
	{0x19, 0x01}, // VSTRT[9:2]
	{0x1a, 0x97}, // VEND[9:2]
	{0x03, 0x0f}, // Bit[3:2]: VEND[1:0]; Bit[1:0]: VSTRT[1:0]
	{0x37, 0x40},
	{0x4f, 0xbb},
	{0x50, 0x9c},
	{0x5a, 0x57},
	{0x6d, 0x80},
	{0x3d, 0x34},
	{0x39, 0x02},
	{0x35, 0x88},
	{0x22, 0x0a},
	{0x37, 0x40},
	{0x34, 0xa0},
	{0x06, 0x02},
	{0x0d, 0xb7},
	{0x0e, 0x01},

	{0xff, 0x00},
	{0xe0, 0x04},
	{0xc0, 0xc8},
	{0xc1, 0x96},
	{0x86, 0x3d},
	{0x50, 0x00},
	{0x51, 0x90},
	{0x52, 0x2c},
	{0x53, 0x00},
	{0x54, 0x00},
	{0x55, 0x88},
	{0x57, 0x00},
	{0x5a, 0x40},
	{0x5b, 0xf0},
	{0x5c, 0x01},
	{0xd3, 0x02},
	{0xe0, 0x00},

  	{0xff, 0xff},
};

   /* JPG 1600x1200 */
static const sensor_reg_t OV2640_1600x1200_JPEG[] =
{
	{0xff, 0x01},
	{0x11, 0x01},
	{0x12, 0x00}, // Bit[6:4]: Resolution selection//0x02Ϊ����
	{0x17, 0x11}, // HREFST[10:3]
	{0x18, 0x75}, // HREFEND[10:3]
	{0x32, 0x36}, // Bit[5:3]: HREFEND[2:0]; Bit[2:0]: HREFST[2:0]
	{0x19, 0x01}, // VSTRT[9:2]
	{0x1a, 0x97}, // VEND[9:2]
	{0x03, 0x0f}, // Bit[3:2]: VEND[1:0]; Bit[1:0]: VSTRT[1:0]
	{0x37, 0x40},
	{0x4f, 0xbb},
	{0x50, 0x9c},
	{0x5a, 0x57},
	{0x6d, 0x80},
	{0x3d, 0x34},
	{0x39, 0x02},
	{0x35, 0x88},
	{0x22, 0x0a},
	{0x37, 0x40},
	{0x34, 0xa0},
	{0x06, 0x02},
	{0x0d, 0xb7},
	{0x0e, 0x01},

	{0xff, 0x00},
	{0xe0, 0x04},
	{0xc0, 0xc8},
	{0xc1, 0x96},
	{0x86, 0x3d},
	{0x50, 0x00},
	{0x51, 0x90},
	{0x52, 0x2c},
	{0x53, 0x00},
	{0x54, 0x00},
	{0x55, 0x88},
	{0x57, 0x00},
	{0x5a, 0x90},
	{0x5b, 0x2C},
	{0x5c, 0x05},              //bit2->1;bit[1:0]->1
	{0xd3, 0x02},
	{0xe0, 0x00},

  	{0xff, 0xff},
};

//========================================================================================

//#define CONFIG_VERIFY

static i2c_arducam_data_t i2c_dev = {0};
uint8_t cam_initialized = 0;
spi_device_handle_t ARDUCAM_SPI = NULL;

static spi_bus_config_t buscfg={
    .miso_io_num=-1,
    .mosi_io_num=-1,
    .sclk_io_num=-1,
    .quadwp_io_num=-1,
    .quadhd_io_num=-1
};

static spi_device_interface_config_t ARDUCAM_SPI_cfg = {
    .clock_speed_hz = 8000000,		// Clock out at 8 MHz
    .mode = 0,						// SPI mode 0
    .spics_io_num = -1,				// use software CS pin
	.spics_ext_io_num = PIN_NUM_CS,	// CS pin
	.spidc_io_num = -1,
    .queue_size = 7,				// We want to be able to queue 7 transactions at a time
    .pre_cb = NULL,					// Specify pre-transfer callback to handle D/C line
};


//----------------------------------------------------------------------------
static int arducam_spi_init(uint8_t sdi, uint8_t sdo, uint8_t sck, uint8_t cs)
{
	if (cs != 0) {
		buscfg.miso_io_num=sdi;
		buscfg.mosi_io_num=sdo;
		buscfg.sclk_io_num=sck;
		buscfg.quadwp_io_num=-1;
		buscfg.quadhd_io_num=-1;

		ARDUCAM_SPI_cfg.clock_speed_hz = 8000000;	// Clock out at 8 MHz
		ARDUCAM_SPI_cfg.mode = 0;					// SPI mode 0
		ARDUCAM_SPI_cfg.spics_io_num = -1;			// use software CS pin
		ARDUCAM_SPI_cfg.spics_ext_io_num = cs;		// CS pin
		ARDUCAM_SPI_cfg.spidc_io_num = -1;
		ARDUCAM_SPI_cfg.queue_size = 3;				// We want to be able to queue 3 transactions at a time
		ARDUCAM_SPI_cfg.pre_cb = NULL;				// Specify pre-transfer callback to handle D/C line
	}
	else if (buscfg.sclk_io_num <= 0) {
		buscfg.miso_io_num=PIN_NUM_MISO;
		buscfg.mosi_io_num=PIN_NUM_MOSI;
		buscfg.sclk_io_num=PIN_NUM_CLK;

		ARDUCAM_SPI_cfg.clock_speed_hz = 8000000;		// Clock out at 8 MHz
		ARDUCAM_SPI_cfg.mode = 0;						// SPI mode 0
		ARDUCAM_SPI_cfg.spics_io_num = -1;				// use software CS pin
		ARDUCAM_SPI_cfg.spics_ext_io_num = PIN_NUM_CS;	// CS pin
		ARDUCAM_SPI_cfg.spidc_io_num = -1;
		ARDUCAM_SPI_cfg.queue_size = 3;					// We want to be able to queue 3 transactions at a time
		ARDUCAM_SPI_cfg.pre_cb = NULL;					// Specify pre-transfer callback to handle D/C line
	}

	driver_error_t *error = espi_init(VSPI_HOST, &ARDUCAM_SPI_cfg, &buscfg, &ARDUCAM_SPI);
	if (error) {
		free(error);
		return -1;
	}
	//spi_device_select(ARDUCAM_SPI, 0);

    return 0;
}

//------------------------------------------------------------------------
static int arducam_i2c_init(uint8_t sensor_addr, uint8_t sda, uint8_t scl)
{
	driver_error_t *error;

    if ((error = i2c_setup(0, 1, 400, sda, scl, 0, 0))) {
    	return -1;
    }
    i2c_dev.unit = 0;
    i2c_dev.transaction = I2C_TRANSACTION_INITIALIZER;

    if ((error = i2c_start(i2c_dev.unit, &i2c_dev.transaction))) {
    	return -2;
    }

	i2c_dev.address = sensor_addr;
	return 0;
}

//-----------------------------------
void arducam_delay_ms(uint32_t delay)
{
	vTaskDelay(delay / portTICK_PERIOD_MS);
}

//-----------------------------------------------------------
static void arducam_spi_write(uint8_t address, uint8_t value)
{
#ifdef CONFIG_VERIFY
	static int counter = 0;
#endif // CONFIG_VERIFY

	uint8_t data[2] = {address, value};

	if (spi_device_select(ARDUCAM_SPI, 0)) return;
	//taskDISABLE_INTERRUPTS();

	spi_transfer_data(ARDUCAM_SPI, (unsigned char *)data, NULL, 2, 0);
	spi_device_deselect(ARDUCAM_SPI);

	//taskENABLE_INTERRUPTS();
#ifdef CONFIG_VERIFY
	data[0] = arducam_spi_read(address & 0x7f);
	if (data[0] != value) {
		printf("arducam_spi_write: verify failed after %d for reg 0x%02x (0x%02x should be 0x%02x)\n", counter, address & 0x7f, data[0], value);
	}
	counter++;
#endif // CONFIG_VERIFY
}

//----------------------------------------------
static uint8_t arducam_spi_read(uint8_t address)
{
	uint8_t data[2] = {address, 0x00};

	if (spi_device_select(ARDUCAM_SPI, 0)) return 0;
	//taskDISABLE_INTERRUPTS();

	spi_transfer_data(ARDUCAM_SPI, data, data, 2, 2);

	spi_device_deselect(ARDUCAM_SPI);

    //taskENABLE_INTERRUPTS();

    return data[1];
}

//-------------------------------------------------------------
static uint8_t arducam_i2c_write(uint8_t regID, uint8_t regDat)
{
	uint8_t data[] = {regID, regDat};
	driver_error_t *error;

    if ((error = i2c_start(i2c_dev.unit, &i2c_dev.transaction))) {
        free(error);
    	return 1;
    }
	if ((error = i2c_write_address(i2c_dev.unit, &i2c_dev.transaction, i2c_dev.address, 0))) {
	    free(error);
    	return 2;
    }
    if ((error = i2c_write(i2c_dev.unit, &i2c_dev.transaction, (char *)&data, 2))) {
        free(error);
    	return 3;
    }
    if ((error = i2c_stop(i2c_dev.unit, &i2c_dev.transaction))) {
        free(error);
    	return 4;
    }

    return 0;
}

//------------------------------------------------------
uint8_t arducam_i2c_read(uint8_t regID, uint8_t* regDat)
{
	driver_error_t *error;

    if ((error = i2c_start(i2c_dev.unit, &i2c_dev.transaction))) {
        free(error);
    	return 1;
    }
    // write regID
	if ((error = i2c_write_address(i2c_dev.unit, &i2c_dev.transaction, i2c_dev.address, 0))) {
	    free(error);
    	return 2;
    }
    if ((error = i2c_write(i2c_dev.unit, &i2c_dev.transaction, (char *)&regID, 1))) {
        free(error);
    	return 3;
    }
    // read regDat
    if ((error = i2c_start(i2c_dev.unit, &i2c_dev.transaction))) {
        free(error);
    	return 4;
    }
	if ((error = i2c_write_address(i2c_dev.unit, &i2c_dev.transaction, i2c_dev.address, 1))) {
	    free(error);
    	return 5;
    }
	if ((error = i2c_read(i2c_dev.unit, &i2c_dev.transaction, (char *)regDat, 1))) {
	    free(error);
    	return 6;
    }
    if ((error = i2c_stop(i2c_dev.unit, &i2c_dev.transaction))) {
        free(error);
    	return 7;
    }

    return 0;
}

//-------------------------------------------------------------
static int arducam_i2c_write_regs(const sensor_reg_t reglist[])
{
	uint16_t reg_addr = 0, reg_idx = 0;
	uint16_t reg_val = 0;
	uint8_t err;
	const sensor_reg_t *next = reglist;

	while ((reg_addr != 0xff) | (reg_val != 0xff))
	{
		reg_addr = next->reg;
		reg_val = next->val;
		err = arducam_i2c_write(reg_addr, reg_val);
		if (err != 0) {
			//printf("arducam_i2c_write_regs failed at register %d, error %d\n", reg_idx, err);
			return err;
		}
	   	next++;
	   	reg_idx++;
	}

	return 0;
}

/*
//----------------------------------------------------------------
static uint8_t arducam_i2c_write16(uint8_t regID, uint16_t regDat)
{
	return 0;
}

//----------------------------------------------------------------
static uint8_t arducam_i2c_read16(uint8_t regID, uint16_t* regDat)
{
	return 0;
}

//-------------------------------------------------------------------
static uint8_t arducam_i2c_word_write(uint16_t regID, uint8_t regDat)
{
	return 0;
}

//-------------------------------------------------------------------
static uint8_t arducam_i2c_word_read(uint16_t regID, uint8_t* regDat)
{
	return 0;
}

//---------------------------------------------------------------
static int arducam_i2c_write_regs16(const sensor_reg_t reglist[])
{
	unsigned int reg_addr = 0, reg_val = 0;
	uint8_t err;
	const sensor_reg_t *next = reglist;
	while ((reg_addr != 0xff) | (reg_val != 0xffff))
	{
		reg_addr = next->reg;
		reg_val = next->val;
		err = arducam_i2c_write16(reg_addr, reg_val);
		if (err != 0) {
			return err;
		}
	   	next++;
	}

	return 0;
}

//------------------------------------------------------------------
static int arducam_i2c_write_word_regs(const sensor_reg_t reglist[])
{
	unsigned int reg_addr = 0, reg_val = 0, reg_idx = 0;
	uint8_t err;
	const sensor_reg_t *next = reglist;

	while ((reg_addr != 0xffff) | (reg_val != 0xff))
	{
		reg_addr = next->reg;
		reg_val = next->val;
		err = arducam_i2c_word_write(reg_addr, reg_val);
		if (err != 0) {
			//printf("arducam_i2c_write_word_regs failed at register %d, error %d\n", reg_idx, err);
			return err;
		}
	   	next++;
	   	reg_idx++;
	}

	return 0;
}
*/

// =======================================================================================

int arducam(sensor_model_t model, uint8_t sda, uint8_t scl, uint8_t sdi, uint8_t sdo, uint8_t sck, uint8_t cs)
{
	int res;

	myCAM.sensor_model = model;
	switch(myCAM.sensor_model)
	{
		case smOV7660:
		case smOV7670:
		case smOV7675:
		case smOV7725:
			myCAM.sensor_addr = 0x21;
			break;
		case smMT9D111:
			myCAM.sensor_addr = 0x5d;
			break;
		case smOV3640:
		case smOV5642:
			myCAM.sensor_addr = 0x3c;
			break;
		case smOV2640:
		case smOV9655:
			myCAM.sensor_addr = 0x30;
			break;
		case smMT9M112:
			myCAM.sensor_addr = 0x5d;
			break;
		default:
			myCAM.sensor_addr = 0x21;
			break;
	}
	// initialize spi
	res = arducam_spi_init(sdi,sdo,sck,cs);
	if (res != 0) {
		//printf("ERROR: SPI init failed (%d)\r\n", res);
		return res;
	}
	// initialize i2c:
	res = arducam_i2c_init(myCAM.sensor_addr, sda, scl);
	if (res != 0) {
		//printf("ERROR: I2C init failed (%d)\r\n", res);
		return res;
	}

    return 0;
}

int arducam_init()
{
	switch(myCAM.sensor_model) {
		case smOV2640:
		{
			if (arducam_i2c_write(0xff, 0x01)) return -1;
			if (arducam_i2c_write(0x12, 0x80)) return -2;
			arducam_delay_ms(100);
			if(myCAM.m_fmt == fmtJPEG)
			{
				if (arducam_i2c_write_regs(OV2640_JPEG_INIT)) return -3;
				if (arducam_i2c_write_regs(OV2640_YUV422)) return -4;
				if (arducam_i2c_write_regs(OV2640_JPEG)) return -5;
				if (arducam_i2c_write(0xff, 0x01)) return -6;
				if (arducam_i2c_write(0x15, 0x00)) return -7;
				if (arducam_i2c_write_regs(OV2640_320x240_JPEG)) return -8;
				//if (arducam_i2c_write(0xff, 0x00)) return -9;
				//if (arducam_i2c_write(0x44, 0x32)) return -10;
			}
			else
			{
				if (arducam_i2c_write_regs(OV2640_QVGA)) return -3;
			}
			break;
		}
		case smOV9655:
		{

			break;
		}
		case smMT9M112:
		{

			break;
		}

		default:

			break;
	}

	return 0;
}

void arducam_flush_fifo(void)
{
	arducam_write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

void arducam_start_capture(void)
{
	arducam_write_reg(ARDUCHIP_FIFO, FIFO_START_MASK);
}

void arducam_clear_fifo_flag(void)
{
	arducam_write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

uint8_t arducam_read_fifo(void)
{
	uint8_t data;
	data = arducam_spi_read(0x3D);
	return data;
}

void arducam_burst_read_fifo(uint8_t *buf, uint32_t len, uint8_t op)
{
	if (op == 10) {
	    spi_device_deselect(ARDUCAM_SPI);
	    return;
	}
	if ((!buf) || (len == 0)) return;

	if (op == 1) {
		buf[0] = 0x3C;
		spi_device_select(ARDUCAM_SPI, 0);
	}

	//taskDISABLE_INTERRUPTS();
	spi_transfer_data(ARDUCAM_SPI, buf, buf, 1, len);
    //taskENABLE_INTERRUPTS();
}

uint8_t arducam_read_reg(uint8_t addr)
{
	uint8_t data;
	data = arducam_spi_read(addr & 0x7F);
	return data;
}

void arducam_write_reg(uint8_t addr, uint8_t data)
{
	arducam_spi_write(addr | 0x80, data);
}

int arducam_set_jpeg_size(jpeg_size_t size)
{
	int err = -99;
#ifdef OV2640_CAM
	switch(size)
	{
		case sz160x120:
			err = arducam_i2c_write_regs(OV2640_160x120_JPEG);
			break;
		case sz176x144:
			err = arducam_i2c_write_regs(OV2640_176x144_JPEG);
			break;
		case sz320x240:
			err = arducam_i2c_write_regs(OV2640_320x240_JPEG);
			break;
		case sz352x288:
			err = arducam_i2c_write_regs(OV2640_352x288_JPEG);
			break;
		case sz640x480:
			err = arducam_i2c_write_regs(OV2640_640x480_JPEG);
			break;
		case sz800x600:
			err = arducam_i2c_write_regs(OV2640_800x600_JPEG);
			break;
		case sz1024x768:
			err = arducam_i2c_write_regs(OV2640_1024x768_JPEG);
			break;
		case sz1280x1024:
			err = arducam_i2c_write_regs(OV2640_1280x1024_JPEG);
			break;
		case sz1600x1200:
			err = arducam_i2c_write_regs(OV2640_1600x1200_JPEG);
			break;
		default:
			err = arducam_i2c_write_regs(OV2640_320x240_JPEG);
			break;
	}
#endif
	return err;
}

void arducam_set_format(image_format_t fmt)
{
	myCAM.m_fmt = fmt;
}

#endif
