//writen by leukocyte,2017
//Glasgow Collage, UESTC
//dirver of MTU100 chip

#include "MTU100.h"
#include <string.h>
#include "mbed.h"

//! 功能码
typedef enum FunctionCode
{
	// 模块公共操作
	FC_SET_AUX_OUTPUT    = 0x14,      //!<设置辅助输出断端口   
	FC_GET_READER_INFO   = 0x15,      //!<获取模块信息
	FC_GET_CARD_SN       = 0x16,      //!<获取卡片序列号

	// Mifare卡片操作
	FC_READ_SECTOR_DATA  = 0x21,      //!<写扇区数据 
	FC_WRITE_SECTOR_DATA = 0x22,      //!<读扇区数据
	FC_MODIFY_SECTOR_KEY = 0x23,      //!<修改扇区密钥 
	
} FunctionCode;


#define COMMAND_PROTOCOL_LEN 	4	  // 命令帧除命令数据以外总长度
#define RESPONSE_PROTOCOL_LEN	5	  // 回应帧除命令数据以外总长度

volatile static unsigned char g_recvCount = 0;              // 接收到数据的数量

// =======================================================================================
//! @brief  串口通讯初始化。
//! @param [in] TX   -- 发送引脚
//! @param [in] RX   -- 接收引脚。
// =======================================================================================
void Com_init(PinName TX, PinName RX);

//通信接口设置，这里使用串口通信
void Com_init(PinName TX,PinName RX) {
	Serial reader(TX, RX, 19200);
}

// 校验码计算，对各数据相加，最后取反
static unsigned char CheckSum(const void *pData, unsigned char dataLen)
{
	const unsigned char *_pData = (const unsigned char *)pData;

	unsigned char checkSum = 0;	// 校验码
	while (dataLen-- > 0)
	{
		checkSum += *_pData++;
	}

	return ~checkSum;
}

// 接收数据。
unsigned char Com_Receive(void *pBuf, unsigned char bufSize)
{
	(void *)response;//接收缓冲
	reader.scanf("%s", response);

	// 从接收缓冲区复制数据到用户数据区
	bufSize = sizeof(response) < bufSize ? sizeof(response) : bufSize;
	memcpy(pBuf, response, bufSize);

	return bufSize;
}

//发送命令
static unsigned char SendCommand(unsigned char functionCode, const void *pData, unsigned char dataLen)
{
	// 帧结构：数据长度(1) + 地址(1) + 功能码(1) + 数据(N) + 校验(1)

	unsigned char buf[32];	// 发送缓冲区

	// 如果要发送的数据长度大于发送缓冲区的大小，则卡死
	if (dataLen + COMMAND_PROTOCOL_LEN > sizeof(buf))
	{
		while (1);
		return RC_ILLEGAL_PARAM;
	}

	// 组装数据
	// 1. 帧长
	buf[0] = dataLen + COMMAND_PROTOCOL_LEN;

	// 2. 模块地址
	buf[1] = READER_MODULE_ADDR;

	// 3. 功能码			                                        
	buf[2] = functionCode;

	// 4. 功能码相关的数据
	memcpy(&buf[COMMAND_PROTOCOL_LEN - 1], pData, dataLen);

	// 5. 校验码
	buf[dataLen + COMMAND_PROTOCOL_LEN - 1] = CheckSum(buf, (unsigned char)(dataLen + COMMAND_PROTOCOL_LEN - 1));

	// 6.发送数据
	reader.printf("%s", buf);

	return RC_OK;
}

