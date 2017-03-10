#ifndef __PROTOCOL
#define __PROTOCOL

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f10x.h>
#include "dma/dma.h"

/**
* @name ��־��ӡ�궨��
* @{
*/
#define PRINTFPROTOCOL                              ///<Э�����ݴ�ӡ
/**@} */

/**
* MCU����汾��
*/
#define SOFTWARE_VERSION "03010101"
/**
* MCUӲ���汾��
*/
#define HARDWARE_VERSION "03010100"


/** WiFiģ�����ò���*/
typedef enum
{
  WIFI_RESET_MODE = 0x00,                           ///< WIFIģ�鸴λ
  WIFI_SOFTAP_MODE,                                 ///< WIFIģ��softAPģʽ
  WIFI_AIRLINK_MODE,                                ///< WIFIģ��AirLinkģʽ
  WIFI_PRODUCTION_TEST,                             ///< MCU����WiFiģ��������ģʽ
  WIFI_NINABLE_MODE                                 ///< MCU����ģ�����ɰ�ģʽ
}WIFI_MODE_TYPE_T;                                  
                                                    
#define MAX_PACKAGE_LEN     950                     ///< ���ݻ�������󳤶�
#define RB_MAX_LEN          (MAX_PACKAGE_LEN*2)     ///< ���λ�������󳤶�
#define min(a, b) (a)<(b)?(a):(b)                   ///< ��ȡ��Сֵ
                                                    
#define SEND_MAX_TIME       200                     ///< 200ms�ط�
#define SEND_MAX_NUM        2                       ///< �ط�����
                                                    
#define protocol_VERSION    "00000004"              ///< protocol�汾��
#define P0_VERSION          "00000002"              ///< P0Э��汾��
#define DEV_IS_GATEWAY   0                          ///< �豸�Ƿ�Ϊ�п���,0��ʾ��,1��ʾ��
#define NINABLETIME  0                              ///< ��ʱ��

/**@name Product Key (��Ʒ��ʶ��)
* @{
*/
#define PRODUCT_KEY "3594edd0487d463e88dcaea7c95e41fb"
/**@} */

/**@name ���ݵ���ض���
* @{
*/
#define AUTO_BYTEOFFSET                    0
#define AUTO_BITOFFSET                     0
#define AUTO_LEN                           1

#define FIXED_POINT_BYTEOFFSET                    0
#define FIXED_POINT_BITOFFSET                     1
#define FIXED_POINT_LEN                           1

#define EDGEWISE_BYTEOFFSET                    0
#define EDGEWISE_BITOFFSET                     2
#define EDGEWISE_LEN                           1

#define SEPARATE_ROOM_BYTEOFFSET                    0
#define SEPARATE_ROOM_BITOFFSET                     3
#define SEPARATE_ROOM_LEN                           1

#define ELECTRICIZE_BYTEOFFSET                    0
#define ELECTRICIZE_BITOFFSET                     4
#define ELECTRICIZE_LEN                           1

#define FORWARD_BYTEOFFSET                    0
#define FORWARD_BITOFFSET                     5
#define FORWARD_LEN                           1

#define BACKWARD_BYTEOFFSET                    0
#define BACKWARD_BITOFFSET                     6
#define BACKWARD_LEN                           1

#define ANTICLOCKWISE_BYTEOFFSET                    0
#define ANTICLOCKWISE_BITOFFSET                     7
#define ANTICLOCKWISE_LEN                           1

#define CLOCKWISE_BYTEOFFSET                    1
#define CLOCKWISE_BITOFFSET                     8
#define CLOCKWISE_LEN                           1

#define POWER_BYTEOFFSET                    1
#define POWER_BITOFFSET                     9
#define POWER_LEN                           1


/**@} */


/** ��д�����ݵ� ������ö�ٱ��� ��ռ�ֽڴ�С*/
#define COUNT_W_BIT 2






