### What is Process？
<img src="https://github.com/ShireHong/algorithm-note/blob/master/linux/process/pic/system_structure.png"  
    alt="图片加载失败时，显示这段字"/>

- 进程（Process）
经典定义是一个执行中的程序的实例，操作系统对一个正在运行的程序的一种抽象。并发运行，指的是一个进程的指令和另一个进程的指令交错执行。操作系统实现这种交错执行的机制称为上下文切换。

- 线程（Thread）
一个进程可以由多个线程的执行单元组成，每个线程都运行在进程的上下文中，并共享同样的代码和全局数据。

- 内核（Kernel）
一个计算机程序，用来管理软件发出的数据I/O（输入与输出）要求，将这些要求转译为数据处理的指令，交由中央处理器（CPU）及计算机中其他电子组件进行处理，是现代操作系统中最基本的部分。

- 外壳（Shell）
指“为使用者提供使用者界面”的软件，通常指的是命令行界面的解析器。一般来说，这个词是指操作系统中提供存取内核所提供之服务的程式。Shell也用于泛指所有为用户提供操作界面的程序，也就是程序和用户交互的层面。内核不提供交互。

- 抢占（Preemption）
分为非抢占式和抢占式。根据调度主体分用户抢占与内核抢占。
非抢占式（Nonpreemptive）——让进程运行直到结束或阻塞的调度方式。
抢占式（Preemptive）——允许将逻辑上可继续运行的在运行过程暂停的调度方式。可防止单一进程长时间独占CPU。

- 异常控制流（ECF，Exceptional Control Flow）
ECF发生在硬件层，操作系统层，应用层。控制转移（control transfer）是指程序计数器对应的指令序列的跳转，控制转移序列的叫做处理器的控制流（control flow）。
某些如跳转、调用和返回是为了使得程序对内部状态变化（event）做出反应而设计的机制，系统通过使控制流发生突变对发生各种状态变化。

#### Exceptions

任何情况下，处理器检测到event发生，通过异常表（exception table）跳转到专门处理这类事件的操作系统子程序（exception handler）。

异步异常由事件产生，同步异常是执行一条指令的直接产物。

类别包含**中断（异步）**，**陷阱（同步）**，**故障（同步）**，**终止（同步）**。

- 中断——异步发生，处理器IO设备信号的结果。
- 陷阱——有意的异常。最重要的用途是在用户程序和内核之间提供一个像过程一样的接口，叫做系统调用。
- 故障——潜在可恢复的错误造成的结果。如果能被修复，则重新执行引起故障的指令，否则终止。
- 终止——不可恢复的致命错误造成的结果。

有高达256种不同的异常类型，如出发错误（0）、一般保护故障（13）、缺页（14）、机器检查（18）、操作系统定义的异常（32-127，129-255）、系统调用（0x80）。

我们常见的段故障（Segmentation fault），是一般保护故障（异常13），通常是因为一个程序引用了一个未定义的虚拟存储器区域，或者因为程序试图写一个只读的文本段。

<img src="https://github.com/ShireHong/algorithm-note/blob/master/linux/process/pic/system_calls.jpg"  
    alt="图片加载失败时，显示这段字"/>

#### Processes

- 逻辑控制流（Logical Control Flow）
程序计数器PC值的序列叫做逻辑控制流（逻辑流）。PC对应于程序的可执行目标文件中的指令，或者是包含在运行时动态链接到程序的共享对象中的指令。

逻辑流看起来就像是在独占处理器地执行程序，每个进程执行逻辑流的一部分然后就被抢占，实际上处理器通过上下文保护好进程间的信息，在不同的进程中切换。

- 并发流（Concurrent Flows）
并发流指逻辑流在执行时间上与另一个流重叠，多个就叫并发（concurrent）。

一个进程和其他进程轮流运行叫多任务（multitasking）。

进程占有CPU执行控制流的每一个时间段叫时间片（time slice）。

多任务也叫做时间分片（time slicing）。

如果两个流并发运行在不同的处理器或者计算机，称为并行流（parallel flow）。

- 私有地址空间（Private Address Space）
一般，进程间地址空间读写保护。进程地址空间32位进程，代码段从0x08048000开始，64位进程从0x00400000开始：

<img src="https://github.com/ShireHong/algorithm-note/blob/master/linux/process/pic/process_addr_space.jpg"  
    alt="图片加载失败时，显示这段字"/>

