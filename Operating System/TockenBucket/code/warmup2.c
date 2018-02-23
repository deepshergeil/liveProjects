#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include "defs.h"
#include "cs402.h"
#include "my402list.h"


/******************************Global Variables**************************************************/
char *filename=NULL;
bucket_params* bucketp;
int flag=1;
int Debug=0;
int filepkt;

My402List *Q1;
My402List *Q2;
int flag_token_stop=0;
bucket_params * bucketp_new =NULL;
int token_bucket=0;
int spent_time_Q1=0;
int packet_arrival_stop=0;
double total_ms = 0;
int cnt_line = 0;
FILE *readfile = NULL;//bucketp_new->fp;
char *buffer_ptr;
char *rem_ptr;
char *line_ptr;
char buffer[MAX_LINE_LENGTH];
char *interarrival_fromfile;//[10];
char *tokenreqd_fromfile;//[10];
char *servicetimereqd_fromfile;//[10];


void error(char * message)
{
	fprintf(stderr, "Error: %s, program abort!\n", message);
	exit(1);
}


void debug(char * message)
{
	fprintf(stdout, "Debug: %s.\n", message);
}

void showParameters()
{

	if(bucketp->fp==NULL)
	{
		fprintf(stdout,"\nEmulation Parameters:");

		fprintf(stdout,"\n	number to arrive = %d\n	lambda = %lf\n	mu = %lf\n	r = %lf\n	B = %d\n	P = %d\n",bucketp->num,bucketp->lambda,bucketp->mu,bucketp->r,bucketp->B,bucketp->P);
		fprintf(stdout,"00000000.000ms: emulation begins\n");
	}
	else if(filepkt)
	{
		fprintf(stdout,"\nEmulation Parameters:");
		fprintf(stdout,"\n	number to arrive in the tsfile= %d\n	r = %lf\n	B = %d\n	tsfile = %s\n",filepkt,bucketp->r,bucketp->B,filename);
		fprintf(stdout,"00000000.000ms: emulation begins\n");
	}

}


double output_time()
{
	double cur_time =0.0;
	double s=0.0;
	double micro_s =0;
	struct timeval mytime;
	gettimeofday(&mytime, NULL);
	micro_s = (double)mytime.tv_usec/SEC_TO_MISEC;
	s=(double)mytime.tv_sec*SEC_TO_MISEC;    
	cur_time = s + micro_s - total_ms;
	return cur_time;
}

int calculatetime(struct timeval ts1,struct timeval ts2)
{
	int spent=0;
	spent=(ts1.tv_sec-ts2.tv_sec)*SEC_TO_MSEC+ts1.tv_usec-ts2.tv_usec;
	return spent;//return time in microsecs
}

/*---------------------------------Server Thread---------------------------------*/

// the server thread
// initially blocked, waiting for the Queue_CV condition to be signaled
// when unblocked, mutex is locked
// if Q2 is not empty, dequeues a packet and unlock mutex
// 		sleeps for an interval matching the service time of the packet, eject the packet from the system
// 		lock mutex, check if Q2 is empty, etc.
// if Q2 is empty, go wait for the Queue_CV condition to be signaled

