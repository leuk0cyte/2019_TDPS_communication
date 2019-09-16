//writen by leukocyte,2017
//Glasgow Collage, UESTC
//dirver of MTU100 chip

#ifndef __MIFARE_READER_H
#define __MIFARE_READER_H


#ifdef __cplusplus
extern "C" {
#endif

#define READER_MODULE_ADDR  1       //!< ����ģ���ַ
#define	TOTAL_TIMEOUT	    200		//!< ���ݽ����ܵĳ�ʱʱ�䣬��λms



//! �����������
typedef enum ResultCode
{
    RC_OK                       = 0x00,     //!< �ɹ�
    RC_ADDR_ERROR               = 0x01,     //!< ��ַ����
    RC_ILLEGAL_PARAM            = 0x02,     //!< �����Ƿ�
    RC_CARD_NO_EXIST            = 0x03,     //!< ��������
    RC_KEY_ERROR                = 0x04,     //!< ��Կ����
    RC_READ_DATA_ERROR          = 0x05,     //!< �����ݴ���
    RC_WIRTE_DATA_ERROR         = 0x06,     //!< д���ݴ���
    RC_RATS_FAIL                = 0x07,     //!< CPU��ִ��RATSʧ��
    RC_READ_FILE_ERROR          = 0x08,     //!< CPU����ȡ�ļ�ʧ��
    RC_WRITE_FILE_ERROR         = 0x09,     //!< CPU��д�ļ�ʧ��
    RC_INIT_CARD_ERROR          = 0x0A,     //!< CPU����ʼ���ļ�ϵͳʧ��
    RC_COMM_ERROR       	        = 0xFE,     //!< ͨ�Ŵ���
    RC_NOT_SUPPORT_OPERATION    = 0xFF      //!< ��֧�ֵĲ���
} ResultCode;


// =======================================================================================
//! @brief  ����ͨѶ��ʼ����
//! @param [in] TX   -- ��������
//! @param [in] RX   -- �������š�
// =======================================================================================
void Com_init(PinName TX, PinName RX)��

// =======================================================================================
//! @brief  �������ݡ�
//! @param [out] pBuf    -- ���ݽ��ջ�������
//! @param [in]  bufSize -- ���ݽ��ջ�������С��
//! @return -- ʵ�ʽ��յ������ݳ��ȡ�
// =======================================================================================
unsigned char Com_Send(const void *pData, unsigned char dataLen);


// =======================================================================================
//! @brief  ��ȡ����ģ����Ϣ��
//! @param [out] pInfo   -- ����ģ����Ϣ��������ģ����Ϣ�ĳ�����󲻻ᳬ��48�ֽڡ�
//! @param [in]  bufSize -- ����ģ����Ϣ��������С��
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
unsigned char RM_GetReaderInformation(char *pInfo, unsigned char bufSize);


// =======================================================================================
//! @brief  ��ȡ��Ƭ���кš�
//! @param [out] pCardSN -- �����кŻ����������鶨��Ϊ8�ֽڴ�С�����顣
//! @param [in]  bufSize -- �����кŻ�������С��
//! @param [out] pSNLen  -- ʵ�ʶ�ȡ���Ŀ�Ƭ�����кų��ȣ������� MF1 S50Ϊ4�ֽڣ�
//                          Mifare 1 S70��Mifare 0Ϊ7�ֽڡ�
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
unsigned char RM_GetCardSN(unsigned char *pCardSN, unsigned char bufSize,
	unsigned char *pSNLen);


// =======================================================================================
//! @brief  ��ȡ��Ƭĳ����������ݣ�һ�����ݴ�С�̶�Ϊ16�ֽڡ�
//! @param [in]  sector -- ��Ƭ�����š�
//! @param [in]  block  -- ��Ƭ�����ڵĿ�š�
//! @param [in]  pKey   -- 6�ֽ�������Կ��Ĭ��Ϊ6�ֽ�0xFF��
//! @param [out] pData  -- ���������ݻ����������С���벻С��16�ֽڡ�
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
unsigned char RM_ReadSectorData(unsigned char sector, unsigned char block,
	const unsigned char *pKey, unsigned char *pData);


// =======================================================================================
//! @brief  ��д��Ƭĳ����������ݣ�һ�����ݴ�С�̶�Ϊ16�ֽڡ�
//! @param [in]  sector -- ��Ƭ�����š�
//! @param [in]  pKey   -- 6�ֽ�������Կ��Ĭ��Ϊ6�ֽ�0xFF��
//! @param [in]  pData  -- ���������ݣ����СΪ16�ֽڡ�
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
unsigned char RM_WriteSectorData(unsigned char sector, unsigned char block,
	const unsigned char *pKey, const unsigned char *pData);


// =======================================================================================
//! @brief  �޸�������Կ
//! @param [in]  sector  -- ��Ƭ�����š�
//! @param [in]  pOldKey -- ����ԭ������Կ��Ĭ��Ϊ6�ֽ�0xFF��
//! @param [in]  pNewKey -- �����µ���Կ����СΪ6�ֽڡ�
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
unsigned char RM_ModifySectorKey(unsigned char sector, const unsigned char *pOldKey,
	const unsigned char *pNewKey);


// =======================================================================================
//! @brief  ����ģ�鸨������˿�״̬
//! @param [in]  cycle    -- ѭ�����ڡ�
//! @param [in]  highTime -- �ߵ�ƽʱ�䣬��λms���ֱ���Ϊ10ms��
//! @param [in]  lowTime  -- �͵�ƽʱ�䣬��λms���ֱ���Ϊ10ms��
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
unsigned char RM_SetAuxOutput(unsigned char cycle, unsigned short highTime, unsigned short lowTime);

// =======================================================================================
//! @brief  ���ͽ�������
//! @param [in]  functionCode -- �����룬���@ref FunctionCode��
//! @param [in]  pData        -- ��������ص����ݣ�Ҳ����Ҫ���͸�ģ������ݡ�
//! @param [in]  dataLen      -- ��������ص����ݳ��ȡ�
//! @param [out] pBuf         -- ģ���Ӧ�����ݽ��ջ�������
//! @param [in]  bufSize      -- ģ���Ӧ�����ݽ��ջ�������С��
//! @param [out] pRecvDataLen -- ʵ��������ģ����յ������ݴ�С��
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
static unsigned char SendReceiveCommand(unsigned char functionCode, const void *pData,
	unsigned char dataLen, void *pBuf, unsigned char bufSize, unsigned char *pRecvDataLen);


// =======================================================================================
//! @brief  ��������
//! @param [in]  functionCode -- �����룬���@ref FunctionCode��
//! @param [in]  pData        -- ��������ص����ݣ�Ҳ����Ҫ���͸�ģ������ݡ�
//! @param [in]  dataLen      -- ��������ص����ݳ��ȡ�
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
static unsigned char SendCommand(unsigned char functionCode, const void *pData, unsigned char dataLen);


// =======================================================================================
//! @brief  ���ջ�Ӧ��
//! @param [out] pFunctionCode -- ����ģ���Ӧ�Ĺ����룬���@ref FunctionCode��
//! @param [out] pBuf          -- ģ���Ӧ�����ݽ��ջ�������
//! @param [in]  bufSize       -- ģ���Ӧ�����ݽ��ջ�������С��
//! @param [out] pRecvDataLen  -- ʵ��������ģ����յ������ݴ�С��
//! @return -- ��������룬��� @ref ResultCode��
// =======================================================================================
static unsigned char ReceiveResponse(unsigned char *pFunctionCode, void *pBuf,
	unsigned char bufSize, unsigned char *pRecvDataLen);


// =======================================================================================
//! @brief  ����У���룬�Ը�������ӣ����ȡ����
//! @param [in]  pData   -- Ҫ����У��͵����ݡ�
//! @param [in]  dataLen -- Ҫ����У������ݵĳ��ȡ�
//! @return -- ���ݵ�У��͡�
// =======================================================================================
static unsigned char CheckSum(const void *pData, unsigned char dataLen);




#ifdef __cplusplus
}
#endif


#endif  //__MIFARE_READER_H