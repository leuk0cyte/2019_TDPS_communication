//writen by leukocyte,2017
//Glasgow Collage, UESTC
//dirver of MTU100 chip

#include "MTU100.h"
#include <string.h>
#include "mbed.h"

//! ������
typedef enum FunctionCode
{
	// ģ�鹫������
	FC_SET_AUX_OUTPUT    = 0x14,      //!<���ø�������϶˿�   
	FC_GET_READER_INFO   = 0x15,      //!<��ȡģ����Ϣ
	FC_GET_CARD_SN       = 0x16,      //!<��ȡ��Ƭ���к�

	// Mifare��Ƭ����
	FC_READ_SECTOR_DATA  = 0x21,      //!<д�������� 
	FC_WRITE_SECTOR_DATA = 0x22,      //!<����������
	FC_MODIFY_SECTOR_KEY = 0x23,      //!<�޸�������Կ 
	
} FunctionCode;


#define COMMAND_PROTOCOL_LEN 	4	  // ����֡���������������ܳ���
#define RESPONSE_PROTOCOL_LEN	5	  // ��Ӧ֡���������������ܳ���

volatile static unsigned char g_recvCount = 0;              // ���յ����ݵ�����

// =======================================================================================
//! @brief  ����ͨѶ��ʼ����
//! @param [in] TX   -- ��������
//! @param [in] RX   -- �������š�
// =======================================================================================
void Com_init(PinName TX, PinName RX);

//ͨ�Žӿ����ã�����ʹ�ô���ͨ��
void Com_init(PinName TX,PinName RX) {
	Serial reader(TX, RX, 19200);
}

// У������㣬�Ը�������ӣ����ȡ��
static unsigned char CheckSum(const void *pData, unsigned char dataLen)
{
	const unsigned char *_pData = (const unsigned char *)pData;

	unsigned char checkSum = 0;	// У����
	while (dataLen-- > 0)
	{
		checkSum += *_pData++;
	}

	return ~checkSum;
}

// �������ݡ�
unsigned char Com_Receive(void *pBuf, unsigned char bufSize)
{
	(void *)response;//���ջ���
	reader.scanf("%s", response);

	// �ӽ��ջ������������ݵ��û�������
	bufSize = sizeof(response) < bufSize ? sizeof(response) : bufSize;
	memcpy(pBuf, response, bufSize);

	return bufSize;
}

//��������
static unsigned char SendCommand(unsigned char functionCode, const void *pData, unsigned char dataLen)
{
	// ֡�ṹ�����ݳ���(1) + ��ַ(1) + ������(1) + ����(N) + У��(1)

	unsigned char buf[32];	// ���ͻ�����

	// ���Ҫ���͵����ݳ��ȴ��ڷ��ͻ������Ĵ�С������
	if (dataLen + COMMAND_PROTOCOL_LEN > sizeof(buf))
	{
		while (1);
		return RC_ILLEGAL_PARAM;
	}

	// ��װ����
	// 1. ֡��
	buf[0] = dataLen + COMMAND_PROTOCOL_LEN;

	// 2. ģ���ַ
	buf[1] = READER_MODULE_ADDR;

	// 3. ������			                                        
	buf[2] = functionCode;

	// 4. ��������ص�����
	memcpy(&buf[COMMAND_PROTOCOL_LEN - 1], pData, dataLen);

	// 5. У����
	buf[dataLen + COMMAND_PROTOCOL_LEN - 1] = CheckSum(buf, (unsigned char)(dataLen + COMMAND_PROTOCOL_LEN - 1));

	// 6.��������
	reader.printf("%s", buf);

	return RC_OK;
}