void* server1_start(void * bucketp)
{
	useconds_t time_sleep_server;

	int spent_time_service=0;
	int spent_time_system=0;
	int spent_time_Q2=0;


	while(flag)
	{
		pthread_mutex_lock(&mutex); //mutex _lock

		while(My402ListEmpty(Q2)&&flag_token_stop==0)
			pthread_cond_wait(&Queue_CV,&mutex); //wait for the signal


		if(!My402ListEmpty(Q2))
		{
			packet* remove_Q2_packet=(packet*)My402ListFirst(Q2)->obj;

			My402ListUnlink(Q2,My402ListFirst(Q2)); //unlink from Q2

			gettimeofday(&remove_Q2_packet->timestamp_Q2_leaves,NULL);

			spent_time_Q2=calculatetime(remove_Q2_packet->timestamp_Q2_leaves,remove_Q2_packet->timestamp_Q2_enters);

			total_Q2_waittime+=spent_time_Q2;

			if(flag==0)//INTR
				break;

			fprintf(stdout,"%012.3lfms: p%d leaves Q2, time in Q2 = %.3fms\n",output_time(),remove_Q2_packet->packet_id,(double)spent_time_Q2/SEC_TO_MISEC);
			fprintf(stdout,"%012.3lfms: p%d begins service at S1, requesting %.0fms of service\n",output_time(),remove_Q2_packet->packet_id,remove_Q2_packet->time_server/SEC_TO_MISEC);
			pthread_mutex_unlock(&mutex);
			time_sleep_server=remove_Q2_packet->transmission_time;
			usleep(time_sleep_server);

			gettimeofday(&remove_Q2_packet->timestamp_server,NULL);

			spent_time_service=calculatetime(remove_Q2_packet->timestamp_server,remove_Q2_packet->timestamp_Q2_leaves);
			total_service_time_S1+=spent_time_service;			
			spent_time_system=calculatetime(remove_Q2_packet->timestamp_server,remove_Q2_packet->timestamp_Q1_enters);
			total_system_time+=spent_time_system;
			square_total_system_time+=(double)spent_time_system/SEC_TO_MSEC*(double)spent_time_system/SEC_TO_MSEC;
			fprintf(stdout,"%012.3lfms: p%d departs from S1, service time = %.3fms, time in system = %.3fms\n",output_time(),remove_Q2_packet->packet_id,(double)spent_time_service/SEC_TO_MISEC,(double)spent_time_system/SEC_TO_MISEC);
			total_number_server+=1;
			if(flag==0)
				break;			
		}
		pthread_mutex_unlock(&mutex);
		if(packet_arrival_stop==1&&total_number_server==total_countof_packets)
			break;

	}
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&Queue_CV); //to signal other server
	pthread_exit(NULL);
	return 0;
}
void* server2_start(void * bucketp)
{
	useconds_t time_sleep_server;
	int spent_time_Q2=0;
	int spent_time_service=0;
	int spent_time_system=0;
	while(flag)
	{
		pthread_mutex_lock(&mutex);
		for(;My402ListEmpty(Q2)&&flag_token_stop==0;)
			pthread_cond_wait(&Queue_CV,&mutex); //waiting for the signal
		
		if(!My402ListEmpty(Q2))
		{
			packet* remove_Q2_packet=(packet*)My402ListFirst(Q2)->obj;
			My402ListUnlink(Q2,My402ListFirst(Q2));						
			gettimeofday(&remove_Q2_packet->timestamp_Q2_leaves,NULL);
			spent_time_Q2=calculatetime(remove_Q2_packet->timestamp_Q2_leaves,remove_Q2_packet->timestamp_Q2_enters);
			total_Q2_waittime+=spent_time_Q2;
			if(flag==0)
				break;
			fprintf(stdout,"%012.3lfms: p%d leaves Q2, time in Q2 = %.3fms\n",output_time(),remove_Q2_packet->packet_id,(double)spent_time_Q2/SEC_TO_MISEC);
			fprintf(stdout,"%012.3lfms: p%d begins service at S2, requesting %.0fms of service\n",output_time(),remove_Q2_packet->packet_id,remove_Q2_packet->time_server/SEC_TO_MISEC);
			pthread_mutex_unlock(&mutex);
			time_sleep_server=remove_Q2_packet->transmission_time;
			usleep(time_sleep_server);
			gettimeofday(&remove_Q2_packet->timestamp_server,NULL);
			spent_time_service=calculatetime(remove_Q2_packet->timestamp_server,remove_Q2_packet->timestamp_Q2_leaves);
			total_service_time_S2+=spent_time_service;			
			spent_time_system=calculatetime(remove_Q2_packet->timestamp_server,remove_Q2_packet->timestamp_Q1_enters);
			total_system_time+=spent_time_system;
			square_total_system_time+=(double)spent_time_system/SEC_TO_MSEC*(double)spent_time_system/SEC_TO_MSEC;			
			//if(flag==0)
			//	break;
			fprintf(stdout,"%012.3lfms: p%d departs from S2, service time = %.3fms, time in system = %.3fms\n",output_time(),remove_Q2_packet->packet_id,(double)spent_time_service/SEC_TO_MISEC,(double)spent_time_system/SEC_TO_MISEC);
			total_number_server+=1;	
			if(flag==0)
				break;		
		}
		pthread_mutex_unlock(&mutex);
		if(packet_arrival_stop==1&&total_number_server>=total_countof_packets)
			break;
		//if(interrupt)
		//	break;
	}
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&Queue_CV); //to signal other server
	pthread_exit(NULL);
	//return 0;
}

