
#include <po_time_benchmarking_TEST.h>
#include <deployment.h>

int send_number = 0;
int receive_number = 0;

double static timeRefArray[TIME_BUFFER_CAPACITY];

void po_hi_put_time_stamp(__po_hi_task_id id, __po_hi_local_port_t port,double timeStamp)
{
    if (id==node_a_pinger_k && port == pinger_local_data_source)
    {
	timeRefArray[send_number]=timeStamp;
	send_number++;
    }
    if (id==node_a_ping_me_k && port==ping_me_local_data_sink)
    {
	timeRefArray[receive_number]=timeStamp-timeRefArray[receive_number];
	/*FOR TESTING*/
	printf("Duration: %f\n", timeRefArray[receive_number]);
	/*END:FOR TESTING*/
	receive_number++;

	if (receive_number>TIME_BUFFER_CAPACITY-1)
		__po_hi_tasks_killall ();
    }
}
