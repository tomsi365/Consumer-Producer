#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

int main();
void produce(int id);
void consume(int id);
void free_list(list_node* node);