//This function is called when migrating packet from Q1 to Q2.
//Unlinks the moved packet from Queue1
//Check for Interruot,if occurs in between,breaks the loop immediately.
//If Length of Q2 is 1,broadcast the signal to servers.

void packetMoveToQ2()
{
	packet *pkt_diff_queue = (packet *)My402ListFirst(Q1)->obj;
	while(token_bucket >= pkt_diff_queue->reqtoken_perpacket && My402ListLength(Q1)>0)//check condition if token is sufficient enough and length of queue1 is valid
	{
		token_bucket=token_bucket-pkt_diff_queue->reqtoken_perpacket;//save the available required token into packet,starts timestamp

		gettimeofday(&pkt_diff_queue->timestamp_Q1_leaves,NULL);
		My402ListUnlink(Q1,My402ListFirst(Q1)); //unlink

		spent_time_Q1 =calculatetime(pkt_diff_queue->timestamp_Q1_leaves,pkt_diff_queue->timestamp_Q1_enters);
		total_Q1_waittime = total_Q1_waittime + spent_time_Q1;
		if(flag == 0) //Interrupt condition occurs
			break;
		fprintf(stdout,"%012.3lfms: p%d leaves Q1, time in Q1 = %.3fms, token bucket now has %d token\n",output_time(),pkt_diff_queue->packet_id,(double)spent_time_Q1/SEC_TO_MISEC,token_bucket);
		My402ListAppend(Q2,pkt_diff_queue);
		if(flag == 0)
			break;
		fprintf(stdout,"%012.3lfms: p%d enters Q2\n",output_time(),pkt_diff_queue->packet_id);
		gettimeofday(&pkt_diff_queue->timestamp_Q2_enters,NULL);

		if(My402ListLength(Q2)==1)
			pthread_cond_signal(&Queue_CV); //send signals to server thread
		if(My402ListLength(Q1)>0)
			pkt_diff_queue = ((packet*)My402ListFirst(Q1)->obj);
	}
}


//If caought signal Interrupt,
//Token Arrival Threads,broadcast the servers,and set the glovbal variable to accept no more any tokens.
void intrCaughtToken()
{
	pthread_cond_broadcast(&Queue_CV);
	pthread_mutex_lock(&mutex);
	flag_token_stop = 1;
	pthread_mutex_unlock(&mutex);
}

// the token arrival thread sits in an infinite  loop
// sleeps for an interval, trying to match a given interarrival time for tokens(best possible,including/considering usage time of previous)
// wakes up,check for interrupt signal,if occured, locks mutex, try to increment token count
// check if it can move a packet from Q1 to Q2,calling function "packetMoveToQ2"
// if packet is added to Q2 and Q2 was empty before, signal or broadcast a queue-not-empty condition
// unlocks mutex
// goes back to sleep for the "right" amount


void *token_arrival(void *bucketp)
{

	//To track the Token Timings
	struct timeval start_time;
	struct timeval end_time;
	//typecasting
	bucket_params* bucketp_new=(bucket_params*)bucketp;
    //Converting rate to microseconds
	useconds_t increase_time=(1/bucketp_new->r)*SEC_TO_MSEC;

	useconds_t sleep_time=increase_time;
	useconds_t usage_time=0;


	while(flag)
	{
		if(My402ListEmpty(Q1)&&packet_arrival_stop==1) //Q1 IS empty and interrupt ocuured
			break;
		//sleep Calculation
		if(!(usage_time<=increase_time))
			sleep_time=0;
		else
			sleep_time=increase_time-usage_time;
		usleep(sleep_time);
        	//Check Again after sleep
		if(My402ListEmpty(Q1)&&packet_arrival_stop==1)
			break;
        
		//Q1 pACKETS STATS START
		gettimeofday(&start_time,NULL);
		pthread_mutex_lock(&mutex);
		total_number_tokens++;
		//If interrupt
		if(flag==0)
			break;
		
		if(!(token_bucket < bucketp_new->B))
		{
			count_dropped_tokens++;
			fprintf(stdout,"%012.3lfms: token t%d arrives, dropped\n",output_time(),total_number_tokens);
		}
		else
		{
			token_bucket=token_bucket +1;
			fprintf(stdout,"%012.3lfms: token t%d arrives, token bucket now has %d token(s)\n",output_time(),total_number_tokens,token_bucket);
		}
		if(My402ListLength(Q1)>0)
			//Move packets to Q2
			packetMoveToQ2();

		pthread_mutex_unlock(&mutex);
		gettimeofday(&end_time,NULL);
		usage_time = calculatetime(end_time,start_time);
	}

	intrCaughtToken();

	return 0;
}

