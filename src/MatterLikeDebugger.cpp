#include "MatterLikeDebugger.h"

void MatterLikeDebugger::print(const MatterPacketWithMac& pkt) {
    Serial.print("MAC: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X", pkt.mac.bytes[i]);
        if (i < 5) Serial.print(":");
    }
    Serial.println();
    print(pkt.packet);
}

void MatterLikeDebugger::print(const MatterLikePacket& pkt) {
    Serial.printf(
        "Version: %u, Counter: %lu, SessionId: %u, Flags: 0x%02X\n",
        pkt.header.version,
        pkt.header.messageCounter,
        pkt.header.sessionId,
        pkt.header.flags
    );

    Serial.printf(
        "FabricId: %lu, NodeId: %u, EndpointId: %u\n",
        pkt.payload.fabricId,
        pkt.payload.nodeId,
        pkt.payload.endpointId
    );

    Serial.printf(
        "ClusterId: %u, AttributeId: %u, CommandId: 0x%02X, Value: %ld\n",
        pkt.payload.clusterId,
        pkt.payload.attributeId,
        pkt.payload.commandId,
        pkt.payload.value
    );
    Serial.println("---------------------------");
}
