#pragma once
#include "MatterLikePacket.h"

class IMatterReceiver {
public:
    virtual void handlePacket(const MatterPacketWithMac &pkt) = 0;
    virtual ~IMatterReceiver() = default;
};
