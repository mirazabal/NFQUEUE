#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "5G_qos_model.h"
#include "mib_qfi_queues.h"
#include "mib_upf_queues.h"
#include "mib_drb_queues.h"

#include "mib_UPF_sched.h"
#include "mib_SDAP_sched.h"
#include "mib_MAC_sched.h"

static int const NFQUEUE_NUM = 0;
static pthread_t pthread_UPF_sched;
static pthread_t pthread_SDAP_sched;
static pthread_t pthread_MAC_sched;

static int init_vars = 0;

static struct QFI_queues qfi_queues; 
static struct UPF_queues upf_queues; 
static struct DRB_queues drb_queues;

static struct SDAP_thread_data sdap_data;
static struct UPF_thread_data upf_data;
static struct MAC_thread_data mac_data;

static void init_qfi_queues(void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  init_QFI_queues(&qfi_queues, verdict);
}

static void init_upf_queues(void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  init_UPF_queues(&upf_queues, verdict);
}

static void init_drb_queues(void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  init_DRB_queues(&drb_queues, verdict);
}

static void init_threads_data(void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  sdap_data.NFQUEUE_NUM = NFQUEUE_NUM;
  sdap_data.send_verdict_cb = verdict;
  sdap_data.qfiQ = &qfi_queues; 
  sdap_data.drbQ = &drb_queues; 

  upf_data.send_verdict_cb = verdict;
  upf_data.qfiQ = &qfi_queues;
  upf_data.upfQ = &upf_queues;

  mac_data.NFQUEUE_NUM = NFQUEUE_NUM;
  mac_data.send_verdict_cb = verdict;
  mac_data.drbQ = &drb_queues;
}

void init_5G_qos_model(void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  init_qfi_queues(verdict);
  init_upf_queues(verdict);
  init_drb_queues(verdict);

  init_threads_data(verdict);
  pthread_create(&pthread_SDAP_sched , NULL, thread_SDAP_sched, &sdap_data );
  pthread_create(&pthread_UPF_sched, NULL, thread_UPF_sched, &upf_data );
  pthread_create(&pthread_MAC_sched, NULL, thread_MAC_sched, &mac_data);

  init_vars = 1;
}

void close_5G_qos_model()
{
  void* ret = NULL;
  close_SDAP_thread();
  if(pthread_join(pthread_SDAP_sched, &ret))
    printf("Error joining the thread \n");

  close_UPF_thread();
  if(pthread_join(pthread_UPF_sched, &ret))
    printf("Error joining the thread \n");

  close_MAC_thread();
  if(pthread_join(pthread_MAC_sched, &ret))
    printf("Error joining the thread \n");

}

void add_packet_5G_qos_model(uint32_t queue, uint32_t id, uint32_t hash)
{
  if(queue != NFQUEUE_NUM)
    printf("Errro assigning the queue, just queue 0 working!!! \n");

  if(init_vars == 0)
    return;

  uint32_t queueIdx = 0;
  if(hash != 0){
   queueIdx  = hash % 64;// 128;
    if(queueIdx == 0)
      queueIdx = 1; 
  }

  addPacketToUPF(&upf_queues, queueIdx, id);
}
