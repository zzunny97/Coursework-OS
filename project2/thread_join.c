#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  thread_join(atoi(argv[0]), (void**)argv[1]);
  exit();
}