- 用户模式和内核模式（User and Kernel Modes）
通过控制寄存器中的模式位（mode bit）描述进程当前享有的特权。
内核模式：（超级用户）可执行指令集中任何指令，并且可以访问系统中任何存储器位置。

用户模式：不允许执行特权指令，不允许直接引用地址空间中内核区内的代码和数据，任何尝试都会引发致命保护故障。可以通过系统调用接口间接访问内核代码和数据。

- 上下文切换（Context Switches）
内核为每个进程维持一个上下文（context），是内核重新启动一个被抢占的进程所需的状态。包括：
通用目的的寄存器、浮点寄存器、程序计数器、用户栈、状态寄存器、内核栈和各种内核数据结构（地址空间的**页表**、有关当前进程信息的**进程表**、进程已打开文件的信息的**文件表**）
内核调度器（scheduler）负责调度进程，抢占当前进程，重新开始先前被抢占的进程。

### Inside Process
Process Control

> 如何控制进程？

#### PID
pid > 0
```
#include <sys/types.h> // for pid_t
#include <unistd.h>

pid_t getpid(void); // 获取进程ID
pid_t getppid(void); // 获取父进程ID
```
#### Creating and Terminating Process
从程序角度来看，进程总处于以下三种状态：

- Running——要么处于CPU执行中，要么处于等待被执行且最终会被内核调度。

- Stopped——进程被挂起（suspend），且不会被调度。当收到SIGSTOP、SIGTSTP、SIGTTIN或者SIGTTOU信号时，进程停止，直到收到SIGCONT信号，进程再次开始运行。

- Terminated——进程永远停止了。三种原因导致终止：
- 1）收到一个默认行为时终止进程的信号；
- 2）从主程序返回；
- 3）调用exit。
```
#include <sys/types.h>
#include <unistd.h>
/* 创建子进程
* 返回：
子进程=0，父进程=子进程PID，出错=-1
*/
pid_t fork(void);

#include <stdlib.h>
void exit(int status);
```
父进程通过调用fork创建一个新的运行子进程，最大的区别在于不同的PID。

- fork()：一次调用，返回两次。
- 1）在调用进程中（父进程），返回子进程PID；
- 2）在新创建的子进程中，在子进程中返回0。

- 并发执行：父子进程是并发运行的独立进程。

- 相同但是独立的地址空间。子进程与父进程用户级虚拟地址空间相同的拷贝，相同的本地变量值、堆、全局变量、以及代码。如代码中print出来不一样的x。

- 共享文件：任何打开文件描述符相同的拷贝，如stdout。
```
int main() {
    pid_t pid;
    int x = 1;

    pid = fork(); // 在此处分裂出了两条时间线！
    if (pid == 0) {// 子进程
        printf("child: x=%d\n", ++x);
        exit(0);
    }
    // 父进程
    printf("parent: x=%d\n", --x);
    exit(0);

    return 0;
}
```
out:
parent: x=0
child: x=2
```
child       |————x=2————
father  ——————————x=0————
          fork          exit
```

#### Reap Child Process
进程终止时，保持位已终止状态，直到被父进程回收（reap）。当父进程回收已终止的子进程，内核将子进程的退出状态传递给父进程，然后抛弃已终止的进程，此刻进程不复存在。

僵尸进程（zombie）：一个终止了但还未被回收的进程。但是如果父进程没有回收就终止了，则内核安排init进程（PID=1）回收僵尸进程。
```
#include <sys/types.h>
#include <sys/wait.h>

/* 进程可以调用waitpid等待子进程终止或者结束。
* 默认options=0，挂起调用进程，直到它等待集合中的一个子进程终止。如果等待集合中的一个进程在刚调用的时刻就已经终止了，那么waitpid立即返回。返回已终止的子进程PID，并去除该子进程。

*输入参数pid：
pid>0，等待集合就是一个单独的子进程，进程ID等于pid。
pid=-1，等待集合是由父进程所有的子进程组成。

*输入参数options:
WNOHANGE：等待集合中任何子进程都还没有终止，立即返回0；默认行为还是挂起调用进程直到子进程终止。
WUNTRACED：挂起调用进程执行，直到集合中有一个进程终止或停止。返回该进程PID。
WNOHANGE|WUNTRACED：立刻返回，0=如果没有终止或停止的子进程；PID=终止或停止的子进程PID。

*输入参数status：
WIFEXITED：True=子进程是通过return或者exit终止的；
WEXITSTATUS：返回exit状态，只有WIFEXITED=True时被定义；
WIFSIGNALED：True=子进程是因为一个未被捕获的信号终止的；
WTERMSIG：返回导致子进程终止信号量，只有WIFSIGNALED=True被定义；
WIFSTOPPED：True=返回的子进程是停止的；
WSTOPSIG：返回引起子进程停止的信号的数量，只有WIFSTOPPED=True被定义；

返回：
成功=子进程PID；if WNOHANG=0；
其他错误=-1（errno=ECHILD，没有子进程；errno=EINTR，被一个信号中断）
*/
pid_t waitpid(pid_t pid, int *status, int options);
pid_t wait(int *status); //等价于waitpid(-1, &status, 0);
```
#### Sleep
```
#include <unistd.h>

// 返回：seconds left to sleep
unsigned int sleep(unsigned int secs);

// 让调用函数休眠，直到收到一个信号
// 返回：-1
int pause(void);
```
#### Loading and Running Programs

