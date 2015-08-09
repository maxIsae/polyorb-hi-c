
#include <po_hi_benchmarking.h>
#include <po_hi_debug.h>
#include <po_hi_gqueue.h>
#include <po_hi_returns.h>
#include <po_hi_queue.h>

#include <deployment.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>

unsigned int number = 0;

typedef struct _log_entry {
    __po_hi_task_id task_id;
    __po_hi_port_t port;
    double timestamp;
} log_entry;

log_entry timeRefArray[TIME_BUFFER_CAPACITY];

void po_hi_put_time_stamp(__po_hi_task_id id, __po_hi_local_port_t port, double timeStamp)
{
    log_entry entry = { id, port, timeStamp };

    timeRefArray[number++] = entry;
    if (number == TIME_BUFFER_CAPACITY) {
        __po_hi_benchmarking_vcd_init_integrated_ports();
        __po_hi_benchmarking_vcd_init_separated_ports();
        __po_hi_tasks_killall();
    }
}

int get_global_port_index(__po_hi_task_id id, __po_hi_local_port_t port)
{
    extern __po_hi_int8_t __po_hi_gqueues_nb_ports[__PO_HI_NB_TASKS];
    int global_port_index = 0;
    int j;
    for (j = 0; j < id; j++)
        global_port_index += __po_hi_gqueues_nb_ports[j];
    global_port_index += port;
    return global_port_index;
}

