// Вводится тип прически(количество времени); Если есть запись, то вставляется в очередь
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HAIRCUTS_N 4
#define MAX_CUSTOMERS 100

int customer_count = 0;
static struct
{
  char creds[21];
  int res_time_m;
  int haircut_time_m;
} queue[MAX_CUSTOMERS];
struct Haircut
{
  char caption[64];
  int time_m;
};

struct Haircut haircuts[HAIRCUTS_N];

void print_haircuts()
{
  const char *captions[] = {
      "Bobrik",
      "pod 0",
      "type3",
      "type4",
  };
  const int time_m_vals[] = {
      15,
      20,
      40,
      70,
  };
  printf("Available haircuts\n");
  for (int i = 0; i < HAIRCUTS_N; i++)
  {
    printf("%d: ", i);
    printf(captions[i]);
    printf(", time to finish: %d mins;\n", time_m_vals[i]);
    strcpy(haircuts[i].caption, captions[i]);
    haircuts[i].time_m = time_m_vals[i];
  }
}

void shift_queue_right(int i) {
  for (int j = customer_count; j > i; j--)
  {
    strcpy(queue[j].creds, queue[j - 1].creds);
    queue[j].res_time_m = queue[j - 1].res_time_m;
    queue[j].haircut_time_m = queue[j - 1].haircut_time_m;
  }
}

void add_customer(char *creds, int res_time_m, int haircut_time_m)
{
  int i;
  time_t seconds;
  struct tm *timeStruct;
  seconds = time(NULL);
  timeStruct = localtime(&seconds);
  int current_time_m = timeStruct->tm_hour * 60 + timeStruct->tm_min;

  if (res_time_m)
    for (i = 0; i < customer_count; i++)
    {
      if (queue[i].res_time_m + queue[i].haircut_time_m > res_time_m + haircut_time_m) 
        break;
    }
  else {
    i = customer_count;
    res_time_m = current_time_m;
  }
  shift_queue_right(i);
  strcpy(queue[i].creds, creds);
  queue[i].res_time_m = res_time_m;
  queue[i].haircut_time_m = haircut_time_m;
  customer_count++;
}

void remove_customer()
{
  for (int j = 0; j < customer_count; j++)
  {
    strcpy(queue[j].creds, queue[j + 1].creds);
    queue[j].res_time_m = queue[j + 1].res_time_m;
    queue[j].haircut_time_m = queue[j + 1].haircut_time_m;
  }
  customer_count--;
}

void print_queue()
{
  printf("---------\n");
  for (int i = 0; i < customer_count; i++) {
    printf("%d: ", i + 1);
    printf(queue[i].creds);
    printf("\n");
  }
  printf("---------\n");
}

void main()
{
  print_haircuts();
  int type = 1, res_time_m;
  char *creds, *reservation, *action; 
  add_customer("a", 0, 20);
  add_customer("b", 0, 20);
  add_customer("c", 0, 20);
  print_queue();
  while (1) {
    scanf("%s", &action);
    if (!strcmp(&action, "+")) {
      scanf("%d", &type);
      scanf("%s", &creds);
      scanf("%s", &reservation);
      if (strcmp(&reservation, "-"))
      {
        char *pEnd;
        res_time_m = strtol(&reservation, &pEnd, 10) * 60 + strtol(pEnd + 1, &pEnd, 10);
      }
      add_customer(&creds, res_time_m, haircuts[type].time_m);
    }
    else if (!strcmp(&action, "-")) {
      remove_customer();
    }
    print_queue();
  }
}