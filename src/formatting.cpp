#include "formatting.h"

#include <cstdio>
#ifdef _MSC_VER
#include <Windows.h>
#include <cuchar>
#endif

using namespace Test;

std::string Formats::to_string(const char *val) { return std::string{"'"} + (val ? val : "(null)") + "'"; }
std::string Formats::to_string(const std::string &val) { return "'" + val + "'"; }

static const bool NATIVE_UTF8 = [] {
#ifdef _MSC_VER
  return GetACP() == CP_UTF8;
#else
  // Linux and macOS use (most likely) UTF-8 for their string encoding, so just assume that.
  // For any other platform we don't know the necessary conversion anyway, so also treat as UTF-8.
  return true;
#endif
}();

std::string Formats::internal::utf8_to_string(const uint8_t *ptr, std::size_t length) {
  if (NATIVE_UTF8) {
    return "'" + std::string(reinterpret_cast<const char *>(ptr), length) + "'";
  }
#ifdef _MSC_VER
  std::wstring tmp(length * 2U, L'\0');
  auto numChars =
      MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char *>(ptr), length, &tmp.front(), tmp.size());
  tmp.resize(numChars);
  std::string result(length * 2U, '\0');
  numChars = WideCharToMultiByte(CP_ACP, 0, tmp.data(), tmp.size(), &result.front(), result.size(), nullptr, nullptr);
  result.resize(numChars);
  return "'" + result + "'";
#else
  return "(unknown encoding)";
#endif
}

static void appendAsUtf8(uint32_t code, std::string &outUtf8String) {
  if (code < 0x80) {
    outUtf8String.push_back(static_cast<char>(code & 0x7F));
  } else if (code < 0x800) {
    outUtf8String.push_back(0xC0 + ((code >> 6) & 0x1F));
    outUtf8String.push_back(0x80 + (code & 0x3F));
  } else if (code < 0x10000) {
    outUtf8String.push_back(0xE0 + ((code >> 12) & 0x0F));
    outUtf8String.push_back(0x80 + ((code >> 6) & 0x3F));
    outUtf8String.push_back(0x80 + (code & 0x3F));
  } else if (code < 0x110000) {
    outUtf8String.push_back(0xF0 + ((code >> 18) & 0x07));
    outUtf8String.push_back(0x80 + ((code >> 12) & 0x3F));
    outUtf8String.push_back(0x80 + ((code >> 6) & 0x3F));
    outUtf8String.push_back(0x80 + (code & 0x3F));
  } else {
    outUtf8String.push_back('?');
  }
}

std::string Formats::internal::utf16_to_string(const char16_t *ptr, std::size_t length) {
  if (NATIVE_UTF8) {
    // Convert UTF-16 to UTF-8
    std::string result{};
    result.reserve(length);

    uint32_t code = 0;
    for (std::size_t i = 0; i < length; ++i) {
      auto val = static_cast<uint16_t>(ptr[i]);
      if ((val & 0xFC00) == 0xD800) {
        code = (val & 0x03FFU) << 10;
        code += 0x10000;
        continue;
      } else if ((val & 0xFC00) == 0xDC00) {
        code |= (val & 0x03FFU);
      } else {
        code = val;
      }
      appendAsUtf8(code, result);
      code = 0;
    }

    result.shrink_to_fit();
    return "'" + result + "'";
  }
#ifdef _MSC_VER
  std::mbstate_t state{};
  std::string result;
  for (std::size_t i = 0; i < length; ++i) {
    std::array<char, MB_LEN_MAX> tmp{};
    std::size_t rc = std::c16rtomb(tmp.data(), ptr[i], &state);
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
#else
  return "(unknown encoding)";
#endif
}

std::string Formats::internal::utf32_to_string(const char32_t *ptr, std::size_t length) {
  if (NATIVE_UTF8) {
    // Convert UTF-32 to UTF-8
    std::string result{};
    result.reserve(length);

    for (std::size_t i = 0; i < length; ++i) {
      auto code = static_cast<uint32_t>(ptr[i]);
      appendAsUtf8(code, result);
    }

    result.shrink_to_fit();
    return "'" + result + "'";
  }
#ifdef _MSC_VER
  std::mbstate_t state{};
  std::string result;
  for (std::size_t i = 0; i < length; ++i) {
    std::array<char, MB_LEN_MAX> tmp{};
    std::size_t rc = std::c32rtomb(tmp.data(), ptr[i], &state);
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
#else
  return "(unknown encoding)";
#endif
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
