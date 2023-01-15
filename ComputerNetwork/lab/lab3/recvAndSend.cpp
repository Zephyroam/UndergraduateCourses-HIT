/*
    * THIS FILE IS FOR IP TEST
*/
// system support
#include "sysInclude.h"
#include <stdio.h>
#include <malloc.h>
extern void ip_DiscardPkt(char *pBuffer, int type);

extern void ip_SendtoLower(char *pBuffer, int length);

extern void ip_SendtoUp(char *pBuffer, int length);

extern unsigned int getIpv4Address();

// implemented by students

int stud_ip_recv(char *pBuffer, unsigned short length)
{
    int version = pBuffer[0] >> 4;
    int head_length = pBuffer[0] & 0xf;
    short ttl = (unsigned short)pBuffer[8];
    short checksum = ntohs(*(unsigned short *)(pBuffer + 10));
    int dest = ntohl(*(unsigned int *)(pBuffer + 16));

    if (ttl == 0)
    {
        ip_DiscardPkt(pBuffer, STUD_IP_TEST_TTL_ERROR);
        return 1;
    }
    else if (version != 4)
    {
        ip_DiscardPkt(pBuffer, STUD_IP_TEST_VERSION_ERROR);
        return 1;
    }
    else if (head_length < 5)
    {
        ip_DiscardPkt(pBuffer, STUD_IP_TEST_HEADLEN_ERROR);
        return 1;
    }
    else if (dest != getIpv4Address() && dest != 0xffff)
    {
        ip_DiscardPkt(pBuffer, STUD_IP_TEST_DESTINATION_ERROR);
        return 1;
    }
    
    // checksum
    unsigned long sum = 0;
    for (int i = 0; i < head_length * 2; i++)
    {
        sum += (unsigned char)pBuffer[i * 2] << 8;
        sum += (unsigned char)pBuffer[i * 2 + 1];
    }
    unsigned short l_word = sum & 0xffff;
    unsigned short h_word = sum >> 16;
    if (l_word + h_word != 0xffff)
    {
        ip_DiscardPkt(pBuffer, STUD_IP_TEST_CHECKSUM_ERROR);
        return 1;
    }

    ip_SendtoUp(pBuffer, length);
    return 0;
}

int stud_ip_Upsend(char *pBuffer, unsigned short len, unsigned int srcAddr,
                   unsigned int dstAddr, byte protocol, byte ttl)
{
    short length = len + 20;
    char *buffer = (char *)malloc(length * sizeof(char));
    memset(buffer, 0, length);

    buffer[0] = 0x45;
    buffer[8] = ttl;
    buffer[9] = protocol;
    
    unsigned short network_length = htons(length);
    
    memcpy(buffer + 2, &network_length, 2);
    unsigned int src = htonl(srcAddr);
    unsigned int dst = htonl(dstAddr);

    memcpy(buffer + 12, &src, 4);
    memcpy(buffer + 16, &dst, 4);

    // checksum
    unsigned long sum = 0;
    for (int i = 0; i < 20; i += 2)
    {
        sum += (unsigned char)buffer[i] << 8;
        sum += (unsigned char)buffer[i + 1];
    }
    unsigned short l_word = sum & 0xffff;
    unsigned short h_word = sum >> 16;
    unsigned short checksum = l_word + h_word;
    checksum = ~checksum;
    unsigned short header_checksum = htons(checksum);
    
    memcpy(buffer + 10, &header_checksum, 2);
    memcpy(buffer + 20, pBuffer, len);
    
    ip_SendtoLower(buffer, len + 20);
    return 0;
}
