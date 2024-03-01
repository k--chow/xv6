#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// just imagine 2 3 4 5 for now
// helper(2,2) ->  fork, child print 2, child waiting to read
// helper(3, 2) -> pipe to child 2, child 2 calls helper(3, 3), back to read
// helper (3, 3) -> so fork child 3, print 3, child 3 blocks on read
// helper (4, 2) -> nothing
// helper (5, 2) -> pipe to child 2, child 2 reads, then child 2 pipes to child 3, child3 reads, child3 calls helper(5, 5), back to read
// helper(5, 5) -> child3 forks child 5 with pipe, print 5, etc  
// close write pipe, read is 0, close its write pipe, should ripple bottom up, first process closing last!

// main function should be separate from forking each time?
void helper(int current, int start, int* p)
{
  int c_pid = getpid();
  printf("helper %d %d pid %d\n", current, start, c_pid);
  if (current == start) { 
    // create pipe between child and parent using p
    pipe(p);
    int pid = fork();
    
    if (pid == 0) {
      close(p[1]); //child will not write to parent!
      printf("prime %d\n", current);
      int ch_pid = getpid(); 
      int val;
      printf("now reading from %d\n", current);
      int counter = 0;
      int bytes;
      while ((bytes = read(p[0], &val, sizeof(current))) > 0) {
        int p2[2];
        printf("read %d from %d count %d pid %d \n", val, current, counter, ch_pid);
        // pipe if already exists
        //write(p[1],
        // call otherwise
        if (counter == 0) {
          helper(val, val, p2);
        } else {
          //helper(val, current, p2);
          write(p2[1], &val, sizeof(val));
        }
        counter++;
        printf("Q: %d\n", current);
      }
      printf("bytes %d from %d", bytes, current);
      if (bytes == 0) {
        printf("close child read %d\n", current);
        close(p[0]); //child close read end!
        printf("close child write %d\n", current);
        close(p[1]);             //child close write end!
        //exit(0);

        } else {
          printf("ERROR %d", current);
        }
    } else if (pid > 0 ) {
      // parent should close its read
      close(p[0]);
    }
 } else if (current % start == 0) {
    //printf("non-prime %d", current);
    return;
  } else {
    write(p[1], &current, sizeof(current));
  } 
}

int
main(int argc, char *argv[])
{
  int p[2]; //p[0] for read, p[1] for write
  int start = 2;
  int end = 6;
    // generator
  for (int i=start; i < end; i++) {
    helper(i, start, p);
    sleep(1); //cheating! 
  }
  
  printf("read close\n");
  // read should be shut from generator. 
  // child 2->3 p2[0] read no need. p2[1] write needed!
  // looks like parent read needs to shut down!
  //close(p[0]); //generators read end
  close(p[1]); //generator write end
  //sleep(100);



}
