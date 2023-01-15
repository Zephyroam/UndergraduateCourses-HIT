/*
* THIS FILE IS FOR IP FORWARD TEST
*/
#include "sysInclude.h"
#include <stdio.h>
#include <vector>
using std::vector;
// system support
extern void fwd_LocalRcv(char *pBuffer, int length);

extern void fwd_SendtoLower(char *pBuffer, int length, unsigned int nexthop);

extern void fwd_DiscardPkt(char *pBuffer, int type);

extern unsigned int getIpv4Address();

// implemented by students

vector<stud_route_msg> route;

void stud_Route_Init()
{
}

void stud_route_add(stud_route_msg *proute)
{
	stud_route_msg tmp;
	tmp.dest = ntohl(proute->dest);
	tmp.masklen = ntohl(proute->masklen);
	tmp.nexthop = ntohl(proute->nexthop);
	route.push_back(tmp);
}

int stud_fwd_deal(char *pBuffer, int length)
{
	int head_length = pBuffer[0] & 0xf;
	short ttl = (unsigned short)pBuffer[8];
	short checksum = ntohs(*(unsigned short *)(pBuffer + 10));
	int dest = ntohl(*(unsigned int *)(pBuffer + 16));

	if (ttl <= 0)
	{
		fwd_DiscardPkt(pBuffer, STUD_FORWARD_TEST_TTLERROR);
		return 1;
	}
	else if (dest == getIpv4Address())
	{
		fwd_LocalRcv(pBuffer, length);
		return 0;
	}

	stud_route_msg *target_route = NULL;
	for (int i = 0; i < route.size(); i++)
		if ((unsigned int)(route[i].dest & ((1 << 31) >> (route[i].masklen - 1))) == dest)
		{
			target_route = &route[i];
			break;
		}

	if (target_route == NULL)
	{
		fwd_DiscardPkt(pBuffer, STUD_FORWARD_TEST_NOROUTE);
		return 1;
	}
	
	char *buffer = new char[length];
	memcpy(buffer, pBuffer, length);
	buffer[8] = ttl - 1;
	memset(buffer + 10, 0, 2);

	//checksum
	unsigned long sum = 0;
	for (int i = 0; i < head_length * 2; i++)
	{
		sum += (unsigned char)buffer[i * 2] << 8;
		sum += (unsigned char)buffer[i * 2 + 1];
	}

	unsigned short l_word = sum & 0xffff;
	unsigned short h_word = sum >> 16;
	checksum = ~(l_word + h_word);
	unsigned short header_checksum = htons(checksum);
	memcpy(buffer + 10, &header_checksum, 2);
	fwd_SendtoLower(buffer, length, target_route->nexthop);
	return 0;
}