/** �¼�ö��*/
typedef enum
{
  WIFI_SOFTAP = 0x00,                               ///< WiFi SOFTAP �����¼�
  WIFI_AIRLINK,                                     ///< WiFiģ�� AIRLINK �����¼�
  WIFI_STATION,                                     ///< WiFiģ�� STATION �����¼�
  WIFI_OPEN_BINDING,                                ///< WiFiģ�鿪�����¼�
  WIFI_CLOSE_BINDING,                               ///< WiFiģ��رհ��¼�
  WIFI_CON_ROUTER,                                  ///< WiFiģ��������·���¼�
  WIFI_DISCON_ROUTER,                               ///< WiFiģ���ѶϿ�����·���¼�
  WIFI_CON_M2M,                                     ///< WiFiģ������������M2M�¼�
  WIFI_DISCON_M2M,                                  ///< WiFiģ���ѶϿ���������M2M�¼�
  WIFI_OPEN_TESTMODE,                               ///< WiFiģ�鿪������ģʽ�¼�
  WIFI_CLOSE_TESTMODE,                              ///< WiFiģ��رղ���ģʽ�¼�
  WIFI_CON_APP,                                     ///< WiFiģ������APP�¼�
  WIFI_DISCON_APP,                                  ///< WiFiģ��Ͽ�APP�¼�
  WIFI_RSSI,                                        ///< WiFiģ���ź��¼�
  WIFI_NTP,                                         ///< ����ʱ���¼�
  TRANSPARENT_DATA,                                 ///< ͸���¼�
  EVENT_AUTO,
  EVENT_FIXED_POINT,
  EVENT_EDGEWISE,
  EVENT_SEPARATE_ROOM,
  EVENT_ELECTRICIZE,
  EVENT_FORWARD,
  EVENT_BACKWARD,
  EVENT_ANTICLOCKWISE,
  EVENT_CLOCKWISE,
  EVENT_POWER,
  EVENT_TYPE_MAX                                    ///< ö�ٳ�Ա�������� (�û���ɾ)
} EVENT_TYPE_T;

/** Э�������¼�����*/
typedef enum
{
  STATELESS_TYPE = 0x00,                            ///< ��״̬����
  ACTION_CONTROL_TYPE,                              ///< Э��4.10 WiFiģ������豸�¼�
  WIFI_STATUS_TYPE,                                 ///< Э��4.5 WiFiģ�����豸MCU֪ͨWiFiģ�鹤��״̬�ı仯�¼�
  ACTION_W2D_TRANSPARENT_TYPE,                      ///< Э��WiFi���豸MCU͸���¼�
  GET_NTP_TYPE,                                     ///< Э��4.13 MCU�����ȡ����ʱ���¼�
  PROTOCOL_EVENT_TYPE_MAX                           ///< ö�ٳ�Ա�������� (�û���ɾ)
} PROTOCOL_EVENT_TYPE_T;

/** �û����豸״̬�ṹ��*/
__packed typedef struct {
  bool valueauto;
  bool valuefixed_point;
  bool valueedgewise;
  bool valueseparate_room;
  bool valueelectricize;
  bool valueforward;
  bool valuebackward;
  bool valueanticlockwise;
  bool valueclockwise;
  bool valuepower;
} dataPoint_t;

/** ��ӦЭ�顰4.10 WiFiģ������豸���еı�־λ"attr_flags" */ 
__packed typedef struct {
  uint8_t flagauto:1;
  uint8_t flagfixed_point:1;
  uint8_t flagedgewise:1;
  uint8_t flagseparate_room:1;
  uint8_t flagelectricize:1;
  uint8_t flagforward:1;
  uint8_t flagbackward:1;
  uint8_t flaganticlockwise:1;
  uint8_t flagclockwise:1;
  uint8_t flagpower:1;
} attrFlags_t;

/** ��ӦЭ�顰4.10 WiFiģ������豸���е�����ֵ"attr_vals" */
__packed typedef struct {
  uint8_t wBitBuf[COUNT_W_BIT];
} attrVals_t;

