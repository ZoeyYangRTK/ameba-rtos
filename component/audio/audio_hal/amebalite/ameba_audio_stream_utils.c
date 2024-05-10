/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ameba_audio_stream_utils.h"

#include "platform_stdlib.h"
#include "basic_types.h"
#include "ameba_sport.h"
#include "ameba_audio.h"
#include "ameba_audio_stream.h"
#include "ameba_audio_types.h"

#include "audio_hw_debug.h"
#include "audio_hw_osal_errnos.h"

AUD_TypeDef *g_audio_analog = AUD_SYS_BASE;
static bool g_native_time = true;

uint32_t ameba_audio_get_channel(uint32_t channel_count)
{
	uint32_t tmp;
	switch (channel_count) {
	case 1:
		tmp = SP_CH_MONO;
		break;
	case 2:
	case 4:
	case 6:
	case 8:
		tmp = SP_CH_STEREO;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] invalid format");
		return -1;
	}
	return tmp;
}

uint32_t ameba_audio_get_sp_tdm(uint32_t channel_count)
{
	uint32_t tmp;
	switch (channel_count) {
	case 1:
	case 2:
		tmp = SP_TX_NOTDM;
		break;
	case 4:
		tmp = SP_TX_TDM4;
		break;
	case 6:
		tmp = SP_TX_TDM6;
		break;
	case 8:
		tmp = SP_TX_TDM8;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] invalid format");
		return -1;
	}
	return tmp;
}

uint32_t ameba_audio_get_codec_tdm(uint32_t channel_count)
{
	uint32_t tmp;
	switch (channel_count) {
	case 1:
	case 2:
		tmp = I2S_NOTDM;
		break;
	case 4:
		tmp = I2S_TDM4;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] invalid format");
		return -1;
	}
	return tmp;
}

uint32_t ameba_audio_get_fifo_num(uint32_t channel_count)
{
	uint32_t tmp;
	switch (channel_count) {
	case 1:
	case 2:
		tmp = SP_RX_FIFO2;
		break;
	case 4:
		tmp = SP_RX_FIFO4;
		break;
	case 6:
		tmp = SP_RX_FIFO6;
		break;
	case 8:
		tmp = SP_RX_FIFO8;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] invalid format");
		return -1;
	}
	return tmp;
}

int ameba_audio_get_sp_format(AudioHwFormat format, uint32_t direction)
{
	int tmp = -1;
	switch (format) {
	case AUDIO_HW_FORMAT_PCM_32_BIT:
		if (direction == STREAM_OUT) {
			tmp = SP_TXWL_32;
		} else {
			tmp = SP_RXWL_32;
		}
		break;
	case AUDIO_HW_FORMAT_PCM_8_24_BIT:
		if (direction == STREAM_OUT) {
			tmp = SP_TXWL_24;
		} else {
			tmp = SP_RXWL_24;
		}
		break;
	case AUDIO_HW_FORMAT_PCM_16_BIT:
		if (direction == STREAM_OUT) {
			tmp = SP_TXWL_16;
		} else {
			tmp = SP_RXWL_16;
		}
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] invalid format");
		return -1;
	}
	return tmp;
}

int ameba_audio_get_sp_rate(uint32_t rate)
{
	int tmp = -1;
	switch (rate) {
	case 8000:
		tmp = SP_8K;
		break;
	case 11025:
		tmp = SP_11P025K;
		break;
	case 16000:
		tmp = SP_16K;
		break;
	case 22050:
		tmp = SP_22P05K;
		break;
	case 24000:
		tmp = SP_24K;
		break;
	case 32000:
		tmp = SP_32K;
		break;
	case 44100:
		tmp = SP_44P1K;
		break;
	case 48000:
		tmp = SP_48K;
		break;
	case 88200:
		tmp = SP_88P2K;
		break;
	case 96000:
		tmp = SP_96K;
		break;
	case 176400:
		tmp = SP_176P4K;
		break;
	case 192000:
		tmp = SP_192K;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] invalid rate");
		return -1;
	}
	return tmp;
}

int ameba_audio_get_codec_format(AudioHwFormat format, uint32_t direction)
{
	int tmp = -1;
	(void)direction;
	switch (format) {
	case AUDIO_HW_FORMAT_PCM_8_24_BIT:
		tmp = WL_24;
		break;
	case AUDIO_HW_FORMAT_PCM_16_BIT:
		tmp = WL_16;
		break;
	case AUDIO_HW_FORMAT_PCM_8_BIT:
		tmp = WL_8;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] invalid format");
		return -1;
	}
	return tmp;
}

