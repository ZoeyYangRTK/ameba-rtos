/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
#include "ameba_secure_boot.h"
#include "bootloader_km4.h"
#include "boot_ota_km4.h"

static const char *TAG = "BOOT";
static Certificate_TypeDef Cert[2]; //Certificate of SlotA & SlotB
static Manifest_TypeDef Manifest[2]; //Manifest of SlotA & SlotB
u8 Signature[2][SIGN_MAX_LEN];
s64 Ver[2] = {0};  //32-bit full version

//static SubImgInfo_TypeDef SubImgInfo[12]; //store sub image addr and length

static u32 ImagePattern[2] = {
	0x35393138,
	0x31313738,
};

static u8 ValidIMGNum = 0;
/*
* @brif	OTA start address. Because KR4 & KM4 IMG2 are combined, users only need to set the start address
*	of KR4 IMG2.
*  In each entry, the first address is OTA1 address(SlotA address), the second address is OTA2 address (SlotB address)
*/
u32 OTA_Region[3][2] = {0};

/**
  * @brief  Get Version Number from OTP
  * @param  none
  * @retval bootloader version in OTP
  */
static u32 SYSCFG_OTP_GetRollbackVer(void)
{
	u32 tmp, Anti_VerNum = 0;

	/* Cert/app version is same, so Anti_VerNum for cert/app can be same */
	if (SYSCFG_OTP_AntiRollback()) {
		tmp = HAL_READ16(OTPC_REG_BASE, SEC_BOOT_IMGVER0);

		for (int i = 0; i < 16; i++) {
			if ((tmp & BIT(i)) == 0) {
				Anti_VerNum++;
			}
		}

		/*Only Check MajorImgVer with Anti_VerNum in otp*/
		Anti_VerNum = Anti_VerNum << 16;
	}

	return Anti_VerNum;


}

#define SYSCFG_OTP_GetRollbackVerIMG SYSCFG_OTP_GetRollbackVer

/**
  * @brief  check Certificate pattern and version
  * @param none
  * @retval 0:Slot A  1:Slot B  2:V1 = V2
  */
u8 BOOT_OTA_SlotSelect(void)
{
	u16 MajorVer[2] = {0}; //16-bit major
	u16 MinorVer[2] = {0}; //16-bit minor
	u32 Vertemp;
	s64 VN_CERT; //32-bit full version in OTP
	u8 ImgIndex, i;

	/* ----1 load certificate(Slot A & Slot B) from flash to SRAM, get full version and check validation---- */
	for (i = 0; i < 2; i++) {
		_memcpy((void *)&Cert[i], (void *)OTA_Region[IMG_CERT][i], sizeof(Certificate_TypeDef));
		_memcpy((void *)&Signature[i], (void *)(OTA_Region[IMG_CERT][i] + Cert[i].TableSize), SIGN_MAX_LEN);
		_memcpy((void *)&Manifest[i], (void *)OTA_Region[IMG_IMG2][i], sizeof(Manifest_TypeDef)); // load img2 manifest together

		if (_memcmp(Cert[i].Pattern, ImagePattern, sizeof(ImagePattern)) == 0) {
			MajorVer[i] = (u16)Cert[i].MajorKeyVer;
			MinorVer[i] = (u16)Cert[i].MinorKeyVer;
			Vertemp = (MajorVer[i] << 16) | MinorVer[i]; // get 32-bit full version number
			Ver[i] = (s64)Vertemp;
		} else {
			Ver[i] = -1;
		}
	}

	/* ---------------------------------2 Get bootldr version in OTP if anti-rollback --------------------------------- */
	VN_CERT = (s64)SYSCFG_OTP_GetRollbackVer();

	/* ---------------------------------3 Check full KeyVerion in certificate--------------------------------- */
	if ((VN_CERT > Ver[0]) && (VN_CERT > Ver[1])) {
		ValidIMGNum = NONEVALIDIMG;
	} else if ((VN_CERT <= Ver[0]) && (VN_CERT <= Ver[1])) {
		ValidIMGNum = TWOVALIDIMG;
	} else {
		ValidIMGNum = ONEVALIDIMG;
	}

	ImgIndex = (Ver[0] >= Ver[1]) ? BOOT_FROM_OTA1 : BOOT_FROM_OTA2;
	return ImgIndex;

}