/** ��ӦЭ�顰4.10 WiFiģ������豸���С�P0Э�������ı�־λ"attr_flags(1B)" + ����ֵ"attr_vals(6B)" */ 
__packed typedef struct {
    attrFlags_t attrFlags;
    attrVals_t  attrVals;
}gizwitsIssued_t;

/** ��ӦЭ�顰4.9 �豸MCU��WiFiģ�������ϱ���ǰ״̬���е��豸״̬"dev_status(11B)" */ 
__packed typedef struct {
  uint8_t wBitBuf[COUNT_W_BIT];





} devStatus_t;                                      
                                                    
/** ��ӦЭ�顰4.9 �豸MCU��WiFiģ�������ϱ���ǰ״̬�� �е� �豸״̬λ"dev_status(11B)"  */ 
__packed typedef struct {                           
    devStatus_t devStatus;                          ///< ���ã�������Ŵ��ϱ����豸״̬����
}gizwitsReport_t;                                   
                                                    
/** �¼����нṹ�� */                               
__packed typedef struct {                           
    uint8_t num;                                    ///< ���г�Ա����
    uint8_t event[EVENT_TYPE_MAX];                  ///< ���г�Ա�¼�����
}eventInfo_t;                                       
                                                    
/** wifi�ź�ǿ����ֵ�ṹ�� */                       
__packed typedef struct {                           
    uint8_t rssi;                                   ///< WIFI�ź�ǿ��
}moduleStatusInfo_t;                                

/** Э������ʱ��ṹ�� */
__packed typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint32_t ntp;
}protocolTime_t;

