#include "MatterLikeCodec.h"

// -------- Public API --------

bool MatterLikeCodec::decode(
    MatterLikePacket& out,
    const uint8_t* data,
    size_t len)
{
    if (!data || len < WireSize) {
        return false;
    }

    size_t off = 0;

    // ---- Header ----
    out.header.messageCounter = readU32(data + off); off += 4;
    out.header.sessionId      = readU16(data + off); off += 2;
    out.header.flags          = data[off];           off += 1;

    // ---- Payload ----
    out.payload.fabricId    = readU32(data + off); off += 4;
    out.payload.nodeId      = readU16(data + off); off += 2;
    out.payload.endpointId  = data[off];           off += 1;
    out.payload.clusterId   = readU16(data + off); off += 2;
    out.payload.attributeId = readU16(data + off); off += 2;
    out.payload.commandId   = data[off];           off += 1;
    out.payload.value       = readI32(data + off); off += 4;

    return true;
}

size_t MatterLikeCodec::encode(
    const MatterLikePacket& pkt,
    uint8_t* out)
{
    if (!out) {
        return 0;
    }

    size_t off = 0;

    writeU32(out + off, pkt.header.messageCounter); off += 4;
    writeU16(out + off, pkt.header.sessionId);      off += 2;
    out[off++] = pkt.header.flags;

    writeU32(out + off, pkt.payload.fabricId);      off += 4;
    writeU16(out + off, pkt.payload.nodeId);        off += 2;
    out[off++] = pkt.payload.endpointId;
    writeU16(out + off, pkt.payload.clusterId);     off += 2;
    writeU16(out + off, pkt.payload.attributeId);   off += 2;
    out[off++] = pkt.payload.commandId;
    writeU32(out + off, static_cast<uint32_t>(pkt.payload.value)); off += 4;

    return off;
}

// -------- Private helpers --------

uint16_t MatterLikeCodec::readU16(const uint8_t* p)
{
    return static_cast<uint16_t>(p[0])
         | static_cast<uint16_t>(p[1]) << 8;
}

uint32_t MatterLikeCodec::readU32(const uint8_t* p)
{
    return static_cast<uint32_t>(p[0])
         | static_cast<uint32_t>(p[1]) << 8
         | static_cast<uint32_t>(p[2]) << 16
         | static_cast<uint32_t>(p[3]) << 24;
}

int32_t MatterLikeCodec::readI32(const uint8_t* p)
{
    return static_cast<int32_t>(readU32(p));
}

void MatterLikeCodec::writeU16(uint8_t* p, uint16_t v)
{
    p[0] = static_cast<uint8_t>(v);
    p[1] = static_cast<uint8_t>(v >> 8);
}

void MatterLikeCodec::writeU32(uint8_t* p, uint32_t v)
{
    p[0] = static_cast<uint8_t>(v);
    p[1] = static_cast<uint8_t>(v >> 8);
    p[2] = static_cast<uint8_t>(v >> 16);
    p[3] = static_cast<uint8_t>(v >> 24);
}
