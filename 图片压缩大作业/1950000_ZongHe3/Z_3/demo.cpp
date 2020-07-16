#define _CRT_SECURE_NO_WARNINGS
/*****************************************
*  Author: Root
*****************************************/

#include "PicReader.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <fstream>
#include <iostream>

using namespace std;

const float PI = 3.1415926f;

const char zigzag[64] = {
		0,	1,	5,	6,	14,	15,	27,	28,
		2,	4,	7,	13,	16,	26,	29,	42,
		3,	8,	12,	17,	25,	30,	41,	43,
		9,	11,	18,	24,	31,	40,	44,	53,
		10,	19,	23,	32,	39,	45,	52,	54,
		20,	22,	33,	38,	46,	51,	55,	60,
		21,	34,	37,	47,	50,	56,	59,	61,
		35,	36,	48,	49,	57,	58,	62,	63
};
const char Qy[64] = {
	16,	11,	10,	16,	24,	40,	51,	61,
	12,	12,	14,	19,	26,	58,	60,	55,
	14,	13,	16,	24,	40,	57,	69,	56,
	14,	17,	22,	29,	51,	87,	80,	62,
	18,	22,	37,	56,	68,	109,103,77,
	24,	35,	55,	64,	81,	104,113,92,
	49,	64,	78,	87,	103,121,120,101,
	72,	92,	95,	98,	112,100,103,99
};
const short Qc[64] = {
	17,	18,	24,	47,	99,	99,	99,	99,
	18,	21,	26,	66,	99,	99,	99,	99,
	24,	26,	56,	99,	99,	99,	99,	99,
	47,	66,	99,	99,	99,	99,	99,	99,
	99,	99,	99,	99,	99,	99,	99,	99,
	99,	99,	99,	99,	99,	99,	99,	99,
	99,	99,	99,	99,	99,	99,	99,	99,
	99,	99,	99,	99,	99,	99,	99,	99
};

const char DCL_CODES[16] = {
	0, 0, 7, 1,
	1, 1, 1, 1,
	0, 0, 0, 0,
	0, 0, 0, 0
};

const unsigned char DCL_VALUE[12] = {
	4, 5, 3, 2,
	6, 1, 0, 7,
	8, 9, 10, 11
};

const char DCC_CODES[16] = {
	0, 3, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 0,
	0, 0, 0, 0
};

const unsigned char DCC_VALUE[12] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11
};

const char ACL_CODES[16] = {
	0, 2, 1, 3,
	3, 2, 4, 3,
	5, 5, 4, 4,
	0, 0, 1, 125
};

const unsigned char ACL_VALUE[162] = {
	0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
	0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
	0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
	0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 240,
	0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
	0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
	0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
	0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
	0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
	0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
	0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
	0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
	0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
	0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa
};

const char ACC_CODES[16] = {
	0, 2, 1, 2,
	4, 4, 3, 4,
	7, 5, 4, 4,
	0, 1, 2, 119
};

const unsigned char ACC_VALUE[162] = {
	0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
	0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
	0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
	0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 240,
	0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
	0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
	0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
	0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
	0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
	0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
	0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
	0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
	0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa
};

unsigned short mask[16] = {
	1,	2,	4,	8,
	16,	32,	64,	128,
	256,512,1024,2048,
	4096,8192,16384,32768
};

char name_tiff[100];
char name_temp[100];
char name_jpg[100];

struct the_size {
	int height;
	int length;
	int hin;
	int vin;
	int value;
};

struct pix {
	unsigned char p_b;
	unsigned char p_g;
	unsigned char p_r;
};

struct pix_ {
	int nY;
	int nCb;
	int nCr;
};

class photo {
private:
	unsigned char QyTable[64], QcTable[64];
	the_size hY_DC[16], hY_AC[256], hC_DC[16], hC_AC[256];

	const char* fileName;
	int size, height, width, _height, _width;
	short bit_count;
	float temp_bit[8][8];

