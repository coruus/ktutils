extern uint64_t kt_verify8_64(const void* _s1, const void* _s2);
#define kt_verify8(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))
extern uint64_t kt_verify16_64(const void* _s1, const void* _s2);
#define kt_verify16(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))
extern uint64_t kt_verify24_64(const void* _s1, const void* _s2);
#define kt_verify24(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))
extern uint64_t kt_verify32_64(const void* _s1, const void* _s2);
#define kt_verify32(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))
extern uint64_t kt_verify40_64(const void* _s1, const void* _s2);
#define kt_verify40(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))
extern uint64_t kt_verify48_64(const void* _s1, const void* _s2);
#define kt_verify48(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))
extern uint64_t kt_verify56_64(const void* _s1, const void* _s2);
#define kt_verify56(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))
extern uint64_t kt_verify64_64(const void* _s1, const void* _s2);
#define kt_verify64(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))
extern uint64_t kt_verify128_64(const void* _s1, const void* _s2);
#define kt_verify128(s1, s2) \
  kt_err_64toi(kt_verify((s1), (s2)))

