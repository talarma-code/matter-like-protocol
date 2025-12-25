#pragma once

#include <cstddef>
#include <cstdint>

// -------- Data structures (logical model) --------
#include "MatterLikePacket.h"

/**
 * @brief Binary codec for Matter-like packets (ESP-NOW friendly).
 *
 * Defines the exact wire format and provides deterministic
 * encode/decode routines with no dynamic allocation.
 *
 * Wire format is little-endian.
 */
class MatterLikeCodec {
public:
    /// Exact packet size on the wire (bytes)
    static constexpr size_t HeaderWireSize =
        sizeof(uint8_t)  + // version
        sizeof(uint32_t) + // messageCounter
        sizeof(uint16_t) + // sessionId
        sizeof(uint8_t);   // flags

    static constexpr size_t PayloadWireSize =
        sizeof(uint32_t) + // fabricId
        sizeof(uint16_t) + // nodeId
        sizeof(uint8_t)  + // endpointId
        sizeof(uint16_t) + // clusterId
        sizeof(uint16_t) + // attributeId
        sizeof(uint8_t)  + // commandId
        sizeof(int32_t);   // value

    static constexpr size_t WireSize =
        HeaderWireSize + PayloadWireSize;

    /**
     * @brief Decode packet from raw byte buffer
     *
     * @param out   Destination packet
     * @param data  Raw input buffer
     * @param len   Buffer length
     * @return true if decoding succeeded
     */
    static bool decode(
        MatterLikePacket& out,
        const uint8_t* data,
        size_t len);

    /**
     * @brief Encode packet into raw byte buffer
     *
     * @param pkt   Source packet
     * @param out   Destination buffer (must be >= WireSize)
     * @return Number of bytes written (WireSize on success)
     */
    static size_t encode(
        const MatterLikePacket& pkt,
        uint8_t* out);

private:
    // ---- Little-endian helpers ----
    static uint16_t readU16(const uint8_t* p);
    static uint32_t readU32(const uint8_t* p);
    static int32_t  readI32(const uint8_t* p);

    static void writeU16(uint8_t* p, uint16_t v);
    static void writeU32(uint8_t* p, uint32_t v);
};
