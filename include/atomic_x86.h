//原子操作
//linux和windows均可使用版本，测试通过
//by xjshi 2019/11/20
#pragma once
#ifndef _atomic_x86_h
#define _atomic_x86_h
#include "basevar.h"
namespace libseabase
{
    class Es_atomic
    {
    public:
        Es_atomic():m_val(0){}
        Es_atomic(unsigned int val):m_val(val){}
        unsigned int get();
        void set(unsigned int value);
        unsigned int add(unsigned int value);
        void sub(unsigned int value);
        unsigned int inc();
        unsigned int dec();
        unsigned int cmp_set(unsigned int old,unsigned int set);  //与old比较，相同则设置m_val为set的值，否则什么也不做。
        unsigned int fetch_add();
        void SE_memory_barrier();
        void SE_cpu_pause();
    private:
#ifdef WIN32
        long m_val;
#else
        volatile unsigned int m_val;
#endif

    };
    unsigned int inline Es_atomic::get()
    {
#ifdef WIN32
        return m_val;
#else
        return m_val;

#endif
    }
    void inline Es_atomic::set(unsigned int value)
    {
#ifdef WIN32
        InterlockedExchange(&m_val,value);
#else
        m_val=value;
#endif
    }
    unsigned int inline Es_atomic::add(unsigned int value)
    {
#ifdef WIN32
        return InterlockedExchangeAdd(&m_val,value);
#else
        __asm__ volatile (
            //"lock; xaddl %1,%0"
            //: "+r"(m_val) /* outputs */
            //: "m"(value)   /* inputs */
            //: "cc","memory");
            "    lock;            "
            "    xaddl  %0, %1;   "

            : "+r" (value) : "m" (m_val) : "cc", "memory");
        return m_val;
#endif
    }
    void inline Es_atomic::sub(unsigned int value)
    {
#ifdef WIN32
        InterlockedExchangeAdd(&m_val,-value);
#else
        __asm__ volatile (
            "lock; subl %1, %0"
            :"+m" (m_val)
            :"r" (value)
            :"cc", "memory");
#endif
    }
    unsigned int inline Es_atomic::inc()
    {
#ifdef WIN32
        return InterlockedIncrement(&m_val) - 1;
#else
        return add(1);
#endif
    }
    unsigned int inline Es_atomic::dec()
    {
#ifdef WIN32
        return InterlockedDecrement(&m_val);
#else
        unsigned char prev;

        asm volatile ("lock; decl %1;\n\t"
            "setnz %%al"
            : "=a" (prev)
            : "m" (m_val)
            :"cc","memory");
        return prev;
#endif
    }
    unsigned int inline Es_atomic::cmp_set(unsigned int old,unsigned int set)
    {
#ifdef WIN32
        return InterlockedCompareExchange(&m_val,set,old);
#else
        unsigned char  res;
        __asm__ volatile (
            "    lock;               "
            "    cmpxchgl  %3, %1;   "
            "    sete      %0;       "
            : "=a" (res) : "m" (m_val), "a" (old), "r" (set) : "cc", "memory");

        return res;
#endif
    }
    unsigned int inline Es_atomic::fetch_add()
    {
        return 0;
    }
	void inline Es_atomic::SE_memory_barrier() {  
#ifdef WIN32
#else
		__asm__ volatile ("" ::: "memory");
#endif
	}
//
//	/* old "as" does not support "pause" opcode */
    void inline Es_atomic::SE_cpu_pause(){
#ifdef WIN32
#else
        __asm__ (".byte 0xf3, 0x90");
#endif
    }
}

#endif