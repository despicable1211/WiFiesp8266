
#include "protocol.h"


/** Э��ȫ�ֱ��� **/
gizwitsProtocol_t gizwitsProtocol;



/**
* @brief ��������У��ͼ���
*
* @param [in] buf   : buf��ַ
* @param [in] len   : �ֽڳ���
*
* @return sum : �ӻ�������3���ֽں����е��ֽ��ۼ����
*/
static uint8_t gizProtocolSum(uint8_t *buf, uint32_t len)
{
    uint8_t     sum = 0;
    uint32_t    i = 0;

    if(buf == NULL || len <= 0)
    {
        printf("ERROR: gizProtocolSum , buf is empty or len error %d \n", len);
        return 0;
    }

    for(i=2; i<len-1; i++)
    {
        sum += buf[i];
    }

    return sum;
}


/**
* @brief 16λ�����ֽ���ת��
*
* @param [in] value : ��Ҫת��������
*
* @return  tmp_value: ת���������
*/
static uint16_t gizProtocolExchangeBytes(uint16_t value)
{
    uint16_t    tmp_value;
    uint8_t     *index_1, *index_2;

    index_1 = (uint8_t *)&tmp_value;
    index_2 = (uint8_t *)&value;

    *index_1 = *(index_2+1);
    *(index_1+1) = *index_2;

    return tmp_value;
}


/**
* @brief Э��ͷ��ʼ��
*
* @param [out] head         : Э��ͷָ��
*
* @return 0�� ִ�гɹ��� �� 0�� ʧ��    
*/
static int8_t gizProtocolHeadInit(protocolHead_t *head)
{
    if(NULL == head)
    {
        printf("ERROR: gizProtocolHeadInit head is empty \n");
        return -1;
    }

    memset((uint8_t *)head, 0, sizeof(protocolHead_t));
    head->head[0] = 0xFF;
    head->head[1] = 0xFF;

    return 0;
}

/*********ms��ʱ����ʱ����**********/

void gizTimerMs(void)
{
    gizwitsProtocol.timerMsCount++;
}

uint32_t gizGetTimerCount(void)
{
    return gizwitsProtocol.timerMsCount;
}
/************************************/


/**
* @brief ͨ�ñ���Э��
* 
* @param [in] head     Э��ͷָ��
*
* @return : ��ȷ������Ч����;���󷵻�-1
*/
static int32_t gizProtocolCommonAck(protocolHead_t *head)
{
    int32_t ret = 0;
    protocolCommon_t ack;

    if(NULL == head)
    {
        printf("ERROR: gizProtocolCommonAck data is empty \n");
        return -1;
    }
    memcpy((uint8_t *)&ack, (uint8_t *)head, sizeof(protocolHead_t));
    ack.head.cmd = ack.head.cmd+1;
    ack.head.len = gizProtocolExchangeBytes(sizeof(protocolCommon_t)-4);
    ack.sum = gizProtocolSum((uint8_t *)&ack, sizeof(protocolCommon_t));

    ret = uartWrite((uint8_t *)&ack, sizeof(protocolCommon_t));
    if(ret < 0)
    {
        
        printf("ERROR: DMA write error %d \n", ret);
    }

    return ret;
}

/**
* @brief Э��ACKУ�鴦����
*
* @param [in] data            : ���ݵ�ַ
* @param [in] len             : ���ݳ���
*
* @return 0�� ִ�гɹ��� �� 0�� ʧ��
*/
static int8_t gizProtocolWaitAck(uint8_t *data, uint32_t len)
{
    if(NULL == data)
    {
        printf("ERROR: data is empty \n");
        return -1;
    }

    memset((uint8_t *)&gizwitsProtocol.waitAck, 0, sizeof(protocolWaitAck_t));
    memcpy((uint8_t *)gizwitsProtocol.waitAck.buf, data, len);
    gizwitsProtocol.waitAck.dataLen = (uint16_t)len;
    
    gizwitsProtocol.waitAck.flag = 1;
    //gizwitsProtocol.waitAck.sendTime = gizGetTimerCount();

    return 0;
}


