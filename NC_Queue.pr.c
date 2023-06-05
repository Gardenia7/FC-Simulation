/* Process model C form file: NC_Queue.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char NC_Queue_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 641D65BE 641D65BE 1 LAPTOP-45T4L382 12085 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include "fc_test_global.h"
#define ARRIVAL    	 (op_intrpt_type() == OPC_INTRPT_STRM)
#define PK_or_REQUEST	     (op_intrpt_type () == OPC_INTRPT_ACCESS)
#define SIM_END		 (op_intrpt_type() == OPC_INTRPT_ENDSIM)


//all type of service and total pks stat and que loserate

extern int period_pks_generate;
extern int tcbm_pks_generate;
extern int burst_pks_generate;
//extern int total_pks_send;

extern int period_loss_queue;
extern int burst_loss_queue;
extern int tcbm_loss_queue;
extern int pks_loss_queue;


extern int total_pks_generate;
extern int request_number[10];
extern int delay_num[32];
extern double delay_sort[32];
extern double delay_value[32];



extern int control_command_nonSensitive_pks_generate;
extern int control_command_nonSensitive_pks_loss_queue;





/* End of Header Block */

#if !defined (VOSD_NO_FIN)
#undef	BIN
#undef	BOUT
#define	BIN		FIN_LOCAL_FIELD(_op_last_line_passed) = __LINE__ - _op_block_origin;
#define	BOUT	BIN
#define	BINIT	FIN_LOCAL_FIELD(_op_last_line_passed) = 0; _op_block_origin = __LINE__;
#else
#define	BINIT
#endif /* #if !defined (VOSD_NO_FIN) */



/* State variable definitions */
typedef struct
	{
	/* Internal state tracking for FSM */
	FSM_SYS_STATE
	/* State Variables */
	double	                 		sim_time                                        ;
	double	                 		Total_data_size_in_queue                        ;	/* 队列中所有数据的总大小 */
	double	                 		Node_cache_size                                 ;
	Stathandle	             		NC_queue_stathandle                             ;
	Stathandle	             		NC_burst_traffic_stathandle                     ;
	} NC_Queue_state;

