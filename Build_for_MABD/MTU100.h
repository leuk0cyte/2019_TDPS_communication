//writen by leukocyte,2017
//Glasgow Collage, UESTC
//dirver of MTU100 chip

#ifndef __MIFARE_READER_H
#define __MIFARE_READER_H


#ifdef __cplusplus
extern "C" {
#endif

#define READER_MODULE_ADDR  1       //!< 读卡模块地址
#define	TOTAL_TIMEOUT	    200		//!< 数据接收总的超时时间，单位ms



//! 操作结果代码
typedef enum ResultCode
{
    RC_OK                       = 0x00,     //!< 成功
    RC_ADDR_ERROR               = 0x01,     //!< 地址错误
    RC_ILLEGAL_PARAM            = 0x02,     //!< 参数非法
    RC_CARD_NO_EXIST            = 0x03,     //!< 卡不存在
    RC_KEY_ERROR                = 0x04,     //!< 密钥错误
    RC_READ_DATA_ERROR          = 0x05,     //!< 读数据错误
    RC_WIRTE_DATA_ERROR         = 0x06,     //!< 写数据错误
    RC_RATS_FAIL                = 0x07,     //!< CPU卡执行RATS失败
    RC_READ_FILE_ERROR          = 0x08,     //!< CPU卡读取文件失败
    RC_WRITE_FILE_ERROR         = 0x09,     //!< CPU卡写文件失败
    RC_INIT_CARD_ERROR          = 0x0A,     //!< CPU卡初始化文件系统失败
    RC_COMM_ERROR       	        = 0xFE,     //!< 通信错误
    RC_NOT_SUPPORT_OPERATION    = 0xFF      //!< 不支持的操作
} ResultCode;


// =======================================================================================
//! @brief  串口通讯初始化。
//! @param [in] TX   -- 发送引脚
//! @param [in] RX   -- 接收引脚。
// =======================================================================================
void Com_init(PinName TX, PinName RX)；

// =======================================================================================
//! @brief  接收数据。
//! @param [out] pBuf    -- 数据接收缓冲区。
//! @param [in]  bufSize -- 数据接收缓冲区大小。
//! @return -- 实际接收到的数据长度。
// =======================================================================================
unsigned char Com_Send(const void *pData, unsigned char dataLen);


// =======================================================================================
//! @brief  获取读卡模块信息。
//! @param [out] pInfo   -- 读卡模块信息缓冲区，模块信息的长度最大不会超过48字节。
//! @param [in]  bufSize -- 读卡模块信息缓冲区大小。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
unsigned char RM_GetReaderInformation(char *pInfo, unsigned char bufSize);


// =======================================================================================
//! @brief  获取卡片序列号。
//! @param [out] pCardSN -- 卡序列号缓冲区，建议定义为8字节大小的数组。
//! @param [in]  bufSize -- 卡序列号缓冲区大小。
//! @param [out] pSNLen  -- 实际读取到的卡片卡序列号长度，常见的 MF1 S50为4字节，
//                          Mifare 1 S70和Mifare 0为7字节。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
unsigned char RM_GetCardSN(unsigned char *pCardSN, unsigned char bufSize,
	unsigned char *pSNLen);


// =======================================================================================
//! @brief  读取卡片某扇区块的数据，一块数据大小固定为16字节。
//! @param [in]  sector -- 卡片扇区号。
//! @param [in]  block  -- 卡片扇区内的块号。
//! @param [in]  pKey   -- 6字节扇区密钥，默认为6字节0xFF。
//! @param [out] pData  -- 扇区块数据缓冲区，其大小必须不小于16字节。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
unsigned char RM_ReadSectorData(unsigned char sector, unsigned char block,
	const unsigned char *pKey, unsigned char *pData);


// =======================================================================================
//! @brief  改写卡片某扇区块的数据，一块数据大小固定为16字节。
//! @param [in]  sector -- 卡片扇区号。
//! @param [in]  pKey   -- 6字节扇区密钥，默认为6字节0xFF。
//! @param [in]  pData  -- 扇区块数据，其大小为16字节。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
unsigned char RM_WriteSectorData(unsigned char sector, unsigned char block,
	const unsigned char *pKey, const unsigned char *pData);


// =======================================================================================
//! @brief  修改扇区密钥
//! @param [in]  sector  -- 卡片扇区号。
//! @param [in]  pOldKey -- 扇区原来的密钥，默认为6字节0xFF。
//! @param [in]  pNewKey -- 扇区新的密钥，大小为6字节。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
unsigned char RM_ModifySectorKey(unsigned char sector, const unsigned char *pOldKey,
	const unsigned char *pNewKey);


// =======================================================================================
//! @brief  设置模块辅助输出端口状态
//! @param [in]  cycle    -- 循环周期。
//! @param [in]  highTime -- 高电平时间，单位ms，分辨率为10ms。
//! @param [in]  lowTime  -- 低电平时间，单位ms，分辨率为10ms。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
unsigned char RM_SetAuxOutput(unsigned char cycle, unsigned short highTime, unsigned short lowTime);

// =======================================================================================
//! @brief  发送接收命令
//! @param [in]  functionCode -- 功能码，详见@ref FunctionCode。
//! @param [in]  pData        -- 功能码相关的数据，也就是要发送给模块的数据。
//! @param [in]  dataLen      -- 功能码相关的数据长度。
//! @param [out] pBuf         -- 模块回应的数据接收缓冲区。
//! @param [in]  bufSize      -- 模块回应的数据接收缓冲区大小。
//! @param [out] pRecvDataLen -- 实际真正从模块接收到的数据大小。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
static unsigned char SendReceiveCommand(unsigned char functionCode, const void *pData,
	unsigned char dataLen, void *pBuf, unsigned char bufSize, unsigned char *pRecvDataLen);


// =======================================================================================
//! @brief  发送命令
//! @param [in]  functionCode -- 功能码，详见@ref FunctionCode。
//! @param [in]  pData        -- 功能码相关的数据，也就是要发送给模块的数据。
//! @param [in]  dataLen      -- 功能码相关的数据长度。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
static unsigned char SendCommand(unsigned char functionCode, const void *pData, unsigned char dataLen);


// =======================================================================================
//! @brief  接收回应。
//! @param [out] pFunctionCode -- 读卡模块回应的功能码，详见@ref FunctionCode。
//! @param [out] pBuf          -- 模块回应的数据接收缓冲区。
//! @param [in]  bufSize       -- 模块回应的数据接收缓冲区大小。
//! @param [out] pRecvDataLen  -- 实际真正从模块接收到的数据大小。
//! @return -- 操作结果码，详见 @ref ResultCode。
// =======================================================================================
static unsigned char ReceiveResponse(unsigned char *pFunctionCode, void *pBuf,
	unsigned char bufSize, unsigned char *pRecvDataLen);


// =======================================================================================
//! @brief  计算校验码，对各数据相加，最后取反。
//! @param [in]  pData   -- 要计算校验和的数据。
//! @param [in]  dataLen -- 要计算校验和数据的长度。
//! @return -- 数据的校验和。
// =======================================================================================
static unsigned char CheckSum(const void *pData, unsigned char dataLen);




#ifdef __cplusplus
}
#endif


#endif  //__MIFARE_READER_H