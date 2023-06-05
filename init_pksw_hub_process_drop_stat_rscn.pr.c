/* Process model C form file: init_pksw_hub_process_drop_stat_rscn.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char init_pksw_hub_process_drop_stat_rscn_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 641ABE74 641ABE74 1 LAPTOP-45T4L382 12085 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include "oms_pr.h"
#include "oms_tan.h"
#include "oms_bgutil.h"
#include "oms_protocol.h"
#include "oms_pipeline.h"
#include "oms_auto_addr_support.h"
#include "oms_pkt_analyzer.h"
//#include "oms_vlan_support.h"
#include "oms_data_def.h"
#include "oms_rr.h"

#include <hsrp.h>
#include <fc_test_global.h>
    

#define TRANSMIT_INSTAT			0

#define RSCN_TIME     1 

#define PK_ARRVL (op_intrpt_type () == OPC_INTRPT_STRM)

#define XMIT_UP	(op_intrpt_type() == OPC_INTRPT_STAT)
							 

#define SIM_END		 (op_intrpt_type() == OPC_INTRPT_ENDSIM)

#define RSCN_SEND  (op_intrpt_type()==OPC_INTRPT_SELF&& op_intrpt_code () == RSCN_TIME)


#define gen_time 0


extern int rscn_num;




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
	Stathandle	             		ete1_gsh                                        ;
	double	                 		sim_time                                        ;
	int	                    		transmit_drop                                   ;
	Distribution *	         		add_frame                                       ;
	Distribution *	         		judge_frame                                     ;
	} init_pksw_hub_process_drop_stat_rscn_state;

#define ete1_gsh                		op_sv_ptr->ete1_gsh
#define sim_time                		op_sv_ptr->sim_time
#define transmit_drop           		op_sv_ptr->transmit_drop
#define add_frame               		op_sv_ptr->add_frame
#define judge_frame             		op_sv_ptr->judge_frame

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	init_pksw_hub_process_drop_stat_rscn_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((init_pksw_hub_process_drop_stat_rscn_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static void route_pk(void)
		{
		int dest_address;
		int i;
		int received_frame_type;
		
		double ete1_delay;
		double pk_create_time;
		double rscn_delay;
		Packet * pkptr;
		Packet * cp_pkptr;
		//int received_frame_type;
		//int received_frame_traffic_type;
		
		
		FIN(route_pk());
		/* receive a frame from upper layer */
		pkptr = op_pk_get(op_intrpt_strm ());
		pk_create_time = op_pk_creation_time_get(pkptr);
		
		op_pk_nfd_get_int32 (pkptr, "D_ID",&dest_address);
		op_pk_nfd_get(pkptr,"Frame_type",&received_frame_type);
		

		
		if(received_frame_type==5)
			{
			rscn_delay = op_sim_time () - op_pk_creation_time_get (pkptr);
			if (rscn_delay > rscn_max_delay)
				{
				rscn_max_delay = rscn_delay;
				}
		
			rscn_receive++;
			op_pk_destroy (pkptr);
			}
		else
			{
			
			if(dest_address ==32 )//接收到的为NC发送的广播帧，
		 
				{
				/* copy and send to all port */
				for (i = 1; i < 32; i++)//分别复制发送给1~31个NT
					{
					cp_pkptr = op_pk_copy (pkptr);
					op_pk_creation_time_set(cp_pkptr, pk_create_time);
					ete1_delay = op_sim_time () - op_pk_creation_time_get (pkptr);
					op_stat_write (ete1_gsh, ete1_delay);
					op_pk_send (cp_pkptr, i);
		   
					}
				op_pk_destroy(pkptr);
				}
		
			else
				{
          
				ete1_delay = op_sim_time () - op_pk_creation_time_get (pkptr);
				op_stat_write (ete1_gsh, ete1_delay);	
				op_pk_send (pkptr,dest_address);
				}
			
			}
		
		
		
		
		  
		 /*多次发送一个包是不可以的，必须先复制，才能发送，也就是说一个包指针只能用做一次发送
		
		 
		  op_pk_send (pkptr, 0);
		  pkptr2 = op_pk_copy (pkptr1);
		  op_pk_send (pkptr1, 1);
		  pkptr3 = op_pk_copy (pkptr2);
		  op_pk_send (pkptr2, 2);
		  op_pk_send (pkptr3, 3);*/
		
		FOUT;
		}