/** Э�������� */                                   
__packed typedef enum                               
{                                                   
    CMD_GET_DEVICE_INTO             = 0x01,         ///< �����֣���ӦЭ�飺4.1 WiFiģ�������豸��Ϣ �� WiFiģ�鷢��
    ACK_GET_DEVICE_INFO             = 0x02,         ///< �����֣���ӦЭ�飺4.1 WiFiģ�������豸��Ϣ �� �豸MCU�ظ�
            
    CMD_ISSUED_P0                   = 0x03,         ///< �����֣���ӦЭ�飺4.8 WiFiģ���ȡ�豸�ĵ�ǰ״̬��4.10 WiFiģ������豸 �� WiFiģ�鷢��
    ACK_ISSUED_P0                   = 0x04,         ///< �����֣���ӦЭ�飺4.8 WiFiģ���ȡ�豸�ĵ�ǰ״̬��4.10 WiFiģ������豸 �� �豸MCU�ظ�
            
    CMD_REPORT_P0                   = 0x05,         ///< �����֣���ӦЭ�飺4.9 �豸MCU��WiFiģ�������ϱ���ǰ״̬ �� �豸MCU����
    ACK_REPORT_P0                   = 0x06,         ///< �����֣���ӦЭ�飺4.9 �豸MCU��WiFiģ�������ϱ���ǰ״̬ �� WiFiģ��ظ�
            
    CMD_HEARTBEAT                   = 0x07,         ///< �����֣���ӦЭ�飺4.2 WiFiģ�����豸MCU������ �� WiFiģ�鷢��
    ACK_HEARTBEAT                   = 0x08,         ///< �����֣���ӦЭ�飺4.2 WiFiģ�����豸MCU������ �� �豸MCU�ظ�
            
    CMD_WIFI_CONFIG                 = 0x09,         ///< �����֣���ӦЭ�飺4.3 �豸MCU֪ͨWiFiģ���������ģʽ �� �豸MCU����
    ACK_WIFI_CONFIG                 = 0x0A,         ///< �����֣���ӦЭ�飺4.3 �豸MCU֪ͨWiFiģ���������ģʽ �� WiFiģ��ظ�
            
    CMD_SET_DEFAULT                 = 0x0B,         ///< �����֣���ӦЭ�飺4.4 �豸MCU����WiFiģ�� �� �豸MCU����
    ACK_SET_DEFAULT                 = 0x0C,         ///< �����֣���ӦЭ�飺4.4 �豸MCU����WiFiģ�� �� WiFiģ��ظ�
            
    CMD_WIFISTATUS                  = 0x0D,         ///< �����֣���ӦЭ�飺4.5 WiFiģ�����豸MCU֪ͨWiFiģ�鹤��״̬�ı仯 �� WiFiģ�鷢��
    ACK_WIFISTATUS                  = 0x0E,         ///< �����֣���ӦЭ�飺4.5 WiFiģ�����豸MCU֪ͨWiFiģ�鹤��״̬�ı仯 �� �豸MCU�ظ�
        
    CMD_MCU_REBOOT                  = 0x0F,         ///< �����֣���ӦЭ�飺4.6 WiFiģ����������MCU �� WiFiģ�鷢��
    ACK_MCU_REBOOT                  = 0x10,         ///< �����֣���ӦЭ�飺4.6 WiFiģ����������MCU �� �豸MCU�ظ�
            
    CMD_ERROR_PACKAGE               = 0x11,         ///< �����֣���ӦЭ�飺4.7 �Ƿ���Ϣ֪ͨ �� WiFiģ���ӦMCU��Ӧ����ŵ����ݰ��Ƿ�
    ACK_ERROR_PACKAGE               = 0x12,         ///< �����֣���ӦЭ�飺4.7 �Ƿ���Ϣ֪ͨ �� MCU��ӦWiFiģ���Ӧ����ŵ����ݰ��Ƿ�

    CMD_PRODUCTION_TEST             = 0x13,         ///< �����֣���ӦЭ�飺4.11 MCU����WiFiģ��������ģʽ �� �豸MCU����
    ACK_PRODUCTION_TEST             = 0x14,         ///< �����֣���ӦЭ�飺4.11 MCU����WiFiģ��������ģʽ �� WiFiģ��ظ�

    CMD_NINABLE_MODE                = 0x15,         ///< �����֣���ӦЭ�飺4.12 MCU֪ͨWiFiģ�����ɰ�ģʽ �� �豸MCU����
    ACK_NINABLE_MODE                = 0x16,         ///< �����֣���ӦЭ�飺4.12 MCU֪ͨWiFiģ�����ɰ�ģʽ �� WiFiģ��ظ�

    CMD_GET_NTP                     = 0x17,         ///< �����֣���ӦЭ�飺4.13 MCU�����ȡ����ʱ�� �� �豸MCU����
    ACK_GET_NTP                     = 0x18,         ///< �����֣���ӦЭ�飺4.13 MCU�����ȡ����ʱ�� �� WiFiģ��ظ�
} PROTOCOL_CMDTYPE;

/** �ط����ƽṹ�� */
__packed typedef struct {
    uint8_t                 num;                    ///< �ط�����
    uint8_t                 flag;                   ///< 1,��ʾ����Ҫ�ȴ���ACK;0,��ʾ����Ҫ�ȴ���ACK
    uint8_t                 buf[MAX_PACKAGE_LEN];   ///< �ط����ݻ�����
    uint16_t                dataLen;                ///< �ط����ݳ���
    uint32_t                sendTime;               ///< �ط���ϵͳʱ��
} protocolWaitAck_t;

/** Э���׼ͷ�ṹ�� */
__packed typedef struct
{
    uint8_t                 head[2];                ///< ��ͷ(header)�̶�Ϊ0xFFFF
    uint16_t                len;                    ///< ����(len)��ָ��cmd ��ʼ���������ݰ�������ռ�õ��ֽ���
    uint8_t                 cmd;                    ///< �����֣�cmd����ʾ���������壬���Э�����
    uint8_t                 sn;                     ///< ��Ϣ���(sn)�ɷ��ͷ�����,���շ���Ӧ����ʱ�����Ϣ��ŷ��ظ����ͷ�
    uint8_t                 flags[2];               ///< ��־λ��flag������Ʒ��дĬ��0
} protocolHead_t;