/**
* @brief Э��4.7 �Ƿ���Ϣ֪ͨ �Ĵ���

* @param[in] head  : Э��ͷָ��
* @param[in] errno : �Ƿ���Ϣ֪ͨ����
* @return 0�� ִ�гɹ��� �� 0�� ʧ��
*/
static int32_t gizProtocolErrorCmd(protocolHead_t *head,errorPacketsType_t errno)
{
    int32_t ret = 0;
    protocolErrorType_t errorType;

    if(NULL == head)
    {
        printf("gizProtocolErrorCmd Error , Illegal Param\n");
        return -1;
    }
    gizProtocolHeadInit((protocolHead_t *)&errorType);
    errorType.head.cmd = ACK_ERROR_PACKAGE;
    errorType.head.sn = head->sn;
    
    errorType.head.len = gizProtocolExchangeBytes(sizeof(protocolErrorType_t)-4);
    errorType.error = errno;
    errorType.sum = gizProtocolSum((uint8_t *)&errorType, sizeof(protocolErrorType_t));
    
    ret = uartWrite((uint8_t *)&errorType, sizeof(protocolErrorType_t));
    if(ret < 0)
    {
        //��ӡ��־
        printf("ERROR: uart write error %d \n", ret);
    }

    return ret;
}


/**
* @brief �ϱ�����
*
* @param [in] action            : PO cmd
* @param [in] data              : ���ݵ�ַ
* @param [in] len               : ���ݳ���
*
* @return : ��ȷ������Ч���ݳ���;-1�����󷵻�
*/
static int32_t gizReportData(uint8_t action, uint8_t *data, uint32_t len)
{
    int32_t ret = 0;
    protocolReport_t protocolReport;

    if(NULL == data)
    {
        printf("gizReportData Error , Illegal Param\n");
        return -1;
    }
    gizProtocolHeadInit((protocolHead_t *)&protocolReport);
    protocolReport.head.cmd = CMD_REPORT_P0;
    protocolReport.head.sn = gizwitsProtocol.sn++;
    protocolReport.action = action;
    protocolReport.head.len = gizProtocolExchangeBytes(sizeof(protocolReport_t)-4);
    memcpy((gizwitsReport_t *)&protocolReport.reportData, (gizwitsReport_t *)data,len);
    protocolReport.sum = gizProtocolSum((uint8_t *)&protocolReport, sizeof(protocolReport_t));
    
    ret = uartWrite((uint8_t *)&protocolReport, sizeof(protocolReport_t));
    if(ret < 0)
    {
        //��ӡ��־
        printf("ERROR: dma write error %d \n", ret);
    }

    gizProtocolWaitAck((uint8_t *)&protocolReport, sizeof(protocolReport_t));

    return ret;
}


