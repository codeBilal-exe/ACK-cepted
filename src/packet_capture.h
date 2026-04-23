#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include "detection_engine.h"

// Forward declare pcap types without including pcap.h in the header
struct pcap;
typedef struct pcap pcap_t;

class PacketCapture {
public:
    // interface: e.g. "eth0", "enp3s0", "any"
    explicit PacketCapture(const std::string& interface,
                           std::shared_ptr<DetectionEngine> engine);
    ~PacketCapture();

    // Start/stop the capture thread
    bool start();
    void stop();

    bool is_running() const { return running_.load(); }
    std::string last_error() const { return last_error_; }

private:
    static void pcap_handler(unsigned char* user,
                             const struct pcap_pkthdr* header,
                             const unsigned char* packet);

    void process_packet(const struct pcap_pkthdr* header,
                        const unsigned char* packet);
    void capture_loop();

    std::string                          interface_;
    std::shared_ptr<DetectionEngine>     engine_;
    pcap_t*                              handle_ = nullptr;
    std::thread                          thread_;
    std::atomic<bool>                    running_{false};
    std::string                          last_error_;
};