execve函数在当前进程的上下文中加载并运行一个新的程序，覆盖当前进程的地址空间，但并没有创建一个新进程，进程PID没有改变。
```
#include <unistd.h>
// 返回：成功=不返回；出错=-1
int execve(const char *filename, const char *argv[],
            const char *envp[]);
// 程序主入口：
int main(int argc, char **argv, char **envp);
int main(int argc, char *argv[], char *envp[]);
```
#### Signal
<img src="https://github.com/ShireHong/algorithm-note/blob/master/linux/process/pic/signal.jpg"  
    alt="图片加载失败时，显示这段字"/>

信号传递到目的进程包括两个步骤：1）发送；2）接收。

- 一个发出却没被接收的信号叫做待处理信号（Pending Signal）。

- 一个进程有一个类型为k的待处理信号，后面发送到这个进程的k信号**都会被丢弃**。

也可以选择性阻塞接收某个信号，信号被阻塞时仍可以发送，但产生的待处理信号**不会被接收**，直到进程取消对这种信号的阻塞。

一个待处理信号最多**只能**被接收一次，内核为每个进程在pending位向量中维护待处理信号集合，而在blocked位向量中维护被阻塞的信号集合。

只有接收了k信号，内核才会清除pending中的k位。


#### Sending Signal

- 每个进程都只属于一个进程组，进程组ID标识。unix所有发送信号的机制都是基于进程组（process group）/
```
#include <unistd.h>

// 返回：调用进程的进程组ID
pid_t getpgrp(void);
// 返回：成功=1，错误=-1
int setpgid(pid_t pid, pid_t pgid);
```
- 用/bin/kill程序发送信号
发送信号9到进程15213
/bin/kill -9 15213
发送信号9到进程组15213中的每个进程。
/bin/kill -9 -15213

- 从键盘发送信号
unix使用作业（job）表示对每一个命令行执行而创建的进程，至多一个前台作业和0个或多个后台作业。通过|unix管道连接起多个进程。

shell位每个作业创建一个独立的进程组。进程组ID是取自job中父进程中的一个。

Ctrl + C发送SIGINT信号到前台进程组中的每一个进程，终止前台作业。

<img src="https://github.com/ShireHong/algorithm-note/blob/master/linux/process/pic/process_group.jpg"  
    alt="图片加载失败时，显示这段字"/>

- 用<font color=red>KILL</font>函数发送信号。
```
#include <signal.h>
// 输入参数pid：
// pid>0:发送SIGKILL给进程pid
// pid<0:发送SIGKILL给进程组abs(pid)
// 返回：成功=0，失败=-1
int kill(pid_t pid, int sig);
```
- alarm函数发送信号
```
#include <unistd.h>
// 发送SIGALRM给调用进程，如果secs位0，则不会调度alarm。任何情况，对alarm调用都将取消任何pending alarm，并返回pending alarm在被发送前还剩下的秒数。
// 返回：前一次alarm剩余的秒数，0=以前没有设定alarm
unsigned int alarm(unsigned int secs);


/* 定时1s触发alarm handler，5s结束 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void handler(int sig) {
   static int beeps = 0;
   printf("BEEP\n");
   if (++beeps < 5) {
       alarm(1);
   } else {
        printf("BOOM!\n");
        exit(0);
   }
}


int main() {
   signal(SIGALRM, handler);
   alarm(1);
   for(;;);
   exit(0);
}
```

