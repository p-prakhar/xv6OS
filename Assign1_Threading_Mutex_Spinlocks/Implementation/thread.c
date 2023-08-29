#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
struct balance
{
	char name[32];
	int amount;
};

struct thread_spinlock {
  uint locked;
};

void thread_spin_init(struct thread_spinlock *lk){
	lk->locked = 0;
};
void thread_spin_lock(struct thread_spinlock *lk){
	// The xchg is atomic.
	while(xchg(&lk->locked, 1) != 0)
	;
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
	__sync_synchronize();
};
void thread_spin_unlock(struct thread_spinlock *lk){
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
	__sync_synchronize();
	asm volatile("movl $0, %0" : "+m" (lk->locked) : );
};

struct thread_mutex {
  uint mutexLocked;
};


void thread_mutex_init(struct thread_mutex *lk){
	lk->mutexLocked = 0;
};
void thread_mutex_lock(struct thread_mutex *lk){
	// The xchg is atomic.
	while(xchg(&lk->mutexLocked, 1) != 0)
	sleep(1);
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
	__sync_synchronize();
};
void thread_mutex_unlock(struct thread_mutex *lk){
  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that all the stores in the critical
  // section are visible to other cores before the lock is released.
  // Both the C compiler and the hardware may re-order loads and
  // stores; __sync_synchronize() tells them both not to.
	__sync_synchronize();
	asm volatile("movl $0, %0" : "+m" (lk->mutexLocked) : );
};

volatile int total_balance = 0;
volatile unsigned int delay (unsigned int d)
{
	unsigned int i;
	for (i = 0; i < d; i++)
	{
		__asm volatile( "nop" ::: );
	}
	return i;
}

struct thread_spinlock lock;		// declaring spinlock
struct thread_mutex mLock;		// declaring mutex lock

void do_work(void *arg)
{
	int i;
	int old;
	struct balance *b = (struct balance*) arg;
	printf(1, "Starting do_work: s:%s\n", b->name);
	for (i = 0; i < b->amount; i++)
	{
		// thread_spin_lock(&lock);		// uncomment this line for spinlock
		// thread_mutex_lock(&mLock);		// uncomment this line for mutex lock
		old = total_balance;
		delay(100000);
		total_balance = old + 1;
		// thread_spin_unlock(&lock);		// uncomment this line for mutex lock
		// thread_mutex_unlock(&mLock);	// uncomment this line for spinlock
	}
	printf(1, "Done s:%x\n", b->name);
	thread_exit();
}



int main(int argc, char *argv[]) {
	thread_spin_init(&lock);
	thread_mutex_init(&mLock);
	struct balance b1 = {"b1", 3200};
	struct balance b2 = {"b2", 2800};
	void *s1, *s2;
	int t1, t2, r1, r2;
	s1 = malloc(4096);
	s2 = malloc(4096);
	t1 = thread_create(do_work, (void*)&b1, s1);
	t2 = thread_create(do_work, (void*)&b2, s2);
	r1 = thread_join();
	r2 = thread_join();
	
	printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n",
	t1, r1, t2, r2, total_balance);
	exit(); 
}

