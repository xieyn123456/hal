#include "CQRCode.h"

CQRCode::CQRCode()
{
}

CQRCode::~CQRCode()
{
}

/* generateStr是待编码的字符串
* dstBmpPath是编成二维码后生成的bmp路径名，如 ./my_qrcode.bmp
*/
string CQRCode::GenerateQrCode(string& generateStr)
{
	string dstBmpPath = "./QRCode.bmp";
	const char* szSourceSring = generateStr.c_str();
	unsigned int    unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
	unsigned char* pRGBData, * pSourceData, * pDestData;
	QRcode* pQRC;
	FILE* f;

	if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_H, QR_MODE_8, 1))
	{
		unWidth = pQRC->width;
		unWidthAdjusted = unWidth * 8 * 3;
		if (unWidthAdjusted % 4)
			unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
		unDataBytes = unWidthAdjusted * unWidth * 8;
		// Allocate pixels buffer
		if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
		{
			return string("");
		}
		// Preset to white
		memset(pRGBData, 0xff, unDataBytes);
		// Prepare bmp headers
		BITMAPFILEHEADER kFileHeader;
		kFileHeader.bfType = 0x4d42;  // "BM"
		kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + unDataBytes;
		kFileHeader.bfReserved1 = 0;
		kFileHeader.bfReserved2 = 0;
		kFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		BITMAPINFOHEADER kInfoHeader;
		kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		kInfoHeader.biWidth = unWidth * 8;
		kInfoHeader.biHeight = -((int)unWidth * 8);
		kInfoHeader.biPlanes = 1;
		kInfoHeader.biBitCount = 24;
		kInfoHeader.biCompression = BI_RGB;
		kInfoHeader.biSizeImage = 0;
		kInfoHeader.biXPelsPerMeter = 0;
		kInfoHeader.biYPelsPerMeter = 0;
		kInfoHeader.biClrUsed = 0;
		kInfoHeader.biClrImportant = 0;

		// Convert QrCode bits to bmp pixels
		pSourceData = pQRC->data;
		for (y = 0; y < unWidth; y++)
		{
			pDestData = pRGBData + unWidthAdjusted * y * 8;
			for (x = 0; x < unWidth; x++)
			{
				if (*pSourceData & 1)
				{
					for (l = 0; l < 8; l++)
					{
						for (n = 0; n < 8; n++)
						{
							*(pDestData + n * 3 + unWidthAdjusted * l) = 0x00;
							*(pDestData + 1 + n * 3 + unWidthAdjusted * l) = 0x00;
							*(pDestData + 2 + n * 3 + unWidthAdjusted * l) = 0x00;
						}
					}
				}
				pDestData += 3 * 8;
				pSourceData++;
			}
		}
		if (!(fopen_s(&f, dstBmpPath.c_str(), "wb")))
		{
			fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
			fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
			fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);
			fclose(f);
		}
		else
		{
			return string("");
		}
		free(pRGBData);
		QRcode_free(pQRC);
	}
	else
	{
		return string("");
	}
	return dstBmpPath;
}
