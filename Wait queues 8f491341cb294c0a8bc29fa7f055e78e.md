# Wait queues

```c
/*
 * A single wait-queue entry structure:
 */
struct wait_queue_entry {
	unsigned int		flags;
	void			*private;
	wait_queue_func_t	func;
	struct list_head	entry;
};

struct wait_queue_head {
	spinlock_t		lock;
	struct list_head	head;
};
typedef struct wait_queue_head wait_queue_head_t;
```

Очередь ожидания - простой список процессов, ожидающих событие, которое должно произойти. Процессы помещают себя в очередь ожидания и помечают себя спящими. Когда происходит событие, ассоциирующееся с данной очередью, процессы в этой очереди просыпаются. 

Создание очереди:

```c
// static 
DECLARE_WAIT_QUEUE_HEAD(wait_queue_name);
// dynamically allocated
wait_queue_head_t my_queue;
init_waitqueue_head(&my_queue);
```

Ожидание завершения:

```c
wait_event(wq_head, condition) //sleep until a condition gets true
// Вариации с timeout (дополнительное условие выхода по таймеру), 
// freezable, idle (спящие задачи уменьшения нагрузки),
// cmd (выполнение указанных команд до и после ожидания),
// interruptible, killable (uninterruptible, кроме критических сигналов),
// lock_irq (отключение прерываний), 
// exclusive (флаг для пробуждения только этой задачи или этой и n следующих).
```

Пробуждение:

```c
// Wakes up all the processes waiting on the queue
static int __wake_up_common(struct wait_queue_head *wq_head, unsigned int mode,
			int nr_exclusive, int wake_flags, void *key,
			wait_queue_entry_t *bookmark)

#define wake_up(x)			__wake_up(x, TASK_NORMAL, 1, NULL)
#define wake_up_nr(x, nr)		__wake_up(x, TASK_NORMAL, nr, NULL)
#define wake_up_all(x)			__wake_up(x, TASK_NORMAL, 0, NULL)
#define wake_up_locked(x)		__wake_up_locked((x), TASK_NORMAL, 1)
#define wake_up_all_locked(x)		__wake_up_locked((x), TASK_NORMAL, 0)

#define wake_up_interruptible(x)	__wake_up(x, TASK_INTERRUPTIBLE, 1, NULL)
#define wake_up_interruptible_nr(x, nr)	__wake_up(x, TASK_INTERRUPTIBLE, nr, NULL)
#define wake_up_interruptible_all(x)	__wake_up(x, TASK_INTERRUPTIBLE, 0, NULL)
#define wake_up_interruptible_sync(x)	__wake_up_sync((x), TASK_INTERRUPTIBLE)
```

### Waitquque.stp

```c
#! /usr/bin/env stap

probe begin
{
	printf("%25s\n", "Start")

}

probe kernel.function("wq_has_sleeper")
{
   printf("[%ld]      wq_has_sleeper: tid = %d\n", gettimeofday_ns(), tid())	
}
probe kernel.function("__wake_up_common")
{
   printf("[%ld]      __wake_up_common: tid = %d\n", gettimeofday_ns(), tid())
}

probe kernel.function("__init_waitqueue_head")
{
	printf("[%ld]      __init_waitqueue_head: tid = %d\n", gettimeofday_ns(), tid())
}

probe kernel.function("finish_wait")
{
	printf("[%ld]      finish_wait: tid = %d\n", gettimeofday_ns(), tid())
}
probe timer.s(1)
{
	printf("End.\n")
	exit()
}
```

### Sources

[http://www.science.unitn.it/~fiorella/guidelinux/tlk/node122.html](http://www.science.unitn.it/~fiorella/guidelinux/tlk/node122.html)

[https://lwn.net/Articles/577370/](https://lwn.net/Articles/577370/)

[https://sysplay.in/blog/linux-kernel-internals/2015/12/waiting-blocking-in-linux-driver-part-3/](https://sysplay.in/blog/linux-kernel-internals/2015/12/waiting-blocking-in-linux-driver-part-3/)

[https://www.doc-developpement-durable.org/file/Projets-informatiques/cours-&-manuels-informatiques/Linux/Linux Kernel Development, 3rd Edition.pdf](https://www.doc-developpement-durable.org/file/Projets-informatiques/cours-&-manuels-informatiques/Linux/Linux%20Kernel%20Development,%203rd%20Edition.pdf)