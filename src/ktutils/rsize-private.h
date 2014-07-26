#ifndef KTUTILS_RSIZE_PRIVATE_H
#define KTUTILS_RSIZE_PRIVATE_H
#define _(STMTS) \
  do {           \
    STMTS        \
  } while (0)

#define _RTC goto rtc_violation;

#define _checkrmax(S, RTC)                  \
  if ((S == NULL) | (S##max > RSIZE_MAX)) { \
    RTC                                     \
  }
#define _checkoplen(s1max, s2max, oplen, RTC) \
  if ((oplen > s1max) || (oplen > s2max)) {   \
    RTC                                       \
  }

#define check2operand(s1, s2)                 \
  _(_checkrmax(s1, _RTC) _checkrmax(s2, _RTC) \
    _checkoplen(s1##max, s2##max, oplen, _RTC))

#define check1operand(s1) _(_checkrmax(s1, _RTC) if (oplen > s1max) {_RTC})

#endif  // KTUTILS_RSIZE_PRIVATE_H
