#include "mib_queue_codel_impl.h"
#include "mib_queue_lfds_impl.h"

#include <math.h>
#include <time.h>

#define MAX_NUM_PACKETS 4096
static int packets_dropped = 0;

static struct PacketTimer pTimer;
static uint32_t first_above_time_; // = 0; //
static uint32_t drop_next_;// = 0;
static uint32_t count_ ; //= 0;
static uint32_t lastcount_; // = 0;
static uint8_t	dropping_ ; //= false;
static const uint32_t interval_  = 100000;// <>  TARGET =  MS2TIME(5);// 5ms TARGET queue delay
static uint32_t const target_ = 10000;

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
		mem_block_t* m;
		uint32_t ok_to_drop;
};

static struct	dodequeue_result dodequeue(mib_list_t* listP, int64_t now)
{
	mem_block_t* p =  listP->head; 
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

void mib_list_init (mib_list_t * listP)
{
	if (listP == NULL)
	 	return;

	mib_init_times_called = mib_init_times_called + 1; 
	for(int i = 0; i < 25; i++){
		printf("number of times mib_init_times called %d \n", mib_init_times_called );
	}

	listP->tail = NULL;
  listP->head = NULL;
  listP->nb_elements = 0;
	init_codel_params();
}

void mib_list_free (mib_list_t * listP)
{
	if (listP == NULL)
	 	return;
	
  mem_block_t      *le;
  while( (le = (mem_block_t*)mib_list_get_head(listP) )){
		 mib_list_remove_head (listP);
	 	 free_mem_block (le, __func__);
  }
	pTimer.pos = 0;
}

static void free_sdu(mib_list_t* listP, rlc_buffer_occupancy_t* rlc_boP)
{
	mem_block_t* sdu_in_buffer = mib_list_get_head(listP);
  struct rlc_um_tx_sdu_management * sdu_mngt_p = ((struct rlc_um_tx_sdu_management *) (sdu_in_buffer->data));
	*rlc_boP -= sdu_mngt_p->sdu_remaining_size;
	mib_list_remove_head(listP);
	free_mem_block (sdu_in_buffer, __func__);
	sdu_mngt_p    = NULL;
}

mem_block_t* mib_list_get_head_or_drop(mib_list_t * listP, rlc_buffer_occupancy_t* rlc_boP)
{
	if (listP == NULL)
		return NULL;
	if (listP->head == NULL)
		return NULL;	

	if(listP->nb_elements == 0){ // size == 0
		first_above_time_ = 0;
		return NULL;
	}

	int64_t now = get_time_us();
	struct dodequeue_result r = dodequeue(listP,now);
	uint32_t delta;
	if (dropping_ == 1) {
		if ( r.ok_to_drop == 0) { //false
			// sojourn time below TARGET - leave drop state
			dropping_ = 0;// false;
			return r.m;
		}
		while (now >= drop_next_ && (dropping_ == 1)) {
			free_sdu(listP, rlc_boP);
			packets_dropped++;
			printf(" dropping packet.. %d \n", packets_dropped );
			if( listP->nb_elements == 0 ){
				first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
				return NULL;
			}
			r = dodequeue(listP, now);
			if (r.ok_to_drop == 0) { //false
				// leave drop state
				dropping_ = 0;// false;
			} else {
				++count_;
				// schedule the next drop.
				drop_next_ = control_law(drop_next_, count_);
			}
		}
	} else if (r.ok_to_drop == 1) {
		free_sdu(listP, rlc_boP);
		packets_dropped++;
		printf(" dropping packet.. %d \n", packets_dropped );
		if( listP->nb_elements == 0){
			first_above_time_ = 0;
			return NULL;
		}
		r = dodequeue(listP, now);
		dropping_ = 1;// true;
		delta = count_ - lastcount_;
		count_ = 1;
		if ((delta > 1) && (now - drop_next_ < 16*interval_))
			count_ = delta;
		drop_next_ = control_law(now, count_);
		lastcount_ = count_;
	}
	return r.m;
}

mem_block_t* mib_list_get_head (mib_list_t * listP)
{
	if (listP == NULL)
		return NULL;

	return listP->head; 
}

void mib_list_remove_head (mib_list_t * listP)
{
	printf("Into mib_list_remove_head \n");
	if (listP == NULL){
	 	return;
	}

	mem_block_t* head = listP->head;
	if (head == NULL)
		return;// head; 

	listP->head = head->next;

	if (listP->head == NULL) 
		listP->tail = NULL;

	pTimer.pos = pTimer.pos - 1;
	listP->nb_elements = listP->nb_elements - 1;
}

void mib_list_add_tail(mem_block_t* elementP, mib_list_t * listP)
{
	if (elementP == NULL || listP == NULL)
	 	return;

//	elementP->next = NULL;
	mem_block_t* tail = listP->tail;

	if (tail == NULL) {
		listP->head = elementP; 
	} else {
		tail->next = elementP;
	}
	listP->tail = elementP;

	pTimer.usecs[pTimer.pos] = get_time_us();
	pTimer.pos = pTimer.pos + 1;
	listP->nb_elements = listP->nb_elements + 1;
}








void mib_queue_codel_init()
{
	mib_queue_lfds_init();
}

void mib_queue_codel_free()
{

}

void mib_queue_codel_enqueu(void* data)
{

}

void* mib_queue_codel_deque()
{

}

size_t mib_queue_codel_size()
{

	mib_queue_lfds_size();
}