void parseFields(char *buffer_ptr)
{
	rem_ptr = buffer_ptr;
	while(*rem_ptr != ' ' && *rem_ptr != '\t')
		rem_ptr++;
	*rem_ptr++='\0';

	interarrival_fromfile=malloc(strlen(buffer)+1);
	interarrival_fromfile=strcpy(interarrival_fromfile,buffer);

	for(buffer_ptr=rem_ptr;*buffer_ptr==' ';buffer_ptr++);
	rem_ptr = buffer_ptr;

	while(*rem_ptr != ' ' && *rem_ptr != '\t')
		rem_ptr++;
	*rem_ptr++='\0';
	tokenreqd_fromfile=malloc(strlen(buffer_ptr)+1);//Token req for Packet Parse
	tokenreqd_fromfile=strcpy(tokenreqd_fromfile,buffer_ptr);
	for(buffer_ptr=rem_ptr;*buffer_ptr==' ';buffer_ptr++);

	for(rem_ptr=buffer_ptr;*rem_ptr!=' '&&*rem_ptr!='\t'&&*rem_ptr!='\n';rem_ptr++);
	*rem_ptr = '\0';
	servicetimereqd_fromfile=malloc(strlen(buffer_ptr)+1); //service time req for packet parse
	servicetimereqd_fromfile=strcpy(servicetimereqd_fromfile,buffer_ptr);
}

void parsefile()
{
	fgets(buffer,sizeof(buffer),readfile);
  //  char *temp_parameter;
	buffer_ptr = buffer;
	if(cnt_line == 0)
	{
		for(line_ptr=buffer_ptr;*line_ptr!='\n';line_ptr++);//can be removed
		char *a=malloc(strlen(buffer)+1);
		a=strcpy(a,buffer);
		if(!(*a>='0'&&*a<='9'))
		{
			fprintf(stderr,"Input file not in right format\n");
			exit(0);
		}	
		bucketp_new->num = atoi(a);
	//	fprintf(stdout,"************************************%d\n",bucketp_new->num);
		filepkt=bucketp_new->num;
	//	fprintf(stdout,"************************************%d\n",filepkt);
		showParameters();

		free(a);
	}
	else if(cnt_line <= bucketp_new->num)
	{
		parseFields(buffer_ptr);

	}
	else
		fclose(readfile);
}




void freeFileTemps()
{
	free(interarrival_fromfile);
	free(servicetimereqd_fromfile);
	free(tokenreqd_fromfile);
}

//set global flag from Packet_arrival if interrupt occurs

void intrPacketThread()
{
	pthread_mutex_lock(&mutex);
	packet_arrival_stop=1;
	pthread_mutex_unlock(&mutex);
}

//time conversion of packets in packet_arrival thread
void initialisePacketFile(packet *new_packet)
{
	new_packet->transmission_time = atoi(servicetimereqd_fromfile)*SEC_TO_MISEC;
	new_packet->time_server = atof(servicetimereqd_fromfile)*SEC_TO_MISEC;
	new_packet->reqtoken_perpacket =atoi(tokenreqd_fromfile);

}
void  cancel_threads()
 {
	 pthread_cancel(packet_thread_id);
	 pthread_cancel(token_thread_id);
	 pthread_mutex_lock(&mutex);
 }


// the packet_arrival thread sits in a loop
// sleeps for an interval, trying to match a given interarrival time (from trace or deterministic)
// wakes up, creates a packet object, locks mutex enqueues the packet to Q1 or Q2
// if Q2 was empty before, need to signal or broadcast a queue-not-empty condition
// unlocks mutex
// goes back to sleep for the "right" amount