int ameba_audio_get_codec_rate(uint32_t rate)
{
	int tmp = -1;
	switch (rate) {
	case 8000:
		tmp = SR_8K;
		break;
	case 11025:
		tmp = SR_11P025K;
		break;
	case 16000:
		tmp = SR_16K;
		break;
	case 22050:
		tmp = SR_22P05K;
		break;
	case 24000:
		tmp = SR_24K;
		break;
	case 32000:
		tmp = SR_32K;
		break;
	case 44100:
		tmp = SR_44P1K;
		break;
	case 48000:
		tmp = SR_48K;
		break;
	case 88200:
		tmp = SR_88P2K;
		break;
	case 96000:
		tmp = SR_96K;
		break;
	case 192000:
		tmp = SR_192K;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] invalid rate");
		return -1;
	}
	return tmp;
}

uint32_t ameba_audio_stream_get_mic_idx(uint32_t mic_category)
{
	uint32_t mic_num = 0;
	switch (mic_category) {
	case AMEBA_AUDIO_AMIC1:
		mic_num = AMIC1;
		break;
	case AMEBA_AUDIO_AMIC2:
		mic_num = AMIC2;
		break;
	case AMEBA_AUDIO_AMIC3:
		mic_num = AMIC3;
		break;
	case AMEBA_AUDIO_DMIC1:
		mic_num = DMIC1;
		break;
	case AMEBA_AUDIO_DMIC2:
		mic_num = DMIC2;
		break;
	case AMEBA_AUDIO_DMIC3:
		mic_num = DMIC3;
		break;
	case AMEBA_AUDIO_DMIC4:
		mic_num = DMIC4;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] mic category %ld not supported", mic_category);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	return mic_num;
}

uint32_t ameba_audio_stream_get_adc_chn_idx(uint32_t index)
{
	uint32_t adc_chn = 0;
	switch (index) {
	case 1:
		adc_chn = ADCHN1;
		break;
	case 2:
		adc_chn = ADCHN2;
		break;
	case 3:
		adc_chn = ADCHN3;
		break;
	case 4:
		adc_chn = ADCHN4;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] adc channel index: %ld not supported", index);
		break;
	}

	return adc_chn;
}

uint32_t ameba_audio_stream_get_adc_idx(uint32_t index)
{
	uint32_t adc_num = 0;
	switch (index) {
	case 1:
		adc_num = ADC1;
		break;
	case 2:
		adc_num = ADC2;
		break;
	case 3:
		adc_num = ADC3;
		break;
	case 4:
		adc_num = ADC4;
		break;
	default:
		HAL_AUDIO_ERROR("[AmebaAudioUtils] adc index: %ld not supported", index);
		break;
	}

	return adc_num;
}

void ameba_audio_dump_gdma_regs(u8 GDMA_ChNum)
{
	GDMA_TypeDef *GDMA = ((GDMA_TypeDef *) GDMA_BASE);

	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].SAR:%lx\r\n", GDMA_ChNum, GDMA->CH[GDMA_ChNum].SAR);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].DAR:%lx\r\n", GDMA_ChNum, GDMA->CH[GDMA_ChNum].DAR);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].CTL_LOW:%lx\r\n", GDMA_ChNum, GDMA->CH[GDMA_ChNum].CTL_LOW);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].CTL_HIGH:%lx\r\n", GDMA_ChNum, GDMA->CH[GDMA_ChNum].CTL_HIGH);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].CFG_LOW:%lx\r\n", GDMA_ChNum, GDMA->CH[GDMA_ChNum].CFG_LOW);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].CFG_HIGH:%lx\r\n", GDMA_ChNum, GDMA->CH[GDMA_ChNum].CFG_HIGH);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].ChEnReg:%lx\r\n", GDMA_ChNum, GDMA->ChEnReg);

	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].MASK_TFR:%lx\r\n", GDMA_ChNum, GDMA->MASK_TFR);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].MASK_BLOCK:%lx\r\n", GDMA_ChNum, GDMA->MASK_BLOCK);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].MASK_ERR:%lx\r\n", GDMA_ChNum, GDMA->MASK_ERR);
	HAL_AUDIO_DUMP_INFO("GDMA->CH[%d].STATUS_BLOCK:%lx\r\n", GDMA_ChNum, GDMA->STATUS_BLOCK);

}

