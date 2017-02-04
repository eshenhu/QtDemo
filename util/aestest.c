#include <stdio.h>
#include <stdlib.h>
#include "aes.h"

int main(int argc, char *argv[])
{
	int i;
#define SZ_KEY		AES_KEYSIZE_128
	int key[SZ_KEY] = {
		0x37,0xFF,0xDA,0x05		,0x42,0x57,0x38,0x38
		,0x37,0x64,0x14,0x43	,0x37,0xFF,0xDA,0x05
// 		0x1,0x2,0x3,0x4d,0x56,0x10,0x11,0x12,
// 		0x1,0x2,0x3,0x4d,0x56,0x10,0x11,0x12
	};
	unsigned char encrypt[16+1] = {
		0x01,0x01,0x01,0x01		,0x01,0x01,0x01,0x01
		,0x01,0x01,0x01,0x01	,0x01,0x01,0x01,0x01
		,0x00};
	unsigned char decrypt[16+1]= {0};    
	unsigned char decrypt2[16+1]= {0};    
	struct crypto_aes_ctx ctx;
	crypto_aes_expand_key(&ctx,(u8 *)key,SZ_KEY);
#if 1
	aes_encrypt(&ctx,decrypt,encrypt);
	printf("加密前:%s\n加密后:",encrypt);
	for(i=0;i<16;i++)
	{
		printf("%02x",decrypt[i]);
	}
	printf("\n");
#endif
	aes_decrypt(&ctx,decrypt2,decrypt);
	printf("解密前:");
	for(i=0;i<16;i++)
	{
		printf("%02x",decrypt[i]);
	}
	printf("\n");
	printf("解密后:%s\n",decrypt2);

	printf("\n");

	return 0;
}

#if 0

#include <iostream>
#include <windows.h>
#include <WINCRYPT.H>
#pragma comment(lib, "advapi32.lib")

int main()
{
	HCRYPTPROV hProvider = 0;
	
	if (!::CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT ))
		return 1;
	
	const DWORD dwLength = 8;
	BYTE pbBuffer[dwLength] = {};
	
	if (!::CryptGenRandom(hProvider, dwLength, pbBuffer))
	{
		::CryptReleaseContext(hProvider, 0);
		return 1;
	}
	
	for (DWORD i = 0; i < dwLength; ++i)
		std::cout << std::hex << static_cast<unsigned int>(pbBuffer[i]) << std::endl;
	
	if (!::CryptReleaseContext(hProvider, 0))
		return 1;
}

#endif
