#ifndef PN532_H
#define PN532_H

typedef struct {
    uint8_t uid[7]; // UID最大长度为7字节
    uint8_t uidLength; // 实际UID长度
} PN532_UID;

typedef struct
{
    uint8_t Preamble;           //序文
    uint8_t StartCode[2];         //起始码
    uint8_t Packet_Len;
    uint8_t LCS;           //长度校验和
    uint8_t TFI;
    uint8_t Command[16];            //命令字
    uint8_t DCS;           //数据校验和
    uint8_t Data[16];            //数据
    uint8_t EndCode;          //结束码
}PN532_Frame;


/**************************Miscellaneous_Command************************/
#define  Miscellaneous_Diagnose                    0x00
#define  Miscellaneous_GetFirmwareVersion          0x02
#define  Miscellaneous_GetGeneralStatus            0x04
#define  Miscellaneous_ReadRegister                0x06
#define  Miscellaneous_WriteRegister               0x08
#define  Miscellaneous_ReadGPIO                    0x0C
#define  Miscellaneous_WriteGPIO                   0x0E
#define  Miscellaneous_SetSerialBaudrate           0x10
#define  Miscellaneous_SetParameters               0x12
#define  Miscellaneous_SAMConfiguration            0x14
#define  Miscellaneous_PowerDown                   0x16
/**************************Miscellaneous_Command************************/

/***********************RF_Communication_Command************************/
#define  RF_Communication_RFConfiguration          0x32
#define  RF_Communication_RFRegulationTest         0x58
/***********************RF_Communication_Command************************/

/***********************Initiator_Command*******************************/
#define  Initiator_InJumpForDEP              0x56
#define  Initiator_InJumpForPSL              0x46
#define  Initiator_InListPassiveTarget      0x4A
#define  Initiator_InATR                     0x50
#define  Initiator_InPSL                     0x4E
#define  Initiator_InDataExchange            0x40
#define  Initiator_InCommunicateWithPICC     0x42
#define  Initiator_InDeselect                0x44
#define  Initiator_InRelease                 0x52
#define  Initiator_InSelect                  0x54
#define  Initiator_InAutoPoll                0x60
/************************Initiator_Command******************************/

/*************************Target_Command********************************/
#define Target_TgInitAsTarget               0x8C
#define Target_TgSetGeneralBytes            0x92
#define Target_TgGetDEPData                 0x86
#define Target_TgSetDEPData                 0x8E
#define Target_TgResponseToInitiator        0x94
#define Target_TgGetInitiator               0x88
#define Target_TgResponseToRequest          0x90
#define Target_TgGetTargetStatus            0x8A
/*************************Target_Command********************************/

void PN532_Init(void);
#endif // PN532_H