void ameba_audio_dump_sport_regs(uint32_t SPORTx)
{
	uint32_t tmp;
	AUDIO_SPORT_TypeDef *sportx = (AUDIO_SPORT_TypeDef *)SPORTx;
	tmp = sportx->SP_REG_MUX;
	HAL_AUDIO_DUMP_INFO("REG_SP_REG_MUX:%lx \n", tmp);
	tmp = sportx->SP_CTRL0;
	HAL_AUDIO_DUMP_INFO("REG_SP_CTRL0:%lx \n", tmp);
	tmp = sportx->SP_CTRL1;
	HAL_AUDIO_DUMP_INFO("REG_SP_CTRL1:%lx \n", tmp);
	tmp = sportx->SP_INT_CTRL;
	HAL_AUDIO_DUMP_INFO("REG_SP_INT_CTRL:%lx \n", tmp);
	tmp = sportx->RSVD0;
	HAL_AUDIO_DUMP_INFO("REG_RSVD0:%lx \n", tmp);
	tmp = sportx->SP_TRX_COUNTER_STATUS;
	HAL_AUDIO_DUMP_INFO("REG_SP_TRX_COUNTER_STATUS:%lx \n", tmp);
	tmp = sportx->SP_ERR;
	HAL_AUDIO_DUMP_INFO("REG_SP_ERR:%lx \n", tmp);
	tmp = sportx->SP_SR_TX_BCLK;
	HAL_AUDIO_DUMP_INFO("REG_SP_SR_TX_BCLK:%lx \n", tmp);
	tmp = sportx->SP_TX_LRCLK;
	HAL_AUDIO_DUMP_INFO("REG_SP_TX_LRCLK:%lx \n", tmp);
	tmp = sportx->SP_FIFO_CTRL;
	HAL_AUDIO_DUMP_INFO("REG_SP_FIFO_CTRL:%lx \n", tmp);
	tmp = sportx->SP_FORMAT;
	HAL_AUDIO_DUMP_INFO("REG_SP_FORMAT:%lx \n", tmp);
	tmp = sportx->SP_RX_BCLK;
	HAL_AUDIO_DUMP_INFO("REG_SP_RX_BCLK:%lx \n", tmp);
	tmp = sportx->SP_RX_LRCLK;
	HAL_AUDIO_DUMP_INFO("REG_SP_RX_LRCLK:%lx \n", tmp);
	tmp = sportx->SP_DSP_COUNTER;
	HAL_AUDIO_DUMP_INFO("REG_SP_DSP_COUNTER:%lx \n", tmp);
	tmp = sportx->RSVD1;
	HAL_AUDIO_DUMP_INFO("REG_RSVD1:%lx \n", tmp);
	tmp = sportx->SP_DIRECT_CTRL0;
	HAL_AUDIO_DUMP_INFO("REG_SP_DIRECT_CTRL0:%lx \n", tmp);
	tmp = sportx->RSVD2;
	HAL_AUDIO_DUMP_INFO("REG_RSVD2:%lx \n", tmp);
	tmp = sportx->SP_FIFO_IRQ;
	HAL_AUDIO_DUMP_INFO("REG_SP_FIFO_IRQ:%lx \n", tmp);
	tmp = sportx->SP_DIRECT_CTRL1;
	HAL_AUDIO_DUMP_INFO("REG_SP_DIRECT_CTRL1:%lx \n", tmp);
	tmp = sportx->SP_DIRECT_CTRL2;
	HAL_AUDIO_DUMP_INFO("REG_SP_DIRECT_CTRL2:%lx \n", tmp);
	tmp = sportx->RSVD3;
	HAL_AUDIO_DUMP_INFO("REG_RSVD3:%lx \n", tmp);
	tmp = sportx->SP_DIRECT_CTRL3;
	HAL_AUDIO_DUMP_INFO("REG_SP_DIRECT_CTRL3:%lx \n", tmp);
	tmp = sportx->SP_DIRECT_CTRL4;
	HAL_AUDIO_DUMP_INFO("REG_SP_DIRECT_CTRL4:%lx \n", tmp);
	tmp = sportx->SP_RX_COUNTER1;
	HAL_AUDIO_DUMP_INFO("REG_SP_RX_COUNTER1:%lx \n", tmp);
	tmp = sportx->SP_RX_COUNTER2;
	HAL_AUDIO_DUMP_INFO("REG_SP_RX_COUNTER2:%lx \n", tmp);
	tmp = sportx->SP_TX_FIFO_0_WR_ADDR;
	HAL_AUDIO_DUMP_INFO("REG_SP_TX_FIFO_0_WR_ADDR:%lx \n", tmp);
	tmp = sportx->SP_RX_FIFO_0_RD_ADDR;
	HAL_AUDIO_DUMP_INFO("REG_SP_RX_FIFO_0_RD_ADDR:%lx \n", tmp);
	tmp = sportx->SP_TX_FIFO_1_WR_ADDR;
	HAL_AUDIO_DUMP_INFO("REG_SP_TX_FIFO_1_WR_ADDR:%lx \n", tmp);
	tmp = sportx->SP_RX_FIFO_1_RD_ADDR;
	HAL_AUDIO_DUMP_INFO("REG_SP_RX_FIFO_1_RD_ADDR:%lx \n", tmp);

}

