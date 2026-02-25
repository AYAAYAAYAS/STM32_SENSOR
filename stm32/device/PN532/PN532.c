#include "Sensor_head.h"
PN532_Frame gPN532Header;
void PN532_Init(void)
{
    gPN532Header.Preamble = 0x00;
    gPN532Header.StartCode[0] = 0x00;
    gPN532Header.StartCode[1] = 0xFF;
    gPN532Header.Packet_Len = 0x03;
    gPN532Header.LCS = 0xFF - gPN532Header.Packet_Len +1;
    gPN532Header.TFI = 0xD4; // 主机到PN532 D4,PN532到主机 D5
    gPN532Header.Command[0] = 0x14; 
    gPN532Header.Command[1] = 0x01;
    gPN532Header.DCS = 0XFF - (gPN532Header.TFI + gPN532Header.Command[0] + gPN532Header.Command[1]) + 1;
    gPN532Header.EndCode=0x00;
}