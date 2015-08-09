#ifndef TIME_BENCH
#define TIME_BENCH


#include <deployment.h>
#define TIME_BUFFER_CAPACITY 4

typedef struct _c_queue_element {
  char task_id_port_name[64];
  unsigned int timestamp_ms;
} _c_queue_element;

void po_hi_put_time_stamp(__po_hi_task_id id, __po_hi_local_port_t port,double timeStamp);

#endif 


