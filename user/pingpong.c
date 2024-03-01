#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2]; //p[0] for read, p[1] for write
  pipe(p);
  int pid = fork();
  char buf, buf2;

  if (pid > 0) {
    write(p[1], "a", 1);
    close(p[1]);
    pid = wait((int*) 0);
    read(p[0], &buf2, 1);
    int cur = getpid();
    printf("%d: received pipe: %s\n", cur, &buf2);
    close(p[0]);
    exit(0);
  } else if (pid == 0) {
    read(p[0], &buf, 1);
    close(p[0]);
    int cur = getpid();
    printf("%d: received ping: %s\n", cur, &buf);
    write(p[1], &buf, 1);
    close(p[1]);
    exit(0);
  } else {
    printf("fork error\n");
  }
}
