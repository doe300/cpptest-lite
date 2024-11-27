#include "formatting.h"

#include <cstdio>

using namespace Test;

std::string Formats::to_string(const char *val) { return std::string{"'"} + (val ? val : "(null)") + "'"; }
std::string Formats::to_string(const std::string &val) { return "'" + val + "'"; }

std::string Formats::to_string(const std::u16string &val) {
  std::mbstate_t state{};
  std::string result;
  for (char16_t c : val) {
    std::array<char, MB_LEN_MAX> tmp{};
    std::size_t rc = std::c16rtomb(tmp.data(), c, &state);
    if (rc == static_cast<std::size_t>(-1)) {
      result.push_back('?');
      state = std::mbstate_t{};
      rc = 0;
    }
    for (std::size_t i = 0; i < rc; ++i) {
      result.push_back(tmp[i]);
    }
  }
  return "'" + result + "'";
}

std::string Formats::to_string(const std::u32string &val) {
  std::mbstate_t state{};
  std::string result;
  for (char32_t c : val) {
    std::array<char, MB_LEN_MAX> tmp{};
    std::size_t rc = std::c32rtomb(tmp.data(), c, &state);
    if (rc == static_cast<std::size_t>(-1)) {
      result.push_back('?');
      state = std::mbstate_t{};
      rc = 0;
    }
    for (std::size_t i = 0; i < rc; ++i) {
      result.push_back(tmp[i]);
    }
  }
  return "'" + result + "'";
}

std::string Formats::to_string(std::nullptr_t) { return "(nullptr)"; }
std::string Formats::to_string(bool val) { return val ? "true" : "false"; }
std::string Formats::to_string(int32_t val) { return std::to_string(val); }
std::string Formats::to_string(int64_t val) { return std::to_string(val); }
std::string Formats::to_string(uint32_t val) { return std::to_string(val); }
std::string Formats::to_string(uint64_t val) { return std::to_string(val); }

std::string Formats::to_string(float val) {
  char buffer[128];
#ifdef _MSC_VER
  sprintf_s(buffer, sizeof(buffer), "%g", static_cast<double>(val));
#else
  sprintf(buffer, "%g", static_cast<double>(val));
#endif
  return buffer;
}

std::string Formats::to_string(double val) {
  char buffer[128];
#ifdef _MSC_VER
  sprintf_s(buffer, sizeof(buffer), "%g", val);
#else
  sprintf(buffer, "%g", val);
#endif
  return buffer;
}

std::string Formats::to_string(long double val) {
  char buffer[128];
#ifdef _MSC_VER
  sprintf_s(buffer, sizeof(buffer), "%Lg", val);
#else
  sprintf(buffer, "%Lg", val);
#endif
  return buffer;
}

std::string Formats::to_string(const void *ptr) {
  char buffer[32];
#ifdef _MSC_VER
  sprintf_s(buffer, sizeof(buffer), "%p", ptr);
#else
  sprintf(buffer, "%p", ptr);
#endif
  return buffer;
}