/**
* @brief Э�� 4.5 WiFiģ�����豸MCU֪ͨWiFiģ�鹤��״̬�ı仯

* @param[in] status WiFiģ�鹤��״̬
* @return none
*/
static int8_t gizProtocolModuleStatus(protocolWifiStatus_t *status)
{
    static wifiStatus_t lastStatus;

    if(NULL == status)
    {
        printf("gizProtocolModuleStatus Error , Illegal Param\n");
        return -1;
    }

    status->ststus.value = gizProtocolExchangeBytes(status->ststus.value);
   
    //OnBoarding mode status
    if(lastStatus.types.onboarding != status->ststus.types.onboarding)
    {
        if(1 == status->ststus.types.onboarding)
        {
            if(1 == status->ststus.types.softap)
            {
                gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_SOFTAP;
                gizwitsProtocol.wifiStatusEvent.num++;
                printf("OnBoarding: SoftAP or Web mode\n");
            }

            if(1 == status->ststus.types.station)
            {
                gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_AIRLINK;
                gizwitsProtocol.wifiStatusEvent.num++;
                printf("OnBoarding: AirLink mode\n");
            }
        }
        else
        {
            if(1 == status->ststus.types.softap)
            {
                gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_SOFTAP;
                gizwitsProtocol.wifiStatusEvent.num++;
                printf("OnBoarding: SoftAP or Web mode\n");
            }

            if(1 == status->ststus.types.station)
            {
                gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_STATION;
                gizwitsProtocol.wifiStatusEvent.num++;
                printf("OnBoarding: Station mode\n");
            }
        }
    }

    //binding mode status
    if(lastStatus.types.binding != status->ststus.types.binding)
    {
        lastStatus.types.binding = status->ststus.types.binding;
        if(1 == status->ststus.types.binding)
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_OPEN_BINDING;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: in binding mode\n");
        }
        else
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_CLOSE_BINDING;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: out binding mode\n");
        }
    }

    //router status
    if(lastStatus.types.con_route != status->ststus.types.con_route)
    {
        lastStatus.types.con_route = status->ststus.types.con_route;
        if(1 == status->ststus.types.con_route)
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_CON_ROUTER;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: connected router\n");
        }
        else
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_DISCON_ROUTER;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: disconnected router\n");
        }
    }

    //M2M server status
    if(lastStatus.types.con_m2m != status->ststus.types.con_m2m)
    {
        lastStatus.types.con_m2m = status->ststus.types.con_m2m;
        if(1 == status->ststus.types.con_m2m)
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_CON_M2M;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: connected m2m\n");
        }
        else
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_DISCON_M2M;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: disconnected m2m\n");
        }
    }

    //APP status
    if(lastStatus.types.app != status->ststus.types.app)
    {
        lastStatus.types.app = status->ststus.types.app;
        if(1 == status->ststus.types.app)
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_CON_APP;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: app connect\n");
        }
        else
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_DISCON_APP;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: no app connect\n");
        }
    }

    //test mode status
    if(lastStatus.types.test != status->ststus.types.test)
    {
        lastStatus.types.test = status->ststus.types.test;
        if(1 == status->ststus.types.test)
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_OPEN_TESTMODE;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: in test mode\n");
        }
        else
        {
            gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_CLOSE_TESTMODE;
            gizwitsProtocol.wifiStatusEvent.num++;
            printf("WiFi status: out test mode\n");
        }
    }

    gizwitsProtocol.wifiStatusEvent.event[gizwitsProtocol.wifiStatusEvent.num] = WIFI_RSSI;
    gizwitsProtocol.wifiStatusEvent.num++;
    gizwitsProtocol.wifiStatusData.rssi = status->ststus.types.rssi;
    printf("RSSI is %d \n", gizwitsProtocol.wifiStatusData.rssi);

    gizwitsProtocol.issuedFlag = WIFI_STATUS_TYPE;

    return 0;
}


