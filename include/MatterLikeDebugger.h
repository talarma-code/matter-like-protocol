#pragma once
#include <Arduino.h>
#include "MatterLikeCodec.h"  // definicje struktur

class MatterLikeDebugger {
public:
    /// Print full packet including MAC address
    static void print(const MatterPacketWithMac& pkt);

    /// Print only the MatterLikePacket
    static void print(const MatterLikePacket& pkt);
};
