#ifndef CCBasicCore_Mutex_IMPL_H
#define CCBasicCore_Mutex_IMPL_H

typedef struct CCMutex_Impl CCMutex_Impl;

/**
 * @brief Create a Mutex object
 *
 * @return CCMutex_Impl*
 */
CCMutex_Impl* CCCoreBasicMutexIMPL_createMutex();

/**
 * @brief lock A Lock
 *
 * @param impl
 */
void CCCoreBasicMutexIMPL_lockMutex(CCMutex_Impl* impl);

/**
 * @brief Unlock A Lock
 *
 * @param impl
 */
void CCCoreBasicMutexIMPL_unlockMutex(CCMutex_Impl* impl);

/**
 * @brief Attempt to lock if the lock if lockable
 *        you should always check if is not 0 to process sth
 *
 * @param impl
 * @return int
 */
int CCCoreBasicMutexIMPL_trylockMutex(CCMutex_Impl* impl);

/**
 * @brief Free A Lock
 *
 * @param impl
 * @return int
 */
int CCCoreBasicMutexIMPL_FreeMutex(CCMutex_Impl* impl);

#endif