void ameba_audio_dump_codec_regs(void)
{
	uint32_t tmp;
	AUDIO_TypeDef *audio_base;
	if (TrustZone_IsSecure()) {
		audio_base = (AUDIO_TypeDef *)AUDIO_REG_BASE_S;
	} else {
		audio_base = (AUDIO_TypeDef *)AUDIO_REG_BASE;
	}

	tmp = g_audio_analog->AUD_ADDA_CTL;
	HAL_AUDIO_DUMP_INFO("ADDA_CTL:%lx \n", tmp);
	tmp = g_audio_analog->AUD_LO_CTL;
	HAL_AUDIO_DUMP_INFO("LO_CTL:%lx \n", tmp);
	tmp = g_audio_analog->AUD_MICBIAS_CTL0;
	HAL_AUDIO_DUMP_INFO("MICBIAS_CTL0:%lx \n", tmp);
	tmp = g_audio_analog->AUD_MICBST_CTL0;
	HAL_AUDIO_DUMP_INFO("MICBST_CTL0:%lx \n", tmp);
	tmp = g_audio_analog->AUD_MICBST_CTL1;
	HAL_AUDIO_DUMP_INFO("MICBST_CTL1:%lx \n", tmp);
	tmp = g_audio_analog->RSVD0;
	HAL_AUDIO_DUMP_INFO("ANALOG_RSVD0:%lx \n", tmp);
	tmp = g_audio_analog->AUD_DTS_CTL;
	HAL_AUDIO_DUMP_INFO("DTS_CTL:%lx \n", tmp);
	tmp = g_audio_analog->AUD_MBIAS_CTL0;
	HAL_AUDIO_DUMP_INFO("MBIAS_CTL0:%lx \n", tmp);
	tmp = g_audio_analog->AUD_MBIAS_CTL1;
	HAL_AUDIO_DUMP_INFO("MBIAS_CTL1:%lx \n", tmp);
	tmp = g_audio_analog->AUD_MBIAS_CTL2;
	HAL_AUDIO_DUMP_INFO("MBIAS_CTL2:%lx \n", tmp);

	/***digital reg dump***/
	tmp = audio_base->CODEC_AUDIO_CONTROL_0;
	HAL_AUDIO_DUMP_INFO("CODEC_AUDIO_CONTROL_0:%lx \n", tmp);
	tmp = audio_base->CODEC_AUDIO_CONTROL_1;
	HAL_AUDIO_DUMP_INFO("CODEC_AUDIO_CONTROL_1:%lx \n", tmp);
	tmp = audio_base->CODEC_CLOCK_CONTROL_1;
	HAL_AUDIO_DUMP_INFO("CODEC_CLOCK_CONTROL_1:%lx \n", tmp);
	tmp = audio_base->CODEC_CLOCK_CONTROL_2;
	HAL_AUDIO_DUMP_INFO("CODEC_CLOCK_CONTROL_1:%lx \n", tmp);
	tmp = audio_base->CODEC_CLOCK_CONTROL_3;
	HAL_AUDIO_DUMP_INFO("CODEC_CLOCK_CONTROL_1:%lx \n", tmp);
	tmp = audio_base->CODEC_CLOCK_CONTROL_4;
	HAL_AUDIO_DUMP_INFO("CODEC_CLOCK_CONTROL_4:%lx \n", tmp);
	tmp = audio_base->CODEC_CLOCK_CONTROL_5;
	HAL_AUDIO_DUMP_INFO("CODEC_CLOCK_CONTROL_5:%lx \n", tmp);
	tmp = audio_base->CODEC_CLOCK_CONTROL_6;
	HAL_AUDIO_DUMP_INFO("CODEC_CLOCK_CONTROL_5:%lx \n", tmp);
	tmp = audio_base->CODEC_CLOCK_CONTROL_7;
	HAL_AUDIO_DUMP_INFO("CODEC_CLOCK_CONTROL_5:%lx \n", tmp);
	tmp = audio_base->CODEC_I2S_0_CONTROL;
	HAL_AUDIO_DUMP_INFO("CODEC_I2S_0_CONTROL:%lx \n", tmp);
	tmp = audio_base->CODEC_I2S_0_CONTROL_1;
	HAL_AUDIO_DUMP_INFO("CODEC_I2S_0_CONTROL_1:%lx \n", tmp);

	for (uint32_t i = 0; i < MAX_AD_NUM; i++) {
		tmp = audio_base->CODEC_ADC_CH_CTRL[i].CODEC_ADC_x_CONTROL_0;
		HAL_AUDIO_DUMP_INFO("CODEC_ADC_%lu_CONTROL_0:%lx", i, tmp);
		tmp = audio_base->CODEC_ADC_CH_CTRL[i].CODEC_ADC_x_CONTROL_1;
		HAL_AUDIO_DUMP_INFO("CODEC_ADC_%lu_CONTROL_1:%lx", i, tmp);
	}

	tmp = audio_base->CODEC_DAC_L_CONTROL_0;
	HAL_AUDIO_DUMP_INFO("CODEC_DAC_L_CONTROL_0:%lx \n", tmp);
	tmp = audio_base->CODEC_DAC_L_CONTROL_1;
	HAL_AUDIO_DUMP_INFO("CODEC_DAC_L_CONTROL_1:%lx \n", tmp);
	tmp = audio_base->CODEC_DAC_L_CONTROL_2;
	HAL_AUDIO_DUMP_INFO("CODEC_DAC_L_CONTROL_2:%lx \n", tmp);

}

