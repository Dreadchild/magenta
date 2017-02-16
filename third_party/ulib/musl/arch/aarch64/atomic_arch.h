#include <stdatomic.h>

#define a_ll a_ll
static inline int a_ll(volatile int* p) {
    int v;
    __asm__ __volatile__("ldaxr %w0,%1"
                         : "=r"(v)
                         : "Q"(*p));
    return v;
}

#define a_sc a_sc
static inline int a_sc(volatile int* p, int v) {
    int r;
    __asm__ __volatile__("stlxr %w0,%w2,%1"
                         : "=&r"(r), "=Q"(*p)
                         : "r"(v)
                         : "memory");
    return !r;
}

#define a_cas a_cas
static inline int a_cas(volatile int* p, int t, int s) {
    int old;
    do {
        old = a_ll(p);
        if (old != t) {
            atomic_thread_fence(memory_order_seq_cst);
            break;
        }
    } while (!a_sc(p, s));
    return old;
}

#define a_ll_p a_ll_p
static inline void* a_ll_p(volatile void* p) {
    void* v;
    __asm__ __volatile__("ldaxr %0, %1"
                         : "=r"(v)
                         : "Q"(*(void* volatile*)p));
    return v;
}

#define a_sc_p a_sc_p
static inline int a_sc_p(volatile int* p, void* v) {
    int r;
    __asm__ __volatile__("stlxr %w0,%2,%1"
                         : "=&r"(r), "=Q"(*(void* volatile*)p)
                         : "r"(v)
                         : "memory");
    return !r;
}

#define a_cas_p a_cas_p
static inline void* a_cas_p(volatile void* p, void* t, void* s) {
    void* old;
    do {
        old = a_ll_p(p);
        if (old != t) {
            atomic_thread_fence(memory_order_seq_cst);
            break;
        }
    } while (!a_sc_p(p, s));
    return old;
}