void *packet_arrival(void *bucketp)
{
	bucketp_new = (bucket_params *)bucketp; //initialise new parameter structure

	//Calculating the Previous Packet Arrival Time,
	struct timeval prevs_pkt_arrival_time;
	prevs_pkt_arrival_time.tv_sec=begin_emulation.tv_sec;

	prevs_pkt_arrival_time.tv_usec=begin_emulation.tv_usec;

	readfile=bucketp_new->fp;



	//Current packet stats parameters
	int pkt_out_time=0;
	useconds_t packet_arrival_time;
	useconds_t sleep_time;
	useconds_t usage_time=0;

	struct timeval start_time;
	struct timeval end_time;
	while(flag)
	{
		packet *new_packet;
		new_packet=(packet*)malloc(sizeof(packet));
		new_packet->packet_id = packet_num;
		if(bucketp_new->fp==NULL)
		{
			if(packet_num>bucketp_new->num)//check for valid no. of tokens
				break;
			//Set To defaults
			packet_arrival_time = (1/bucketp_new->lambda)*SEC_TO_MSEC;
			new_packet->reqtoken_perpacket=bucketp_new->P;
			new_packet->transmission_time =(1/bucketp_new->mu)*SEC_TO_MSEC; 
			new_packet->time_server=(1/bucketp_new->mu)*SEC_TO_MSEC;          
		}
		else
		{
			parsefile();
			if(cnt_line==0)
			{
				cnt_line++;
				continue; //First line stores the No. of  PACKETS TO EMulate
			}
			else if(cnt_line<=bucketp_new->num)
			{
				packet_arrival_time = atoi(interarrival_fromfile)*SEC_TO_MISEC;
				initialisePacketFile(new_packet);
				freeFileTemps();
				cnt_line++;
			}
			else
				break;
		}
		if(packet_arrival_time>MAX_ARR_RATE)
			packet_arrival_time = MAX_ARR_RATE;

		if(new_packet->transmission_time > MAX_ARR_RATE)
			{
				new_packet->transmission_time = MAX_ARR_RATE;
				new_packet->time_server = MAX_ARR_RATE;
			}

		sleep_time = packet_arrival_time;
		if(!(usage_time <=packet_arrival_time))
			sleep_time = 0;
		else
			sleep_time = packet_arrival_time - usage_time;
		usleep(sleep_time); //sleep starts

		gettimeofday(&new_packet->timestamp_Q1_enters,NULL);
		pkt_out_time=calculatetime(new_packet->timestamp_Q1_enters,prevs_pkt_arrival_time);
		prevs_pkt_arrival_time.tv_sec = new_packet->timestamp_Q1_enters.tv_sec;
		prevs_pkt_arrival_time.tv_usec = new_packet->timestamp_Q1_enters.tv_usec;

		total_interarrival_time = total_interarrival_time + pkt_out_time;
		if(flag == 0)
			break;
		//fprintf(stdout,"%012.3lfms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms\n",output_time(),new_packet->packet_id,new_packet->reqtoken_perpacket,(double)pkt_out_time/SEC_TO_MISEC);
		if(new_packet->reqtoken_perpacket > bucketp_new->B)
		{
			fprintf(stdout,"%012.3lfms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms, dropped\n",output_time(),packet_num,new_packet->reqtoken_perpacket,(double)pkt_out_time/SEC_TO_MISEC);
			packet_num+=1;
			total_dropped_pkts+=1;
			pthread_mutex_unlock(&mutex);
			continue;
		}
		fprintf(stdout,"%012.3lfms: p%d arrives, needs %d tokens, inter-arrival time = %.3fms\n",output_time(),new_packet->packet_id,new_packet->reqtoken_perpacket,(double)pkt_out_time/SEC_TO_MISEC);
		gettimeofday(&start_time,NULL);
		pthread_mutex_lock(&mutex);
		My402ListAppend(Q1,new_packet);
		if(flag == 0)
			break;
		fprintf(stdout,"%012.3lfms: p%d enters Q1\n",output_time(),new_packet->packet_id);

		total_countof_packets+=1;
		packet_num+=1;
		packetMoveToQ2();
		pthread_mutex_unlock(&mutex);
		gettimeofday(&end_time,NULL);
		usage_time = calculatetime(end_time,start_time);
	}
	intrPacketThread();
	return (0);
}
int is_Valid(const char *str)
{
	int i = 0, flag = 0;
	while(str[i] != '\0' && str[i] != '\n')
	{
		if(str[i] < '0' || str[i] > '9')
		{
			if(str[i] == '.' && flag == 0)
				flag++;
			else
				return 0;
		}
		i++;
	}
	return 1;
}
void printUsageWarning()
{
fprintf(stdout,"Usage information: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
}

void statsUnexpected()
{
	fprintf(stdout,"	average number of packets in Q1 = N/A : emulation ends prematurely\n");
	fprintf(stdout,"	average number of packets in Q2 = N/A : emulation ends prematurely\n");
	fprintf(stdout,"	average number of packets in S1 = N/A : emulation ends prematurely\n");
	fprintf(stdout,"	average number of packets in S2 = N/A : emulation ends prematurely\n\n");
}

void dropProbability()
{
	if(total_number_tokens>0)
	{
		double token_drop_probability=(double)count_dropped_tokens/(double)total_number_tokens;
		fprintf(stdout,"	token drop probability = %.6g\n",token_drop_probability);
	}
	else
		fprintf(stdout,"	token drop probability = N/A : No tokens generated\n");
	if(packet_num==1)
		fprintf(stdout,"	packet drop probability = N/A : No packets produced\n");
	else
	{
		double packet_drop_probability=(double)total_dropped_pkts/(double)(packet_num-1+packets_removed);
		fprintf(stdout,"	packet drop probability = %.6g\n",packet_drop_probability);
	}

}

//Parsing Arguement Commands and checking their validity
void readinput(int argc,char **argv)
{
	int arg_count;
	struct stat dir;
	for(arg_count=1;arg_count<argc;arg_count++)
	{
		if(argv[argc-1][0]=='-')
		{
			printUsageWarning();			
			error("Malformed commandline argument");
		}		
		if(strcmp(argv[arg_count],"-lambda")==0)
		{
			arg_count++;
			if(is_Valid(argv[arg_count]))			
				bucketp->lambda=atof(argv[arg_count]);//sscanf(argv[i],"%f",bucketp->lambda) for lambda,mu,r and lambda,mu,r <0.1 then =0.1,
			else			
			//if(bucketp->lambda==0)
			{
				printUsageWarning();				
				error("Malformed command line argument: lambda value should be numeric!");
			}
		}
		else if(strcmp(argv[arg_count],"-r")==0)
		{
			arg_count++;
			if(is_Valid(argv[arg_count]))
			{
				bucketp->r=atof(argv[arg_count]);
				if(bucketp->r<0.1)
					bucketp->r=0.1;
			}
			else//if(bucketp->r==0)
			{
				printUsageWarning();
				error("Malformed command line argument: r value should be numeric!");
			}
		}
		else if(strcmp(argv[arg_count],"-mu")==0)
		{
			arg_count++;
			if(is_Valid(argv[arg_count]))			
				bucketp->mu=atof(argv[arg_count]);
			else	//if(bucketp->mu==0)
			{
				printUsageWarning();				
				error("Malformed command line argument: mu value should be numeric!");
			}
		}
		else if(strcmp(argv[arg_count],"-B")==0)
		{
			arg_count++;
			if(is_Valid(argv[arg_count]))			
			{
				bucketp->B=atoi(argv[arg_count]);
				if(bucketp->B>MAX_B_P_NUM)
				{
					printUsageWarning();					
					error("Malformed command line argument: B value limit exceeded!");
				}
			}
			else	//	if(bucketp->B==0||bucketp->B<0)
			{
				printUsageWarning();				
				error("Malformed command line argument: B value  should be positive number!");
			}
		}
		else if(strcmp(argv[arg_count],"-n")==0)//num integer limit check
		{
			arg_count++;
			if(is_Valid(argv[arg_count]))
			{
				bucketp->num=atoi(argv[arg_count]);
				if(bucketp->num>MAX_B_P_NUM)
				{
					printUsageWarning();
					error("Malformed command line argument: num value limit exceeded!\n");
				}
			}
			else//if(bucketp->num==0||bucketp->num<0)
			{
				printUsageWarning();
				fprintf(stderr,"Malformed command line argument: num value  should be positive number!");
			}
		}
		else if(strcmp(argv[arg_count],"-P")==0)
		{
			arg_count++;
			if(is_Valid(argv[arg_count]))			
			{	
				bucketp->P=atoi(argv[arg_count]);
				if(bucketp->P>MAX_B_P_NUM)
				{
					printUsageWarning();					
					error("Malformed command line argument: P value limit exceeded!");

				}
			}
			else//	if(bucketp->P==0||bucketp->P<0)
			{
				printUsageWarning();				
				error("Malformed command line argument: P value  should be positive number!");
			}
		}
		else if(strcmp(argv[arg_count],"-t")==0)//filename directory check
		{
			arg_count++;			
			bucketp->fp=fopen(argv[arg_count],"r");
			if(bucketp->fp==NULL)
			{

				fprintf(stderr,"Malformed command for '%s' %s\n",argv[arg_count],strerror(errno));
				exit(0);
			}
			filename=argv[arg_count];
			if( stat(argv[arg_count],&dir)==0)
				if( dir.st_mode & S_IFDIR )
				{	

					fprintf(stderr,"The %s belongs to a directory\n",argv[arg_count]);
			    		exit(0);
				}

		}
		else
		{
			printUsageWarning();			
			fprintf(stderr,"Invalid command line argument %s\n",argv[arg_count]);
			arg_count++;
			exit(0); //return 0;
		}		
	}
}
void statistic_print()
{
	double total_emulation_time=calculatetime(finish_emulation,begin_emulation);	

	fprintf(stdout,"\nStatistics:\n\n");
	if(packet_num==1)
		fprintf(stdout,"	average packet inter-arrival time = N/A : No packets produced\n");
	else
	{
		double average_interarrival_time=total_interarrival_time/SEC_TO_MSEC/(packet_num-1+packets_removed);
		fprintf(stdout,"	average packet inter-arrival time = %.6gs\n",average_interarrival_time);
	}	
	if(total_number_server==1)
		fprintf(stdout,"	average packet service time = N/A : No packets served/arrived\n\n");
	else
	{
		double average_service_time=(total_service_time_S1+total_service_time_S2)/SEC_TO_MSEC/(total_number_server-1);
		fprintf(stdout,"	average packet service time = %.6gs\n\n",average_service_time);		
	}
	if(total_emulation_time!=0)
	{
		double average_packets_Q1=total_Q1_waittime/total_emulation_time;
		fprintf(stdout,"	average number of packets in Q1 = %.6g\n",average_packets_Q1);
		double average_packets_Q2=total_Q2_waittime/total_emulation_time;
		fprintf(stdout,"	average number of packets in Q2 = %.6g\n",average_packets_Q2);
		double average_packets_S1=total_service_time_S1/total_emulation_time;
		fprintf(stdout,"	average number of packets in S1 = %.6g\n",average_packets_S1);
		double average_packets_S2=total_service_time_S2/total_emulation_time;
		fprintf(stdout,"	average number of packets in S2 = %.6g\n\n",average_packets_S2);		
	}
	else
	{
		statsUnexpected();

	}
	if(total_number_server!=1)	
	{
		double average_system_time=1.0*total_system_time/SEC_TO_MSEC/(total_number_server-1);
		double average_square_system_time=1.0*square_total_system_time/(total_number_server-1);

		double variance_system_time=1.0*average_square_system_time-(average_system_time*average_system_time);
		fprintf(stdout,"	average time a packet spent in system = %.6gs\n",average_system_time);
		if(!isnan(sqrt(variance_system_time)))
			fprintf(stdout,"	standard deviation for time spent in system = %.6g\n\n",sqrt(variance_system_time));
		else
			fprintf(stdout,"	standard deviation for time spent in system = N/A : No packets served/arrived\n\n");
	}
	else
	{
		fprintf(stdout,"	average time a packet spent in system = N/A : No packets served/arrived\n");
		fprintf(stdout,"	standard deviation for time spent in system = N/A : No packets served/arrived\n\n");		
	}
	dropProbability();
	free(bucketp);
}
void remove_packets()
{
	
	if(My402ListEmpty(Q1))
    {
	fprintf(stdout,"%012.3lfms: No packet in Q1 that has to be removed\n",output_time());
    }
    while(!My402ListEmpty(Q1))
    {
        packet* remove_packet=(packet*)My402ListFirst(Q1)->obj;//first_q1->obj;
        fprintf(stdout,"%012.3lfms: p%d in Q1 is removed\n",output_time(),remove_packet->packet_id);
        My402ListUnlink(Q1,My402ListFirst(Q1));
        packets_removed++;
    }
    if(My402ListEmpty(Q2))//if(first_q2 == NULL)
    {
        fprintf(stdout,"%012.3lfms: No packet in Q2 that has to be removed\n",output_time());
    }
    while(!My402ListEmpty(Q2))//while(first_q2!=NULL)
    {
        packet* remove_packet2=(packet*)My402ListFirst(Q2)->obj;//first_q2->obj;
        fprintf(stdout,"%012.3lfms: p%d in Q2 is removed\n",output_time(),remove_packet2->packet_id);
        My402ListUnlink(Q2,My402ListFirst(Q2));//first_q2);
	packets_removed++;
    }

	flag=0;
	packet_arrival_stop=1;
	flag_token_stop=1;
}
/*******************Cntrl+C**************************************/
// arrival thread will stop generating packets and terminate
// 	the arrival thread needs to stop the token thread
// 	the arrival thread needs to clear out Q1 and Q2
// server threads must finish serving its current packet must print statistics for all packet seen
// 	need to make sure that packets deleted this way do not participate in certain statistics calculation

void* sigint_handler()
{
	int sig;

		sigwait(&set,&sig);
		fprintf(stdout,"\n%012.3lfms: SIGINT caught, no new packets or tokens will be allowed\n",output_time());		

        //sending thread cancel signals
	   cancel_threads();


	   //Remove packets
		remove_packets(); //remove packets and set global variables to signal interrupt
		pthread_cond_broadcast(&Queue_CV); //To finish servicing the packet by server
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);


	return 0;
}

