#include "firewall.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>


std::set<std::string> Firewall::blocked_ips_;

int Firewall::run_command(const std::string &cmd) {
  std::string full_cmd = cmd + " 2>/dev/null";
  int ret = std::system(full_cmd.c_str());
  return WEXITSTATUS(ret);
}

bool Firewall::block_ip(const std::string &ip) {
  if (is_blocked(ip)) {
    std::cerr << "[Firewall] " << ip << " is already blocked.\n";
    return true;
  }

  if (ip.find_first_not_of("0123456789.") != std::string::npos) {
    std::cerr << "[Firewall] Invalid IP: " << ip << "\n";
    return false;
  }

  std::string cmd = "iptables -I INPUT 1 -s " + ip + " -j DROP";
  int result = run_command(cmd);

  if (result == 0) {
    blocked_ips_.insert(ip);
    std::cout << "[Firewall] Blocked " << ip << "\n";
    return true;
  } else {
    std::cerr << "[Firewall] Failed to block " << ip << " (exit code " << result
              << "). Are you root?\n";
    return false;
  }
}

bool Firewall::unblock_ip(const std::string &ip) {
  if (!is_blocked(ip))
    return true;

  std::string cmd = "iptables -D INPUT -s " + ip + " -j DROP";
  int result = run_command(cmd);

  if (result == 0) {
    blocked_ips_.erase(ip);
    std::cout << "[Firewall] Unblocked " << ip << "\n";
    return true;
  } else {
    std::cerr << "[Firewall] Failed to unblock " << ip << "\n";
    return false;
  }
}

bool Firewall::is_blocked(const std::string &ip) {
  return blocked_ips_.count(ip) > 0;
}

std::vector<std::string> Firewall::get_blocked_ips() {
  return std::vector<std::string>(blocked_ips_.begin(), blocked_ips_.end());
}

bool Firewall::flush_all() {
  bool all_ok = true;
  for (const auto &ip : blocked_ips_) {
    std::string cmd = "iptables -D INPUT -s " + ip + " -j DROP";
    if (run_command(cmd) != 0)
      all_ok = false;
  }
  blocked_ips_.clear();
  return all_ok;
}