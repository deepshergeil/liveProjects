#ifndef _WARMUP2_H_
#define _WARMUP2_H_


#define SEC_TO_MSEC 1000000
#define MAX_ARR_RATE 10000000

#ifndef DEFAULT_PACKET_ARRIVE_RATE
#define DEFAULT_PACKET_ARRIVE_RATE 1.0
#endif

#ifndef DEFAULT_PACKET_SERVE_RATE
#define DEFAULT_PACKET_SERVE_RATE 0.35
#endif

#ifndef DEFAULT_TOKEN_ARRIVE_RATE
#define DEFAULT_TOKEN_ARRIVE_RATE 1.5
#endif

#ifndef DEFAULT_TOKEN_REQUIRE_AMOUNT
#define DEFAULT_TOKEN_REQUIRE_AMOUNT 3
#endif

#ifndef DEFAULT_TOKEN_BUCKET_CAPACITY
#define DEFAULT_TOKEN_BUCKET_CAPACITY 10
#endif

#ifndef DEFAULT_TOTAL_PACKET_NUMBER
#define DEFAULT_TOTAL_PACKET_NUMBER 20
#endif

#ifndef MAX_B_P_NUM
#define MAX_B_P_NUM 2147483647
#endif

#ifndef MAX_LINE_LENGTH
#define MAX_LINE_LENGTH 1024
#endif


#ifndef SEC_TO_MISEC
#define SEC_TO_MISEC 1000
#endif
//typedef void (*sighandler_t)(int);
//sighandler_t sigset(int sig, sighandler_t disp);
//some_state_t state;
sigset_t set;

typedef struct
{
	double lambda;
	double mu;
	double r;
	int B;
	int P;
	int num;
	FILE* fp;
}bucket_params;

struct timeval begin_emulation;
struct timeval finish_emulation;

pthread_t token_thread_id, packet_thread_id, server1_thread_id,server2_thread_id,interrupt_thread_id;

void* packet_arrival(void* values);
void* token_arrival(void* values);
void* server1_start(void* values);
void* server2_start(void* values);
void signal_handler(int);

typedef struct
{
	int packet_id;
	int reqtoken_perpacket;
	struct timeval timestamp_Q1_enters;
	struct timeval timestamp_Q1_leaves;
	struct timeval timestamp_Q2_enters;
	struct timeval timestamp_Q2_leaves;
	struct timeval timestamp_server;
	useconds_t transmission_time;
	double time_server;
}packet;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Queue_CV = PTHREAD_COND_INITIALIZER;

int total_dropped_pkts=0;
int total_countof_packets=1;
int packets_removed=0;
int packet_num=1;
int count_dropped_tokens=0;
int total_number_tokens=0;
int total_number_server=1;
double total_interarrival_time=0;
double total_Q1_waittime=0;
double total_Q2_waittime=0;
double total_service_time_S1=0;
double total_service_time_S2=0;
double total_system_time=0;
double square_total_system_time=0;

#endif