/* End of Function Block */

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
	void init_pksw_hub_process_drop_stat_rscn (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_init_pksw_hub_process_drop_stat_rscn_init (int * init_block_ptr);
	void _op_init_pksw_hub_process_drop_stat_rscn_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_init_pksw_hub_process_drop_stat_rscn_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_init_pksw_hub_process_drop_stat_rscn_alloc (VosT_Obtype, int);
	void _op_init_pksw_hub_process_drop_stat_rscn_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
init_pksw_hub_process_drop_stat_rscn (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (init_pksw_hub_process_drop_stat_rscn ());

		{
		/* Temporary Variables */
		Packet  *CommondFrame_ptr;
		Packet  *copy1;
		//Packet  *copy2;
		
		int dest = 0;
		
		double time= 0.0;
		//double rscn_delay;
		
		int Logi_node_num ;
		int i;
		/* End of Temporary Variables */


		FSM_ENTER ("init_pksw_hub_process_drop_stat_rscn")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (0, "idle", state0_enter_exec, "init_pksw_hub_process_drop_stat_rscn [idle enter execs]")
				FSM_PROFILE_SECTION_IN ("init_pksw_hub_process_drop_stat_rscn [idle enter execs]", state0_enter_exec)
				{
				/*
				time = op_sim_time();
				
				value = (int)(time);
				
				//printf("the time is %d\n",value);
				
				if(time == 10 )
					{
						printf("this is rscn ------\n");
						op_intrpt_schedule_self(op_sim_time()+gen_time, RSCN_TIME);
					}
				*/
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"init_pksw_hub_process_drop_stat_rscn")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "idle", "init_pksw_hub_process_drop_stat_rscn [idle exit execs]")


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("init_pksw_hub_process_drop_stat_rscn [idle trans conditions]", state0_trans_conds)
			FSM_INIT_COND (PK_ARRVL)
			FSM_TEST_COND (SIM_END)
			FSM_TEST_COND (XMIT_UP)
			FSM_TEST_COND (RSCN_SEND)
			FSM_DFLT_COND
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state0_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 0, state0_enter_exec, route_pk();, "PK_ARRVL", "route_pk()", "idle", "idle", "tr_0", "init_pksw_hub_process_drop_stat_rscn [idle -> idle : PK_ARRVL / route_pk()]")
				FSM_CASE_TRANSIT (1, 1, state1_enter_exec, ;, "SIM_END", "", "idle", "result", "tr_3", "init_pksw_hub_process_drop_stat_rscn [idle -> result : SIM_END / ]")
				FSM_CASE_TRANSIT (2, 2, state2_enter_exec, ;, "XMIT_UP", "", "idle", "drop_stat", "tr_6", "init_pksw_hub_process_drop_stat_rscn [idle -> drop_stat : XMIT_UP / ]")
				FSM_CASE_TRANSIT (3, 4, state4_enter_exec, ;, "RSCN_SEND", "", "idle", "RSCN_PRO", "tr_9", "init_pksw_hub_process_drop_stat_rscn [idle -> RSCN_PRO : RSCN_SEND / ]")
				FSM_CASE_TRANSIT (4, 0, state0_enter_exec, ;, "default", "", "idle", "idle", "tr_12", "init_pksw_hub_process_drop_stat_rscn [idle -> idle : default / ]")
				}
				/*---------------------------------------------------------*/



			/** state (result) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "result", state1_enter_exec, "init_pksw_hub_process_drop_stat_rscn [result enter execs]")
				FSM_PROFILE_SECTION_IN ("init_pksw_hub_process_drop_stat_rscn [result enter execs]", state1_enter_exec)
				{
				sim_time = op_sim_time();
				printf("the sw drop pks %d\n",transmit_drop);
				printf("the sim time is %f\n",sim_time);
				
				printf("the rscn num is %d\n",rscn_num);
				printf("the RSCN delay :%f\n",rscn_max_delay);
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"init_pksw_hub_process_drop_stat_rscn")


			/** state (result) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "result", "init_pksw_hub_process_drop_stat_rscn [result exit execs]")


			/** state (result) transition processing **/
			FSM_TRANSIT_MISSING ("result")
				/*---------------------------------------------------------*/



			/** state (drop_stat) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "drop_stat", state2_enter_exec, "init_pksw_hub_process_drop_stat_rscn [drop_stat enter execs]")
				FSM_PROFILE_SECTION_IN ("init_pksw_hub_process_drop_stat_rscn [drop_stat enter execs]", state2_enter_exec)
				{
				//printf("######## %f\n #########",op_stat_local_read (op_intrpt_stat()));
				if(op_stat_local_read (op_intrpt_stat()) == 1.0)
				{
				printf(" the SW lost packet in --------%f------\n",op_sim_time());
				transmit_drop ++;
				}	
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (drop_stat) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "drop_stat", "init_pksw_hub_process_drop_stat_rscn [drop_stat exit execs]")


			/** state (drop_stat) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "drop_stat", "idle", "tr_7", "init_pksw_hub_process_drop_stat_rscn [drop_stat -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (3, "init", "init_pksw_hub_process_drop_stat_rscn [init enter execs]")
				FSM_PROFILE_SECTION_IN ("init_pksw_hub_process_drop_stat_rscn [init enter execs]", state3_enter_exec)
				{
				ete1_gsh = op_stat_reg ("ETE1_Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
				//trans_drop = op_stat_reg ("TRANS_DROP", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				transmit_drop = 0;
				
				rscn_num = 0;
				
				op_intrpt_schedule_self(op_sim_time()+0, RSCN_TIME);
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (init) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "init", "init_pksw_hub_process_drop_stat_rscn [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "init", "idle", "tr_8", "init_pksw_hub_process_drop_stat_rscn [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (RSCN_PRO) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "RSCN_PRO", state4_enter_exec, "init_pksw_hub_process_drop_stat_rscn [RSCN_PRO enter execs]")
				FSM_PROFILE_SECTION_IN ("init_pksw_hub_process_drop_stat_rscn [RSCN_PRO enter execs]", state4_enter_exec)
				{
				add_frame = op_dist_load ("uniform_int", 1,5);
				
				Logi_node_num = (int)op_dist_outcome (add_frame);
				
				CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
				op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 5);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
				op_pk_nfd_set (CommondFrame_ptr, "D_ID", dest);//
				//copy1=op_pk_copy(CommondFrame_ptr);
				//copy2=op_pk_copy(CommondFrame_ptr);
				for(i=0;i<Logi_node_num;i++)
				{
					for(dest=0;dest<10;dest++)
						{
						copy1=op_pk_copy(CommondFrame_ptr);
						op_pk_send (copy1, dest);
						
						}
					
				}
				op_pk_destroy (CommondFrame_ptr);
				//op_pk_send (copy1, dest);
				//op_pk_send (copy2, dest);
				
				
				
				//printf("the next rscn time is %f\n",time);
				time=op_dist_exponential(0.1);
				op_intrpt_schedule_self(op_sim_time()+time, RSCN_TIME);//
				//printf("RSCN time:%f-----\n",time);
				rscn_num++;
				
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (RSCN_PRO) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "RSCN_PRO", "init_pksw_hub_process_drop_stat_rscn [RSCN_PRO exit execs]")


			/** state (RSCN_PRO) transition processing **/
			FSM_TRANSIT_FORCE (0, state0_enter_exec, ;, "default", "", "RSCN_PRO", "idle", "tr_13", "init_pksw_hub_process_drop_stat_rscn [RSCN_PRO -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (3,"init_pksw_hub_process_drop_stat_rscn")
		}
	}