//Initialise With Default bucketp
void init(int argc,char **argv)
{
	bucketp=(bucket_params*)malloc(sizeof(bucket_params));
	bucketp->lambda=DEFAULT_PACKET_ARRIVE_RATE;
	bucketp->mu=DEFAULT_PACKET_SERVE_RATE;
	bucketp->r=DEFAULT_TOKEN_ARRIVE_RATE;
	bucketp->B=DEFAULT_TOKEN_BUCKET_CAPACITY;
	bucketp->P=DEFAULT_TOKEN_REQUIRE_AMOUNT;
	bucketp->num=DEFAULT_TOTAL_PACKET_NUMBER;
	bucketp->fp=NULL;
	readinput(argc,argv);

}
//Initial Sig parameters Set up

void signalSetUp()
{
	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	sigprocmask(SIG_BLOCK,&set,0);
}

void initial()
{
	signalSetUp();
	Q1=(My402List*)malloc(sizeof(My402List));
	Q2=(My402List*)malloc(sizeof(My402List));
	if(!My402ListInit(Q1))
	{
		error("list Q1 could not be initialized");
	}
	if(!My402ListInit(Q2))
	{
		error("list Q2 could not be initialized");
	}
	if(Debug)
		debug("initializing...");
}


void prepareThreads()
{
	gettimeofday(&begin_emulation,NULL);//main emulation begins()
	struct timeval mytime;
	gettimeofday(&mytime, NULL);
	float micro_s = (float)mytime.tv_usec/SEC_TO_MISEC; //EDIT
	double s=(double)mytime.tv_sec*SEC_TO_MISEC;
	total_ms = s + micro_s;
	pthread_create(&token_thread_id,0,token_arrival,(void *)bucketp);
	pthread_create(&packet_thread_id,0,packet_arrival,(void *)bucketp);
	pthread_create(&server1_thread_id,0,server1_start,(void *)bucketp);
	pthread_create(&server2_thread_id,0,server2_start,(void *)bucketp);
	pthread_create(&interrupt_thread_id,0,sigint_handler,0);
	//long_running_proc();

	pthread_join(token_thread_id,0);
	pthread_join(packet_thread_id,0);
	pthread_join(server1_thread_id,0);
	pthread_join(server2_thread_id,0);
	//pthread_join(interrupt_thread_id, 0);
	gettimeofday(&finish_emulation,NULL);
	fprintf(stdout,"%012.3lfms: emulation ends\n",output_time());

}
int main(int argc, char **argv)
{
	//sigset(SIGINT,signal_handler);
	initial();

	//DefaultInit
	init(argc,argv);

	showParameters();

	prepareThreads();

	statistic_print();

	return 0;
}
