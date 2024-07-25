/*
* Copyright (c) 2024 Realtek Semiconductor Corp.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include "platform_autoconf.h"
#include <wifi_conf.h>

struct wifi_user_conf wifi_user_config __attribute__((aligned(64)));

_WEAK void wifi_set_user_config(void)
{
	memset(&wifi_user_config, 0, sizeof(struct wifi_user_conf));

	/* below items for user config, for details, see wifi_user_conf in wifi_intf_drv_to_app_basic.h */
	wifi_user_config.concurrent_enabled = (u8)_TRUE;
	wifi_user_config.softap_addr_offset_idx = 1;
	wifi_user_config.auto_reconnect_count = 8;
	wifi_user_config.auto_reconnect_interval = 5;
#ifdef CONFIG_HIGH_TP_TEST /*enable high tp in make menuconfig*/
	wifi_user_config.skb_num_np = 10;
	wifi_user_config.skb_num_ap = 8;
	wifi_user_config.rx_ampdu_num = 8;
#else
	wifi_user_config.skb_num_np = 6; /*4 for rx_ampdu + 2 for mgnt trx*/
	wifi_user_config.skb_num_ap = 8; /*adjust to 8 for ping 10k*/
	wifi_user_config.rx_ampdu_num = 4;
#endif
#ifdef CONFIG_SINGLE_CORE_WIFI
	wifi_user_config.skb_num_ap = 0;
#endif
	wifi_user_config.skb_buf_size = 0;
	wifi_user_config.wifi_wpa_mode_force = WPA_AUTO_MODE;

	/*Regulatory related*/
	wifi_user_config.country_code[0] = 0;
	wifi_user_config.country_code[1] = 0;
	wifi_user_config.rtw_tx_pwr_lmt_enable = 2;
	wifi_user_config.rtw_tx_pwr_by_rate	= 2;
	wifi_user_config.rtw_802_11d_en = DISABLE;
	wifi_user_config.rtw_trp_tis_cert_en = RTW_TRP_TIS_DISABLE;
	wifi_user_config.rtw_edcca_mode = RTW_EDCCA_NORM;
	wifi_user_config.tdma_dig_enable = 0; /*add for customer ctrl tdma dig on/off*/
	wifi_user_config.antdiv_mode = RTW_ANTDIV_DISABLE;

	/* IPS(Inactive Power Save), power save when wifi unconnected */
	wifi_user_config.ips_enable = 1;
	wifi_user_config.ips_level = IPS_WIFI_OFF;

	/* LPS(Leisure Power Save), power save when wifi connected, has 2 mode - legacy and uapsd, uapsd not support right now */
	wifi_user_config.lps_enable = 1;
	wifi_user_config.lps_mode = PS_MODE_LEGACY;
	wifi_user_config.legacy_ps_listen_interval = 0;

	/* Softap related */
	wifi_user_config.ap_sta_num = 5;	/*should not exceed AP_STA_NUM */
	wifi_user_config.ap_polling_sta = 0;

	/* MISC */
	wifi_user_config.en_mcc = (u8) DISABLE;
	wifi_user_config.max_roaming_times = 2;
	wifi_user_config.ampdu_rx_enable = (u8)_TRUE;
	wifi_user_config.ampdu_tx_enable = (u8)_TRUE;
	wifi_user_config.bCheckDestAddress = (u8)_TRUE;
	wifi_user_config.ap_compatibilty_enabled = 0x0B;
	wifi_user_config.set_channel_api_do_rfk = 0;
	wifi_user_config.rf_calibration_disable = 0;
	wifi_user_config.tx_shortcut_enable = 1;
	wifi_user_config.rx_shortcut_enable = 1;
	wifi_user_config.keepalive_interval = 20;

	/* WPS */
	wifi_user_config.wps_retry_count = 4;
	wifi_user_config.wps_retry_interval = 5000;
}

