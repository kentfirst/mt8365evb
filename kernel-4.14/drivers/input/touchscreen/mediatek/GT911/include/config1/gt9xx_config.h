/*
 *  Driver for Goodix Touchscreens
 *
 *  Copyright (c) 2014 Red Hat Inc.
 *  Copyright (c) 2015 K. Merker <merker@debian.org>
 *
 *  This code is based on gt9xx.c authored by andrew@goodix.com:
 *
 *  2010 - 2012 Goodix Technology.
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; version 2 of the License.
 */
#ifndef _GT9XX_CONFIG_H_
#define _GT9XX_CONFIG_H_

/*
 ***************************PART2:TODO
 * define**********************************
 * STEP_1(REQUIRED):Change config table.
 * TODO: puts the config info corresponded to your TP here,
 * the following is just
 * a sample config, send this config should cause the chip cannot work normally
 */
#define CTP_CFG_GROUP1                                                         \
	{                                                                      \
		0x41, 0xD0, 0x02, 0x00, 0x05, 0x05, 0x35, 0x20, 0x22, 0x0F,    \
			0x2D, 0x0F, 0x55, 0x41, 0x03, 0x07, 0x00, 0x00, 0x00,  \
			0x00, 0x00, 0x00, 0x05, 0x18, 0x1A, 0x1E, 0x14, 0x89,  \
			0x29, 0x0A, 0x4D, 0x4F, 0x33, 0x0F, 0x00, 0x00, 0x01,  \
			0x9A, 0x03, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
			0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x69, 0x94, 0xE5,  \
			0x02, 0x08, 0x00, 0x00, 0x04, 0x7B, 0x52, 0x00, 0x78,  \
			0x56, 0x00, 0x75, 0x5B, 0x00, 0x72, 0x60, 0x00, 0x70,  \
			0x66, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
			0x15, 0x00, 0x00, 0x14, 0x12, 0x10, 0x0E, 0x0C, 0x0A,  \
			0x08, 0x06, 0x04, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  \
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  \
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x02, 0x04,  \
			0x06, 0x08, 0x0A, 0x0F, 0x10, 0x12, 0x22, 0x21, 0x20,  \
			0x1F, 0x1E, 0x1D, 0x1C, 0x18, 0x16, 0xFF, 0xFF, 0xFF,  \
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  \
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  \
			0xFF, 0xFF, 0xFF, 0x60, 0x01,                          \
	}

#define CTP_CFG_GROUP1_CHARGER                                                 \
	{                                                                      \
	}

/* TODO puts your group2 config info here,if need. */
#define CTP_CFG_GROUP2                                                         \
	{                                                                      \
	}

/* TODO puts your group2 config info here,if need. */
#define CTP_CFG_GROUP2_CHARGER                                                 \
	{                                                                      \
	}

/* TODO puts your group3 config info here,if need. */
#define CTP_CFG_GROUP3                                                         \
	{                                                                      \
	}

/* TODO puts your group3 config info here,if need. */
#define CTP_CFG_GROUP3_CHARGER                                                 \
	{                                                                      \
	}

/* TODO: define your config for Sensor_ID == 3 here, if needed */
#define CTP_CFG_GROUP4                                                         \
	{                                                                      \
	}

/* TODO: define your config for Sensor_ID == 4 here, if needed */
#define CTP_CFG_GROUP5                                                         \
	{                                                                      \
	}

/* TODO: define your config for Sensor_ID == 5 here, if needed */
#define CTP_CFG_GROUP6                                                         \
	{                                                                      \
	}

#endif /* _GT1X_CONFIG_H_ */