/** 4.1 WiFiģ�������豸��Ϣ Э��ṹ�� */
__packed typedef struct
{
    protocolHead_t          head;                   ///< Э���׼ͷ�ṹ��
    uint8_t                 protocolVer[8];         ///< Э��汾��
    uint8_t                 p0Ver[8];               ///< p0Э��汾��
    uint8_t                 hardVer[8];             ///< Ӳ���汾��
    uint8_t                 softVer[8];             ///< ����汾��
    uint8_t                 productKey[32];         ///< ��Ʒ��ʶ��
    uint16_t                ninableTime;            ///< �󶨳�ʱ(��)
    uint8_t                 devAttr[8];             ///< �豸����
    uint8_t                 sum;                    ///< �����
} protocolDeviceInfo_t;

/** Э��ͨ������֡(4.2��4.4��4.6��4.9��4.10) Э��ṹ�� */
__packed typedef struct
{
    protocolHead_t          head;                   ///< Э���׼ͷ�ṹ��
    uint8_t                 sum;                    ///< �����
} protocolCommon_t;

/** 4.3 �豸MCU֪ͨWiFiģ���������ģʽ Э��ṹ�� */
__packed typedef struct
{
    protocolHead_t          head;                   ///< Э���׼ͷ�ṹ��
    uint8_t                 cfgMode;                ///< ���ò���
    uint8_t                 sum;                    ///< �����
} protocolCfgMode_t;

/** 4.13 MCU�����ȡ����ʱ�� Э��ṹ�� */
__packed typedef struct
{
    protocolHead_t          head;                   ///< Э���׼ͷ�ṹ��
    uint8_t                 time[7];                ///< Ӳ���汾��
    uint8_t                 ntp_time[4];            ///< ����汾��
    uint8_t                 sum;                    ///< �����
} protocolUTT_t;

/** WiFiģ�鹤��״̬*/
__packed typedef union
{
    uint16_t                value;
    __packed  struct
    {
        uint16_t            softap:1;               ///< ��ʾWiFiģ��������SOFTAPģʽ״̬������Ϊbool
        uint16_t            station:1;              ///< ��ʾWiFiģ��������STATIONģʽ״̬������Ϊbool
        uint16_t            onboarding:1;           ///< ��ʾWiFiģ������������״̬������Ϊbool
        uint16_t            binding:1;              ///< ��ʾWiFiģ�������İ�״̬������Ϊbool
        uint16_t            con_route:1;            ///< ��ʾWiFiģ����·����������״̬������Ϊbool
        uint16_t            con_m2m:1;              ///< ��ʾWiFiģ�����ƶ�m2m��״̬������Ϊbool
        uint16_t            reserve1:2;             ///< ����λ����
        uint16_t            rssi:3;                 ///< ��ʾ·�ɵ��ź�ǿ�ȣ�����Ϊ��ֵ
        uint16_t            app:1;                  ///< ��ʾWiFiģ����APP�˵�����״̬������Ϊbool
        uint16_t            test:1;                 ///< ��ʾWiFiģ�������ĳ���ģʽ״̬������Ϊbool
        uint16_t            reserve2:3;             ///< ����λ����
    }types; 
    
} wifiStatus_t;

/** WiFi״̬���� Э��ṹ�� */
__packed typedef struct
{
    protocolHead_t          head;                   ///< Э���׼ͷ�ṹ��
    wifiStatus_t            ststus;                 ///< WIFI״̬
    uint8_t                 sum;                    ///< �����
} protocolWifiStatus_t;

/** �Ƿ���������*/  
typedef enum
{
    ERROR_ACK_SUM = 0x01,                           ///< У�����
    ERROR_CMD     = 0x02,                           ///< ���������
    ERROR_OTHER   = 0x03,                           ///< ����
} errorPacketsType_t;

/** 4.7 �Ƿ���Ϣ֪ͨ Э��ṹ��*/
__packed    typedef struct
{
    protocolHead_t          head;                   ///< Э���׼ͷ�ṹ��
    uint8_t                 error;                  ///< ����ֵ
    uint8_t                 sum;                    ///< �����
} protocolErrorType_t;

