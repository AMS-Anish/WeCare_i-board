/**
 * @file IPC_PacketCreator.hpp
 * @author Team Hugo
 * @brief 
 * @version 0.1
 * @date 2020-12-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef _IPC_PACKET_CREATOR_H_
#define _IPC_PACKET_CREATOR_H_

#include "Arduino.h"

#define IPC_HEADER_BYTE1        0xA5
#define IPC_HEADER_BYTE2        0xEE
#define IPC_HEADER_BYTE3        0x5A

#define INITIAL_PACKET_NO       0x00
#define MAX_PACKET_NO           256

typedef struct IPC_Packet
{
    // Header Bytes for packet recognisition
    uint8_t HeaderByte1;
    uint8_t HeaderByte2;
    uint8_t HeaderByte3;
    // Packet type to recognise it for further processing
    uint8_t packetType;
    //packet number to remove duplicates or request again
    uint8_t packetNumber;
    // number of the bytes in the payload
    uint8_t payloadByteCount;
    uint8_t *payload;
    uint8_t payload_CRC;
}IPC_Packet_t;

bool InitialiseIPC_PacketCreator(int pixelCount);
void DeinitialiseIPC_PacketCreator(void);
bool PacketCreator(CubeModule_t interactiveBoard[], int pixelCount);
void PacketParser(CubeModule_t interactiveBoard[], int pixelCount);
void GetOutgoingPacketCreated(IPC_Packet_t** IPC_packet_ref);

#endif /* _IPC_PACKET_CREATOR_H_ */