u8 BOOT_LoadSubImage(SubImgInfo_TypeDef *SubImgInfo, u32 StartAddr, u8 Num, char **ImgName, u8 ErrLog)
{
	IMAGE_HEADER ImgHdr;
	u32 DstAddr, Len;
	u32 i;
	RRAM_TypeDef *rram = RRAM_DEV;

	for (i = 0; i < Num; i++) {
		_memcpy((void *)&ImgHdr, (void *)StartAddr, IMAGE_HEADER_LEN);

		if ((ImgHdr.signature[0] != 0x35393138) || (ImgHdr.signature[1] != 0x31313738)) {
			if (ErrLog == _TRUE) {
				RTK_LOGE(TAG, "%s Invalid\n", ImgName[i]);
			}
			return _FALSE;
		}

		DstAddr = ImgHdr.image_addr - IMAGE_HEADER_LEN;
		Len = ImgHdr.image_size + IMAGE_HEADER_LEN;
		if (_strcmp(ImgName[i], "DSP DTCM0") == 0 || _strcmp(ImgName[i], "DSP DTCM1") == 0) {
			DstAddr = StartAddr;
			if (_strcmp(ImgName[i], "DSP DTCM0") == 0) {
				rram->DSPIMG_DTCM0_ADDR = StartAddr;
			} else if (_strcmp(ImgName[i], "DSP DTCM1") == 0) {
				rram->DSPIMG_DTCM1_ADDR = StartAddr;
			}
		} else {
			/* If not XIP sub-image, then copy it to specific address (include the IMAGE_HEADER)*/
			if ((!IS_FLASH_ADDR(DstAddr)) && (Len > IMAGE_HEADER_LEN)) {
				_memcpy((void *)DstAddr, (void *)StartAddr, Len);
				DCache_CleanInvalidate(DstAddr, Len);
			}

			/* empty Image, Just put in flash, for image hash later */
			if (Len == IMAGE_HEADER_LEN) {
				DstAddr = StartAddr;
			}
		}
		if (SubImgInfo != NULL) {
			SubImgInfo[i].Addr = DstAddr;
			SubImgInfo[i].Len = Len;

			RTK_LOGI(TAG, "%s[%08lx:%lx]\n", ImgName[i], DstAddr, Len);
		}

		StartAddr += Len;
	}

	return _TRUE;
}


void BOOT_RSIPIvSet(Manifest_TypeDef *Manifest, u32 IV_index)
{
	u8 iv[8] = {0};

	/* 1. check if RSIP enable */
	if (SYSCFG_OTP_RSIPEn() == FALSE) {
		return;
	}

	/* get iv from manifest */
	_memcpy(iv, Manifest->RsipIV, 8);
	RSIP_IV_Set(IV_index, iv);

}


/* start addr --> logical addr */
void BOOT_OTFCheck(u32 start_addr, u32 end_addr, u32 IV_index, u32 OTF_index)
{
	u32 mode;

	/* 1. check if RSIP enable */
	if (SYSCFG_OTP_RSIPEn() == FALSE) {
		return;
	}

	if (IS_FLASH_ADDR(start_addr) && IS_FLASH_ADDR(end_addr)) {
		RTK_LOGI(TAG, "IMG2 OTF EN\n");
		mode = SYSCFG_OTP_RSIPMode();

		switch (mode) {
		case RSIP_XTS_MODE:
			mode = OTF_XTS_MODE;
			break;
		case RSIP_CTR_MODE:
			mode = OTF_CTR_MODE;
			break;
		default:
			RTK_LOGE(TAG, "OTF Mode error\r\n");
			return;
		}

		RSIP_OTF_Enable(OTF_index, start_addr, end_addr, ENABLE, IV_index, mode);
		RSIP_OTF_Cmd(ENABLE);
	}

	return;
}


