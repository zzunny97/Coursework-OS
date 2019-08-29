 #include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  thread_create((void*)argv[0], (void*)argv[1], (void*)argv[2]);
  exit();
}




