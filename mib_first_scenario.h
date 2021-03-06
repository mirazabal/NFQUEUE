#ifndef MIB_FIRST_SCENARIO
#define MIB_FIRST_SCENARIO

#if FIRST_1st
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_2nd_10 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 10
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_2nd_20 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 20
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_2nd_30 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 30
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_3rd 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 1
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_4th_10 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 10
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_4th_20 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 20
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_4th_30 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 30
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_5th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 10500
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 1
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_6th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 1
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_7th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_8th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 0
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_9th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 1
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 1
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_10th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1 
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
        #define DYN_RLC 1
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 0
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_11th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
        #define DYN_RLC 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 0
        #define CQI_PACER 1
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_12th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
        #define DYN_RLC 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 0
        #define CQI_PACER 0
        #define CQI_PACER_ASYNC 1
        #define DYN_RADIO_CHANNEL 1
#endif


#if FIRST_13th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 1
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 1
        #define DYN_RLC 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 0
        #define CQI_PACER 0
        #define CQI_PACER_ASYNC 0
        #define DYN_RADIO_CHANNEL 1
#endif




/// QFI Pacer family 


#if FIRST_14th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
        #define DYN_RLC 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 1
        #define CQI_PACER 1
        #define CQI_PACER_ASYNC 0
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_15th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
        #define DYN_RLC 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 1
        #define CQI_PACER 0
        #define CQI_PACER_ASYNC 1
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_16th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 0
        #define DYN_RLC 1
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 1
        #define CQI_PACER 0
        #define CQI_PACER_ASYNC 0
        #define DYN_RADIO_CHANNEL 1
#endif

#if FIRST_17th 
	#define FIRST_SCENARIO 1
	#define UPF_SCHED_TICK 1000
	#define SDAP_SCHED_TICK 1000
	#define UPF_QUEUES_CODEL 0
	#define QFI_QUEUES_CODEL 0
	#define DRB_QUEUES_CODEL 0
	#define DYNAMIC_QUEUE 1
        #define DYN_RLC 0
	#define MAX_NUM_PACK_DRB 1024
	#define MAX_NUM_PACK_QFI 1024
	#define QFI_QUEUE_PACER 1
        #define CQI_PACER 0
        #define CQI_PACER_ASYNC 0
        #define DYN_RADIO_CHANNEL 1
#endif


#endif
