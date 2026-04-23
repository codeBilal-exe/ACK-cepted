#pragma once

#include <chrono>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>


struct SynEntry {
  std::string src_ip;
  uint16_t src_port;
  uint16_t dst_port;
  std::chrono::steady_clock::time_point timestamp;
  bool completed;
};

enum class ConnectionState { SYN_SENT, ESTABLISHED };

struct ConnectionInfo {
  std::string src_ip;
  uint16_t src_port;
  uint16_t dst_port;
  ConnectionState state;
  std::string state_str() const {
    return state == ConnectionState::SYN_SENT ? "SYN_SENT" : "ESTABLISHED";
  }
};

using AlertCallback =
    std::function<void(const std::string &src_ip, int syn_count)>;

class DetectionEngine {
public:
  explicit DetectionEngine(int syn_threshold = 20, int window_seconds = 5);

  void on_syn_received(const std::string &src_ip, uint16_t src_port,
                       uint16_t dst_port);
  void on_ack_received(const std::string &src_ip, uint16_t src_port);

  void set_alert_callback(AlertCallback cb);

  std::vector<ConnectionInfo> get_connections() const;

  std::vector<std::string> get_flagged_ips() const;

  void cleanup_stale(int max_age_seconds = 30);

  void set_threshold(int t);
  void set_window(int seconds);

private:
  void check_threshold(const std::string &src_ip);

  mutable std::mutex mutex_;
  int syn_threshold_;
  int window_seconds_;
  AlertCallback alert_cb_;

  std::unordered_map<std::string, std::vector<SynEntry>> syn_table_;

  std::unordered_map<std::string, int> flagged_ips_;
};