/* Load km0/km4 img and check pattern if secure boot enabled*/
u8 BOOT_OTA_LoadIMG2(u8 ImgIndex)
{
	SubImgInfo_TypeDef SubImgInfo[18];
	u32 LogAddr, PhyAddr, ImgAddr, TotalLen = 0;
	u8 Cnt;
	u8 i;
	u32 Index = 0;
	u8 ret;

	char *Kr4Label[] = {"KR4 XIP IMG", "KR4 PSRAM", "KR4 SRAM", "KR4 BOOT", "KR4 PMC"};
	char *Km4Label[] = {"KM4 XIP IMG", "KM4 PSRAM", "KM4 SRAM", "KM4 BOOT", "KM4 PMC"};

	PhyAddr = OTA_Region[IMG_IMG2][ImgIndex];

	/* set IMG2 IV */
	BOOT_RSIPIvSet(&Manifest[ImgIndex], 1);

	/* remap KR4 XIP image */
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	LogAddr = (u32)__kr4_flash_text_start__ - IMAGE_HEADER_LEN;

	RSIP_MMU_Config(1, LogAddr, LogAddr + 0x01000000 - 0x20, PhyAddr);
	RSIP_MMU_Cmd(1, ENABLE);
	RSIP_MMU_Cache_Clean();
	/*KR4 IMG2 RSIP configurations*/
	BOOT_OTFCheck(LogAddr, LogAddr + 0x01000000 - 0x20, 1, 1);


	/* KR4 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Kr4Label) / sizeof(char *);
	ImgAddr = LogAddr;
	if (BOOT_LoadSubImage(&SubImgInfo[Index], ImgAddr, Cnt, Kr4Label, _TRUE) == FALSE) {
		return FALSE;
	}

	for (i = 0; i < Cnt; i++) {
		TotalLen += SubImgInfo[i].Len;
	}
	Index += Cnt;

	/* add 4K-align padding info */
	SubImgInfo[Index].Addr = ImgAddr + TotalLen;
	SubImgInfo[Index].Len = ((((TotalLen - 1) >> 12) + 1) << 12) - TotalLen;
	TotalLen += SubImgInfo[Index].Len;
	Index ++;

	/* remap KM4 XIP image */
	PhyAddr += TotalLen;
	LogAddr = (u32)__km4_flash_text_start__ - IMAGE_HEADER_LEN;

	RSIP_MMU_Config(2, LogAddr, LogAddr + 0x01000000 - 0x20, PhyAddr);
	RSIP_MMU_Cmd(2, ENABLE);
	RSIP_MMU_Cache_Clean();

	BOOT_OTFCheck(LogAddr, LogAddr + 0x01000000 - 0x20, 1, 2);

	/* KM4 XIP & SRAM, read with virtual addr in case of encryption */
	Cnt = sizeof(Km4Label) / sizeof(char *);
	ImgAddr = LogAddr;
	if (BOOT_LoadSubImage(&SubImgInfo[Index], ImgAddr, Cnt, Km4Label, _TRUE) == FALSE) {
		return FALSE;
	}
	Index += Cnt;

	/* check if RDP enable */
	if (SYSCFG_OTP_RDPEn() == TRUE) {
		/* Load from OTA and ECC check for IMG3 */
		RTK_LOGI(TAG, "RDP EN\n");
		Cnt = BOOT_OTA_RDP(SubImgInfo, Index, ImgIndex);
		Index += Cnt;
	} else {
#if defined (CONFIG_TRUSTZONE_EN) && (CONFIG_TRUSTZONE_EN == 1U)
		RTK_LOGE(TAG, "RDP Shall En when TZ Configed.\n");
		assert_param(FALSE);
#endif
	}

	if (DSP_WITHIN_IMG2_Enable) {
		/* This Function Must be Called in order to Config Index */
		Cnt = BOOT_LoadDSPImg_FromImg2(SubImgInfo, Index, OTA_Region[IMG_IMG2][ImgIndex]);
		Index += Cnt;
	} else {
		BOOT_LoadDSP_SetCert(&Cert[ImgIndex]);
	}

	assert_param(Index <= sizeof(SubImgInfo) / sizeof(SubImgInfo_TypeDef));

	/* IMG2(KR4 and KM4)+(IMG3)+(DSP) ECC verify if need */
	ret = BOOT_SignatureCheck(&Manifest[ImgIndex], SubImgInfo, Index, &Cert[ImgIndex], KEYID_NSPE);

	return ret;
}