/**
* @brief WiFi���ýӿ�

* �û����Ե��øýӿ�ʹWiFiģ�������Ӧ������ģʽ���߸�λģ��

* @param[in] mode ����ģʽѡ��0x0�� ģ�鸴λ ;0x01�� SoftApģʽ ;0x02�� AirLinkģʽ ;0x03�� ����ģʽģʽ; 0x04:�����û����豸

* @return ����������
*/
int32_t gizwitsSetMode(uint8_t mode)
{
    int32_t ret = 0;
    protocolCfgMode_t cfgMode;
    protocolCommon_t setDefault;
    switch(mode)
    {
        case WIFI_RESET_MODE:
            gizProtocolHeadInit((protocolHead_t *)&setDefault);
            setDefault.head.cmd = CMD_SET_DEFAULT;
            setDefault.head.sn = gizwitsProtocol.sn++;
            setDefault.head.len = gizProtocolExchangeBytes(sizeof(protocolCommon_t)-4);
            setDefault.sum = gizProtocolSum((uint8_t *)&setDefault, sizeof(protocolCommon_t));
            ret = uartWrite((uint8_t *)&setDefault, sizeof(protocolCommon_t));
            if(ret < 0)
            {
                printf("ERROR: uart write error %d \n", ret);
            }

            gizProtocolWaitAck((uint8_t *)&setDefault, sizeof(protocolCommon_t));   
            break;
        case WIFI_SOFTAP_MODE:
            gizProtocolHeadInit((protocolHead_t *)&cfgMode);
            cfgMode.head.cmd = CMD_WIFI_CONFIG;
            cfgMode.head.sn = gizwitsProtocol.sn++;
            cfgMode.cfgMode = mode;
            cfgMode.head.len = gizProtocolExchangeBytes(sizeof(protocolCfgMode_t)-4);
            cfgMode.sum = gizProtocolSum((uint8_t *)&cfgMode, sizeof(protocolCfgMode_t));
            ret = uartWrite((uint8_t *)&cfgMode, sizeof(protocolCfgMode_t));
            if(ret < 0)
            {
                printf("ERROR: uart write error %d \n", ret);
            }
            gizProtocolWaitAck((uint8_t *)&cfgMode, sizeof(protocolCfgMode_t)); 
            break;
        case WIFI_AIRLINK_MODE:
            gizProtocolHeadInit((protocolHead_t *)&cfgMode);
            cfgMode.head.cmd = CMD_WIFI_CONFIG;
            cfgMode.head.sn = gizwitsProtocol.sn++;
            cfgMode.cfgMode = mode;
            cfgMode.head.len = gizProtocolExchangeBytes(sizeof(protocolCfgMode_t)-4);
            cfgMode.sum = gizProtocolSum((uint8_t *)&cfgMode, sizeof(protocolCfgMode_t));
            ret = uartWrite((uint8_t *)&cfgMode, sizeof(protocolCfgMode_t));
            if(ret < 0)
            {
                printf("ERROR: uart write error %d \n", ret);
            }
            gizProtocolWaitAck((uint8_t *)&cfgMode, sizeof(protocolCfgMode_t)); 
            break;
        case WIFI_PRODUCTION_TEST:
            gizProtocolHeadInit((protocolHead_t *)&setDefault);
            setDefault.head.cmd = CMD_PRODUCTION_TEST;
            setDefault.head.sn = gizwitsProtocol.sn++;
            setDefault.head.len = gizProtocolExchangeBytes(sizeof(protocolCommon_t)-4);
            setDefault.sum = gizProtocolSum((uint8_t *)&setDefault, sizeof(protocolCommon_t));
            ret = uartWrite((uint8_t *)&setDefault, sizeof(protocolCommon_t));
            if(ret < 0)
            {
                printf("ERROR: dma write error %d \n", ret);
            }

            gizProtocolWaitAck((uint8_t *)&setDefault, sizeof(protocolCommon_t));
            break;
        case WIFI_NINABLE_MODE:
            gizProtocolHeadInit((protocolHead_t *)&setDefault);
            setDefault.head.cmd = CMD_NINABLE_MODE;
            setDefault.head.sn = gizwitsProtocol.sn++;
            setDefault.head.len = gizProtocolExchangeBytes(sizeof(protocolCommon_t)-4);
            setDefault.sum = gizProtocolSum((uint8_t *)&setDefault, sizeof(protocolCommon_t));
            ret = uartWrite((uint8_t *)&setDefault, sizeof(protocolCommon_t));
            if(ret < 0)
            {
                printf("ERROR: dma write error %d \n", ret);
            }

            gizProtocolWaitAck((uint8_t *)&setDefault, sizeof(protocolCommon_t)); 
            break;
        default:
            printf("ERROR: CfgMode error!\n");
            break;
    }

    return ret;
}

