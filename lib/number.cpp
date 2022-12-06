#include "number.h"

#include <cmath>
#include <iomanip>
#include <string>

uint2022_t from_uint(uint32_t i) {
  uint2022_t num;
  if (i >= uint2022_t::MAX_NUM) {
    num.number[0] = i % uint2022_t::MAX_NUM;
    num.number[1] = i / uint2022_t::MAX_NUM;
  } else {
    num.number[0] = i;
  }

  return num;
}

uint2022_t from_string(const char* buff) {
  uint2022_t num;
  int num_len = strlen(buff);
  int cur_num = 0;
  for (int i = 0; i < num_len; i++) {
    if ((num_len - 1 - i) % uint2022_t::BASE == 0) {
      cur_num = cur_num * 10 + static_cast<int>(buff[i] - '0');
      num.number[(num_len - 1 - i) / uint2022_t::BASE] = cur_num;
      cur_num = 0;
    } else {
      cur_num = cur_num * 10 + static_cast<int>(buff[i] - '0');
    }
  }

  return num;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint32_t t = 0;
  uint2022_t num;
  for (int i = 0; i < uint2022_t::SIZE; i++) {
    if (lhs.number[i] == 0 && rhs.number[i] == 0 && t == 0) break;
    if (uint2022_t::MAX_NUM <= lhs.number[i] + rhs.number[i] + t) {
      num.number[i] = lhs.number[i] + rhs.number[i] + t - uint2022_t::MAX_NUM;
      t = 1;
    } else {
      num.number[i] = lhs.number[i] + rhs.number[i] + t;
      t = 0;
    }
  }

  return num;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint32_t t = 0;
  uint2022_t num;
  for (int i = 0; i < uint2022_t::SIZE; i++) {
    if (lhs.number[i] < rhs.number[i] + t) {
      num.number[i] = uint2022_t::MAX_NUM + lhs.number[i] - rhs.number[i] - t;
      t = 1;
    } else {
      num.number[i] = lhs.number[i] - rhs.number[i] - t;
      t = 0;
    }
  }

  return num;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t ans;
  uint64_t num = 0;
  uint32_t next = 0;
  for (int n = 0; n < uint2022_t::SIZE; n++) {
    for (int r_digit = 0; r_digit <= n; r_digit++) {
      num += (uint64_t) lhs.number[r_digit] * (uint64_t) rhs.number[n - r_digit];
    }
    ans.number[n] = (num + next) % uint2022_t::MAX_NUM;
    next = (num + next) / uint2022_t::MAX_NUM;
    num = 0;
  }

  return ans;
}

void num_shift(uint2022_t& value) {
  for (int i = uint2022_t::SIZE - 1; i >= 1; i--)
    value.number[i] = value.number[i - 1];
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t res;
  uint2022_t cur_val;
  for (int i = uint2022_t::SIZE - 1; i >= 0; i--) {
    num_shift(cur_val);
    cur_val.number[0] = lhs.number[i];
    int x = 0;
    int l = 0;
    int r = uint2022_t::MAX_NUM;
    while (l <= r) {
      int b = (l + r) / 2;
      uint2022_t cur = rhs * from_uint(b);
      if (cur <= cur_val) {
        x = b;
        l = b + 1;
      } else
        r = b - 1;
    }
    res.number[i] = x;
    cur_val = cur_val - rhs * from_uint(x);
  }

  return res;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
  for (int i = 0; i < uint2022_t::SIZE; i++) {
    if (lhs.number[i] != rhs.number[i]) {
      return false;
    }
  }

  return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
  return !(lhs == rhs);
}

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs) {
  bool t = false;
  for (int i = uint2022_t::SIZE - 1; i >= 0; i--) {
    if (lhs.number[i] != 0 || rhs.number[i] != 0) {
      if (lhs.number[i] < rhs.number[i]) {
        t = true;
      } else if (lhs.number[i] > rhs.number[i] && !t) {
        break;
      }
    }
  }

  return t;
}

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs) {
  return rhs < lhs;
}

bool operator<=(const uint2022_t& lhs, const uint2022_t& rhs) {
  return !(lhs > rhs);
}

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs) {
  return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
  bool t = false;
  if (value.number[uint2022_t::SIZE - 1] != 0) {
    stream << value.number[uint2022_t::SIZE - 1];
    t = true;
  }
  for (int i = uint2022_t::SIZE - 2; i >= 0; i--) {
    if (t) {
      stream << std::setw(uint2022_t::BASE) << std::setfill('0') << value.number[i];
    } else if (value.number[i] != 0) {
      stream << value.number[i];
      t = true;
    }
  }
  if (!t) {
    stream << 0;
  }

  return stream;
}
