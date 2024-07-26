/*
*******************************************************************************
* Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include "platform_autoconf.h"
#include <bt_api_config.h>
#if (defined(CONFIG_BT_TMAP_SUPPORT) && CONFIG_BT_TMAP_SUPPORT)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <log_service.h>
#include <bt_utils.h>
#include <rtk_bt_def.h>
#include <rtk_bt_common.h>
#include <rtk_bt_tmap.h>
#include <atcmd_bt_impl.h>

static int atcmd_bt_tmap_broadcast_media_receiver_cfg(int argc, char **argv)
{
	(void)argc;
	uint8_t channel = 0;
	uint8_t neighbor_addr[6] = {0};

	if (strcmp(argv[0], "left") == 0) {
		/* RTK_BT_LE_AUDIO_LEFT */
		channel = 1;
	} else if (strcmp(argv[0], "right") == 0) {
		/* RTK_BT_LE_AUDIO_RIGHT */
		channel = 2;
	} else if (strcmp(argv[0], "stereo") == 0) {
		/* RTK_BT_LE_AUDIO_STEREO */
		channel = 3;
	} else {
		BLETMAP_AT_PRINTK("Unknown channel allocation");
		return -1;
	}

	if (argc == 2) {
		hexdata_str_to_bd_addr(argv[1], neighbor_addr, 6);
		if (rtk_bt_tmap_broadcast_media_receiver_cfg(channel, neighbor_addr)) {
			BLETMAP_AT_PRINTK("TMAP broadcast media receiver config channel neighbor address fail \r\n");
			return -1;
		}
		BLETMAP_AT_PRINTK("TMAP broadcast media receiver config channel and neighbor address successfully \r\n");
	} else {
		if (rtk_bt_tmap_broadcast_media_receiver_cfg(channel, NULL)) {
			BLETMAP_AT_PRINTK("TMAP broadcast media receiver config channel fail \r\n");
			return -1;
		}
		BLETMAP_AT_PRINTK("TMAP broadcast media receiver config channel successfully \r\n");
	}

	return 0;
}

static int atcmd_bt_tmap_unicast_media_receiver_cfg(int argc, char **argv)
{
	(void)argc;
	uint8_t channel = 0;
	uint8_t neighbor_addr[6] = {0};

	if (strcmp(argv[0], "left") == 0) {
		/* RTK_BT_LE_AUDIO_LEFT */
		channel = 1;
	} else if (strcmp(argv[0], "right") == 0) {
		/* RTK_BT_LE_AUDIO_RIGHT */
		channel = 2;
	} else if (strcmp(argv[0], "stereo") == 0) {
		/* RTK_BT_LE_AUDIO_STEREO */
		channel = 3;
	} else {
		BLETMAP_AT_PRINTK("Unknown channel allocation");
		return -1;
	}
	if (argc == 2) {
		hexdata_str_to_bd_addr(argv[1], neighbor_addr, 6);
		if (rtk_bt_tmap_unicast_media_receiver_cfg(channel, neighbor_addr)) {
			BLETMAP_AT_PRINTK("TMAP unicast media receiver config channel neighbor address fail \r\n");
			return -1;
		}
		BLETMAP_AT_PRINTK("TMAP unicast media receiver config channel and neighbor address successfully \r\n");
	} else {
		if (rtk_bt_tmap_unicast_media_receiver_cfg(channel, NULL)) {
			BLETMAP_AT_PRINTK("TMAP unicast media receiver config channel fail \r\n");
			return -1;
		}
		BLETMAP_AT_PRINTK("TMAP unicast media receiver config channel successfully \r\n");
	}

	return 0;
}

static const cmd_table_t tmap_broadcast_media_receiver_cmd_table[] = {
	{"cfg",       atcmd_bt_tmap_broadcast_media_receiver_cfg,        2, 3},
	{NULL,},
};

static const cmd_table_t tmap_unicast_media_receiver_cmd_table[] = {
	{"cfg",       atcmd_bt_tmap_unicast_media_receiver_cfg,          2, 3},
	{NULL,},
};

int atcmd_bt_tmap_cmd(int argc, char *argv[])
{
	int ret = 0;
#if (!defined(ATCMD_BT_CUT_DOWN) || !ATCMD_BT_CUT_DOWN)
	char tag[80] = "[AT+BLETMAP]";
#else
	char tag[80] = "[ATBC][tmap]";
#endif
	if (strcmp(argv[0], "bmr") == 0) {
		BLETMAP_AT_PRINTK("Set tmap broadcast media receiver cmd");
		strcat(tag, "[bmr]");
		ret = atcmd_bt_excute(argc - 1, &argv[1], tmap_broadcast_media_receiver_cmd_table, tag);
	} else if (strcmp(argv[0], "umr") == 0) {
		BLETMAP_AT_PRINTK("Set tmap unicast media receiver cmd");
		strcat(tag, "[umr]");
		ret = atcmd_bt_excute(argc - 1, &argv[1], tmap_unicast_media_receiver_cmd_table, tag);
	} else {
		AT_PRINTK("[%s]Error: tmap do not support %s \r\n", __func__, argv[0]);
		ret = -1;
	}

	return ret;
}
#endif