	the_size y_size, cb_size, cr_size;
	pix** rgb_board;
	pix_** yc_board;

	void getHuffmanTable(const char* numTable, const unsigned char* posTable, the_size* huffmanTable) {
		int index = 0;
		int code = 0;
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < numTable[i]; j++) {
				huffmanTable[posTable[index]].length = i + 1;
				huffmanTable[posTable[index]].value = code;
				code++;
				index++;
			}
			code <<= 1;
		}
	}

	int huffmanCoding(int& last, int* data, the_size* HD, the_size* HA, the_size* output) {
		int index = 0;
		//DC分量处理
		int diff = data[0] - last;
		last = data[0];
		if (diff == 0) {
			output[index++] = HD[0x00];
		}
		else {
			the_size bs = getValueBit(diff);
			output[index++] = HD[bs.length];
			output[index++] = bs;
		}
		//AC分量处理
		the_size EOB = HA[0x00];
		the_size SIXTEENZEROS = HA[240];
		int endpos = 63;
		while (data[endpos] == 0 && endpos > 0)
			endpos--;
		for (int i = 1; i <= endpos; i++) {
			int startpos = i;
			while (data[i] == 0 && i <= endpos)
				i++;
			int count = i - startpos;
			while (count >= 16) {
				output[index++] = SIXTEENZEROS;
				count -= 16;
			}
			the_size bs = getValueBit(data[i]);
			output[index++] = HA[count << 4 | bs.length];
			output[index++] = bs;
		}
		if (endpos != 63)
			output[index++] = EOB;
		return index;
	}

	void make_it(int flag, int& x, int& y, int H, int V, int _H, int _V, int& last, int& byte, int& bytepos, FILE* fp) {
		int data[64]; the_size output[128];
		int count;
		double temp;
		float s = 1 / sqrt(8);

		for (int i = 0; i < H; i++) {
			for (int j = 0; j < V; j++) {
				for (int i = x; i < x + 8; i++) {
					for (int j = y; j < y + 8; j++) {
						if (flag == 1) {
							temp = 0.299 * rgb_board[i][j].p_r + 0.587 * rgb_board[i][j].p_g + 0.114 * rgb_board[i][j].p_b;
							yc_board[i][j].nY = round(temp);
						}
						else if (flag == 2) {
							temp = -0.1687 * rgb_board[i][j].p_r - 0.3313 * rgb_board[i][j].p_g + 0.5 * rgb_board[i][j].p_b + 128;
							yc_board[i][j].nCb = round(temp);
						}
						else if (flag == 3) {
							temp = 0.5 * rgb_board[i][j].p_r - 0.4187 * rgb_board[i][j].p_g - 0.0813 * rgb_board[i][j].p_b + 128;
							yc_board[i][j].nCr = round(temp);
						}
					}
				}
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++) {
						float sum = 0;
						for (int u = 0; u < 8; u++)
							for (int v = 0; v < 8; v++) {
								if (flag == 1)
									sum += temp_bit[u][i] * temp_bit[v][j] * (yc_board[x + u][y + v].nY - 128);
								else if (flag == 2)
									sum += temp_bit[u][i] * temp_bit[v][j] * (yc_board[x + u][y + v].nCb - 128);
								else if (flag == 3)
									sum += temp_bit[u][i] * temp_bit[v][j] * (yc_board[x + u][y + v].nCr - 128);
							}
						float alphaU = i == 0 ? s : 0.5f;
						float alphaV = j == 0 ? s : 0.5f;
						float temp = alphaU * alphaV;
						if (flag == 1)
							data[zigzag[i * 8 + j]] = round(temp * sum / QyTable[zigzag[i * 8 + j]]);
						else
							data[zigzag[i * 8 + j]] = round(temp * sum / QcTable[zigzag[i * 8 + j]]);
					}
				if (flag == 1)
					count = huffmanCoding(last, data, hY_DC, hY_AC, output);
				else
					count = huffmanCoding(last, data, hC_DC, hC_AC, output);

				for (int i = 0; i < count; i++) {
					int value = output[i].value;
					int pos = output[i].length - 1;
					while (pos >= 0) {
						if (value & mask[pos]) {
							byte = byte | mask[bytepos];
						}
						pos--;
						bytepos--;
						if (bytepos < 0) {
							_writeByte((unsigned char)byte, fp);
							if (byte == 0xff)
								_writeByte((unsigned char)0, fp);
							byte = 0;
							bytepos = 7;
						}
					}
				}
			}
		}
		y += V * 8 * _V;
		if (y >= _width) {
			y = 0;
			x += H * 8 * _H;
		}
		return;
	}

	the_size getValueBit(int value) {
		int v = value > 0 ? value : -value;
		int length = 0;
		while (v > 0) {
			length++;
			v >>= 1;
		}
		the_size bs;
		bs.length = length;
		bs.value = value > 0 ? value : (1 << length) + value - 1;
		return bs;
	}

	//这里定义用于向fp写入内容的函数
	void _writeByte(unsigned char value, FILE* fp) {
		_write(&value, 1, fp);
	}

	void _writeWord(unsigned short value, FILE* fp) {
		unsigned short _value = ((value >> 8) & 0xFF) | ((value & 0xFF) << 8);
		_write(&_value, 2, fp);
	}

	void _write(const void* p, int byteSize, FILE* fp) {
		fwrite(p, 1, byteSize, fp);
	}

