#include "../include/Ex_shmem.h"

namespace libseabase
{
int Se_shmem::SE_shm_alloc(Se_shm_t *shm)
{
    shm->addr = (unsigned char *) mmap(NULL, shm->size,
        PROT_READ|PROT_WRITE,
        MAP_ANON|MAP_SHARED, -1, 0);

    if (shm->addr == MAP_FAILED) {
//         ngx_log_error(NGX_LOG_ALERT, shm->log, ngx_errno,
//             "mmap(MAP_ANON|MAP_SHARED, %uz) failed", shm->size);
        return SE_ERROR;
    }

    return SE_OK;
}
void Se_shmem::SE_shm_free(Se_shm_t *shm)
{
    if (munmap((void *) shm->addr, shm->size) == -1) {
//         ngx_log_error(NGX_LOG_ALERT, shm->log, ngx_errno,
//             "munmap(%p, %uz) failed", shm->addr, shm->size);
    }
}
};