void BOOT_OTA_Region_Init(void)
{
	flash_get_layout_info(IMG_APP_OTA1, &OTA_Region[IMG_CERT][0], NULL);
	flash_get_layout_info(IMG_APP_OTA2, &OTA_Region[IMG_CERT][1], NULL);
	flash_get_layout_info(IMG_DSP, &OTA_Region[IMG_DSPIMG][0], NULL);
	flash_get_layout_info(IMG_DSP, &OTA_Region[IMG_DSPIMG][1], NULL);

	OTA_Region[IMG_IMG2][0] = OTA_Region[IMG_CERT][0] + 0x1000;
	OTA_Region[IMG_IMG2][1] = OTA_Region[IMG_CERT][1] + 0x1000;
}

u8 BOOT_OTA_IMG2(void)
{
	u8 ImgIndex = 0;
	u8 ret, i;
	u32 version;

	/* step 1: init OTA region */
	BOOT_OTA_Region_Init();

	/* step2: Select Slot according to Cert Version */
	ImgIndex = BOOT_OTA_SlotSelect();

	/* step2.1: boot fail if no valid, trap */
	if (ValidIMGNum == NONEVALIDIMG) {
		goto Fail;
	}
	/* step3: check img from selected idx */
	for (i = 0; i < ValidIMGNum; i++) {

		/* step3.1: check cert if sboot enabled */
		ret = BOOT_CertificateCheck(&Cert[ImgIndex], ImgIndex);

		/* step3.2: try another cert if bigger ver failed and smaller ver is valid */
		if (ret != TRUE) {
			ImgIndex = (ImgIndex + 1) % 2;
			continue;
		}

		/* step3.3: load and check img2 if cert passed */
		ret = BOOT_OTA_LoadIMG2(ImgIndex);

		/* step3.4: try another ver from cert if valid ver exsit */
		if (ret != TRUE) {
			ImgIndex = (ImgIndex + 1) % 2;
			continue;
		} else {
			break;
		}
	}

	/* step4: check if boot pass */
	if (i == ValidIMGNum) {
		goto Fail;
	}
	version = (u32)(Ver[ImgIndex] & 0xFFFFFFFF);

	RTK_LOGI(TAG, "IMG2 BOOT from OTA %d, Version: %lx.%lx \n", ImgIndex + 1, ((version >> 16) & 0xFFFF), (version & 0xFFFF));
	return ImgIndex;


Fail:
	RTK_LOGE(TAG, "OTA Certificate & IMG2 invalid, BOOT FAIL!!\n");

	/* clear stack */
	__set_MSP(MSP_RAM_HP);
	_memset((void *)MSPLIM_RAM_HP, 0, MSP_RAM_HP - 128 - MSPLIM_RAM_HP);
	DCache_CleanInvalidate(MSPLIM_RAM_HP, MSP_RAM_HP - 128 - MSPLIM_RAM_HP);

	while (1) {
		DelayMs(1000);
	}

	return 0;
}


u8 BOOT_OTA_RDP(SubImgInfo_TypeDef *SubImgInfo, u8 Index, u8 ImgIndex)
{
	u8 i;
	u8 Cnt = 0;
	u32 PhyAddr;
	u32 Img2Addr = OTA_Region[IMG_IMG2][ImgIndex];

	/* calculate rdp phy addr */
	PhyAddr = Img2Addr;
	PhyAddr += MANIFEST_SIZE_4K_ALIGN;
	for (i = 0; i < Index; i++) {
		PhyAddr += SubImgInfo[i].Len;
	}

	// Check sub-image pattern and load RDP sub-image
	if ((Cnt = BOOT_LoadRDPImg(&Manifest[ImgIndex], &SubImgInfo[Index], PhyAddr)) == FALSE) {
		goto Fail;
	}

	return Cnt;

Fail:
	RTK_LOGE(TAG, "Fail to load RDP image!\n");
	while (1) {
		DelayMs(1000);
	}
}