public:
	photo(const char* sFileName = name_temp) {
		fileName = sFileName;
		rgb_board = NULL;
		yc_board = NULL;


		FILE* fp = fopen(name_temp, "rb");

		BITMAPFILEHEADER fh;//文件头
		BITMAPINFOHEADER ih;//信息头
		fread(&fh, sizeof(BITMAPFILEHEADER), 1, fp);
		fread(&ih, sizeof(BITMAPINFOHEADER), 1, fp);

		size = fh.bfSize;
		height = ih.biHeight;
		width = ih.biWidth;
		bit_count = ih.biBitCount;

		_height = (height + 31) / 32 * 32;
		_width = (width + 31) / 32 * 32;

		rgb_board = (pix**)malloc(sizeof(pix*) * _height);
		for (int i = 0; i < _height; i++) {
			rgb_board[i] = (pix*)malloc(sizeof(pix) * _width);
		}
		int offset = ((width * 3 + 3) / 4 * 4) - width * 3;
		for (int i = height - 1; i >= 0; i--) {
			for (int j = 0; j < width; j++) {
				fread(&rgb_board[i][j], 3, 1, fp);
			}
			fseek(fp, offset, SEEK_CUR);
		}

		fclose(fp);

		fp = fopen(name_temp, "w");
		fclose(fp);
	}

	bool write_photo() {
		int qualityScale = 50;
		unsigned char Ythe_size = 17;
		unsigned char Cb_size = 17;
		unsigned char Cr_size = 17;

		FILE* fp = fopen(name_jpg, "wb");
		if (!fp) {
			cout << "false" << endl;
			return false;
		}
		if (qualityScale <= 0) qualityScale = 1;
		if (qualityScale >= 100) qualityScale = 99;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				int temp = ((int)(Qy[i * 8 + j] * qualityScale + 50) / 100);
				if (temp <= 0) temp = 1;
				if (temp > 0xFF) temp = 0xFF;
				QyTable[zigzag[i * 8 + j]] = (unsigned char)temp;

				temp = ((int)(Qc[i * 8 + j] * qualityScale + 50) / 100);
				if (temp <= 0) 	temp = 1;
				if (temp > 0xFF) temp = 0xFF;
				QcTable[zigzag[i * 8 + j]] = (unsigned char)temp;
			}
		}

		//这里开始写jpg的头

		_writeWord(0xFFD8, fp);

		_writeWord(0xFFE0, fp);
		_writeWord(16, fp);
		_write("JFIF", 5, fp);
		_writeByte(1, fp);
		_writeByte(1, fp);
		_writeByte(1, fp);


		_writeWord(0x78, fp);
		_writeWord(0x78, fp);
		_writeByte(0, fp);
		_writeByte(0, fp);


		_writeWord(0xFFDB, fp);
		_writeWord(132, fp);
		_writeByte(0x00, fp);
		_write(QyTable, 64, fp);
		_writeByte(0x01, fp);
		_write(QcTable, 64, fp);

		_writeWord(0xFFC0, fp);
		_writeWord(17, fp);
		_writeByte(8, fp);
		_writeWord(height & 0xFFFF, fp);
		_writeWord(width & 0xFFFF, fp);
		_writeByte(3, fp);


		_writeByte(1, fp);
		_writeByte(Ythe_size, fp);
		_writeByte(0, fp);

		_writeByte(2, fp);
		_writeByte(Cb_size, fp);
		_writeByte(1, fp);

		_writeByte(3, fp);
		_writeByte(Cr_size, fp);
		_writeByte(1, fp);

		_writeWord(0xFFC4, fp);
		_writeWord(0x01A2, fp);



		_writeByte(0x00, fp);
		_write(DCL_CODES, sizeof(DCL_CODES), fp);
		_write(DCL_VALUE, sizeof(DCL_VALUE), fp);

		_writeByte(0x10, fp);
		_write(ACL_CODES, sizeof(ACL_CODES), fp);
		_write(ACL_VALUE, sizeof(ACL_VALUE), fp);

		_writeByte(0x01, fp);
		_write(DCC_CODES, sizeof(DCC_CODES), fp);
		_write(DCC_VALUE, sizeof(DCC_VALUE), fp);

		_writeByte(0x11, fp);
		_write(ACC_CODES, sizeof(ACC_CODES), fp);
		_write(ACC_VALUE, sizeof(ACC_VALUE), fp);



		_writeWord(0xFFDA, fp);
		_writeWord(12, fp);
		_writeByte(3, fp);

		_writeByte(1, fp);
		_writeByte(0x00, fp);

		_writeByte(2, fp);
		_writeByte(0x11, fp);

		_writeByte(3, fp);
		_writeByte(0x11, fp);

		_writeByte(0, fp);
		_writeByte(0x3F, fp);
		_writeByte(0, fp);


		//接下来开始写图片的存储内容

		int maxH, maxV;

		int lastY = 0, lastCb = 0, lastCr = 0;
		int byte = 0, bytepos = 7;

		y_size.height = (Ythe_size & 240) >> 4;
		cb_size.height = (Cb_size & 240) >> 4;
		cr_size.height = (Cr_size & 240) >> 4;

		y_size.length = Ythe_size & 15;
		cb_size.length = Cb_size & 15;
		cr_size.length = Cr_size & 15;

		maxH = max(max(y_size.height, cb_size.height), cr_size.height);
		maxV = max(max(y_size.length, cb_size.length), cr_size.length);

		y_size.vin = maxV / y_size.length;
		cb_size.vin = maxV / cb_size.length;
		cr_size.vin = maxV / cr_size.length;

		y_size.hin = maxH / y_size.height;
		cb_size.hin = maxH / cb_size.height;
		cr_size.hin = maxH / cr_size.height;

		_height = (height + maxH * 8 - 1) / (maxH * 8) * maxH * 8;
		_width = (width + maxV * 8 - 1) / (maxV * 8) * maxV * 8;
		//为yc_board分配内存
		yc_board = (pix_**)malloc(sizeof(pix_*) * _height);
		for (int i = 0; i < _height; i++)
			yc_board[i] = (pix_*)malloc(sizeof(pix_) * _width);
		//初始化temp_bit
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				temp_bit[i][j] = cos((2 * i + 1) * j * PI / 16);
			}
		}

		//以下初始化哈夫曼表
		memset(hY_DC, 0, sizeof(hY_DC));
		getHuffmanTable(DCL_CODES, DCL_VALUE, hY_DC);

		memset(hY_AC, 0, sizeof(hY_AC));
		getHuffmanTable(ACL_CODES, ACL_VALUE, hY_AC);

		memset(hC_DC, 0, sizeof(hC_DC));
		getHuffmanTable(DCC_CODES, DCC_VALUE, hC_DC);

		memset(hC_AC, 0, sizeof(hC_AC));
		getHuffmanTable(ACC_CODES, ACC_VALUE, hC_AC);

		int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;
		while (1) {
			make_it(1, x1, y1, y_size.height, y_size.length, y_size.hin, y_size.vin, lastY, byte, bytepos, fp);
			make_it(2, x2, y2, cb_size.height, cb_size.length, cb_size.hin, cb_size.vin, lastCb, byte, bytepos, fp);
			make_it(3, x3, y3, cr_size.height, cr_size.length, cr_size.hin, cr_size.vin, lastCr, byte, bytepos, fp);

			if (x1 >= _height)
				break;
		}

		//最后写入结束标志
		_writeWord(0xFFD9, fp);
		fclose(fp);

		remove(name_temp);
		return true;
	}
};

