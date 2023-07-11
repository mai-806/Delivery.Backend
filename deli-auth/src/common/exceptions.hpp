#pragma once

namespace deli_auth::common::exceptions {
  class ParseArgException : public std::exception {
  public:
    explicit ParseArgException(std::string msg) : msg_(std::move(msg)) {}

    const char* what() const noexcept final { return msg_.c_str(); }

  private:
    std::string msg_;
  };

  class UserNotFound : public std::runtime_error {
        using runtime_error::runtime_error;
    };
} // namespace deli_auth::common::exceptions