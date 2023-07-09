#pragma once

namespace deli_main::common::exceptions {
class ParseArgException : public std::exception {
 public:
  explicit ParseArgException(std::string msg) : msg_(std::move(msg)) {}

  const char* what() const noexcept final { return msg_.c_str(); }

 private:
  std::string msg_;
};
} // namespace deli_main::common::exceptions