#### Receiving Signals
```
wtf：
当异常处理程序返回时，准备转移控制权给进程p时，会检查非被阻塞的待处理信号的集合（pending&～blocked）
if 集合为空：
   进程p的逻辑控制流下一跳指令
else：
   选择某个最小信号k，强制p接收信号k
   goto wtf
```
每个信号类型预定义的默认行为[signal](#signal)：
- 进程终止
- 进程终止并转存储器（dump core）
- 进程停止直到被SIGCONT信号重启
- 进程忽略该信号

```
#include <signal.h>


// 定义信号处理函数（signal handler）
// 输入int为信号量
typedef void (*sighandler_t)(int);
// 输入函数sighandler_t：
// handler=SIG_IGN，忽略类型为signum的信号；
// handler=SIG_DFL，重置类型为signum信号的行为。
//
// 返回：成功=指向前次处理程序指针，出错=SIG_ERR（不设置errno）
sighandler_t signal(int signum, sighandler_t handler); // installing the handler




/* ctrl-c中断sleep，并打印睡眠时间 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


void handler(int sig) {} // 改变SIGINT处理函数


int snooze(unsigned int sec) {
   int left_sleep_sec = sleep(sec);
   printf("Slept for %d of %d secs.\tUser hits ctrl-c after %d seconds\n",
           left_sleep_sec, sec, sec-left_sleep_sec);
}


int main(int argc, char *argv[]) {
   if (SIG_ERR == signal(SIGINT, handler)) {
       exit(-1);
   }


   unsigned int sleep_sec = 0;
   if (argc > 1) {
       sleep_sec = atoi(argv[1]);
   } else {
       exit(0);
   }
   printf("sleep for %d seconds\n", sleep_sec);
   snooze(sleep_sec);
   exit(0);
}
```
#### Signal Handling Issues

当程序需要捕获多个信号时，问题产生了。

- 待处理信号被**阻塞**。Unix信号处理程序通常会阻塞当前处理程序正在处理的类型的待处理信号k。如果另一个信号k传递到该进程，则信号k将变成待处理，但是不会被接收，直到处理程序返回。再次检查发现仍有待处理信号k，则再次调用信号处理函数。

- 待处理信号**不会排队等待**。任意类型最多只有一个待处理信号。当目的进程正在执行信号k的处理程序时是阻塞的，当发送两个信号k，仅第一个信号k会变成待处理，第二个则直接被丢弃，不会排队等待。

- 系统调用可以被中断。像read、wait和accept调用过程会阻塞进程的称谓慢速系统调用，当捕获到一个信号时，被中断的慢速系统调用在信号处理返回时不再继续，而是立即返回用户一个错误条件，并将errno设置为EINTR。（即使sleep被信号处理捕获后仍会返回）

#### Explicitly Blocking and Unblocking Signals
```
#include <signal.h>


// how = SIG_BLOCK, blocked=blocked | set
// how = SIG_UNBLOCK, blocked=blocked &~ set
// how = SIG_SETMASK, blocked = set
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset); int sigemptyset(sigset_t *set);
// 将每个信号添加到set
int sigfillset(sigset_t *set);
// 添加signum到set
int sigaddset(sigset_t *set, int signum);
// 从set中删除signum
int sigdelset(sigset_t *set, int signum);
//Returns: 0 if OK, −1 on error


int sigismember(const sigset_t *set, int signum);
//Returns: 1 if member, 0 if not, −1 on error
``` 
#### Nonlocal Jump

作用允许从一个深层嵌套的函数调用中立即返回。

另一个作用是使一个信号处理程序分支到一个特殊的位置sigsetjmp/siglongjmp。
```
#include <setjmp.h>


int setjmp(jmp_buf env);
int sigsetjmp(sigjmp_buf env, int savesigs);
// Returns: 0 from setjmp, nonzero from longjmps


void longjmp(jmp_buf env, int retval);
void siglongjmp(sigjmp_buf env, int retval);
// Never returns


jmp_buf env;
rc=setjmp(env); // 保存当前调用环境
if(rc == 0) dosomething();
else if (rc == 1) dosomething1(); // 如果
else if (rc == 2) dosomething2();


int dosomething() {
   longjmp(buf,1); // 跳转到setjmp，返回1
   // longjmp(buf,2); // 跳转到setjmp，返回2
}
```

#### 操作进程工具
- STRACE：打印一个正在运行的程序和它的子程序调用的每个系统调用的轨迹。
- PS：列出当前系统中的进程（包括僵尸进程）。
- TOP：打印关于当前进程资源使用的信息。
- PMAP：显示进程的存储器映射。
- /proc：一个虚拟文件系统，以ASCII输出大量内核数据结构的内容。如cat /proc/loadavg，观察Linux系统上的当前的平均负载。