void Write_temp(BYTE* img, int h, int w)
{
	int l = (w * 3 + 3) / 4 * 4;


	int bmi[] = { l * h + 54,0,54,40,w,h,1950000,0,l * h,0,0,100,0 };

	FILE* fp;
	int err = fopen_s(&fp, name_temp, "wb");
	if (!fp) {
		printf("%s", "error!");
		exit(-1);
	}
	fprintf_s(fp, "BM");//标识“BM”
	fwrite(&bmi, 52, 1, fp);
	fwrite(img, 1, h*l, fp);
	fclose(fp);
}

void readLena() {
	PicReader imread;
	BYTE* data = nullptr;
	UINT x, y;
	imread.readPic(name_tiff);
	imread.getData(data, x, y);

	//imread.showPic(data, x, y);
	BYTE* get_pix = new BYTE[(size_t)x * y * 3];

	int index = 0;
	for (int i = x - 1; i >= 0; i--) {
		for (int j = 0; j < y * 4; j += 4) {
			get_pix[index++] = data[i*y * 4 + j + 2];
			get_pix[index++] = data[i*y * 4 + j + 1];
			get_pix[index++] = data[i*y * 4 + j];
		}
	}

	Write_temp(get_pix, x, y);
	delete[] data;
	data = nullptr;
}

