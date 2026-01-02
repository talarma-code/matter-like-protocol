#include "EspNowTransport.h"
#include "MatterLikePacket.h"
#include "MatterLikeCodec.h"
#include <cstring>
#include <WiFi.h>
#include <esp_wifi.h>


static uint8_t MAX_ESP_NOW_FRAME = 250;

IMatterReceiver* EspNowTransport::userReceiver = nullptr;



bool EspNowTransport::begin(const uint8_t *myMac, const uint8_t *peerMac) {
    delay(300);
    WiFi.mode(WIFI_STA);

    // set MAC address for this device (NEW: esp_wifi_set_mac requiered seperate include)
    if (esp_wifi_set_mac(WIFI_IF_STA, myMac) != ESP_OK) {
        Serial.println("⚠ Could not set custom MAC!");
    }

    Serial.print("Local MAC set to: ");
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                   myMac[0], myMac[1], myMac[2],
                   myMac[3], myMac[4], myMac[5]);

    // Start ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("❌ ESP-NOW init failed!");
        return false;
    }

    // Add reciver (heater) as PEER
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, peerMac, 6);
    peerInfo.channel = 0;  // the same channel as in WiFi
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) == ESP_OK) {
        Serial.println("Peer added OK");
    } else {
        Serial.println("❌ Peer add FAILED!");
    }

    esp_now_register_recv_cb(onDataRecv);
    esp_now_register_send_cb(onDataSent);

    Serial.println("ESP-NOW initialized successfully");
    return true;
}

bool EspNowTransport::send(const uint8_t *peerMac, const MatterLikePacket &packet) {

    if (MatterLikeCodec::WireSize > MAX_ESP_NOW_FRAME) { // ESP-NOW max payload ~250B
        Serial.println("Packet too large for ESP-NOW!");
        return false;
    }

    uint8_t buffer[MatterLikeCodec::WireSize];
    MatterLikeCodec::encode(packet, buffer);
   
    esp_err_t result = esp_now_send(peerMac, buffer, MatterLikeCodec::WireSize);
    return (result == ESP_OK);
}


void EspNowTransport::onPacketReceived(IMatterReceiver *receiver) {
    userReceiver = receiver;
}

void EspNowTransport::onDataRecv(const uint8_t *mac_addr, const uint8_t *data, int len) {

    Serial.println("EspNowTransport::onDataRecv");
    if (!userReceiver) {
        Serial.printf("EspNowTransport - userReceiver NOT REGISTERED!!!\n");
        return;
    }
 
    MatterPacketWithMac pkt;
    // copy MAC address
    memcpy(pkt.mac.bytes, mac_addr, 6);

    if (!MatterLikeCodec::decode(pkt.packet, data, len)) {
        Serial.printf("EspNowTransport - Packet decode FAILED, len=%d\n", len);
        return;
    }

    userReceiver->handlePacket(pkt);
}


void EspNowTransport::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.printf("ESP-NOW send status: %s\n",
                  status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}
