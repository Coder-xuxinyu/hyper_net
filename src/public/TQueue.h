#ifndef TQUEUE_H
#define TQUEUE_H
#include "util.h"
#include "spin_mutex.h"
#include <stdio.h>
#include <string.h>

enum {
    NOT_EXISTS_DATA = 0,
    EXISTS_DATA = 1
};

namespace olib {
    typedef void(*VOID_FUN_TYPE)(void);
    inline void _void_fun(void) {

    }

    template <typename type, bool lock, const s32 size, typename funtype = VOID_FUN_TYPE, funtype pfun = _void_fun, typename... Args>
    class TQueue {
    public:
        TQueue() {
            m_nRIndex = 0;
            m_nWIndex = 0;
            m_nRCount = 0;
            m_nWCount = 0;
            if (lock) {
                m_pRlock = NEW SpinLock;
                m_pWlock = NEW SpinLock;
                OASSERT(m_pRlock && m_pWlock, "wtf");
            }

            //memset(m_queue, 0, sizeof(m_queue));
            memset(m_sign, 0, sizeof(m_sign));
        }

        ~TQueue() {
            if (lock) {
                delete m_pWlock;
                delete m_pRlock;
            }
        }

        inline void addByArgs(Args... args) {
            QUEUE_OPT_LOCK(lock, m_pWlock);
            while (m_sign[m_nWIndex] != NOT_EXISTS_DATA) {
                CSLEEP(1);
            }

            if (pfun != NULL) {
                (pfun)(m_queue[m_nWIndex], args...);
            }
            m_nWCount++;
            m_sign[m_nWIndex++] = EXISTS_DATA;

            if (m_nWIndex >= size) {
                m_nWIndex = 0;
            }
            QUEUE_OPT_FREELOCK(lock, m_pWlock);
        }

        inline void add(type src) {
            QUEUE_OPT_LOCK(lock, m_pWlock);
            while (m_sign[m_nWIndex] != NOT_EXISTS_DATA) {
                CSLEEP(1);
            }

            m_queue[m_nWIndex] = src;
            m_sign[m_nWIndex++] = EXISTS_DATA;
            m_nWCount++;
            if (m_nWIndex >= size) {
                m_nWIndex = 0;
            }
            QUEUE_OPT_FREELOCK(lock, m_pWlock);
        }

        inline bool tryAdd(type src) {
            QUEUE_OPT_LOCK(lock, m_pWlock);
            while (m_sign[m_nWIndex] != NOT_EXISTS_DATA) {
                QUEUE_OPT_FREELOCK(lock, m_pWlock);
                return false;
            }

            m_queue[m_nWIndex] = src;
            m_sign[m_nWIndex++] = EXISTS_DATA;
            m_nWCount++;
            if (m_nWIndex >= size) {
                m_nWIndex = 0;
            }
            QUEUE_OPT_FREELOCK(lock, m_pWlock);
            return true;
        }

        inline bool read(type & value) {
            QUEUE_OPT_LOCK(lock, m_pRlock);
            while (m_sign[m_nRIndex] != EXISTS_DATA) {
                QUEUE_OPT_FREELOCK(lock, m_pRlock);
                return false;
            }

            value = m_queue[m_nRIndex];
            m_sign[m_nRIndex++] = NOT_EXISTS_DATA;
            m_nRCount++;

            if (m_nRIndex >= size) {
                m_nRIndex = 0;
            }
            QUEUE_OPT_FREELOCK(lock, m_pRlock);
            return true;
        }

        inline bool IsEmpty() {
            return (m_nRCount == m_nWCount);
        }

    private:
        SpinLock * m_pWlock;
        SpinLock * m_pRlock;
        type m_queue[size];
        s8 m_sign[size];
        u32 m_nRIndex;
        u32 m_nWIndex;
        u32 m_nRCount;
        u32 m_nWCount;
    };
}
#endif //CQUEUE_H
