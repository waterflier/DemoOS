#include <pthread.h>
#ifdef ANDROID
#include <linux/sem.h>
#include <linux/types.h>
#include <linux/ipc.h>
#include <linux/types.h>
#else
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#endif
#include <unistd.h>
