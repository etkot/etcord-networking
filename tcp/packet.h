#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <stdint.h>

#include "sockets.h"


namespace tcp {
	enum class PacketType;
	struct Packet;
}

enum class tcp::PacketType { None=0, Connect=1, Disconnect=2, Message=3 };

struct tcp::Packet
{
	SOCKET sender;
	PacketType type;
	char* data;
	uint16_t length;
};

#endif // TCPPACKET_H
