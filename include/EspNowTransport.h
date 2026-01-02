#pragma once
#include <Arduino.h>
#include <esp_now.h>
#include "IMatterLikeTransport.h"
#include "MatterLikePacket.h"

class EspNowTransport : public IMatterLikeTransport {
public:

    bool begin(const uint8_t *myMac, const uint8_t *peerMac) override;
    bool send(const uint8_t *peerMac, const MatterLikePacket &packet) override;
    void onPacketReceived(IMatterReceiver *receiver) override;

private:
    static void onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len);
    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

    static IMatterReceiver *userReceiver;

};
