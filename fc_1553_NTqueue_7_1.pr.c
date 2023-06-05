/* Process model C form file: fc_1553_NTqueue_7_1.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char fc_1553_NTqueue_7_1_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 55CD931D 55CD931D 1 dell-PC dell 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                           ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#define REQUEST_TYPE		0
#define DATA_TYPE			1

#define ARRIVAL    	 (op_intrpt_type() == OPC_INTRPT_STRM)
#define SEND	     (op_intrpt_type () == OPC_INTRPT_ACCESS && ici_access_type ==DATA_TYPE)
#define REQUEST	     (op_intrpt_type () == OPC_INTRPT_ACCESS && ici_access_type ==REQUEST_TYPE)
#define SIM_END		 (op_intrpt_type() == OPC_INTRPT_ENDSIM)



extern long packet_lost_global;
extern long packet_generate_global;




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
	} fc_1553_NTqueue_7_1_state;

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

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	fc_1553_NTqueue_7_1_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((fc_1553_NTqueue_7_1_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void fc_1553_NTqueue_7_1 (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_fc_1553_NTqueue_7_1_init (int * init_block_ptr);
	void _op_fc_1553_NTqueue_7_1_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_fc_1553_NTqueue_7_1_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_fc_1553_NTqueue_7_1_alloc (VosT_Obtype, int);
	void _op_fc_1553_NTqueue_7_1_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
fc_1553_NTqueue_7_1 (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (fc_1553_NTqueue_7_1 ());

		{
		/* Temporary Variables */
		int strm_index;
		int data_unit_num;
		int data_queue_index;
		int i;
		int subqueue_pk_num;
		int destination_adress;
		
		double pksize;
		
		Packet *pkptr;
		Packet *data_unit_pk;
		Packet *pkptr_rq;
		
		
		/* End of Temporary Variables */


		FSM_ENTER ("fc_1553_NTqueue_7_1")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "init", "fc_1553_NTqueue_7_1 [init enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_7_1 [init enter execs]", state0_enter_exec)
				{
				
				
				//revised by yusy in 2012/05/04
				node_id=op_topo_parent(op_id_self());
				subnet_id=op_topo_parent(node_id);
				
				
				
				//statistical，我们只需要在队列模块统计丢包率
				Total_received_packets_handle = op_stat_reg("Global Net.Total received packets",OPC_STAT_INDEX_NONE,	OPC_STAT_GLOBAL);  //统计总的接收到的数据包数量
				Total_lossed_packets_handle = op_stat_reg("Global Net.Total lossed packets",OPC_STAT_INDEX_NONE,	OPC_STAT_GLOBAL);  //统计总的丢弃的数据包数量
				Total_loss_rate_handle = op_stat_reg("Global Net.Total loss rate",OPC_STAT_INDEX_NONE,	OPC_STAT_GLOBAL);  //统计丢包率
				
				
				
				
				//测试两个ON/OFF源设计的两个统计量
				burst1_bitssec_handle = op_stat_reg("burst1 ONOFF.burst1 bitssec handle",OPC_STAT_INDEX_NONE,	OPC_STAT_LOCAL);  
				burst2_bitssec_handle = op_stat_reg("burst2 ONOFF.burst2 bitssec handle",OPC_STAT_INDEX_NONE,	OPC_STAT_LOCAL);  
				burst3_bitssec_handle = op_stat_reg("burst3 ONOFF.burst3 bitssec handle",OPC_STAT_INDEX_NONE,	OPC_STAT_LOCAL);  
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "init", "fc_1553_NTqueue_7_1 [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_29", "fc_1553_NTqueue_7_1 [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "fc_1553_NTqueue_7_1 [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"fc_1553_NTqueue_7_1")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "fc_1553_NTqueue_7_1 [idle exit execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_7_1 [idle exit execs]", state1_exit_exec)
				{
				if(op_intrpt_type() == OPC_INTRPT_SELF)
					intrpt_code = op_intrpt_code();
				if(op_intrpt_type() == OPC_INTRPT_ACCESS)
					{
					input_strm_index=op_intrpt_strm();
					iciptr = op_intrpt_ici (); 
					if(op_ici_attr_get(iciptr,"Request_or_data",&ici_access_type) == OPC_COMPCODE_FAILURE)
						op_sim_end("err: unable to get the ici access type!","","","");
					//
					}
				//testtime=op_sim_time();
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_7_1 [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (ARRIVAL)
			FSM_TEST_COND (SIM_END)
			FSM_TEST_COND (SEND)
			FSM_TEST_COND (REQUEST)
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "ARRIVAL", "", "idle", "fifo_insert", "tr_31", "fc_1553_NTqueue_7_1 [idle -> fifo_insert : ARRIVAL / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "SIM_END", "", "idle", "result", "tr_36", "fc_1553_NTqueue_7_1 [idle -> result : SIM_END / ]")
				FSM_CASE_TRANSIT (2, 4, state4_enter_exec, ;, "SEND", "", "idle", "send_pk", "tr_44", "fc_1553_NTqueue_7_1 [idle -> send_pk : SEND / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "REQUEST", "", "idle", "send_request", "tr_46", "fc_1553_NTqueue_7_1 [idle -> send_request : REQUEST / ]")
				}
				/*---------------------------------------------------------*/



			/** state (fifo_insert) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "fifo_insert", state2_enter_exec, "fc_1553_NTqueue_7_1 [fifo_insert enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_7_1 [fifo_insert enter execs]", state2_enter_exec)
				{
				//use the maximum queue size to control the burst assembling;
				
				strm_index = op_intrpt_strm();
				pkptr = op_pk_get(strm_index);//获取从数据源到达的数据包
				pksize = op_pk_total_size_get (pkptr);
				op_pk_nfd_get(pkptr,"destination",&destination_adress);
				
				/*想在这里统计两个ON/OFF源产生的数据,只为测量数据源*/
				
				if(destination_adress==0)
					{
					op_stat_write (burst1_bitssec_handle, 	pksize);
					op_stat_write (burst1_bitssec_handle, 	0.0);
					
					}
				if(destination_adress==1)
					{
					op_stat_write (burst2_bitssec_handle, 	pksize);
					op_stat_write (burst2_bitssec_handle, 	0.0);
					}
				if(destination_adress==2)
					{
					op_stat_write (burst3_bitssec_handle, 	pksize);
					op_stat_write (burst3_bitssec_handle, 	0.0);
					}
				
				packet_generate_global++;
				if(strm_index==0)//流索引为0说明到达的数据为周期性业务，应插入放入队列号为32
					{
					if(op_subq_pk_insert (32, pkptr, OPC_QPOS_TAIL)==OPC_QINS_FAIL)
					  {
					   op_pk_destroy(pkptr);
					   packet_lost_global++;
					  }
					}
				else//到达的数据为突发业务，插入的队列号与数据包的目的地址相对应
					{
					if(op_subq_pk_insert (destination_adress, pkptr, OPC_QPOS_TAIL)==OPC_QINS_FAIL)
					  {
					   op_pk_destroy(pkptr);
					   packet_lost_global++;
					  }
					}
				
				
				
				
				
				
				
				//统计数据数据源产生的总包数和丢失的总包数
				
				op_stat_write (Total_received_packets_handle, 	packet_generate_global);
				op_stat_write (Total_lossed_packets_handle, 	packet_lost_global);
				op_stat_write (Total_loss_rate_handle, 	(double)packet_lost_global/(double)packet_generate_global);
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (fifo_insert) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "fifo_insert", "fc_1553_NTqueue_7_1 [fifo_insert exit execs]")


			/** state (fifo_insert) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "fifo_insert", "idle", "tr_32", "fc_1553_NTqueue_7_1 [fifo_insert -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (result) enter executives **/
			FSM_STATE_ENTER_UNFORCED (3, "result", state3_enter_exec, "fc_1553_NTqueue_7_1 [result enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (7,"fc_1553_NTqueue_7_1")


			/** state (result) exit executives **/
			FSM_STATE_EXIT_UNFORCED (3, "result", "fc_1553_NTqueue_7_1 [result exit execs]")


			/** state (result) transition processing **/
			FSM_TRANSIT_MISSING ("result")
				/*---------------------------------------------------------*/



			/** state (send_pk) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "send_pk", state4_enter_exec, "fc_1553_NTqueue_7_1 [send_pk enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_7_1 [send_pk enter execs]", state4_enter_exec)
				{
				//command has been made to the assemble module to get a OB;
				iciptr = op_intrpt_ici (); 
				op_ici_attr_get(iciptr,"data_size2NTprocess",&data_unit_num);
				op_ici_attr_get(iciptr,"queue_index",&data_queue_index);
				op_ici_destroy(iciptr);
				for(i=1;i<=data_unit_num;i++)
					{
					data_unit_pk = op_subq_pk_remove(data_queue_index,OPC_QPOS_HEAD);
					op_pk_send_quiet(data_unit_pk,0);
					}
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (send_pk) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "send_pk", "fc_1553_NTqueue_7_1 [send_pk exit execs]")


			/** state (send_pk) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send_pk", "idle", "tr_45", "fc_1553_NTqueue_7_1 [send_pk -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (send_request) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "send_request", state5_enter_exec, "fc_1553_NTqueue_7_1 [send_request enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTqueue_7_1 [send_request enter execs]", state5_enter_exec)
				{
				//command has been made to the assemble module to report node bandwidth request;
				pkptr_rq = op_pk_create_fmt("fc_1553_Requestreport_frame_format");
				
				
				subqueue_pk_num = op_subq_stat(0,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_1", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_1", 1);
				//printf("reques to NC %d\n",subqueue_pk_num);
				
				
				subqueue_pk_num = op_subq_stat(1,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_2", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_2", 2);
				
				
				subqueue_pk_num = op_subq_stat(2,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_3", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_3", 3);
				
				subqueue_pk_num = op_subq_stat(3,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_4", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_4", 4);
				
				subqueue_pk_num = op_subq_stat(4,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_5", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_5", 5);
				
				subqueue_pk_num = op_subq_stat(5,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_6", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_6", 6);
				
				subqueue_pk_num = op_subq_stat(6,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_7", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_7", 7);
				
				subqueue_pk_num = op_subq_stat(7,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_8", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_8", 8);
				
				subqueue_pk_num = op_subq_stat(8,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_9", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_9", 9);
				
				subqueue_pk_num = op_subq_stat(9,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_10", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_10", 10);
				
				subqueue_pk_num = op_subq_stat(10,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_11", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_11", 11);
				
				subqueue_pk_num = op_subq_stat(11,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_12", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_12", 12);
				
				subqueue_pk_num = op_subq_stat(12,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_13", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_13", 13);
				
				subqueue_pk_num = op_subq_stat(13,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_14", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_14", 14);
				
				subqueue_pk_num = op_subq_stat(14,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_15", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_15", 15);
				
				subqueue_pk_num = op_subq_stat(15,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_16", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_16", 16);
				
				subqueue_pk_num = op_subq_stat(16,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_17", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_17", 17);
				
				subqueue_pk_num = op_subq_stat(17,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_18", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_18", 18);
				
				subqueue_pk_num = op_subq_stat(18,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_19", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_19", 19);
				
				subqueue_pk_num = op_subq_stat(19,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_20", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_20", 20);
				
				subqueue_pk_num = op_subq_stat(20,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_21", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_21", 21);
				
				subqueue_pk_num = op_subq_stat(21,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_22", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_22", 22);
				
				subqueue_pk_num = op_subq_stat(22,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_23", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_23",23);
				
				subqueue_pk_num = op_subq_stat(23,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_24", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_24", 24);
				
				subqueue_pk_num = op_subq_stat(24,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_25", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_25", 25);
				
				
				
				subqueue_pk_num = op_subq_stat(25,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_26", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_26", 26);
				
				subqueue_pk_num = op_subq_stat(26,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_27", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_27", 27);
				
				
				
				subqueue_pk_num = op_subq_stat(27,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_28", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_28", 28);
				
				
				
				subqueue_pk_num = op_subq_stat(28,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_29", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_29", 29);
				
				
				
				subqueue_pk_num = op_subq_stat(29,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_30", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_30", 30);
				
				
				subqueue_pk_num = op_subq_stat(30,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_31", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_31", 31);
				
				
				subqueue_pk_num = op_subq_stat(31,OPC_QSTAT_PKSIZE);
				op_pk_nfd_set (pkptr_rq, "Request_32", subqueue_pk_num);
				op_pk_nfd_set (pkptr_rq, "D_32", 32);
				
				
				op_pk_send_quiet(pkptr_rq,0);
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (send_request) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "send_request", "fc_1553_NTqueue_7_1 [send_request exit execs]")


			/** state (send_request) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send_request", "idle", "tr_47", "fc_1553_NTqueue_7_1 [send_request -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"fc_1553_NTqueue_7_1")
		}
	}




void
_op_fc_1553_NTqueue_7_1_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_fc_1553_NTqueue_7_1_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_fc_1553_NTqueue_7_1_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_fc_1553_NTqueue_7_1_svar function. */
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

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_fc_1553_NTqueue_7_1_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_fc_1553_NTqueue_7_1_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (fc_1553_NTqueue_7_1)",
		sizeof (fc_1553_NTqueue_7_1_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_fc_1553_NTqueue_7_1_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	fc_1553_NTqueue_7_1_state * ptr;
	FIN_MT (_op_fc_1553_NTqueue_7_1_alloc (obtype))

	ptr = (fc_1553_NTqueue_7_1_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "fc_1553_NTqueue_7_1 [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_fc_1553_NTqueue_7_1_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	fc_1553_NTqueue_7_1_state		*prs_ptr;

	FIN_MT (_op_fc_1553_NTqueue_7_1_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (fc_1553_NTqueue_7_1_state *)gen_ptr;

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
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

