/*
* Copyright (c) 2019 MediaTek Inc.
 *
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files
* (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
 *
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef BUILD_LK
#include <string.h>
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#include <platform/sync_write.h>
#else
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of_gpio.h>
#include <asm-generic/gpio.h>

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/regulator/consumer.h>
#endif
#endif
#include "lcm_drv.h"


#define GPIO_LCD_RST          (GPIO20)

/* --------------------------------------------------------------- */
/*  Local Constants */
/* --------------------------------------------------------------- */
#define FRAME_WIDTH		(720)
#define FRAME_HEIGHT		(1280)
#define GPIO_OUT_ONE		1
#define GPIO_OUT_ZERO		0

#define REGFLAG_DELAY             						0xFFE
#define REGFLAG_END_OF_TABLE      						0xFFF

/* ----------------------------------------------------------------- */
/*  Local Variables */
/* ----------------------------------------------------------------- */
extern void mdelay(unsigned long msec);

static LCM_UTIL_FUNCS lcm_util = {
	.set_reset_pin = NULL,
	.udelay = NULL,
	.mdelay = NULL,
};

#define SET_RESET_PIN(v)		(lcm_util.set_reset_pin((v)))

#define UDELAY(n)				(lcm_util.udelay(n))
#define MDELAY(n)				(lcm_util.mdelay(n))

/* ----------------------------------------------------------------- */
/* Local Functions */
/* ----------------------------------------------------------------- */
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) \
		 (lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update))
#define dsi_set_cmdq(pdata, queue_size, force_update) \
		 (lcm_util.dsi_set_cmdq(pdata, queue_size, force_update))
#define wrtie_cmd(cmd) \
		 (lcm_util.dsi_write_cmd(cmd))
#define write_regs(addr, pdata, byte_nums) \
		 (lcm_util.dsi_write_regs(addr, pdata, byte_nums))
#define read_reg \
		 (lcm_util.dsi_read_reg())
#define read_reg_v2(cmd, buffer, buffer_size) \
		 (lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size))
		 