void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  
      while(i--) ;    
   }
}
/**
* @brief ��DMA���ջ�������ץȡһ������
*
* @param [in]  rb                  : �������ݵ�ַ
* @param [out] data                : ������ݵ�ַ
* @param [out] len                 : ������ݳ���
*
* @return : 0,��ȷ����;-1�����󷵻�;-2������У��ʧ��
*/
static uint8_t erropCount = 0;
static int8_t gizProtocolGetOnePacket(u8 *rb, uint8_t *data, uint16_t *len)
{
    uint8_t sum = 0;
    uint8_t i = 0,temp = 0;
    uint8_t tmpData;
    uint16_t tmpCount = 0;
    static uint8_t protocolFlag = 0;
    static uint16_t protocolCount = 0;
    static uint8_t lastData = 0;
    static uint8_t debugCount=0;
    
    uint8_t *protocolBuff = data;
    protocolHead_t *head = NULL;

    if((NULL == rb) || (NULL == data) ||(NULL == len))
    {
        printf("gizProtocolGetOnePacket Error , Illegal Param\n");
        return -1;
    }
    temp = BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6); //��ý��յ����ֽ���
    if(temp < 5){
      return 1;
    }
    for(i=0;i<150;i++)
    {     
      tmpData = rb[i];
      if((0xFF == lastData) && (0xFF == tmpData))
      {
          if(0 == protocolFlag)
          {
              protocolBuff[0] = 0xFF;
              protocolBuff[1] = 0xFF;
              protocolCount = 2;
              protocolFlag = 1;
          }
          else
          {
              if((protocolCount > 4) && (protocolCount != tmpCount))
              {
                  protocolBuff[0] = 0xFF;
                  protocolBuff[1] = 0xFF;
                  protocolCount = 2;
              }
          }
      }
      else if((0xFF == lastData) && (0x55 == tmpData))
      {
      }
      else
      {
          if(1 == protocolFlag)
          {
              protocolBuff[protocolCount] = tmpData;
              protocolCount++;

              if(protocolCount > 4)
              {
                  head = (protocolHead_t *)protocolBuff;
                  tmpCount = gizProtocolExchangeBytes(head->len)+4;
                  if(tmpCount>temp)
                  {
                    erropCount++;
                    return 1;
                  } 
                  if(protocolCount == tmpCount)
                  {
                    printf("\nhead:%x\n",head->cmd);  
                    break;
                  }
              }
          }
      }

      lastData = tmpData;
    }

    if((protocolCount > 4) && (protocolCount == tmpCount))
    {
        sum = gizProtocolSum(protocolBuff, protocolCount);
        
        if(protocolBuff[protocolCount-1] == sum)
        {
            memcpy(data, protocolBuff, tmpCount);
            *len = tmpCount;
            protocolFlag = 0;

            protocolCount = 0;
            debugCount = 0;
            erropCount = 0;
            lastData = 0;
            return 0;
        }
        else
        {  
          printf("sum error[rb]=:");
            for(i=0;i<tmpCount;i++)
            {
              printf("%02x",protocolBuff[i]);
            }
            printf("\n");
            return -2;
        }
    } 
    return 1;
}

/**
* @brief Э��4.1 WiFiģ�������豸��Ϣ
*
* @param head : �����͵�Э�鱨������
*
* @return ������Ч���ݳ���; -1�����󷵻�
*/
static int32_t gizProtocolGetDeviceInfo(protocolHead_t * head)
{  
    int32_t ret = 0;
    protocolDeviceInfo_t deviceInfo;

    if(NULL == head)
    {
        printf("gizProtocolGetDeviceInfo Error , Illegal Param\n");
        return -1;
    }

    gizProtocolHeadInit((protocolHead_t *)&deviceInfo);
    deviceInfo.head.cmd = ACK_GET_DEVICE_INFO;
    deviceInfo.head.sn = head->sn;
    memcpy((uint8_t *)deviceInfo.protocolVer, protocol_VERSION, 8);
    memcpy((uint8_t *)deviceInfo.p0Ver, P0_VERSION, 8);
    memcpy((uint8_t *)deviceInfo.softVer, SOFTWARE_VERSION, 8);
    memcpy((uint8_t *)deviceInfo.hardVer, HARDWARE_VERSION, 8);
    memcpy((uint8_t *)deviceInfo.productKey, PRODUCT_KEY, 32);
    memset((uint8_t *)deviceInfo.devAttr, 0, 8);
    deviceInfo.devAttr[7] |= DEV_IS_GATEWAY<<0;
    deviceInfo.devAttr[7] |= (0x01<<1);
    deviceInfo.ninableTime = gizProtocolExchangeBytes(NINABLETIME);
    deviceInfo.head.len = gizProtocolExchangeBytes(sizeof(protocolDeviceInfo_t)-4);
    deviceInfo.sum = gizProtocolSum((uint8_t *)&deviceInfo, sizeof(protocolDeviceInfo_t));
    ret = uartWrite((uint8_t *)&deviceInfo, sizeof(protocolDeviceInfo_t));
    if(ret < 0)
    {
        //��ӡ��־
        printf("ERROR: DMA write error %d \n", ret);
    }
    
    return ret;
}

