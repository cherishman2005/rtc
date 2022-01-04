fflush()的作用是用来刷新缓冲区，fflush(stdin)刷新标准输入缓冲区，把输入缓冲区里的东西丢弃； fflush(stdout)刷新标准输出缓冲区，把输出缓冲区里的东西强制打印到标准输出设备上。

fflush(stdin)不太常用，在有些编译器中是错误的用法，可以用以下方法替代：while(getchar()!='/n');

fflush(stdout)在单进程程序中作用不大，但在多进程程序中很有用。程序的输出内容一般不会立即输出，而是在程序结束后再输出。fflush(stdout)会强制每次printf()都立即显示在标准输出设备上。

```
#include<stdio.h>
#include<unistd.h>

int main()
{
    printf("hello");
 // fflush(stdout);
    fork();            //父子进程共享同样的文件位置和内容
    return 0;
}
```

第一种没\n，在fork之前hello还在缓冲区，所以父子进程均要输出hello；

第二种有\n，即 printf("hello\n");   由于printf打印到标准输出时，终端是行缓存， 遇到\n就将缓存输出，所以 fork之前缓存无内容，只打印一次；

stdout默认是是行缓冲的，遇到 \n 就写内容清缓存。

而加上fflush(stdout); 与 有 \n  作用一样，只是不换行。
