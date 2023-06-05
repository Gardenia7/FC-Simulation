/* Process model C form file: fc_1553_NTqueue_report1destination_1DBA.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char fc_1553_NTqueue_report1destination_1DBA_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 641D657B 641D657B 1 LAPTOP-45T4L382 12085 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fc_test_global.h>



#define REQUEST_TYPE		0
#define DATA_TYPE			1


#define ARRIVAL    	 (op_intrpt_type() == OPC_INTRPT_STRM)
#define SEND	     (op_intrpt_type () == OPC_INTRPT_ACCESS && ici_access_type ==DATA_TYPE)
#define REQUEST	     (op_intrpt_type () == OPC_INTRPT_ACCESS && ici_access_type ==REQUEST_TYPE)
#define SIM_END		 (op_intrpt_type() == OPC_INTRPT_ENDSIM)
// the next is que stat

extern int pks_loss_queue;

extern int total_pks_generate;





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
	Objid	                  		node_id                                         ;
	Objid	                  		subnet_id                                       ;
	Stathandle	             		Total_received_packets_handle                   ;
	Stathandle	             		Total_lossed_packets_handle                     ;
	Stathandle	             		Total_loss_rate_handle                          ;
	int	                    		intrpt_code                                     ;
	int	                    		input_strm_index                                ;
	Ici *	                  		iciptr                                          ;
	int	                    		ici_access_type                                 ;
	int	                    		packet_generate_global                          ;
	int	                    		packet_lost_global                              ;
	Stathandle	             		burst1_bitssec_handle                           ;
	Stathandle	             		burst2_bitssec_handle                           ;
	Stathandle	             		burst3_bitssec_handle                           ;
	int	                    		node_num                                        ;
	int	                    		destination_adress                              ;
	int	                    		pkptr_destination                               ;
	Packet *	               		source_pkptr                                    ;
	Distribution *	         		pkptr_destination_dist_handle                   ;
	int	                    		self_adress                                     ;
	Stathandle	             		node_traffic_stathandle                         ;
	double	                 		Total_data_size_in_queue                        ;
	double	                 		Node_cache_size                                 ;
	Stathandle	             		NT_queue_stathandle                             ;
	int	                    		DBA_num                                         ;
	} fc_1553_NTqueue_report1destination_1DBA_state;

#define node_id                 		op_sv_ptr->node_id
#define subnet_id               		op_sv_ptr->subnet_id
#define Total_received_packets_handle		op_sv_ptr->Total_received_packets_handle
#define Total_lossed_packets_handle		op_sv_ptr->Total_lossed_packets_handle
#define Total_loss_rate_handle  		op_sv_ptr->Total_loss_rate_handle
#define intrpt_code             		op_sv_ptr->intrpt_code
#define input_strm_index        		op_sv_ptr->input_strm_index
#define iciptr                  		op_sv_ptr->iciptr
#define ici_access_type         		op_sv_ptr->ici_access_type
#define packet_generate_global  		op_sv_ptr->packet_generate_global
#define packet_lost_global      		op_sv_ptr->packet_lost_global
#define burst1_bitssec_handle   		op_sv_ptr->burst1_bitssec_handle
#define burst2_bitssec_handle   		op_sv_ptr->burst2_bitssec_handle
#define burst3_bitssec_handle   		op_sv_ptr->burst3_bitssec_handle
#define node_num                		op_sv_ptr->node_num
#define destination_adress      		op_sv_ptr->destination_adress
#define pkptr_destination       		op_sv_ptr->pkptr_destination
#define source_pkptr            		op_sv_ptr->source_pkptr
#define pkptr_destination_dist_handle		op_sv_ptr->pkptr_destination_dist_handle
#define self_adress             		op_sv_ptr->self_adress
#define node_traffic_stathandle 		op_sv_ptr->node_traffic_stathandle
#define Total_data_size_in_queue		op_sv_ptr->Total_data_size_in_queue
#define Node_cache_size         		op_sv_ptr->Node_cache_size
#define NT_queue_stathandle     		op_sv_ptr->NT_queue_stathandle
#define DBA_num                 		op_sv_ptr->DBA_num

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	fc_1553_NTqueue_report1destination_1DBA_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((fc_1553_NTqueue_report1destination_1DBA_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void fc_1553_NTqueue_report1destination_1DBA (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_fc_1553_NTqueue_report1destination_1DBA_init (int * init_block_ptr);
	void _op_fc_1553_NTqueue_report1destination_1DBA_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_fc_1553_NTqueue_report1destination_1DBA_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_fc_1553_NTqueue_report1destination_1DBA_alloc (VosT_Obtype, int);
	void _op_fc_1553_NTqueue_report1destination_1DBA_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
fc_1553_NTqueue_report1destination_1DBA (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (fc_1553_NTqueue_report1destination_1DBA ());

		{
		/* Temporary Variables */
		int strm_index;
		int data_unit_num;
		int data_queue_index;
		int i;
		int subqueue_pk_num;
		int maxdelay_sub_queue=0;
		//int destination_adress;
		int periodorburst;
		//double pksize;
		double max_pk_delay=0;
		
		double sub_queue_delay;
		double sub_queue_max_delay=0.0;
		int bit_number_NTqueue;
		Packet *pkptr;
		Packet *data_unit_pk;
		Packet *pkptr_rq;
		
		
		/* End of Temporary Variables */


		FSM_ENTER ("fc_1553_NTqueue_report1destination_1DBA")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "fc_1553_NTqueue_report1destination_1DBA [init enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_report1destination_1DBA [init enter execs]", state0_enter_exec)
				{
				
				
				//revised by yusy in 2012/05/04
				node_id=op_topo_parent(op_id_self());
				subnet_id=op_topo_parent(node_id);
				
				//node_num = op_topo_object_count(OPC_OBJTYPE_NODE_FIX);
				op_ima_obj_attr_get(node_id,"user id",&self_adress);
				
				op_ima_obj_attr_get(node_id,"Node Number",&node_num);
				
				op_ima_obj_attr_get(node_id,"Node cache size",&Node_cache_size);
				
				
				pkptr_destination_dist_handle = op_dist_load("uniform_int",0,31);
				
				
				
				//注册一个统计量，统计NT的合成流量
				node_traffic_stathandle 	= op_stat_reg ("node traffic.node Traffic Sent (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				//注册统计量统计队列大小随时间的变化、
				
				NT_queue_stathandle = op_stat_reg ("Queue static.NT queue static", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				
				pks_loss_queue=0;
				total_pks_generate=0;
				Total_data_size_in_queue=0.0;
				
				DBA_num=0;
				/*
				strcpy(Request_field[0],"Request_0");
				Request_field[1]="Request_1";
				Request_field[2]="Request_2";
				Request_field[3]="Request_3";
				Request_field[4]="Request_4";
				Request_field[5]="Request_5";
				Request_field[6]="Request_6";
				Request_field[7]="Request_7";
				Request_field[8]="Request_8";
				Request_field[9]="Request_9";
				Request_field[10]="Request_10";
				Request_field[11]="Request_11";
				Request_field[12]="Request_12";
				Request_field[13]="Request_13";
				Request_field[14]="Request_14";
				Request_field[15]="Request_15";
				Request_field[16]="Request_16";
				Request_field[17]="Request_17";
				Request_field[18]="Request_18";
				Request_field[19]="Request_19";
				Request_field[20]="Request_20";
				Request_field[21]="Request_21";
				Request_field[22]="Request_22";
				Request_field[23]="Request_23";
				Request_field[24]="Request_24";
				Request_field[25]="Request_25";
				Request_field[26]="Request_26";
				Request_field[27]="Request_27";
				Request_field[28]="Request_28";
				Request_field[29]="Request_29";
				Request_field[30]="Request_30";
				Request_field[31]="Request_31";
				
				*/
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "fc_1553_NTqueue_report1destination_1DBA [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_29", "fc_1553_NTqueue_report1destination_1DBA [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "fc_1553_NTqueue_report1destination_1DBA [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"fc_1553_NTqueue_report1destination_1DBA")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "fc_1553_NTqueue_report1destination_1DBA [idle exit execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_report1destination_1DBA [idle exit execs]", state1_exit_exec)
				{
				if(op_intrpt_type() == OPC_INTRPT_SELF)
					intrpt_code = op_intrpt_code();
				if(op_intrpt_type() == OPC_INTRPT_ACCESS)
					{
					input_strm_index=op_intrpt_strm();
					iciptr = op_intrpt_ici (); 
					if(op_ici_attr_get(iciptr,"Request_or_data",&ici_access_type) == OPC_COMPCODE_FAILURE)
						op_sim_end("err: unable to get the ici access type!","","","");
					
					}
				
				//testtime=op_sim_time();
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_report1destination_1DBA [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (ARRIVAL)
			FSM_TEST_COND (SIM_END)
			FSM_TEST_COND (SEND)
			FSM_TEST_COND (REQUEST)
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "ARRIVAL", "", "idle", "fifo_insert", "tr_31", "fc_1553_NTqueue_report1destination_1DBA [idle -> fifo_insert : ARRIVAL / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "SIM_END", "", "idle", "result", "tr_36", "fc_1553_NTqueue_report1destination_1DBA [idle -> result : SIM_END / ]")
				FSM_CASE_TRANSIT (2, 4, state4_enter_exec, ;, "SEND", "", "idle", "send_pk", "tr_44", "fc_1553_NTqueue_report1destination_1DBA [idle -> send_pk : SEND / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "REQUEST", "", "idle", "send_request", "tr_46", "fc_1553_NTqueue_report1destination_1DBA [idle -> send_request : REQUEST / ]")
				}
				/*---------------------------------------------------------*/



			/** state (fifo_insert) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "fifo_insert", state2_enter_exec, "fc_1553_NTqueue_report1destination_1DBA [fifo_insert enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_report1destination_1DBA [fifo_insert enter execs]", state2_enter_exec)
				{
				//use the maximum queue size to control the burst assembling;
				
				
				int pksize;
				strm_index = op_intrpt_strm();
				pkptr = op_pk_get(strm_index);//获取从数据源到达的数据包
				//pksize = op_pk_total_size_get (pkptr);
				//op_pk_nfd_get(pkptr,"destination",&destination_adress);
				
				//printf("NT generate !!!!!!!!!!!!!!!!!\n");
				
				total_pks_generate++;
				if(strm_index==0)
					{
					period_pks_generate++;
					}
				else
					{
					burst_pks_generate++;
					}
				
				if(strm_index==0)//流索引为0说明到达的数据为周期性业务，应插入放入队列号为32
					{
					if(Total_data_size_in_queue+ op_pk_total_size_get(pkptr) <= Node_cache_size )
						{
						Total_data_size_in_queue=Total_data_size_in_queue+ op_pk_total_size_get(pkptr);
						if(op_subq_pk_insert (32, pkptr, OPC_QPOS_TAIL)==OPC_QINS_FAIL)
							printf("NT this subqueue lost----%d---\n",self_adress);
						}
					else
						{
						op_pk_destroy(pkptr);
					
						//统计周期性丢包率
						period_loss_queue ++;
						}
					}
				
				
				if(strm_index!=0)//为突发业务
					{
					op_pk_nfd_get(pkptr,"destination",&destination_adress);
					pksize = 2112*8;
					op_stat_write (node_traffic_stathandle, 	pksize);
					op_stat_write (node_traffic_stathandle, 	0.0);
					
					
					if(Total_data_size_in_queue+ op_pk_total_size_get(pkptr) <= Node_cache_size)// 
						{
						Total_data_size_in_queue=Total_data_size_in_queue+ op_pk_total_size_get(pkptr);
						if(op_subq_pk_insert (destination_adress, pkptr, OPC_QPOS_TAIL)==OPC_QINS_FAIL)
							printf("this subqueue lost-----%d----\n",destination_adress);
						}
					else
						{
						op_pk_destroy(pkptr);
						burst_loss_queue++;
						op_sim_end("NT lost","","","");
						//printf("Total_data_size_in_queue:%f----NT%d-----\n",Total_data_size_in_queue,self_adress);
						}
					}
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (fifo_insert) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "fifo_insert", "fc_1553_NTqueue_report1destination_1DBA [fifo_insert exit execs]")


			/** state (fifo_insert) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "fifo_insert", "idle", "tr_32", "fc_1553_NTqueue_report1destination_1DBA [fifo_insert -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (result) enter executives **/
			FSM_STATE_ENTER_UNFORCED (3, "result", state3_enter_exec, "fc_1553_NTqueue_report1destination_1DBA [result enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_report1destination_1DBA [result enter execs]", state3_enter_exec)
				{
				//仿真时间结束时打印每一个子队列的长度
				//	printf("the PK number ");
				
				/*
					for(i=0;i<=32;i++)
					{
					subqueue_pk_num = op_subq_stat (i, OPC_QSTAT_PKSIZE);
					if(subqueue_pk_num!=0)
					    printf("NT%dthe PK number in %d subqueue is %d \n ",self_adress,i,subqueue_pk_num);
					}
				
				*/
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (7,"fc_1553_NTqueue_report1destination_1DBA")


			/** state (result) exit executives **/
			FSM_STATE_EXIT_UNFORCED (3, "result", "fc_1553_NTqueue_report1destination_1DBA [result exit execs]")


			/** state (result) transition processing **/
			FSM_TRANSIT_MISSING ("result")
				/*---------------------------------------------------------*/



			/** state (send_pk) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "send_pk", state4_enter_exec, "fc_1553_NTqueue_report1destination_1DBA [send_pk enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_report1destination_1DBA [send_pk enter execs]", state4_enter_exec)
				{
				//command has been made to the assemble module to get a OB;
				iciptr = op_intrpt_ici (); 
				op_ici_attr_get(iciptr,"data_size2NTprocess",&data_unit_num);
				//printf("------data_unit_num:%d------\n",data_unit_num);
				op_ici_attr_get(iciptr,"queue_index",&data_queue_index);
				op_ici_attr_get(iciptr,"period_or_burst",&periodorburst);
				op_ici_destroy(iciptr);
				
				
				if(periodorburst==1)
					{
					data_unit_pk = op_subq_pk_remove(32,OPC_QPOS_HEAD);
					Total_data_size_in_queue=Total_data_size_in_queue-op_pk_total_size_get(data_unit_pk);
					op_pk_send_quiet(data_unit_pk,2);
					
					}
				
				
				if(periodorburst==0)
					{
					for(i=1;i<=data_unit_num;i++)
					  {
					    data_unit_pk = op_subq_pk_remove(data_queue_index,OPC_QPOS_HEAD);
						Total_data_size_in_queue=Total_data_size_in_queue-op_pk_total_size_get(data_unit_pk);
					    op_pk_send_quiet(data_unit_pk,0);
						
				
					  }
					}
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (send_pk) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "send_pk", "fc_1553_NTqueue_report1destination_1DBA [send_pk exit execs]")


			/** state (send_pk) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send_pk", "idle", "tr_45", "fc_1553_NTqueue_report1destination_1DBA [send_pk -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (send_request) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "send_request", state5_enter_exec, "fc_1553_NTqueue_report1destination_1DBA [send_request enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_report1destination_1DBA [send_request enter execs]", state5_enter_exec)
				{
				//command has been made to the assemble module to report node bandwidth request;
				
				
				//char *Request_field[] ={"Request_0","Request_1"};
				char *Request_field[]={"Request_1","Request_2","Request_3","Request_4","Request_5","Request_6","Request_7","Request_8","Request_9","Request_10","Request_11","Request_12","Request_13","Request_14","Request_15","Request_16","Request_17","Request_18","Request_19","Request_20","Request_21","Request_22","Request_23","Request_24","Request_25","Request_26","Request_27","Request_28","Request_29","Request_30","Request_31","Request_32"};
				
				pkptr_rq = op_pk_create_fmt("fc_1553_Requestreport_frame_format");
				
				DBA_num=DBA_num+1;
				
				
				//上报请求，每个DBA上报队头包时延最大的那个队列
				//****上报时延最大的那个队列****
				   for(i=0;i<=31;i++)
					 {
					   if(op_subq_empty(i)==OPC_FALSE)
						 {
						   pkptr = op_subq_pk_access(i,OPC_QPOS_HEAD);
						   sub_queue_delay = op_sim_time() - op_pk_creation_time_get(pkptr);
						   if(sub_queue_delay>sub_queue_max_delay)
							{
							maxdelay_sub_queue = i;
							sub_queue_max_delay = sub_queue_delay;
							}
						 }
					 }
				
					subqueue_pk_num = op_subq_stat(maxdelay_sub_queue,OPC_QSTAT_PKSIZE);
					
						
					
					op_pk_nfd_set (pkptr_rq, Request_field[maxdelay_sub_queue], subqueue_pk_num);
					
					op_pk_send_quiet(pkptr_rq,1);
					
				
				op_ici_destroy(iciptr);
				
				//统计NT的队列长度
					
				    bit_number_NTqueue=0;
					for(i=0;i<33;i++)
						{
						bit_number_NTqueue = bit_number_NTqueue+op_subq_stat (i, OPC_QSTAT_BITSIZE);
						}
					
					//printf("bit_number_NTqueue:%f----\n",bit_number_NTqueue);
					op_stat_write (NT_queue_stathandle, bit_number_NTqueue);
					
				
				
				/*burst_source_scenario=0为多节点情形，数据源一个DBA只产生去往一个目的地址的数据,
				每个DBA上报请求时决定队列中的数据的地址，并将队列中的数据包移动到子队列1，当前DBA周期子队列1中的数据为完全传输时，
				下一个DBA的上报子队列1中未传输的数据包数*/
				/*
				if(node_num>=11)
					{
					
					if(op_subq_empty (1)==OPC_FALSE )//说明上一个DBA周期上报的请求未全部传输完，因此需要重新上报未传输完成的数据包（即子队列1中的数据）的个数
						{
						subqueue_pk_num = op_subq_stat(1,OPC_QSTAT_PKSIZE);
				        op_pk_nfd_set (pkptr_rq, Request_field[pkptr_destination], subqueue_pk_num);//
						//op_pk_nfd_set (pkptr_rq, "D_0", pkptr_destination);
					    op_pk_send_quiet(pkptr_rq,1);
						//printf("the-----%d's---- destination is %d \n",self_adress,pkptr_destination);
						}
				    if(op_subq_empty (1)==OPC_TRUE)
						{
						pkptr_destination = op_dist_outcome (pkptr_destination_dist_handle);
						while(pkptr_destination==self_adress)//如果随机产生的一个地址是NT本身，则需要重新产生一个，知道产生成功
							{
							pkptr_destination = op_dist_outcome (pkptr_destination_dist_handle);
							}
						subqueue_pk_num = op_subq_stat(0,OPC_QSTAT_PKSIZE);
						op_pk_nfd_set (pkptr_rq,  Request_field[pkptr_destination], subqueue_pk_num);
						//op_pk_nfd_set (pkptr_rq, "D_0", pkptr_destination);
						op_pk_send_quiet(pkptr_rq,1);
						//将子队列0中的包一个个移除，添加地址后，插入子队列1
					    for(i=1;i<=subqueue_pk_num;i++)
					      {
					       source_pkptr = op_subq_pk_remove(0,OPC_QPOS_HEAD);
						  // op_pk_destroy (source_pkptr);
						   op_pk_nfd_set (source_pkptr, "destination", pkptr_destination);
					       op_subq_pk_insert (1, source_pkptr, OPC_QPOS_TAIL);
					      }
						}
					
					 
					 }
				*/
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (send_request) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "send_request", "fc_1553_NTqueue_report1destination_1DBA [send_request exit execs]")


			/** state (send_request) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send_request", "idle", "tr_47", "fc_1553_NTqueue_report1destination_1DBA [send_request -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"fc_1553_NTqueue_report1destination_1DBA")
		}
	}




void
_op_fc_1553_NTqueue_report1destination_1DBA_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_fc_1553_NTqueue_report1destination_1DBA_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_fc_1553_NTqueue_report1destination_1DBA_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_fc_1553_NTqueue_report1destination_1DBA_svar function. */
#undef node_id
#undef subnet_id
#undef Total_received_packets_handle
#undef Total_lossed_packets_handle
#undef Total_loss_rate_handle
#undef intrpt_code
#undef input_strm_index
#undef iciptr
#undef ici_access_type
#undef packet_generate_global
#undef packet_lost_global
#undef burst1_bitssec_handle
#undef burst2_bitssec_handle
#undef burst3_bitssec_handle
#undef node_num
#undef destination_adress
#undef pkptr_destination
#undef source_pkptr
#undef pkptr_destination_dist_handle
#undef self_adress
#undef node_traffic_stathandle
#undef Total_data_size_in_queue
#undef Node_cache_size
#undef NT_queue_stathandle
#undef DBA_num

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_fc_1553_NTqueue_report1destination_1DBA_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_fc_1553_NTqueue_report1destination_1DBA_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (fc_1553_NTqueue_report1destination_1DBA)",
		sizeof (fc_1553_NTqueue_report1destination_1DBA_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_fc_1553_NTqueue_report1destination_1DBA_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	fc_1553_NTqueue_report1destination_1DBA_state * ptr;
	FIN_MT (_op_fc_1553_NTqueue_report1destination_1DBA_alloc (obtype))

	ptr = (fc_1553_NTqueue_report1destination_1DBA_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "fc_1553_NTqueue_report1destination_1DBA [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_fc_1553_NTqueue_report1destination_1DBA_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	fc_1553_NTqueue_report1destination_1DBA_state		*prs_ptr;

	FIN_MT (_op_fc_1553_NTqueue_report1destination_1DBA_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (fc_1553_NTqueue_report1destination_1DBA_state *)gen_ptr;

	if (strcmp ("node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_id);
		FOUT
		}
	if (strcmp ("subnet_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->subnet_id);
		FOUT
		}
	if (strcmp ("Total_received_packets_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Total_received_packets_handle);
		FOUT
		}
	if (strcmp ("Total_lossed_packets_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Total_lossed_packets_handle);
		FOUT
		}
	if (strcmp ("Total_loss_rate_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Total_loss_rate_handle);
		FOUT
		}
	if (strcmp ("intrpt_code" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->intrpt_code);
		FOUT
		}
	if (strcmp ("input_strm_index" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->input_strm_index);
		FOUT
		}
	if (strcmp ("iciptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->iciptr);
		FOUT
		}
	if (strcmp ("ici_access_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ici_access_type);
		FOUT
		}
	if (strcmp ("packet_generate_global" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packet_generate_global);
		FOUT
		}
	if (strcmp ("packet_lost_global" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packet_lost_global);
		FOUT
		}
	if (strcmp ("burst1_bitssec_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->burst1_bitssec_handle);
		FOUT
		}
	if (strcmp ("burst2_bitssec_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->burst2_bitssec_handle);
		FOUT
		}
	if (strcmp ("burst3_bitssec_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->burst3_bitssec_handle);
		FOUT
		}
	if (strcmp ("node_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_num);
		FOUT
		}
	if (strcmp ("destination_adress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->destination_adress);
		FOUT
		}
	if (strcmp ("pkptr_destination" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkptr_destination);
		FOUT
		}
	if (strcmp ("source_pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->source_pkptr);
		FOUT
		}
	if (strcmp ("pkptr_destination_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkptr_destination_dist_handle);
		FOUT
		}
	if (strcmp ("self_adress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->self_adress);
		FOUT
		}
	if (strcmp ("node_traffic_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_traffic_stathandle);
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
	if (strcmp ("NT_queue_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NT_queue_stathandle);
		FOUT
		}
	if (strcmp ("DBA_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->DBA_num);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