// ���ջ�Ӧ
static unsigned char ReceiveResponse(unsigned char *pFunctionCode, void *pBuf, unsigned char bufSize,
	unsigned char *pRecvDataLen)
{
	// ֡�ṹ�����ݳ���(1) + ��ַ(1) + ������(1) + ״̬(1) + ����(N) + У��(1)

	unsigned char buf[40];			// ���ջ�����
	unsigned char totalLen = 0;		// �ܹ����յ���������
	unsigned char curLen = 0; 	    // ��ǰ���յ���������
	int timeout = TOTAL_TIMEOUT;  	// ��ʱʱ��


									// ѭ���������ݣ�������ղ����κ����ݣ���ȵ����ճ�ʱ���˳�ѭ����
									// ������յ����ݣ�������������ݣ�ֱ���������ݼ����ʱ��
	while (timeout > 0)
	{
		curLen = Com_Receive(&buf[totalLen], (unsigned char)(sizeof(buf) - totalLen));
		if (curLen > 0)
		{
			totalLen += curLen;
			timeout = 20;         // �ѽ��ճ�ʱ��Ϊ���ݼ����ʱ 
		}
		else
		{
			Delay1ms(10);
			timeout -= 10;
		}
	}

	// ������ֶεĺϷ���

	// ���������ݳ����Ƿ�Ϸ�
	if (totalLen < RESPONSE_PROTOCOL_LEN)
	{
		return RC_COMM_ERROR;
	}

	// ����֡���ݳ���
	if (buf[0] != totalLen)
	{
		return RC_COMM_ERROR;
	}

	// ����У����
	if (CheckSum(buf, (unsigned char)(totalLen - 1)) != buf[totalLen - 1])
	{
		return RC_COMM_ERROR;
	}

	// ������Ϣ�������Ƿ��㹻��
	if (bufSize < totalLen - RESPONSE_PROTOCOL_LEN)
	{
		return RC_ILLEGAL_PARAM;
	}

	// ��װ����
	*pFunctionCode = buf[2];                // ������
	memcpy(pBuf, &buf[4], totalLen - 5);   // ��������ص�����

	if (pRecvDataLen != NULL)
	{
		*pRecvDataLen = totalLen - 5;      // ��������ص����ݳ���
	}

	return buf[3];                          // ״̬��
}

// ���ͽ�������
static unsigned char SendReceiveCommand(unsigned char functionCode, const void *pData,
	unsigned char dataLen, void *pBuf, unsigned char bufSize, unsigned char *pRecvDataLen)
{
	unsigned char result;

	result = SendCommand(functionCode, pData, dataLen);
	if (result != RC_OK)
	{
		return result;//���緢�ʹ��� ���ش������
	}

	return ReceiveResponse(&functionCode, pBuf, bufSize, pRecvDataLen);
}


/*=====================================================================================================*/
/*==========================================����Ϊ�߼�����=============================================*/
/*=====================================================================================================*/


// ��ȡ����ģ����Ϣ��
unsigned char RM_GetReaderInformation(char *pInfo, unsigned char bufSize)
{
	return SendReceiveCommand(FC_GET_READER_INFO, NULL, 0, pInfo, bufSize, NULL);
}


// ��ȡ��Ƭ���кš�
unsigned char RM_GetCardSN(unsigned char *pCardSN, unsigned char bufSize, unsigned char *pSNLen)
{
	return SendReceiveCommand(FC_GET_CARD_SN, NULL, 0, pCardSN, bufSize, pSNLen);
}


// ��ȡ��Ƭĳ����������ݣ�һ�����ݴ�С�̶�Ϊ16�ֽڡ�
unsigned char RM_ReadSectorData(unsigned char sector, unsigned char block,
	const unsigned char *pKey, unsigned char *pData)
{
	unsigned char buf[8];
	buf[0] = sector;
	buf[1] = block;
	memcpy(&buf[2], pKey, 6);

	return SendReceiveCommand(FC_READ_SECTOR_DATA, buf, 8, pData, 16, NULL);
}


// ��д��Ƭĳ����������ݣ�һ�����ݴ�С�̶�Ϊ16�ֽڡ�
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


// �޸�������Կ
unsigned char RM_ModifySectorKey(unsigned char sector, const unsigned char *pOldKey,
	const unsigned char *pNewKey)
{
	unsigned char buf[13];
	buf[0] = sector;
	memcpy(&buf[1], pOldKey, 6);
	memcpy(&buf[7], pNewKey, 6);

	return SendReceiveCommand(FC_MODIFY_SECTOR_KEY, buf, 13, NULL, 0, NULL);
}


// ����ģ�鸨������˿�״̬
unsigned char RM_SetAuxOutput(unsigned char cycle, unsigned short highTime, unsigned short lowTime)
{
	unsigned char buf[3];
	buf[0] = cycle;
	buf[1] = highTime / 10;
	buf[2] = lowTime / 10;

	return SendReceiveCommand(FC_SET_AUX_OUTPUT, buf, 3, NULL, 0, NULL);

}