void
_op_init_pksw_hub_process_drop_stat_rscn_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_init_pksw_hub_process_drop_stat_rscn_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_init_pksw_hub_process_drop_stat_rscn_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_init_pksw_hub_process_drop_stat_rscn_svar function. */
#undef ete1_gsh
#undef sim_time
#undef transmit_drop
#undef add_frame
#undef judge_frame

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_init_pksw_hub_process_drop_stat_rscn_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_init_pksw_hub_process_drop_stat_rscn_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (init_pksw_hub_process_drop_stat_rscn)",
		sizeof (init_pksw_hub_process_drop_stat_rscn_state));
	*init_block_ptr = 6;

	FRET (obtype)
	}

VosT_Address
_op_init_pksw_hub_process_drop_stat_rscn_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	init_pksw_hub_process_drop_stat_rscn_state * ptr;
	FIN_MT (_op_init_pksw_hub_process_drop_stat_rscn_alloc (obtype))

	ptr = (init_pksw_hub_process_drop_stat_rscn_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "init_pksw_hub_process_drop_stat_rscn [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_init_pksw_hub_process_drop_stat_rscn_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	init_pksw_hub_process_drop_stat_rscn_state		*prs_ptr;

	FIN_MT (_op_init_pksw_hub_process_drop_stat_rscn_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (init_pksw_hub_process_drop_stat_rscn_state *)gen_ptr;

	if (strcmp ("ete1_gsh" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ete1_gsh);
		FOUT
		}
	if (strcmp ("sim_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sim_time);
		FOUT
		}
	if (strcmp ("transmit_drop" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->transmit_drop);
		FOUT
		}
	if (strcmp ("add_frame" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->add_frame);
		FOUT
		}
	if (strcmp ("judge_frame" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->judge_frame);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