/**
* @brief ����Э�鱨�Ĳ�������Ӧ��Э�鴦��

* Wifiģ����������ƶ˻�APP���·������Э�����ݷ��͵�MCU�ˣ�����Э�鱨�Ľ��������Э�����ݴ���κ�����������һ����Э�鴦��

* @param[in] inData   : �����Э������
* @param[in] inLen    : �������ݵĳ���
* @param[out] outData : �����Э������
* @param[out] outLen  : ������ݵĳ���
* @return 0�� ִ�гɹ��� �� 0�� ʧ��
*/
static int8_t gizProtocolIssuedProcess(uint8_t *inData, uint32_t inLen, uint8_t *outData, uint32_t *outLen)
{
    protocolReport_t *protocolIssuedData = (protocolReport_t *)inData;
    uint8_t issuedAction = 0;
    issuedAction = protocolIssuedData->action;

    if((NULL == inData)||(NULL == outData)||(NULL == outLen))
    {
        printf("gizProtocolIssuedProcess Error , Illegal Param\n");
        return -1;
    }
    
    memset((uint8_t *)&gizwitsProtocol.issuedProcessEvent, 0, sizeof(eventInfo_t));
    switch(issuedAction)
    {
        case ACTION_CONTROL_DEVICE:
            //gizDataPoint2Event((gizwitsIssued_t *)(inData+sizeof(protocolP0Head_t)), &gizwitsProtocol.issuedProcessEvent,&gizwitsProtocol.gizCurrentDataPoint);
            gizwitsProtocol.issuedFlag = ACTION_CONTROL_TYPE;
            outData = NULL;
            *outLen = 0;
            break;
        
        case ACTION_READ_DEV_STATUS:
            /*if(0 == gizDataPoints2ReportData(&gizwitsProtocol.gizLastDataPoint,&gizwitsProtocol.reportData.devStatus))
            {
                memcpy(outData, (uint8_t *)&gizwitsProtocol.reportData.devStatus, sizeof(gizwitsReport_t));
                *outLen = sizeof(gizwitsReport_t);
            }
            else
            {
                return -1;
            }*/
            break;
        case ACTION_W2D_TRANSPARENT_DATA:
            memcpy(gizwitsProtocol.transparentBuff, inData+sizeof(protocolP0Head_t), inLen-sizeof(protocolP0Head_t)-1);
            gizwitsProtocol.transparentLen = inLen - sizeof(protocolP0Head_t) - 1;
            gizwitsProtocol.issuedProcessEvent.event[gizwitsProtocol.issuedProcessEvent.num] = TRANSPARENT_DATA;
            gizwitsProtocol.issuedProcessEvent.num++;
            gizwitsProtocol.issuedFlag = ACTION_W2D_TRANSPARENT_TYPE;
            outData = NULL;
            *outLen = 0;
            break;
        
        default:
            break;
    }

    return 0;
}
void DMA_Resart(void)
{
  DMA_Cmd(DMA1_Channel6, DISABLE);       // �ر�DMA ����ֹ����
  memset(ReceiBuff, 0, BUF_SIZE);
  
  DMA1_Channel6->CNDTR = BUF_SIZE;       //  ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ

  DMA_Cmd(DMA1_Channel6, ENABLE);        // DMA �������ȴ����ݡ�
}
/********************************
@brief  :����Э�����ݴ������û������ϴ�
@param  :currentData �û�����       
**********************************/
int protocolHandle(dataPoint_t *currentData)
{
  int8_t ret = 0;
  uint8_t ackData[RB_MAX_LEN];
  uint16_t protocolLen = 0;
  uint32_t ackLen = 0;
  protocolHead_t *recvHead = NULL;
  
  ret = gizProtocolGetOnePacket(ReceiBuff,gizwitsProtocol.protocolBuf, &protocolLen);
  if(erropCount>8)
  {
    erropCount = 0;
    //DMA_Resart();
    //printf("receive error more than 3\n");
  }
  if(ret == 0)
  {
#ifdef PRINTFPROTOCOL  //��ӡ���յ������ݰ�
  printf("get one backpack\n");
  printf("WIFI to MCU:");
  for(int i=0;i<protocolLen;i++) printf("%02x",gizwitsProtocol.protocolBuf[i]);
  printf("\n");
#endif

    recvHead = (protocolHead_t *)gizwitsProtocol.protocolBuf;
    DMA_Resart();
    switch(recvHead->cmd)
    {
      case CMD_GET_DEVICE_INTO:   //WiFiģ���ȡ�豸��Ϣ  ����0x01
        gizProtocolGetDeviceInfo(recvHead);
        break;
        
      case CMD_ISSUED_P0://wifiģ������豸		����0x03
        if(0 == gizProtocolIssuedProcess(gizwitsProtocol.protocolBuf, protocolLen, ackData, &ackLen))
        {
            gizProtocolCommonAck(recvHead);
        }
        break;
              
      case CMD_HEARTBEAT://WiFi����ά��		����0x07
        gizProtocolCommonAck(recvHead);
        break;
      
      case CMD_WIFISTATUS://wifiģ�齫��ǰ�乤��״̬���͸�mcu
        gizProtocolCommonAck(recvHead);
        //����WiFi����״̬ ���������������������ݲ�����
        break;
      default:
        break;
    }
  }
  else if(ret == -2)
  {
    //У��ʧ�ܣ������쳣
    recvHead = (protocolHead_t *)gizwitsProtocol.protocolBuf;
    gizProtocolErrorCmd(recvHead,ERROR_ACK_SUM);
    printf("ERROR: check sum error!\n");
    return -2;
  }
  
   switch(gizwitsProtocol.issuedFlag)
    {
        case ACTION_CONTROL_TYPE:
            gizwitsProtocol.issuedFlag = STATELESS_TYPE;
            //gizwitsEventProcess(&gizwitsProtocol.issuedProcessEvent, (uint8_t *)&gizwitsProtocol.gizCurrentDataPoint, sizeof(dataPoint_t));
            memset((uint8_t *)&gizwitsProtocol.issuedProcessEvent,0x0,sizeof(gizwitsProtocol.issuedProcessEvent));  
            break;
        case WIFI_STATUS_TYPE:
            gizwitsProtocol.issuedFlag = STATELESS_TYPE;
            //gizwitsEventProcess(&gizwitsProtocol.wifiStatusEvent, (uint8_t *)&gizwitsProtocol.wifiStatusData, sizeof(moduleStatusInfo_t));
            memset((uint8_t *)&gizwitsProtocol.wifiStatusEvent,0x0,sizeof(gizwitsProtocol.wifiStatusEvent));
            break;
        case ACTION_W2D_TRANSPARENT_TYPE:
            gizwitsProtocol.issuedFlag = STATELESS_TYPE;
            //gizwitsEventProcess(&gizwitsProtocol.issuedProcessEvent, (uint8_t *)gizwitsProtocol.transparentBuff, gizwitsProtocol.transparentLen);
            break;
        case GET_NTP_TYPE:
            gizwitsProtocol.issuedFlag = STATELESS_TYPE;
            //gizwitsEventProcess(&gizwitsProtocol.NTPEvent, (uint8_t *)&gizwitsProtocol.TimeNTP, sizeof(protocolTime_t));
            memset((uint8_t *)&gizwitsProtocol.NTPEvent,0x0,sizeof(gizwitsProtocol.NTPEvent));
            break;      
   }
  
  /*if(0 == (gizGetTimerCount() % (1000*60*10)))
  {
      printf("Info: 600S report data\n");
      gizReportData(ACTION_REPORT_DEV_STATUS, (uint8_t *)&gizwitsProtocol.reportData.devStatus, sizeof(devStatus_t));
      memcpy((uint8_t *)&gizwitsProtocol.gizLastDataPoint, (uint8_t *)currentData, sizeof(dataPoint_t));
  }
  memset(gizwitsProtocol.protocolBuf, 0, protocolLen);*/
  
  return  1;
}