struct LCM_setting_table {
    unsigned int cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initial_setting[] = {
{0x28, 0, {0x00} },
{REGFLAG_DELAY, 120, {} },
{0x10, 0, {0x00} },
{REGFLAG_DELAY, 120, {} },
	
{0xFF,3,{0x98,0x81,0x03}},
{0x01,1,{0x00}},           
{0x02,1,{0x00}},           
{0x03,1,{0x53}},           
{0x04,1,{0x13}},           
{0x05,1,{0x13}},           
{0x06,1,{0x06}},           
{0x07,1,{0x00}},           
{0x08,1,{0x04}},           
{0x09,1,{0x00}},           
{0x0A,1,{0x00}},
{0x0B,1,{0x00}},
{0x0C,1,{0x00}},
{0x0D,1,{0x00}},
{0x0E,1,{0x00}},
{0x0F,1,{0x00}},

{0x10,1,{0x00}},           
{0x11,1,{0x00}},
{0x12,1,{0x00}},           
{0x13,1,{0x00}},           
{0x14,1,{0x00}},           
{0x15,1,{0x00}},           
{0x16,1,{0x00}},           
{0x17,1,{0x00}},           
{0x18,1,{0x08}},           
{0x19,1,{0x00}},           
{0x1A,1,{0x00}},
{0x1B,1,{0x00}},
{0x1C,1,{0x00}},
{0x1D,1,{0x00}},
{0x1E,1,{0xC0}},
{0x1F,1,{0x80}},

{0x20,1,{0x04}},
{0x21,1,{0x0B}},
{0x22,1,{0x00}},
{0x23,1,{0x00}},
{0x24,1,{0x00}},
{0x25,1,{0x00}},
{0x26,1,{0x00}},           
{0x27,1,{0x00}}, 
{0x28,1,{0x55}},           
{0x29,1,{0x03}},           
{0x2A,1,{0x00}},           
{0x2B,1,{0x00}},           
{0x2C,1,{0x00}},           
{0x2D,1,{0x00}},           
{0x2E,1,{0x00}},           
{0x2F,1,{0x00}}, 
         
{0x30,1,{0x00}},           
{0x31,1,{0x00}},           
{0x32,1,{0x00}},           
{0x33,1,{0x00}},           
{0x34,1,{0x04}},      
{0x35,1,{0x05}},           
{0x36,1,{0x05}},           
{0x37,1,{0x00}},           
{0x38,1,{0x3C}},           
{0x39,1,{0x50}},           
{0x3A,1,{0x01}},           
{0x3B,1,{0x40}},           
{0x3C,1,{0x00}},           
{0x3D,1,{0x01}},           
{0x3E,1,{0x00}},           
{0x3F,1,{0x00}},
           
{0x40,1,{0x50}},           
{0x41,1,{0x88}},           
{0x42,1,{0x00}},           
{0x43,1,{0x00}},
{0x44,1,{0x1F}},         
{0x50,1,{0x01}},           
{0x51,1,{0x23}},           
{0x52,1,{0x45}},           
{0x53,1,{0x67}},           
{0x54,1,{0x89}},           
{0x55,1,{0xAB}},           
{0x56,1,{0x01}},           
{0x57,1,{0x23}},           
{0x58,1,{0x45}},           
{0x59,1,{0x67}},           
{0x5A,1,{0x89}},           
{0x5B,1,{0xAB}},           
{0x5C,1,{0xCD}},           
{0x5D,1,{0xEF}},      
{0x5E,1,{0x03}},           
{0x5F,1,{0x14}}, 
          
{0x60,1,{0x15}},           
{0x61,1,{0x0C}},           
{0x62,1,{0x0D}},           
{0x63,1,{0x0E}},           
{0x64,1,{0x0F}},           
{0x65,1,{0x10}},           
{0x66,1,{0x11}},           
{0x67,1,{0x08}},           
{0x68,1,{0x02}},           
{0x69,1,{0x0A}},           
{0x6A,1,{0x02}},           
{0x6B,1,{0x02}},           
{0x6C,1,{0x02}},
{0x6D,1,{0x02}},           
{0x6E,1,{0x02}},           
{0x6F,1,{0x02}},
           
{0x70,1,{0x02}},           
{0x71,1,{0x02}},           
{0x72,1,{0x06}},           
{0x73,1,{0x02}},           
{0x74,1,{0x02}},           
{0x75,1,{0x14}},           
{0x76,1,{0x15}},           
{0x77,1,{0x11}},           
{0x78,1,{0x10}},           
{0x79,1,{0x0F}},           
{0x7A,1,{0x0E}},           
{0x7B,1,{0x0D}},           
{0x7C,1,{0x0C}},           
{0x7D,1,{0x06}},           
{0x7E,1,{0x02}},           
{0x7F,1,{0x0A}},
          
{0x80,1,{0x02}},           
{0x81,1,{0x02}},          
{0x83,1,{0x02}},           
{0x84,1,{0x02}},           
{0x85,1,{0x02}},           
{0x86,1,{0x02}},           
{0x87,1,{0x02}},           
{0x88,1,{0x08}},           
{0x89,1,{0x02}},           
{0x8A,1,{0x02}},
{REGFLAG_DELAY, 5, {}}, 
//Page 4 command;           
{0xFF,3,{0x98,0x81,0x04}},           
{0x70,1,{0x00}},           
{0x71,1,{0x00}},
{0x66,1,{0xFE}},           
{0x6F,1,{0x05}},
{0x82,1,{0x1F}},
{0x84,1,{0x1F}},
{0x85,1,{0x0C}},
{0x32,1,{0xAC}}, 
{0x8C,1,{0x80}},          
{0x3C,1,{0xF5}},
{0x3A,1,{0x24}},                     
{0xB5,1,{0x02}},           
{0x31,1,{0x25}},           
{0x88,1,{0x33}},  
{REGFLAG_DELAY, 5, {}}, 
      
{0xFF,3,{0x98,0x81,0x01}},
{0x22,1,{0x0A}},    
{0x31,1,{0x00}},           
{0x53,1,{0x6E}},                 
{0x55,1,{0x78}},                   
{0x50,1,{0x6B}},                    
{0x51,1,{0x6B}},                     
{0x60,1,{0x20}},  
{0x61,1,{0x00}}, 
{0x62,1,{0x0D}}, 
{0x63,1,{0x00}},  
      
{0xA0,1,{0x00}},    
{0xA1,1,{0x11}},        //VP251         
{0xA2,1,{0x1D}},        //VP247 
{0xA3,1,{0x13}},        //VP243         
{0xA4,1,{0x15}},        //VP239         
{0xA5,1,{0x27}},        //VP231         
{0xA6,1,{0x1C}},        //VP219         
{0xA7,1,{0x1E}},        //VP203         
{0xA8,1,{0x7E}},        //VP175         
{0xA9,1,{0x1E}},        //VP144         
{0xAA,1,{0x2A}},        //VP111         
{0xAB,1,{0x72}},        //VP80          
{0xAC,1,{0x1A}},        //VP52          
{0xAD,1,{0x1A}},        //VP36          
{0xAE,1,{0x4D}},        //VP24          
{0xAF,1,{0x23}},        //VP16          
{0xB0,1,{0x29}},        //VP12          
{0xB1,1,{0x4A}},        //VP8           
{0xB2,1,{0x59}},        //VP4           
{0xB3,1,{0x3C}},        //VP0  
     
{0xC0,1,{0x00}},        //VN255 GAMMA N           
{0xC1,1,{0x10}},        //VN251         
{0xC2,1,{0x1D}},        //VN247         
{0xC3,1,{0x12}},        //VN243         
{0xC4,1,{0x16}},        //VN2339         
{0xC5,1,{0x28}},        //VN231         
{0xC6,1,{0x1B}},        //VN219         
{0xC7,1,{0x1D}},        //VN203         
{0xC8,1,{0x7C}},        //VN175         
{0xC9,1,{0x1E}},        //VN144         
{0xCA,1,{0x29}},        //VN111         
{0xCB,1,{0x71}},        //VN80          
{0xCC,1,{0x1A}},        //VN52          
{0xCD,1,{0x19}},        //VN36          
{0xCE,1,{0x4E}},        //VN24          
{0xCF,1,{0x22}},        //VN16          
{0xD0,1,{0x28}},        //VN12          
{0xD1,1,{0x49}},        //VN8           
{0xD2,1,{0x59}},        //VN4           
{0xD3,1,{0x3C}},        //VN0
{REGFLAG_DELAY, 5, {}}, 

{0xFF,3,{0x98,0x81,0x00}},
{REGFLAG_DELAY, 10, {}}, 
{0x11, 0, {0x00} },
{REGFLAG_DELAY, 20, {} },
{0x29, 0, {0x00} },
{REGFLAG_DELAY, 120, {} },
{REGFLAG_END_OF_TABLE, 0x00, {} }
};

static void push_table(struct LCM_setting_table *table, unsigned int count,
			unsigned char force_update)
{
	unsigned int i;

	for (i = 0; i < count; i++) {

		unsigned int cmd;

		cmd = table[i].cmd;
		switch (cmd) {

		case REGFLAG_DELAY:
			MDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_set_cmdq_V2(cmd, table[i].count,
				table[i].para_list, force_update);
		}
	}
}		 

static void lcm_set_gpio_output(unsigned int GPIO, unsigned int output)
{
#ifdef BUILD_LK
	mt_set_gpio_mode(GPIO, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO, output);
#else
	gpio_direction_output(GPIO, output);
	gpio_set_value(GPIO, output);
#endif
}

/* ----------------------------------------------------------------- */
/*  LCM Driver Implementations */
/* ----------------------------------------------------------------- */
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type   = LCM_TYPE_DSI;
	params->width  = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;
	params->dsi.mode   = BURST_VDO_MODE;