void compress() {
	readLena();
	photo bmp;
	bmp.write_photo();
	cout << "jpg文件已成功生成！";
}

void read() {
	PicReader imread;
	BYTE* data = nullptr;
	UINT x, y;

	imread.readPic(name_jpg);
	imread.getData(data, x, y);
	imread.showPic(data, x, y);
}

int main(int argc, char* argv[]) {
	char temp[100];
	if (argc != 3) {
		return -1;
	}
	if (strcmp(argv[1], "-compress") == 0) {
		strcpy(name_tiff, argv[2]);
		int i = 0;
		for (i = 0; name_tiff[i] != '.'; i++) {
			name_temp[i] = name_tiff[i];
			name_jpg[i] = name_tiff[i];
		}

		name_temp[i] = '.';
		name_temp[i + 1] = 't';
		name_temp[i + 2] = 'e';
		name_temp[i + 3] = 'm';
		name_temp[i + 4] = 'p';
		name_temp[i + 5] = '\0';

		name_jpg[i] = '.';
		name_jpg[i + 1] = 'j';
		name_jpg[i + 2] = 'p';
		name_jpg[i + 3] = 'g';
		name_jpg[i + 4] = '\0';

		compress();
	}
	else if (strcmp(argv[1], "-read") == 0) {
		strcpy(name_jpg, argv[2]);
		read();
	}
	else {
		return -1;
	}

	return 0;
}