void ameba_audio_set_native_time(void)
{
	g_native_time = true;
}

int64_t ameba_audio_get_now_ns(void)
{
	return rtos_time_get_current_system_time_ms() * 1000000LL;;
}

AUDIO_SPORT_TypeDef *ameba_audio_get_sport_addr(uint32_t index)
{
	AUDIO_SPORT_TypeDef *addr;
	switch (index) {
	case 0:
		addr = AUDIO_SPORT0_DEV;
		break;
	case 1:
		addr = AUDIO_SPORT1_DEV;
		break;
	default:
		HAL_AUDIO_ERROR("unsupported sport:%lu", index);
		addr = NULL;
		break;
	}
	return addr;
}

uint32_t ameba_audio_get_sport_irq(uint32_t index)
{
	uint32_t irq = 0;
	switch (index) {
	case 0:
		irq = SPORT0_IRQ;
		break;
	case 1:
		irq = SPORT1_IRQ;
		break;
	default:
		HAL_AUDIO_ERROR("unsupported sport:%lu", index);
		break;
	}
	return irq;
}

void ameba_audio_set_sp_data_out(uint32_t index)
{
	uint32_t tmp;
	tmp = HAL_READ32(PINMUX_REG_BASE, REG_I2S_CTRL);
	if (index == 1) {
		tmp |= PAD_BIT_SP1_DIO0_MUXSEL | PAD_BIT_SP1_DIO1_MUXSEL | PAD_BIT_SP1_DIO2_MUXSEL | PAD_BIT_SP1_DIO3_MUXSEL;
	} else if (index == 0) {
		tmp |= PAD_BIT_SP0_DIO0_MUXSEL | PAD_BIT_SP0_DIO1_MUXSEL | PAD_BIT_SP0_DIO2_MUXSEL | PAD_BIT_SP0_DIO3_MUXSEL;
	}
	HAL_WRITE32(PINMUX_REG_BASE, REG_I2S_CTRL, tmp);
}

void ameba_audio_set_sp_data_in(uint32_t index)
{
	uint32_t tmp;
	tmp = HAL_READ32(PINMUX_REG_BASE, REG_I2S_CTRL);
	if (index == 1) {
		tmp &= ~(PAD_BIT_SP1_DIO0_MUXSEL | PAD_BIT_SP1_DIO1_MUXSEL | PAD_BIT_SP1_DIO2_MUXSEL | PAD_BIT_SP1_DIO3_MUXSEL);
	} else if (index == 0) {
		tmp &= ~(PAD_BIT_SP0_DIO0_MUXSEL | PAD_BIT_SP0_DIO1_MUXSEL | PAD_BIT_SP0_DIO2_MUXSEL | PAD_BIT_SP0_DIO3_MUXSEL);
	}
	HAL_WRITE32(PINMUX_REG_BASE, REG_I2S_CTRL, tmp);
}