void create_vcd_file(char *var_buf, unsigned int var_buf_size_to_write,
                     char *dumpvar_buf, unsigned int dumpvar_buf_size_to_write,
                     char *buf, unsigned int buf_size_to_write, char *file_name)
{

    int __po_hi_vcd_file;
    pthread_mutex_t __po_hi_vcd_mutex;
    pthread_mutexattr_t __po_hi_vcd_mutex_attr;

    char time_buf[128];
    int time_buf_size_to_write = 0;
    memset(time_buf, '\0', 128);
    
    time_t current_time;

    pthread_mutexattr_init(&__po_hi_vcd_mutex_attr);
    pthread_mutex_init(&__po_hi_vcd_mutex, &__po_hi_vcd_mutex_attr);

    pthread_mutex_lock(&__po_hi_vcd_mutex);
    __po_hi_vcd_file = open(file_name, O_WRONLY | O_CREAT | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (__po_hi_vcd_file > 0) {
        write(__po_hi_vcd_file, "$date\n", 6);


        time(&current_time);
        time_buf_size_to_write = sprintf(time_buf, "%s", ctime(&current_time));
        write(__po_hi_vcd_file, time_buf, time_buf_size_to_write);
        write(__po_hi_vcd_file, "$end\n", 5);
        write(__po_hi_vcd_file, "$version\n", 9);
        write(__po_hi_vcd_file, "VCD generator tool version info text.\n", 38);
        write(__po_hi_vcd_file, "$end\n", 5);
        write(__po_hi_vcd_file, "$comment\n", 9);
        write(__po_hi_vcd_file, "This file has been create by polyorb-hi-c runtime of ocarina.\n", 62);
        write(__po_hi_vcd_file, "$end\n", 5);
        write(__po_hi_vcd_file, "$timescale 1mus $end\n", 21);

        write(__po_hi_vcd_file, "$scope module tasks $end\n", 25);
        write(__po_hi_vcd_file, var_buf, var_buf_size_to_write);
        write(__po_hi_vcd_file, "$upscope $end\n", 14);

        write(__po_hi_vcd_file, "$enddefinitions $end\n", 21);

        write(__po_hi_vcd_file, "$dumpvars\n", 10);
        write(__po_hi_vcd_file, dumpvar_buf, dumpvar_buf_size_to_write);
        write(__po_hi_vcd_file, "$end\n", 5);

        write(__po_hi_vcd_file, buf, buf_size_to_write);

        pthread_mutex_unlock(&__po_hi_vcd_mutex);

    } else {
        __DEBUGMSG("[TIME-BENCHMARKING] Could not create file !\n");
    }

}

extern __po_hi_port_kind_t __po_hi_port_global_kind[__PO_HI_NB_PORTS];
/************************************************************************************************/
void __po_hi_benchmarking_vcd_init_integrated_ports()
{

    extern __po_hi_uint8_t *__po_hi_gqueues_n_destinations[__PO_HI_NB_TASKS];
    extern __po_hi_port_t **__po_hi_gqueues_destinations[__PO_HI_NB_TASKS];

    typedef struct _established_bus {
        unsigned int out_port_global_id;
        unsigned int in_port_global_id;
        char bus_name[64];
        unsigned int send_msg_time;
    } established_bus;

    int port, task, i, j, flag_port_found;
    int global_port_index;
    unsigned int timestamp_ms;
//Flaw: 10 here is a random selected value - in fact the size of array should be exactly the number of all connections between ports
    established_bus established_buses[10];
    unsigned int established_buses_actual_size = 0;
    unsigned int min_send_msg_time, bus_min_send_msg_time;
    
    char buf[1024];
    char var_buf[1024];
    char dumpvar_buf[1024];
    int buf_size_to_write = 0;
    int var_buf_size_to_write = 0;
    int dumpvar_buf_size_to_write = 0;

    memset(buf, '\0', 1024);
    memset(var_buf, '\0', 1024);
    memset(dumpvar_buf, '\0', 1024);
    
    for (i = 0; i < TIME_BUFFER_CAPACITY; i++) {
        global_port_index = get_global_port_index(timeRefArray[i].task_id, timeRefArray[i].port);
        timestamp_ms = timeRefArray[i].timestamp * 10e6;
//Flaw: Multiplication here is not safe
        buf_size_to_write = sprintf(buf, "%s#%i\n", buf, timestamp_ms);
//Description: If port is OUT:
        if (__po_hi_port_global_kind[global_port_index] % 4 != 0) {
            j = 0;
            flag_port_found = 0;
//Description: Iteration through array of known OUT-ports
            while (j < established_buses_actual_size)
            {
//Description: Determination if this OUT port is known
            	if (global_port_index == established_buses[j].out_port_global_id){
		        	flag_port_found = 1;
//Flaw: Buf can be overflowed - there should be a mechanism to check 
		            buf_size_to_write = sprintf(buf, "%s1%s\n", buf, established_buses[j].bus_name);
                    established_buses[j].send_msg_time = timestamp_ms;
            	}
                j++;
            }
//Description: If this OUT port is a new one - finding all destinations of this port in __po_hi_gqueues_n_destinations       
            if (flag_port_found == 0) 
            {
                for (j = 0; j < __po_hi_gqueues_n_destinations[timeRefArray[i].task_id][timeRefArray[i].port]; j++) {
                    established_buses[established_buses_actual_size].out_port_global_id = global_port_index;
//Here the second argument of __po_hi_gqueues_destinations could be local or global port index - so it is eighter timeRefArray[i].port - if local - or global_port_index - if global
                    established_buses[established_buses_actual_size].in_port_global_id =
                        __po_hi_gqueues_destinations[timeRefArray[i].task_id][timeRefArray[i].port][j];
//Description: Creating a pair - out_port_global_id:in_port_global_id - as a name of variable in .vcd file                       
                    sprintf(established_buses[established_buses_actual_size].bus_name, "b%i_%i",
                            established_buses[established_buses_actual_size].out_port_global_id,
                            established_buses[established_buses_actual_size].in_port_global_id);
//Description: Buffer for variables diclaration of .vcd file                              
                    var_buf_size_to_write =
                        sprintf(var_buf, "%s$$var wire 1 %s OUT_global_port_%i_IN_global_port_%i $end\n", var_buf,
                                established_buses[established_buses_actual_size].bus_name,
                                established_buses[established_buses_actual_size].out_port_global_id,
                                established_buses[established_buses_actual_size].in_port_global_id);
//Description: Buffer for dump values of .vcd file                        
                    dumpvar_buf_size_to_write =
                        sprintf(dumpvar_buf, "%s0%s\n", dumpvar_buf,
                                established_buses[established_buses_actual_size].bus_name);
//Description: Buffer for data of .vcd file   
                    buf_size_to_write =
                        sprintf(buf, "%s1%s\n", buf, established_buses[established_buses_actual_size].bus_name);
                    established_buses[established_buses_actual_size].send_msg_time = timestamp_ms;
                    established_buses_actual_size++;
                }
            }

        } else {
            j = 0;
            flag_port_found = 0;
//Flaw: an assumption that time should be strictly greater than 0
            min_send_msg_time = 0;
//Description: an assumption - if there is an incomming msg for IN-port the assumption that it is related to one of the 
//coupled OUT-port which sent a msg at earliest.
            for (j = 0; j < established_buses_actual_size; j++)
            	if (global_port_index == established_buses[j].in_port_global_id)
            	{
            		flag_port_found = 1;
            		if (min_send_msg_time > established_buses[j].send_msg_time || min_send_msg_time == 0)
            		{
            			min_send_msg_time = established_buses[j].send_msg_time;
            			bus_min_send_msg_time = j;
            		}
                }
            if (flag_port_found == 1)
            {     
            	buf_size_to_write = sprintf(buf, "%s0%s\n", buf, established_buses[bus_min_send_msg_time].bus_name);
//Flaw: assumption that IN-port can receive msgs at much higher frequency than any OUT port can send - so there is no more than one unreceived msg between two distinct ports            
            	established_buses[bus_min_send_msg_time].send_msg_time = 0;
            }
            else
                printf("Error: there is a msg for IN-port which has been send by unknown port\n");
        }

    }

    create_vcd_file(var_buf, var_buf_size_to_write,
                    dumpvar_buf, dumpvar_buf_size_to_write, buf, buf_size_to_write, "TIME_BENCH_integrated_ports.vcd");
}

/************************************************************************************************/
void __po_hi_benchmarking_vcd_init_separated_ports()
{

//If I am printing values in ms that there is a possibility that int will overflow

#define SIGNAL_DURATION_MS 10

    typedef struct _task_port_element_vcd {
        __po_hi_task_id task_id;
        __po_hi_port_t port;
    } task_port_element_vcd;

    int port, task, i, j;
    unsigned int timestamp_ms;
    char port_type[4];
    unsigned int end_time_signal;
    char complete_task_port_name[64];
    task_port_element_vcd task_port_element_vcd_array[__PO_HI_NB_PORTS];
    unsigned int task_port_element_vcd_array_size = 0;   
    char buf[1024];
    char var_buf[1024];
    char dumpvar_buf[1024];
    int buf_size_to_write = 0;
    int var_buf_size_to_write = 0;
    int dumpvar_buf_size_to_write = 0;  

//Description: queue is required in order to contain timestamps of signal ending It is required 
//It is required because with any value of SIGNAL_DURATION_MS there is a possibility that
//there is an timestamp from other signal during it
	queueADT c_queue;
    c_queue = QueueCreate();
    _c_queue_element *c_queue_element;
    _c_queue_element retrValue = { NULL, 0 };
    
    memset(buf, '\0', 1024);
    memset(var_buf, '\0', 1024);
    memset(dumpvar_buf, '\0', 1024);

    for (i = 0; i < TIME_BUFFER_CAPACITY; i++) {

        sprintf(complete_task_port_name, "t%ip%i", timeRefArray[i].task_id, timeRefArray[i].port);
        j = 0;
//Description: searching if port is already known
        while (j < task_port_element_vcd_array_size &&
               (task_port_element_vcd_array[j].task_id != timeRefArray[i].task_id
                || task_port_element_vcd_array[j].port != timeRefArray[i].port)) 
            j++;
//Description: if port is unknown       
        if (j == task_port_element_vcd_array_size) {
            if (__po_hi_port_global_kind[get_global_port_index(timeRefArray[i].task_id, timeRefArray[i].port)] % 4 != 0)
                sprintf(port_type, "%s", "OUT");
            else
                sprintf(port_type, "%s", "IN");
            var_buf_size_to_write =
                sprintf(var_buf, "%s$$var wire 1 %s task_id_%i__port_%i_%s $end\n",
                        var_buf, complete_task_port_name, timeRefArray[i].task_id, timeRefArray[i].port, port_type);
            dumpvar_buf_size_to_write = sprintf(dumpvar_buf, "%s0%s\n", dumpvar_buf, complete_task_port_name);
//Description: creating new record for a port
            task_port_element_vcd_array[task_port_element_vcd_array_size].task_id = timeRefArray[i].task_id;
            task_port_element_vcd_array[task_port_element_vcd_array_size].port = timeRefArray[i].port;
            task_port_element_vcd_array_size++;
        }

        timestamp_ms = timeRefArray[i].timestamp * 10e6;
//Description: if retrValue (the last element form queue is not determined)
        if (QueueIsEmpty(c_queue) == 0 && retrValue.timestamp_ms == 0) {
            retrValue = QueueDelete(c_queue);
        }
//Description: pop element from queue till their timestamps are earlier than currrent port timestamp or untill queue is empty
//Flaw: if there are two overlaped signal from the same port?      
        while (retrValue.timestamp_ms != 0 && retrValue.timestamp_ms < timestamp_ms) {
            buf_size_to_write = sprintf(buf, "%s#%i\n", buf, retrValue.timestamp_ms);
            buf_size_to_write = sprintf(buf, "%s0%s\n", buf, retrValue.task_id_port_name);
            if (QueueIsEmpty(c_queue) == 0)
                retrValue = QueueDelete(c_queue);
            else
                retrValue.timestamp_ms = 0;
        }

        buf_size_to_write = sprintf(buf, "%s#%i\n", buf, timestamp_ms);
        buf_size_to_write = sprintf(buf, "%s1%s\n", buf, complete_task_port_name);

        end_time_signal = timestamp_ms + SIGNAL_DURATION_MS;
        c_queue_element = calloc(1, sizeof(_c_queue_element));
        sprintf(c_queue_element->task_id_port_name, "%s", complete_task_port_name);
        c_queue_element->timestamp_ms = end_time_signal;
        QueueEnter(c_queue, *c_queue_element);
    }
    if (retrValue.timestamp_ms != 0) {
//Potential memory leak
        buf_size_to_write = sprintf(buf, "%s#%i\n", buf, retrValue.timestamp_ms);
        buf_size_to_write = sprintf(buf, "%s0%s\n", buf, retrValue.task_id_port_name);
    }
    while (QueueIsEmpty(c_queue) == 0) {
        *c_queue_element = QueueDelete(c_queue);
        buf_size_to_write = sprintf(buf, "%s#%i\n", buf, c_queue_element->timestamp_ms);
        buf_size_to_write = sprintf(buf, "%s0%s\n", buf, c_queue_element->task_id_port_name);
        free(c_queue_element);
    }

    QueueDestroy(c_queue);
    create_vcd_file(var_buf, var_buf_size_to_write,
                    dumpvar_buf, dumpvar_buf_size_to_write, buf, buf_size_to_write, "TIME_BENCH_separated_ports.vcd");
}