	params->dsi.LANE_NUM				= LCM_FOUR_LANE;
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;
	
	params->dsi.packet_size = 256;

	params->dsi.intermediat_buffer_num = 2;

	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
	params->dsi.word_count=FRAME_WIDTH*3; 
	
	params->dsi.vertical_sync_active		= 4;
	params->dsi.vertical_backporch			= 16;
	params->dsi.vertical_frontporch			= 40;
	params->dsi.vertical_active_line		= FRAME_HEIGHT;

	params->dsi.horizontal_sync_active		= 20;
	params->dsi.horizontal_backporch		= 80;
	params->dsi.horizontal_frontporch		= 80;
	params->dsi.horizontal_active_pixel		= FRAME_WIDTH;

	params->dsi.PLL_CLOCK = 228;
}

static void lcm_init_power(void)
{
	printf("[LK/LCM] %s enter\n", __func__);
	pmic_vsim1_enable(true, 1800);
	pmic_vldo28_enable(true);
	MDELAY(100);

	lcm_set_gpio_output(GPIO_LCD_RST, GPIO_OUT_ONE);
	MDELAY(10);
	lcm_set_gpio_output(GPIO_LCD_RST, GPIO_OUT_ZERO);
	MDELAY(30);
	lcm_set_gpio_output(GPIO_LCD_RST, GPIO_OUT_ONE);
	MDELAY(200);	
}

static void lcm_init(void)
{
	printf("[LK/LCM] %s enter\n", __func__);

	push_table(lcm_initial_setting,
		sizeof(lcm_initial_setting) / sizeof(struct LCM_setting_table),
		1);
	printf("[LK/LCM] %s done\n", __func__);		
}

LCM_DRIVER ili9881c_xy_dsi_vdo_tv101wxu_lcm_drv = {
	.name			= "ili9881c_xy_dsi_vdo_tv101wxu",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init_power		= lcm_init_power,
	.init           = lcm_init,
};
