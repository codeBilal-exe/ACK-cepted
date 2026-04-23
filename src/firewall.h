#pragma once

#include <set>
#include <string>
#include <vector>

class Firewall {
public:
  static bool block_ip(const std::string &ip);

  static bool unblock_ip(const std::string &ip);

  static bool is_blocked(const std::string &ip);

  static std::vector<std::string> get_blocked_ips();

  static bool flush_all();

private:
  static int run_command(const std::string &cmd);
  static std::set<std::string> blocked_ips_;
};