#define sim_time                		op_sv_ptr->sim_time
#define Total_data_size_in_queue		op_sv_ptr->Total_data_size_in_queue
#define Node_cache_size         		op_sv_ptr->Node_cache_size
#define NC_queue_stathandle     		op_sv_ptr->NC_queue_stathandle
#define NC_burst_traffic_stathandle		op_sv_ptr->NC_burst_traffic_stathandle

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	NC_Queue_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((NC_Queue_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* No Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ };
#endif

/* Undefine optional tracing in FIN/FOUT/FRET */
/* The FSM has its own tracing code and the other */
/* functions should not have any tracing.		  */
#undef FIN_TRACING
#define FIN_TRACING

#undef FOUTRET_TRACING
#define FOUTRET_TRACING

#if defined (__cplusplus)
extern "C" {
#endif
	void NC_Queue (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_NC_Queue_init (int * init_block_ptr);
	void _op_NC_Queue_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_NC_Queue_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_NC_Queue_alloc (VosT_Obtype, int);
	void _op_NC_Queue_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
NC_Queue (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (NC_Queue ());

		{
		/* Temporary Variables */
		Packet *pkptr;
		Packet *request_ptr;
		Packet	*pk_data;
		
		Ici* iciptr;
		
		int ici_type;
		int num_pkts;
		int subque;
		int frame;
		
		int strm_num;
		
		int i,j,n;
		
		int bit_number_NCqueue;
		double vMax;
		int iMax;
		double m;
		
		
		
		/* End of Temporary Variables */


		FSM_ENTER ("NC_Queue")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "NC_Queue [init enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Queue [init enter execs]", state0_enter_exec)
				{
				//init hb variable
				
				
				op_ima_obj_attr_get(op_topo_parent(op_id_self()),"Node cache size",&Node_cache_size);
				
				//注册统计量统计队列大小随时间的变化、
				
				NC_queue_stathandle = op_stat_reg ("Queue static.NC queue static", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				NC_burst_traffic_stathandle = op_stat_reg ("NC burst traffic static.NC burst static", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				
				period_loss_queue = 0;
				burst_loss_queue = 0;
				tcbm_loss_queue = 0;
				pks_loss_queue = 0;
				
				total_pks_generate = 0;
				for(i=0;i<10;i++)
					{
					request_number[i]=0;
					}
				
				Total_data_size_in_queue=0.0;
				for(i=0;i<32;i++)
					{
					delay_num[i]=0;
					delay_sort[i]=0.0;
				    delay_value[i]=0.0;
					}
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "NC_Queue [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_4", "NC_Queue [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "NC_Queue [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"NC_Queue")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "NC_Queue [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("NC_Queue [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (PK_or_REQUEST)
			FSM_TEST_COND (SIM_END)
			FSM_TEST_COND (ARRIVAL)
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "PK_or_REQUEST", "", "idle", "send_pk_request", "tr_6", "NC_Queue [idle -> send_pk_request : PK_or_REQUEST / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "SIM_END", "", "idle", "result", "tr_7", "NC_Queue [idle -> result : SIM_END / ]")
				FSM_CASE_TRANSIT (2, 3, state3_enter_exec, ;, "ARRIVAL", "", "idle", "fifo_insert", "tr_8", "NC_Queue [idle -> fifo_insert : ARRIVAL / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send_pk_request) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "send_pk_request", state2_enter_exec, "NC_Queue [send_pk_request enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Queue [send_pk_request enter execs]", state2_enter_exec)
				{
				
				char *p[]={"Request_1","Request_2","Request_3","Request_4","Request_5","Request_6","Request_7","Request_8","Request_9","Request_10","Request_11","Request_12","Request_13","Request_14","Request_15","Request_16","Request_17","Request_18","Request_19","Request_20","Request_21","Request_22","Request_23","Request_24","Request_25","Request_26","Request_27","Request_28","Request_29","Request_30","Request_31","Request_32"};
				
				
				iciptr = op_intrpt_ici ();
				
				if(op_ici_attr_get(iciptr,"ici_access_type",&ici_type) == OPC_COMPCODE_FAILURE)
						op_sim_end("err: unable to get the ici access type!","","","");
				//op_ici_destroy(iciptr);
				if(ici_type == 1)//要求上报自身请求(突发业务请求)，子队列32~63
				{
					request_ptr = op_pk_create_fmt("NC_self_request_format");	
					
					
					
				//NC下行带宽修改为按照最大时延排序分配带宽  每个队列的时延为队头的数据包时延   按照时延值将队列排序  并将队列顺序存入数组delay_num[iMax]
					for(i=0;i<32;i++)
						{
							if(op_subq_empty(i+32)==OPC_FALSE)
								{
									pkptr = op_subq_pk_access(i+32,OPC_QPOS_HEAD);
									delay_sort[i] = op_sim_time() - op_pk_creation_time_get(pkptr);
									
								}
						}
					
					
					for(i=0;i<32;i++)
						{
							delay_value[i] = delay_sort[i];
							delay_num[i] = i;
						}
					
					for(i=0;i<32;i++)
						{
							iMax=i;
							vMax=delay_value[i];
							
							for(j=i;j<32;j++)
							{
								if(delay_value[j] > vMax)
								{
									iMax = j;
									vMax = delay_value[j];
								}
							}
							
							m = delay_value[i];
							delay_value[i] = delay_value[iMax];
							delay_value[iMax] = m;
							
							n = delay_num[i];  
							delay_num[i] = delay_num[iMax];
							delay_num[iMax] = n;
						}
				
				
					//上报各队列请求大小
					for(i=0;i<32;i++)
						{
						num_pkts = op_subq_stat (i+32, OPC_QSTAT_PKSIZE);//子队列中的帧个数
					
						op_pk_nfd_set (request_ptr, p[i], num_pkts);
						}
					
					op_pk_send_quiet(request_ptr,0);
					
					
					
					
					
					//统计NC的队列长度
					bit_number_NCqueue=0;
					for(i=0;i<65;i++)
						{
						bit_number_NCqueue = bit_number_NCqueue+op_subq_stat (i, OPC_QSTAT_BITSIZE);
						}
					
					op_stat_write (NC_queue_stathandle, bit_number_NCqueue);
				
					
				}
				else if(ici_type == 0)//要求发送子队列数据
				{
					//解析需要取的子队列号，如果0~31之间，则默认取一个帧发送下去，如果32~63之间，则需要再解析取帧的个数
					op_ici_attr_get(iciptr,"subque_num",&subque);
					
					//发送周期性队列数据，0~31
					if(subque >=0 && subque<32)
					{
						if(op_subq_empty(subque) == OPC_FALSE)
						{
							pk_data = op_subq_pk_remove(subque,OPC_QPOS_HEAD);
							
							//period_pks_send ++;
							Total_data_size_in_queue=Total_data_size_in_queue-op_pk_total_size_get(pk_data);
							op_pk_send_quiet(pk_data,0);
							
						}
					}
					
					//发送突发性队列数据，32~63
					if(subque >= 32 && subque<64)
					{
						op_ici_attr_get(iciptr,"frame_num",&frame);//需要取的帧个数
						for(i=1;i<=frame;i++)
						{
							if(op_subq_empty(subque) == OPC_FALSE)
							{
								pk_data = op_subq_pk_remove(subque,OPC_QPOS_HEAD);
								
								//burst_pks_send ++;
								Total_data_size_in_queue=Total_data_size_in_queue-op_pk_total_size_get(pk_data);
								op_pk_send_quiet(pk_data,2);
								
							}
						}
					}
					
					
					
					
					//发送强时效队列的数据，64
					if(subque == 64)
					{
						if(op_subq_empty(subque) == OPC_FALSE)
						{
							pk_data = op_subq_pk_remove(subque,OPC_QPOS_HEAD);
							
							//tcbm_pks_send ++;
							Total_data_size_in_queue=Total_data_size_in_queue-op_pk_total_size_get(pk_data);
							op_pk_send_quiet(pk_data,1);
							
						}
					}
					
				}
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (send_pk_request) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "send_pk_request", "NC_Queue [send_pk_request exit execs]")


			/** state (send_pk_request) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send_pk_request", "idle", "tr_5", "NC_Queue [send_pk_request -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (fifo_insert) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "fifo_insert", state3_enter_exec, "NC_Queue [fifo_insert enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Queue [fifo_insert enter execs]", state3_enter_exec)
				{
				int destination_adress;
				strm_num = op_intrpt_strm();
				pkptr = op_pk_get(strm_num);
				
				total_pks_generate ++;
				
				if(strm_num >= 1 && strm_num <= 32)//周期性业务
				{
					period_pks_generate++;
					//printf("strm_num-1:%d-----op_sim_time():%f------\n",strm_num-1,op_sim_time());
					if(Total_data_size_in_queue+ op_pk_total_size_get(pkptr) <= Node_cache_size )
						{
						Total_data_size_in_queue=Total_data_size_in_queue+ op_pk_total_size_get(pkptr);
						if(op_subq_pk_insert (strm_num-1, pkptr, OPC_QPOS_TAIL)==OPC_QINS_FAIL)
							{
							//printf("NC this subqueue lost-------\n");
							//printf("strm_num-1:%d-----op_sim_time():%f------\n",strm_num-1,op_sim_time());
							}
						
						}
					else
						{
						op_pk_destroy(pkptr);
					//printf("NC lost period packet\n");
						//统计周期性丢包率
						period_loss_queue ++;
						}
				}
				
				else if(strm_num >= 33 && strm_num <= 64)//突发业务
				{
				   burst_pks_generate++;
				   op_pk_nfd_get(pkptr,"destination",&destination_adress);
				 
				   
					if(Total_data_size_in_queue+ op_pk_total_size_get(pkptr) <= Node_cache_size )//
						{
						Total_data_size_in_queue=Total_data_size_in_queue+ op_pk_total_size_get(pkptr);
						if(op_subq_pk_insert (destination_adress+31, pkptr, OPC_QPOS_TAIL)==OPC_QINS_FAIL)
							printf("this subqueue lost-----%d----\n",destination_adress);
						}
					else
						{
						op_pk_destroy(pkptr);
						burst_loss_queue++;
						printf("NC lost-------subqueue:%d----subqueue long:%f--\n",destination_adress+31,Total_data_size_in_queue);
						op_sim_end("NC lost","","","");
						}
					
					if(strm_num==33)
						{
						op_stat_write (NC_burst_traffic_stathandle, 16896);
						}
						
				}
				
				if(strm_num == 65)//强时效
				{
					tcbm_pks_generate++;
					//printf("TCBM packet generated------%f----\n",op_sim_time());
					if(Total_data_size_in_queue+ op_pk_total_size_get(pkptr) <= Node_cache_size)//
						{
						Total_data_size_in_queue=Total_data_size_in_queue+ op_pk_total_size_get(pkptr);
						if(op_subq_pk_insert (strm_num-1, pkptr, OPC_QPOS_TAIL)==OPC_QINS_FAIL)
							printf("NC this subqueue lost\n");
						
						}
					else
						{
						op_pk_destroy(pkptr);
						printf("NC lost TCBM packet\n");
						//统计强时效丢包率
						tcbm_loss_queue ++;
						}
				}
				
				
				if(strm_num == 0)//时延不敏感的控制指令
				{
					control_command_nonSensitive_pks_generate++;
					op_pk_nfd_get(pkptr,"destination",&destination_adress);
					if(Total_data_size_in_queue+ op_pk_total_size_get(pkptr) <= Node_cache_size )
						{
						Total_data_size_in_queue=Total_data_size_in_queue+ op_pk_total_size_get(pkptr);
						if(op_subq_pk_insert (destination_adress+31, pkptr, OPC_QPOS_TAIL)==OPC_QINS_FAIL)
							printf("control_command_nonSensitive_pks this subqueue lost\n");
						
						}
					else
						{
						op_pk_destroy(pkptr);
						printf("NC lost  packet\n");
						//时延不敏感的控制指令丢包率
						control_command_nonSensitive_pks_loss_queue ++;
						}
				}
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (fifo_insert) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "fifo_insert", "NC_Queue [fifo_insert exit execs]")


			/** state (fifo_insert) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "fifo_insert", "idle", "tr_9", "NC_Queue [fifo_insert -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (result) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "result", state4_enter_exec, "NC_Queue [result enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Queue [result enter execs]", state4_enter_exec)
				{
				
				sim_time = op_sim_time();
				
				//all stat putout 
				printf("the que module total generate pks %d\n",(period_pks_generate + burst_pks_generate + tcbm_pks_generate));
				printf("the que module period_pks_generate %d\n ",period_pks_generate);
				printf("the que module burst_pks_generate %d\n ",burst_pks_generate);
				printf("the que module tcbm_pks_generate %d\n ",tcbm_pks_generate);
				pks_loss_queue = period_loss_queue + burst_loss_queue + tcbm_loss_queue+pks_loss_queue;
				printf("the que module total loss pks %d\n",pks_loss_queue);
				printf("the que module period_loss_queue %d\n ",period_loss_queue);
				printf("the que module burst_loss_queue %d\n ",burst_loss_queue);
				printf("the que module tcbm_loss_queue %d\n ",tcbm_loss_queue);
				
				printf("the que module total_pks_generate %d\n ",total_pks_generate);
				
				
				//仿真时间结束时打印每一个子队列的长度
				
				for(i=0;i<=64;i++)
					{
					num_pkts = op_subq_stat (i, OPC_QSTAT_PKSIZE);
					if(num_pkts!=0)
					   printf("the  NC PK number in %d subqueue is %d \n ",i,num_pkts);
					}
				
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (9,"NC_Queue")


			/** state (result) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "result", "NC_Queue [result exit execs]")


			/** state (result) transition processing **/
			FSM_TRANSIT_MISSING ("result")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"NC_Queue")
		}
	}




void
_op_NC_Queue_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_NC_Queue_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_NC_Queue_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_NC_Queue_svar function. */
#undef sim_time
#undef Total_data_size_in_queue
#undef Node_cache_size
#undef NC_queue_stathandle
#undef NC_burst_traffic_stathandle

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_NC_Queue_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_NC_Queue_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (NC_Queue)",
		sizeof (NC_Queue_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_NC_Queue_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	NC_Queue_state * ptr;
	FIN_MT (_op_NC_Queue_alloc (obtype))

	ptr = (NC_Queue_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "NC_Queue [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_NC_Queue_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	NC_Queue_state		*prs_ptr;

	FIN_MT (_op_NC_Queue_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (NC_Queue_state *)gen_ptr;

	if (strcmp ("sim_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sim_time);
		FOUT
		}
	if (strcmp ("Total_data_size_in_queue" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Total_data_size_in_queue);
		FOUT
		}
	if (strcmp ("Node_cache_size" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Node_cache_size);
		FOUT
		}
	if (strcmp ("NC_queue_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NC_queue_stathandle);
		FOUT
		}
	if (strcmp ("NC_burst_traffic_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NC_burst_traffic_stathandle);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