/** P0 command ������*/
typedef enum
{
    ACTION_CONTROL_DEVICE       = 0x01,             ///< Э��4.10 WiFiģ������豸 WiFiģ�鷢��
    ACTION_READ_DEV_STATUS      = 0x02,             ///< Э��4.8 WiFiģ���ȡ�豸�ĵ�ǰ״̬ WiFiģ�鷢��
    ACTION_READ_DEV_STATUS_ACK  = 0x03,             ///< Э��4.8 WiFiģ���ȡ�豸�ĵ�ǰ״̬ �豸MCU�ظ�
    ACTION_REPORT_DEV_STATUS    = 0X04,             ///< Э��4.9 �豸MCU��WiFiģ�������ϱ���ǰ״̬ �豸MCU����
    ACTION_W2D_TRANSPARENT_DATA = 0x05,             ///< WiFi���豸MCU͸��
    ACTION_D2W_TRANSPARENT_DATA = 0x06,             ///< �豸MCU��WiFi͸��
} actionType_t;

/** 4.8 WiFiģ���ȡ�豸�ĵ�ǰ״̬ �豸MCU�ظ��ṹ�� */
__packed typedef struct
{
    protocolHead_t          head;                   ///< Э���׼ͷ�ṹ��
    uint8_t                 action;                 ///< p0 ����λ
    gizwitsReport_t         reportData;             ///< �ϱ�������
    uint8_t                 sum;                    ///< �����
} protocolReport_t;

/** P0���ı�׼ͷ */
__packed typedef struct
{
    protocolHead_t          head;                   ///< Э���׼ͷ�ṹ��
    uint8_t                 action;                 ///< p0 ����λ
} protocolP0Head_t;

/** ���λ��������ݽṹ */
typedef struct {
    size_t rbCapacity;
    uint8_t  *rbHead;
    uint8_t  *rbTail;
    uint8_t  *rbBuff;
}rb_t;

/** Э�鴦���ܽṹ�� */
__packed typedef struct
{
    uint8_t issuedFlag;                             ///< �����Ӧ�� p0��������
    uint8_t protocolBuf[MAX_PACKAGE_LEN];           ///< Э�鱨�Ĵ洢��
    uint8_t transparentBuff[MAX_PACKAGE_LEN];       ///< ͸�����ݴ洢��
    uint32_t transparentLen;                        ///< ͸�����ݳ���
    
    uint32_t sn;                                    ///< ��Ϣ���
    uint32_t timerMsCount;                          ///< ʱ�����(Ms)
    protocolWaitAck_t waitAck;                      ///< Э��ACK���ݽṹ
    
    eventInfo_t issuedProcessEvent;                 ///< �����¼�
    eventInfo_t wifiStatusEvent;                    ///< WIFI״̬ �¼�
    eventInfo_t NTPEvent;                           ///< NTP �¼�

    dataPoint_t gizCurrentDataPoint;                ///< ��ǰ�豸״̬����
    dataPoint_t gizLastDataPoint;                   ///< �ϴ��ϱ����豸״̬����
    gizwitsReport_t reportData;                     ///< Э���ϱ�ʵ������
    moduleStatusInfo_t wifiStatusData;              ///< WIFI ״̬��Ϣ(�ź�ǿ��)
    protocolTime_t TimeNTP;                         ///< ����ʱ����Ϣ
}gizwitsProtocol_t;

/**@name Gizwits �û�API�ӿ�
* @{
*/

int protocolHandle(dataPoint_t *currentData);

void gizwitsInit(void);
int32_t gizwitsSetMode(uint8_t mode);
void gizwitsGetNTP(void);
int32_t gizwitsHandle(dataPoint_t *currentData);
int32_t gizwitsPassthroughData(uint8_t * data, uint32_t len);
/**@} */

int32_t gizPutData(uint8_t *buf, uint32_t len);
uint32_t gizGetTimerCount(void);
void gizTimerMs(void);
void wifiAlink(void);

#endif