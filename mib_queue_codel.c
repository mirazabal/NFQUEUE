#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mib_queue_codel.h"
#include "mib_queue.h"

#define MAX_NUM_PACKETS 4096

static uint64_t packets_dropped = 0;
static struct PacketTimer pTimer;
static uint32_t first_above_time_; // = 0; //
static uint32_t drop_next_;// = 0;
static uint32_t count_ ; //= 0;
static uint32_t lastcount_; // = 0;
static uint8_t	dropping_ ; //= false;
static const uint32_t interval_ = 2000000;// <>  TARGET =  MS2TIME(5);// 5ms TARGET queue delay
static uint32_t const target_ = 20000;

static const int queueNum = 0;
static const int dropPacket = 0;

static void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);

struct PacketTimer
{
	int64_t usecs[MAX_NUM_PACKETS];
	uint64_t pos;
};
	
static int64_t get_time_us(void)
{
	struct timespec tms;

    /* The C11 way */
    /* if (! timespec_get(&tms, TIME_UTC)) { */

    /* POSIX.1-2008 way */
    if (clock_gettime(CLOCK_REALTIME,&tms)) {
        return -1;
    }
    /* seconds, multiplied with 1 million */
    int64_t micros = tms.tv_sec * 1000000;
    /* Add full microseconds */
    micros += tms.tv_nsec/1000;
    /* round up if necessary */
    if (tms.tv_nsec % 1000 >= 500) {
        ++micros;
    }
		return micros;
}

static void	init_codel_params(void)
{
	first_above_time_ = 0; 
	drop_next_ = 0;
 	count_ = 0;
	lastcount_ = 0;
	dropping_ = 0; // {false}
	pTimer.pos = 0;
}

static int64_t control_law(int64_t t, uint32_t count)
{
		double temp =  (double)interval_ / sqrt( (double)count );
		return t +  (int64_t)temp;
}

struct dodequeue_result
{
		void* m;
		uint32_t ok_to_drop;
};

static struct	dodequeue_result dodequeue(int64_t now)
{
	void* p = mib_queue_deque();
	if(p == NULL){
		struct dodequeue_result ret = { .m = p, .ok_to_drop = 0};
		return ret;
	}

	int64_t sojourn_time = now - pTimer.usecs[pTimer.pos-1];
		if (sojourn_time < target_ ){ //|| bytes() <= maxpacket_) {
			// went below - stay below for at least INTERVAL
			first_above_time_ = 0; // time_stamp(std::chrono::microseconds(0));
			struct dodequeue_result ret = { .m = p, .ok_to_drop = 0};
			return ret;
		}

		if (first_above_time_ == 0){ // time_stamp(std::chrono::microseconds(0))) {
			// just went above from below. if still above at
			// first_above_time, will say it’s ok to drop.
			first_above_time_ = now + interval_; // INTERVAL;
		} else if((now >= first_above_time_) ){
				printf("Sojourn time = %lld \n", (long long)sojourn_time);
				struct dodequeue_result ret = { .m = p, .ok_to_drop = 1};
				return ret;
			//return {&p, true}; // ok_to_drop = true;
		}
		struct dodequeue_result ret = { .m = p, .ok_to_drop = 0};
		return ret;
}

void mib_queue_codel_init( void(*verdict)(uint32_t, uint32_t, uint32_t))
{
	init_codel_params();
	send_verdict_cb = verdict;
	mib_queue_init(verdict);
}

static void drop_packet()
{
		void* data = mib_queue_deque();
		if(data == NULL) return;

		send_verdict_cb( queueNum, *(uint32_t*)data, dropPacket);
		free(data);
		pTimer.pos = pTimer.pos - 1;
}


void* mib_queue_codel_deque()
{
	int64_t now = get_time_us();
	struct dodequeue_result r = dodequeue(now);
	uint32_t delta;
	if (dropping_ == 1) {
		if ( r.ok_to_drop == 0) { //false
			// sojourn time below TARGET - leave drop state
			dropping_ = 0;// false;
			return r.m;
		}
		while (now >= drop_next_ && (dropping_ == 1)) {
			drop_packet(); // free_sdu(listP, rlc_boP);
			packets_dropped++;
			printf(" dropping packet.. %lu \n", packets_dropped );
			size_t queueSize = mib_queue_size();
			if(queueSize == 0){
				first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
				return NULL;
			}
			r = dodequeue(now);
		/*	if(r.m == NULL){
				first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
				return NULL;
			}
*/
			if (r.ok_to_drop == 0 || r.m == NULL) { //false
				// leave drop state
				dropping_ = 0;// false;
			} else {
				++count_;
				// schedule the next drop.
				drop_next_ = control_law(drop_next_, count_);
			}
		}
	} else if (r.ok_to_drop == 1) {
		drop_packet(); // free_sdu(listP, rlc_boP);
		packets_dropped++;
		printf(" dropping packet.. %lu \n", packets_dropped );
		size_t queueSize = mib_queue_size();
		if(queueSize == 0){
			first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
			return NULL;
		}
		r = dodequeue(now);
	/*	if(r.m == NULL){
			first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
			return NULL;
		}*/
		dropping_ = 1;// true;
		delta = count_ - lastcount_;
		count_ = 1;
		if ((delta > 1) && (now - drop_next_ < 16*interval_))
			count_ = delta;
		drop_next_ = control_law(now, count_);
		lastcount_ = count_;
	}
	pTimer.pos = pTimer.pos - 1;
	return r.m;
}

void mib_queue_codel_enqueu(void* data)
{
	mib_queue_enqueu(data);
	pTimer.usecs[pTimer.pos] = get_time_us();
	pTimer.pos = pTimer.pos + 1;
}

size_t mib_queue_codel_size()
{
	return mib_queue_size();
}

void mib_queue_codel_free()
{
	mib_queue_free();
}

