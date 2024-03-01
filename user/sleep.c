#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    write(1, "No arg\n", 6);
  } else {
    int t = atoi(argv[1]);
    sleep(t);
  }
  exit(0);
}