// 接收回应
static unsigned char ReceiveResponse(unsigned char *pFunctionCode, void *pBuf, unsigned char bufSize,
	unsigned char *pRecvDataLen)
{
	// 帧结构：数据长度(1) + 地址(1) + 功能码(1) + 状态(1) + 数据(N) + 校验(1)

	unsigned char buf[40];			// 接收缓冲区
	unsigned char totalLen = 0;		// 总共接收到的数据量
	unsigned char curLen = 0; 	    // 当前接收到的数据量
	int timeout = TOTAL_TIMEOUT;  	// 超时时间


									// 循环接收数据，如果接收不到任何数据，会等到接收超时才退出循环；
									// 如果接收到数据，则继续接收数据，直到发送数据间隔超时。
	while (timeout > 0)
	{
		curLen = Com_Receive(&buf[totalLen], (unsigned char)(sizeof(buf) - totalLen));
		if (curLen > 0)
		{
			totalLen += curLen;
			timeout = 20;         // 把接收超时改为数据间隔超时 
		}
		else
		{
			Delay1ms(10);
			timeout -= 10;
		}
	}

	// 检验各字段的合法性

	// 检验总数据长度是否合法
	if (totalLen < RESPONSE_PROTOCOL_LEN)
	{
		return RC_COMM_ERROR;
	}

	// 检验帧数据长度
	if (buf[0] != totalLen)
	{
		return RC_COMM_ERROR;
	}

	// 检验校验码
	if (CheckSum(buf, (unsigned char)(totalLen - 1)) != buf[totalLen - 1])
	{
		return RC_COMM_ERROR;
	}

	// 检验信息缓冲区是否足够大
	if (bufSize < totalLen - RESPONSE_PROTOCOL_LEN)
	{
		return RC_ILLEGAL_PARAM;
	}

	// 解装数据
	*pFunctionCode = buf[2];                // 功能码
	memcpy(pBuf, &buf[4], totalLen - 5);   // 功能码相关的数据

	if (pRecvDataLen != NULL)
	{
		*pRecvDataLen = totalLen - 5;      // 功能码相关的数据长度
	}

	return buf[3];                          // 状态码
}

// 发送接收命令
static unsigned char SendReceiveCommand(unsigned char functionCode, const void *pData,
	unsigned char dataLen, void *pBuf, unsigned char bufSize, unsigned char *pRecvDataLen)
{
	unsigned char result;

	result = SendCommand(functionCode, pData, dataLen);
	if (result != RC_OK)
	{
		return result;//假如发送错误， 返回错误代码
	}

	return ReceiveResponse(&functionCode, pBuf, bufSize, pRecvDataLen);
}


/*=====================================================================================================*/
/*==========================================以下为高级函数=============================================*/
/*=====================================================================================================*/


// 获取读卡模块信息。
unsigned char RM_GetReaderInformation(char *pInfo, unsigned char bufSize)
{
	return SendReceiveCommand(FC_GET_READER_INFO, NULL, 0, pInfo, bufSize, NULL);
}


// 获取卡片序列号。
unsigned char RM_GetCardSN(unsigned char *pCardSN, unsigned char bufSize, unsigned char *pSNLen)
{
	return SendReceiveCommand(FC_GET_CARD_SN, NULL, 0, pCardSN, bufSize, pSNLen);
}


// 读取卡片某扇区块的数据，一块数据大小固定为16字节。
unsigned char RM_ReadSectorData(unsigned char sector, unsigned char block,
	const unsigned char *pKey, unsigned char *pData)
{
	unsigned char buf[8];
	buf[0] = sector;
	buf[1] = block;
	memcpy(&buf[2], pKey, 6);

	return SendReceiveCommand(FC_READ_SECTOR_DATA, buf, 8, pData, 16, NULL);
}


// 改写卡片某扇区块的数据，一块数据大小固定为16字节。
unsigned char RM_WriteSectorData(unsigned char sector, unsigned char block,
	const unsigned char *pKey, const unsigned char *pData)
{
	unsigned char buf[24];
	buf[0] = sector;
	buf[1] = block;
	memcpy(&buf[2], pKey, 6);
	memcpy(&buf[8], pData, 16);

	return SendReceiveCommand(FC_WRITE_SECTOR_DATA, buf, 24, NULL, 0, NULL);
}


// 修改扇区密钥
unsigned char RM_ModifySectorKey(unsigned char sector, const unsigned char *pOldKey,
	const unsigned char *pNewKey)
{
	unsigned char buf[13];
	buf[0] = sector;
	memcpy(&buf[1], pOldKey, 6);
	memcpy(&buf[7], pNewKey, 6);

	return SendReceiveCommand(FC_MODIFY_SECTOR_KEY, buf, 13, NULL, 0, NULL);
}


// 设置模块辅助输出端口状态
unsigned char RM_SetAuxOutput(unsigned char cycle, unsigned short highTime, unsigned short lowTime)
{
	unsigned char buf[3];
	buf[0] = cycle;
	buf[1] = highTime / 10;
	buf[2] = lowTime / 10;

	return SendReceiveCommand(FC_SET_AUX_OUTPUT, buf, 3, NULL, 0, NULL);

}