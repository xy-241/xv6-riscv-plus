#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");

    // System Info
    printf("System Info: \n");
    printf("int takes %d bytes\n", sizeof(int));
    printf("long takes %d bytes\n", sizeof(long));
    printf("\n");

    // Feature Testing
    printf("Feature Testing: \n");
    printf("dec:%d, hex:%x, str:%s, oct:%o, long: %l, stretch: %l \n", 10, 10, "10", 10, 2147483650, -9223372036854775807);
    printf("\n");

    // Playground
    printf("Playground: \n");
    printf("sign? %d, unsigned? %d, actual: %d, actualB: %d", 32768, 65536, 2147483647, 2147483648);
    printf("\n");

    printf("\n");
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode table
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;
  } else {
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
