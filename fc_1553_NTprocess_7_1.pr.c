/* Process model C form file: fc_1553_NTprocess_7_1.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char fc_1553_NTprocess_7_1_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 6420FA6C 6420FA6C 1 LAPTOP-45T4L382 12085 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <fc_test_global.h>



/* �����ж����ֵ	*/
#define	NT_send_data		10
#define	NT_send_request		20
#define	NT_send_state		30

#define UNIT_CONVERT		1E6

/*�������жϵ�����.	*/
#define frame_arrive		(op_intrpt_type() == OPC_INTRPT_STRM)
#define	send_data	        (op_intrpt_type() == OPC_INTRPT_SELF && intrpt_code == NT_send_data)
#define	send_state_frame	(op_intrpt_type() == OPC_INTRPT_SELF && intrpt_code == NT_send_state)
#define sim_end		 		(op_intrpt_type() == OPC_INTRPT_ENDSIM)
 
	
	
// the next is que stat

extern int pks_loss_queue;

extern int total_pks_generate;

//the next is pro stat
extern int pro_period_pks_send;
extern int pro_tcbm_pks_send;
extern int pro_burst_pks_send;
extern int pro_total_pks_send;

extern double period_ete_max_delay_1;
extern double period_ete_max_delay_2;
extern double period_ete_min_delay_1;
extern double period_ete_min_delay_2;

extern double tcbm_ete_max_delay;
extern double burst_ete_max_delay;
extern double NC_send_burst_ete_max_delay;

extern double period_ete_sum_delay_1;
extern double period_ete_sum_delay_2;

extern double tcbm_ete_sum_delay;
extern double burst_ete_sum_delay;

extern int period_pks_receive;
extern int tcbm_pks_receive;
extern int burst_pks_receive;

extern double period_ete_aveg_delay;
extern double tcbm_ete_aveg_delay;
extern double burst_ete_aveg_delay;

extern double ete_aveg_delay;
//extern int total_rec_pks;

extern double total_throughput;
extern double total_source_throughput;//ͳ�ƴ�ҵ����������������������ͨ����֡�Ϳ���
extern double throughput_different_link[32][32];//������ͳ��������

extern double total_lossrate_pks;
extern int rscn_receive;
extern double rscn_max_delay;



//ͳ�Ʋ�ͬ���ӵ�ͻ��ҵ������ʱ�ӣ�ƽ��ʱ��

extern double burst_ete_max_delay_different_link[32][32];
extern double burst_ete_sum_delay_different_link[32][32];
extern double burst_ete_aveg_delay_different_link[32][32];
extern int burst_packet_number_different_link[32][32];


extern int control_command_nonSensitive_pks_receive;
extern double control_command_nonSensitive_pks_ete_max_delay;
extern double control_command_nonSensitive_pks_ete_sum_delay;


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
	Stathandle	             		burst_delay_stahandle                           ;
	Stathandle	             		burst_send_total_stahandle                      ;
	Stathandle	             		burst_receive_total_stahandle                   ;
	Stathandle	             		period_delay_stahandle                          ;
	Stathandle	             		period_send_total_stahandle                     ;
	Stathandle	             		period_receive_total_stahandle                  ;
	Stathandle	             		tcbm_delay_stahandle                            ;
	Stathandle	             		tcbm_send_total_stahandle                       ;
	Stathandle	             		tcbm_receive_total_stahandle                    ;
	int	                    		intrpt_code                                     ;
	int	                    		input_strm_index                                ;
	Packet *	               		pkptr                                           ;
	Topology *	             		topo_ptr                                        ;
	Objid	                  		process_id                                      ;
	int	                    		node_num                                        ;
	double	                 		timing                                          ;
	double	                 		channel_rate                                    ;
	double	                 		DBA_period                                      ;
	double	                 		start_time                                      ;
	Objid	                  		node_id                                         ;
	Objid	                  		subnet_id                                       ;
	int*	                   		node_user_id                                    ;
	int	                    		received_frame_type                             ;
	int	                    		data_size_NTsend                                ;
	int	                    		NTsend_destination_adress                       ;
	int	                    		NC_adress                                       ;
	int	                    		self_adress                                     ;
	int	                    		state_destination_adress                        ;
	int	                    		Send_data_flag                                  ;
	int	                    		Send_state_flag                                 ;
	double	                 		pre_period_receive_time5                        ;
	double	                 		period_receive_max_interval5                    ;
	double	                 		pre_period_receive_time10                       ;
	double	                 		period_receive_max_interval10                   ;
	double	                 		pre_period_receive_time15                       ;
	double	                 		period_receive_max_interval15                   ;
	} fc_1553_NTprocess_7_1_state;

#define burst_delay_stahandle   		op_sv_ptr->burst_delay_stahandle
#define burst_send_total_stahandle		op_sv_ptr->burst_send_total_stahandle
#define burst_receive_total_stahandle		op_sv_ptr->burst_receive_total_stahandle
#define period_delay_stahandle  		op_sv_ptr->period_delay_stahandle
#define period_send_total_stahandle		op_sv_ptr->period_send_total_stahandle
#define period_receive_total_stahandle		op_sv_ptr->period_receive_total_stahandle
#define tcbm_delay_stahandle    		op_sv_ptr->tcbm_delay_stahandle
#define tcbm_send_total_stahandle		op_sv_ptr->tcbm_send_total_stahandle
#define tcbm_receive_total_stahandle		op_sv_ptr->tcbm_receive_total_stahandle
#define intrpt_code             		op_sv_ptr->intrpt_code
#define input_strm_index        		op_sv_ptr->input_strm_index
#define pkptr                   		op_sv_ptr->pkptr
#define topo_ptr                		op_sv_ptr->topo_ptr
#define process_id              		op_sv_ptr->process_id
#define node_num                		op_sv_ptr->node_num
#define timing                  		op_sv_ptr->timing
#define channel_rate            		op_sv_ptr->channel_rate
#define DBA_period              		op_sv_ptr->DBA_period
#define start_time              		op_sv_ptr->start_time
#define node_id                 		op_sv_ptr->node_id
#define subnet_id               		op_sv_ptr->subnet_id
#define node_user_id            		op_sv_ptr->node_user_id
#define received_frame_type     		op_sv_ptr->received_frame_type
#define data_size_NTsend        		op_sv_ptr->data_size_NTsend
#define NTsend_destination_adress		op_sv_ptr->NTsend_destination_adress
#define NC_adress               		op_sv_ptr->NC_adress
#define self_adress             		op_sv_ptr->self_adress
#define state_destination_adress		op_sv_ptr->state_destination_adress
#define Send_data_flag          		op_sv_ptr->Send_data_flag
#define Send_state_flag         		op_sv_ptr->Send_state_flag
#define pre_period_receive_time5		op_sv_ptr->pre_period_receive_time5
#define period_receive_max_interval5		op_sv_ptr->period_receive_max_interval5
#define pre_period_receive_time10		op_sv_ptr->pre_period_receive_time10
#define period_receive_max_interval10		op_sv_ptr->period_receive_max_interval10
#define pre_period_receive_time15		op_sv_ptr->pre_period_receive_time15
#define period_receive_max_interval15		op_sv_ptr->period_receive_max_interval15

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	fc_1553_NTprocess_7_1_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((fc_1553_NTprocess_7_1_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void fc_1553_NTprocess_7_1 (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_fc_1553_NTprocess_7_1_init (int * init_block_ptr);
	void _op_fc_1553_NTprocess_7_1_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_fc_1553_NTprocess_7_1_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_fc_1553_NTprocess_7_1_alloc (VosT_Obtype, int);
	void _op_fc_1553_NTprocess_7_1_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
fc_1553_NTprocess_7_1 (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (fc_1553_NTprocess_7_1 ());

		{
		/* Temporary Variables */
		int command_type;
		int Traffic_type_send2NC;
		//int data_unit_size;
		int Traffic_type_send2NT;
		int dataframe_location;
		//int data_size_NT2NC;
		int destination_adress;
		int burst_packet_big_delay_number=0;
		int i,j;
		int source;
		
		double period_packet_delay;
		double tcbm_packet_delay;
		double burst_packet_delay;
		double command_pkptr_size;
		double	command_pkptr_send_time;
		double data_frame_size;
		double data_send_time=0;
		double state_size;
		double state_send_time;
		double pk_create_time;
		
		
		//double period_receive_interval_time;
		
		Ici *ici_ptr;
		Packet	*state_pkptr;
		Packet	*Dataunit_ptr;
		Packet *command_pkptr_NT2NT;
		Packet *request_ptr;
		Packet *Data_pkptr;
		/* End of Temporary Variables */


		FSM_ENTER ("fc_1553_NTprocess_7_1")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (Init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "Init", "fc_1553_NTprocess_7_1 [Init enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTprocess_7_1 [Init enter execs]", state0_enter_exec)
				{
				//��̬���˳�ʼ��
				process_id=op_id_self(); //��ȡ��ǰ����id����NTģ��
				node_id=op_topo_parent(op_id_self());//��ȡ��ǰ�ڵ�id
				subnet_id=op_topo_parent(node_id);//��ȡ��ǰ����id
				topo_ptr=op_rte_topo_from_objids();
				node_num=op_topo_object_count(OPC_OBJTYPE_NDFIX);//��ȡ�����еĽڵ���Ŀ
				node_user_id=(int*)op_prg_mem_alloc(sizeof(int)*node_num);//
				
				
				//init input parameter
				
				op_ima_obj_attr_get(node_id,"Channel rate(Mbits/s)",&channel_rate);
				//op_ima_obj_attr_get(node_id,"DBA Period",&DBA_period);
				op_ima_obj_attr_get(node_id,"Start Time",&start_time);
				op_ima_obj_attr_get(node_id,"user id",&self_adress);
				NC_adress=0;
				
				channel_rate=channel_rate*UNIT_CONVERT;
				
				
				
				//the next is pro stat
				pro_period_pks_send=0;
				pro_tcbm_pks_send=0;
				pro_burst_pks_send=0;
				pro_total_pks_send=0;
				
				period_ete_max_delay_1=0.0;
				period_ete_max_delay_2=0.0;
				period_ete_min_delay_1=1.0;
				period_ete_min_delay_2=1.0;
				
				tcbm_ete_max_delay=0.0;
				 burst_ete_max_delay=0.0;
				NC_send_burst_ete_max_delay=0.0;
				
				period_ete_sum_delay_1=0.0;
				period_ete_sum_delay_2=0.0;
				
				tcbm_ete_sum_delay=0.0;
				burst_ete_sum_delay=0.0;
				
				period_pks_receive=0;
				tcbm_pks_receive=0;
				burst_pks_receive=0;
				rscn_receive=0;
				rscn_max_delay=0.0;
				
				
				
				total_throughput=0;
				total_source_throughput=0.0;//ͳ�ƴ�ҵ����������������������ͨ����֡�Ϳ���
				
				
				
				//ͳ������ʼ��
				for(i=0;i<32;i++)
					{
					for(j=0;j<32;j++)
						{
						burst_ete_max_delay_different_link[i][j]=0.0;
						burst_ete_sum_delay_different_link[i][j]=0.0;
						burst_ete_aveg_delay_different_link[i][j]=0.0;
						burst_packet_number_different_link[i][j]=0;
						}
					}
				
				for(i=0;i<32;i++)
					{
					for(j=0;j<32;j++)
						{
						throughput_different_link[i][j]=0.0;//������ͳ��������
						}
					}
				
				
				
				control_command_nonSensitive_pks_receive=0;
				control_command_nonSensitive_pks_ete_max_delay=0.0;
				control_command_nonSensitive_pks_ete_sum_delay=0.0;
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (Init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "Init", "fc_1553_NTprocess_7_1 [Init exit execs]")


			/** state (Init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Init", "Idle", "tr_0", "fc_1553_NTprocess_7_1 [Init -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "Idle", state1_enter_exec, "fc_1553_NTprocess_7_1 [Idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"fc_1553_NTprocess_7_1")


			/** state (Idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "Idle", "fc_1553_NTprocess_7_1 [Idle exit execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTprocess_7_1 [Idle exit execs]", state1_exit_exec)
				{
				//frame_type=0;
				
				if(op_intrpt_type()==OPC_INTRPT_STRM)
				  {
				    input_strm_index=op_intrpt_strm();
				    //kptr=op_pk_get(input_strm_index);
					//op_pk_nfd_get(pkptr,"type",&frame_type);
					//ȷ������֡��֡ͷ��Ŀ�ģ�1.���ͱ���OB֡��2.����֡����OB
				   }
				else if(op_intrpt_type()==OPC_INTRPT_SELF)
					{
					intrpt_code=op_intrpt_code();
					}
				timing=op_sim_time();
				
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (Idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("fc_1553_NTprocess_7_1 [Idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (sim_end)
			FSM_TEST_COND (frame_arrive)
			FSM_TEST_COND (send_data)
			FSM_TEST_COND (send_state_frame)
			FSM_TEST_LOGIC ("Idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 4, state4_enter_exec, ;, "sim_end", "", "Idle", "Result", "tr_1", "fc_1553_NTprocess_7_1 [Idle -> Result : sim_end / ]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "frame_arrive", "", "Idle", "Receive", "tr_16", "fc_1553_NTprocess_7_1 [Idle -> Receive : frame_arrive / ]")
				FSM_CASE_TRANSIT (2, 3, state3_enter_exec, ;, "send_data", "", "Idle", "Send_data", "tr_18", "fc_1553_NTprocess_7_1 [Idle -> Send_data : send_data / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "send_state_frame", "", "Idle", "Send_state", "tr_19", "fc_1553_NTprocess_7_1 [Idle -> Send_state : send_state_frame / ]")
				}
				/*---------------------------------------------------------*/



			/** state (Receive) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "Receive", state2_enter_exec, "fc_1553_NTprocess_7_1 [Receive enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTprocess_7_1 [Receive enter execs]", state2_enter_exec)
				{
				timing=op_sim_time();
				
				//input_strm_index=op_intrpt_strm();//��ȡ��������
				//op_sim_message("wefcs1","defa");
				pkptr = op_pk_get(input_strm_index);//�����жϻ�ȡ��
				
				
				/*�Ӱ��л�ȡtype����������Frame_type�У�
				type==0������֡  ����+������ҵ��֡������+ǿʱЧ��ҵ��֡����������֡��
				type==1������֡
				type==2��״̬֡
				*/
				op_pk_nfd_get(pkptr,"Frame_type",&received_frame_type);
				
				total_throughput=total_throughput+op_pk_total_size_get(pkptr);
				
				op_pk_nfd_get(pkptr,"S_ID",&source);
				throughput_different_link[source][self_adress]=throughput_different_link[source][self_adress]+op_pk_total_size_get(pkptr);
				
				/*----------����֡֡����������---------*/
				if(received_frame_type==0)   //����֡����
					{
					op_pk_nfd_get(pkptr,"Task_type",&command_type);
					/*��ȡNT���յ�������֡���ͣc
					command_type==0,����NT�������ݵ�NC
					command_type==1,����NT��������
					command_type==2,����NT�����NT���ͽ������ݵ������NT��NT������NC����NT���͵�����֡
					*/
					
					if(command_type==0)
						{
						op_pk_nfd_get(pkptr,"Traffic_type",&Traffic_type_send2NC);//�ж�����NT���͸�NC����������ҵ����ͻ����ҵ��
						if(Traffic_type_send2NC==2)//����NT����ͻ��ҵ���NC��ͻ��ҵ���Traffic_typeΪ2
						  {
						   op_pk_nfd_get(pkptr,"Data_bytes_count",&data_size_NTsend);
						   op_pk_nfd_get(pkptr,"S_ID",&NTsend_destination_adress);
					       ici_ptr = op_ici_create("fc_1553_NTprocess_ICIformat_7_1");//����ICI�����ƶ���ģ�鷢�����ݵ�NT����ģ��
					       op_ici_attr_set(ici_ptr,"Request_or_data",1);//Request_or_data==1��ʾ����ģ����Ҫ�����ݣ�Request_or_data==0��ʾ����ģ����Ҫ������
					       op_ici_attr_set(ici_ptr,"data_size2NTprocess",data_size_NTsend);
						   op_ici_attr_set(ici_ptr,"queue_index",NTsend_destination_adress);//ָ�������ģ����ȡ���ݵ��Ӷ��кţ�NT2NC��
					       op_ici_attr_set(ici_ptr,"period_or_burst",0);
						   op_ici_install (ici_ptr);
					       op_strm_access(2);//�����жϵ�����ģ��ķ�������,����������queue2process
					       op_ici_install (OPC_NIL);
					       if (op_strm_empty (2) == OPC_FALSE)
							   op_intrpt_schedule_self(timing,NT_send_data);	//NT���յ�������֡Ϊ����NT�������ݣ�����NT�������ݵ����ж�
						   else
							   op_sim_end("err:There is no data in the input strm of queue2process!","","","");
						  }
						
						
						if(Traffic_type_send2NC==0)//����NT��������ҵ���NC
							{
							 //op_pk_nfd_get(pkptr,"Data_bytes_count",&data_size_NT2NC);
						     //op_pk_nfd_get(pkptr,"S_ID",&source_adress);
					         ici_ptr = op_ici_create("fc_1553_NTprocess_ICIformat_7_1");//����ICI�����ƶ���ģ�鷢�����ݵ�NT����ģ��
					         op_ici_attr_set(ici_ptr,"Request_or_data",1);//Request_or_data==1��ʾ����ģ����Ҫ�����ݣ�Request_or_data==0��ʾ����ģ����Ҫ������
					         op_ici_attr_set(ici_ptr,"data_size2NTprocess",1);
							 op_ici_attr_set(ici_ptr,"queue_index",32);//ָ�������ģ����ȡ���ݵ��Ӷ��кţ�NT��������ҵ����Ӷ��к�Ϊ32
					          op_ici_attr_set(ici_ptr,"period_or_burst",1);
							 op_ici_install (ici_ptr);
					         op_strm_access(3);//�����жϵ�����ģ��ķ�������,����������queue2process
					         op_ici_install (OPC_NIL);
							 //op_sim_message("wefcs1","defa");
					         if (op_strm_empty (3) == OPC_FALSE)
							    {
								state_pkptr = op_pk_create_fmt("fc_1553_Stateframe_format");
					            op_pk_nfd_set (state_pkptr, "Frame_type", 2);
					            op_pk_nfd_set (state_pkptr, "D_ID", NC_adress);
					            op_pk_nfd_set (state_pkptr, "S_ID", self_adress);
					            op_pk_nfd_set (state_pkptr, "Traffic_type", 0);
					            Dataunit_ptr=op_pk_get(3);
								pk_create_time=op_pk_creation_time_get(Dataunit_ptr);//��ȡ������ҵ���������ʱ��
					            //data_unit_size = op_pk_total_size_get(Dataunit_ptr);
								op_pk_nfd_set (state_pkptr, "data", Dataunit_ptr);
								op_pk_creation_time_set(state_pkptr, pk_create_time);//����ǰ���ͳ�ȥ�Ĵ�������ҵ���״̬֡��ʱ������Ϊ������ҵ�����ݰ�������ʱ��
					            //op_pk_destroy (Dataunit_ptr);
								//op_pk_total_size_set (state_pkptr,4624);//һ��Я��512Byte�ֽڵ�״̬֡���ܵĴ�СΪ4624bit
					            pro_period_pks_send++;//ͳ�Ʒ���ȥ������ҵ����ܰ���
								
								op_pk_send(state_pkptr,0);
								
							    }
						     else
							    op_sim_end("err:There is no data in the input strm of queue2process!","","","");
							}
					    
					    }
					if(command_type==1)//NT���յ�������֡Ϊ����NT�������ݣ�
						{
						op_pk_nfd_get(pkptr,"Traffic_type",&Traffic_type_send2NT);//�жϷ��͸�NT����������ҵ��ǿʱЧ��ͻ��ҵ����ͻ��ҵ��
						op_pk_nfd_get(pkptr,"S_ID",&state_destination_adress);
						op_pk_nfd_get(pkptr,"D_ID",&destination_adress);
						//op_sim_message("","periodstate2");
						if(Traffic_type_send2NT==0)  //���յ�NC���͵�������ҵ������֡��ͳ��������ҵ���ʱ�ӡ�����������������NC����״̬֡�����ж�
						   {
						   op_pk_nfd_get(pkptr,"data",&Dataunit_ptr);
						   op_pk_destroy(Dataunit_ptr);
						   total_source_throughput=total_source_throughput+4096;//ͳ�ƴ�ҵ�����ݵ�������������������֡�Լ�����
						    period_pks_receive++;//ͳ�ƽ��յ���������ҵ�����
						    period_packet_delay = op_sim_time () - op_pk_creation_time_get (pkptr);  //ͳ��������ҵ���ʱ��
						    
							if(destination_adress==1)
								{
								period_ete_sum_delay_1=period_ete_sum_delay_1+period_packet_delay;//ͳ��������ҵ�����ʱ��
							
								if(period_packet_delay>period_ete_max_delay_1)
									{
									period_ete_max_delay_1=period_packet_delay;
									}
								if(period_packet_delay < period_ete_min_delay_1)
									{
									period_ete_min_delay_1 = period_packet_delay;
									}
								}
							if(destination_adress==2)
								{
								period_ete_sum_delay_2=period_ete_sum_delay_2+period_packet_delay;//ͳ��������ҵ�����ʱ��
							
								if(period_packet_delay>period_ete_max_delay_2)
									{
									period_ete_max_delay_2=period_packet_delay;
									}
								if(period_packet_delay < period_ete_min_delay_2)
									{
									period_ete_min_delay_2 = period_packet_delay;
									}
								}
				
							
							
						    if(destination_adress!=32)//����NC�㲥��������ҵ������Ҫ�ظ�״̬֡
								{
								state_pkptr = op_pk_create_fmt("fc_1553_Stateframe_format");
					           op_pk_nfd_set (state_pkptr, "Frame_type", 2);
					           op_pk_nfd_set (state_pkptr, "D_ID", 0);
					           op_pk_nfd_set (state_pkptr, "S_ID", self_adress);
					           op_pk_send(state_pkptr,0);				}
						       
						       }
						
						if(Traffic_type_send2NT==1)//���յ�ǿʱЧ��ͻ��ҵ������֡��ͳ��ǿʱЧ��ͻ��ҵ���ʱ�ӡ�����������������NC����״̬֡�����ж�
							{
							
							op_pk_nfd_get(pkptr,"data",&Dataunit_ptr);
							op_pk_destroy(Dataunit_ptr);
							
							total_source_throughput=total_source_throughput+800;//ͳ�ƴ�ҵ�����ݵ�������������������֡�Լ�����
							tcbm_packet_delay = op_sim_time () - op_pk_creation_time_get (pkptr);  //ͳ��ǿʱЧ��ҵ���ʱ��
						    
							tcbm_pks_receive++;//ͳ�ƽ��յ���ǿʱЧ��ҵ�����
							//printf(" TCBM packet received:%d---time:%f\n",tcbm_pks_receive,op_sim_time());
						    //printf("TCBM����ʱ�ӣ�ǰ����%f\n",tcbm_ete_sum_delay);
						    tcbm_ete_sum_delay=tcbm_ete_sum_delay+tcbm_packet_delay;//ͳ��ǿʱЧ����ҵ�����ʱ��
							
							//printf("TCBM��������%d\n",tcbm_pks_receive);
							//printf("Time��%f\n",op_sim_time());
							//printf("TCBM����ǰ����ʱ���ȡ��%f\n",op_sim_time ());
							//printf("TCBM������ʱ�䣺%f\n",op_pk_creation_time_get (pkptr));
							//printf("TCBM����ʱ�ӣ�%f\n",tcbm_ete_sum_delay);
							
							if(destination_adress!=32)
								{
								state_pkptr = op_pk_create_fmt("fc_1553_Stateframe_format");
								op_pk_nfd_set (state_pkptr, "Frame_type", 2);
								op_pk_nfd_set (state_pkptr, "D_ID", 0);
								op_pk_nfd_set (state_pkptr, "S_ID", self_adress);
								op_pk_send(state_pkptr,0);
								}
							
							if(tcbm_packet_delay>tcbm_ete_max_delay)
							  {
							    tcbm_ete_max_delay=tcbm_packet_delay;
								if(tcbm_ete_max_delay>=0.0001)
									{
									printf(" this TVBM's max delay >28us--------%f-------\n",op_sim_time());
									printf(" it's creation time  is --------%f-------\n",op_pk_creation_time_get (pkptr));
									op_sim_end("TCBM ","","","");
									}
							  }
							
							//op_stat_write (tcbm_delay_stahandle, tcbm_packet_delay);
						   
						   
							}
						if(Traffic_type_send2NT==2)//����NT����ͻ��ҵ��
							{
							//����Ҫ����ʲô����
							}
						}
						
				
					if(command_type==2)//NT���յ�������֡Ϊ����NT����һ��NT�������ݣ�
					   {
					   op_pk_nfd_get(pkptr,"ReceiveNT_adress",&NTsend_destination_adress);
					   op_pk_nfd_get (pkptr, "Data_bytes_count", &data_size_NTsend);
					   
					   command_pkptr_NT2NT = op_pk_copy (pkptr);//��NT���յ�������֡���Ƶ�command_pkptr
					  // op_pk_destroy (pkptr);//������ɺ�����ԭ��������֡
					   op_pk_nfd_set (command_pkptr_NT2NT, "Frame_type", 0);
					   op_pk_nfd_set (command_pkptr_NT2NT, "Task_type", 1);
					   op_pk_nfd_set (command_pkptr_NT2NT, "Traffic_type", 2);
					   op_pk_nfd_set (command_pkptr_NT2NT, "D_ID", NTsend_destination_adress);
					   op_pk_nfd_set (command_pkptr_NT2NT, "S_ID", self_adress);
					   op_pk_send(command_pkptr_NT2NT,0);
					   
					   command_pkptr_size = op_pk_total_size_get(command_pkptr_NT2NT);
					   command_pkptr_send_time = command_pkptr_size/channel_rate;//���㷢��һ������֡��ʱ��
					   
					   
					   ici_ptr = op_ici_create("fc_1553_NTprocess_ICIformat_7_1");//����ICI�����ƶ���ģ�鷢�����ݵ�NT����ģ��
					   op_ici_attr_set(ici_ptr,"Request_or_data",1);//Request_or_data==1��ʾ����ģ����Ҫ�����ݣ�Request_or_data==0��ʾ����ģ����Ҫ������
					   op_ici_attr_set(ici_ptr,"data_size2NTprocess",data_size_NTsend);
					   op_ici_attr_set(ici_ptr,"queue_index",NTsend_destination_adress);//ָ�������ģ����ȡ���ݵ��Ӷ��кţ�NT2NT��
					   op_ici_attr_set(ici_ptr,"period_or_burst",0);    
					   op_ici_install (ici_ptr);
					       op_strm_access(2);//�����жϵ�����ģ��ķ�������,����������queue2process
					       op_ici_install (OPC_NIL);
					       if (op_strm_empty (2) == OPC_FALSE)
							   op_intrpt_schedule_self(timing+command_pkptr_send_time,NT_send_data);	//NT���յ�������֡Ϊ����NT�������ݣ�����NT�������ݵ����ж�
						   else
							   op_sim_end("err:There is no data in the input strm of queue2process!","","","");
					 
					   }
					
					op_pk_destroy (pkptr);
					
					}
				
				
				/*----------����֡����������---------*/
				if(received_frame_type==1) //����֡һ����ͻ��ҵ�񣬽��յ�ͻ��ҵ������֡��ͳ����ʱ�ӣ����������ݰ�����Ϊ���һ������֡����Ҫ��NC�ظ�����֡ 
					{
					op_pk_nfd_get(pkptr,"Data_frame_location",&dataframe_location);
					/*��ȡNT���յ�������֡��λ�ã�
					dataframe_location==0,�������յ�������֡Ϊ���ǽ����е����һ������֡
					dataframe_location==1,�������յ�������֡Ϊ�ǽ����е����һ������֡
					*/
					op_pk_nfd_get(pkptr,"S_ID",&state_destination_adress);
					op_pk_nfd_get(pkptr,"D_ID",&destination_adress);
					op_pk_nfd_get(pkptr,"data",&Dataunit_ptr);
					
					
					
					total_source_throughput=total_source_throughput+2112*8;//ͳ�ƴ�ҵ�����ݵ�������������������֡�Լ�����
					data_frame_size  =  op_pk_total_size_get(pkptr);//��ȡ���յ�������֡���ܴ�С
					
				    burst_packet_delay = op_sim_time () - op_pk_creation_time_get (pkptr);  //ͳ��ͻ����ҵ���ʱ��
					//op_stat_write (burst_delay_stahandle, burst_packet_delay);
					
					burst_pks_receive++;//ͳ�ƽ��յ���ͻ����ҵ��ҵ�����
				    
					burst_ete_sum_delay=burst_ete_sum_delay+burst_packet_delay;//ͳ��ͻ����ҵ�����ʱ��
					if(state_destination_adress==0)//��NC���͵����ӵ����ʱ�Ӻ��������ӵ����ʱ�ӷֿ�ͳ��
						{
						if(burst_packet_delay>NC_send_burst_ete_max_delay)
							{
							NC_send_burst_ete_max_delay=burst_packet_delay;
							}
							
						}
						else
						{
						if(burst_packet_delay>burst_ete_max_delay)
							{
							burst_ete_max_delay=burst_packet_delay;
							}
						}
					
					
					
					//ͳ�Ʋ�ͬ���ӵ����ʱ�ӣ�ÿ�����ӵ�ʱ���ܺͣ����������ӵ�ƽ��ʱ�ӣ���ÿ�����ӵ�ͻ��ҵ�����
					if(burst_packet_delay > burst_ete_max_delay_different_link[state_destination_adress][destination_adress])
						{
						burst_ete_max_delay_different_link[state_destination_adress][destination_adress]=burst_packet_delay;
						}
					burst_ete_sum_delay_different_link[state_destination_adress][destination_adress]=burst_ete_sum_delay_different_link[state_destination_adress][destination_adress]+burst_packet_delay;
					burst_packet_number_different_link[state_destination_adress][destination_adress]++;
				
					
					//ͳ��ʱ�Ӳ����п���ָ������ʱ�Ӻ�ƽ��ʱ��
					if(op_pk_total_size_get(Dataunit_ptr)<16896)
						{
						control_command_nonSensitive_pks_receive++;
						control_command_nonSensitive_pks_ete_sum_delay=control_command_nonSensitive_pks_ete_sum_delay+burst_packet_delay;
					
						if(burst_packet_delay>control_command_nonSensitive_pks_ete_max_delay)
							{
							control_command_nonSensitive_pks_ete_max_delay=burst_packet_delay;
							}
						}
				
					
					op_pk_destroy(Dataunit_ptr);
					op_pk_destroy (pkptr);
					
					if(destination_adress!=32)//����NC�㲥��ҵ��֮�⣬NT��Ҫ�ڽ��յ����һ������֮֡��ظ�״̬֡
						{
				         if(dataframe_location==1)
						    {
							state_pkptr = op_pk_create_fmt("fc_1553_Stateframe_format");
					        op_pk_nfd_set (state_pkptr, "Frame_type", 2);
					       op_pk_nfd_set (state_pkptr, "D_ID", state_destination_adress);
					       op_pk_nfd_set (state_pkptr, "S_ID", self_adress);
					       op_pk_send(state_pkptr,0);		    }
						 }
					
					
					}
				
				
				/*----------״̬֡����������---------*/
				
				if(received_frame_type==2) //��NT�յ�״̬֡ʱ��˵������NT2NT�����н���NT���ص�״̬֡����ʱ����NT��Ҫ��NC����״̬֡
					{
					state_pkptr = op_pk_copy (pkptr);//��NT���յ�������֡���Ƶ�command_pkptr
					op_pk_nfd_set (state_pkptr, "D_ID", NC_adress);
					op_pk_nfd_set (state_pkptr, "S_ID", self_adress);//��״̬֡�н�Ŀ�ĵ�ַ��ΪNC��Դ��ַ��ΪNT����ĵ�ַ
					op_pk_send(state_pkptr,0);//״̬֡ͨ������process_send�İ������͵����Ż�
					op_pk_destroy (pkptr);
					}
				
				
				/*----------�ռ�����Ĺ㲥����֡����������---------*/
				
				if(received_frame_type==3)
					{
					pk_create_time=op_pk_creation_time_get(pkptr);
					
					
					ici_ptr = op_ici_create("fc_1553_NTprocess_ICIformat_7_1");//����ICI�����ƶ���ģ�鷢��ҵ������NT����ģ��
					op_ici_attr_set(ici_ptr,"Request_or_data",0);//Request_or_data==1��ʾ����ģ����Ҫ�����ݣ�Request_or_data==0��ʾ����ģ����Ҫ������
					op_ici_install (ici_ptr);
				    op_strm_access(1);//�����жϵ�����ģ��ķ���ҵ������,����������queue2process
					op_ici_install (OPC_NIL);
					if (op_strm_empty (1) == OPC_FALSE)
						{
						request_ptr = op_pk_get(1); //��������queue2process
						op_pk_nfd_set (request_ptr, "D_ID", NC_adress);
						op_pk_nfd_set (request_ptr, "S_ID", self_adress);
						op_pk_nfd_set (request_ptr, "Frame_type", 4);//request_ptrΪ�ϱ������״̬֡
						op_pk_creation_time_set(request_ptr, pk_create_time);
						op_pk_send(request_ptr,0);//״̬֡ͨ������0���͵����Ż�
						
						}
					else
						{
						op_sim_end("err:There is no data in the input strm of queue2process!","","","");
						}
					op_pk_destroy (pkptr);
					}
				
				
				
				/*------------��������RSCN֡�Ĵ���--------------------*/
				
				if(received_frame_type==5)
					{
						
					op_pk_send(pkptr,0);
					}
				
				
				/*---------------���յ�ʱ�Ӳ����еĿ���ָ��--------------*/
				if(received_frame_type==6)
					{	
					
					
					op_pk_destroy(pkptr);
					}
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (Receive) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "Receive", "fc_1553_NTprocess_7_1 [Receive exit execs]")


			/** state (Receive) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Receive", "Idle", "tr_15", "fc_1553_NTprocess_7_1 [Receive -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Send_data) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "Send_data", state3_enter_exec, "fc_1553_NTprocess_7_1 [Send_data enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTprocess_7_1 [Send_data enter execs]", state3_enter_exec)
				{
				timing=op_sim_time();
				
				if(intrpt_code==NT_send_data)//���������ж���NT��Ҫ��NC����ͻ��ҵ������
					{
					Send_data_flag = 1;
					Data_pkptr = op_pk_create_fmt("fc_1553_Dataframe_format");
					op_pk_nfd_set (Data_pkptr, "Frame_type", 1);
					op_pk_nfd_set (Data_pkptr, "D_ID", NTsend_destination_adress);
					op_pk_nfd_set (Data_pkptr, "S_ID", self_adress);
					op_pk_nfd_set (Data_pkptr, "Traffic_type", 2);
					//�жϵ�ǰҪ��������֡�ǲ������һ֡������ǣ���Ҫ������֡�е�Data_frame_location����Ϊ1
					if(data_size_NTsend==1)
						{
						 op_pk_nfd_set (Data_pkptr, "Data_frame_location", 1);
						}
					
					Dataunit_ptr=op_pk_get(2);
					pk_create_time=op_pk_creation_time_get(Dataunit_ptr);
					
					op_pk_nfd_set(Data_pkptr,"data",Dataunit_ptr);
					op_pk_creation_time_set(Data_pkptr, pk_create_time);
					op_pk_send(Data_pkptr,0);
					
					//op_pk_destroy(Dataunit_ptr);//���ٴӶ��з������������ݵ�Ԫ
					
					pro_burst_pks_send=pro_burst_pks_send+1;//ͳ�Ʒ��͵�ͻ��ҵ��İ���
					
					
					data_frame_size = op_pk_total_size_get(Data_pkptr);
					data_send_time = data_frame_size/channel_rate;//���㷢��һ������֡��ʱ��
					data_size_NTsend = data_size_NTsend-1;
					if(data_size_NTsend>0)//
						{
						//op_sim_message("wefcs4","defa");
						if(Send_state_flag == 1)//��ʾ��ǰ�Ƿ���Ҫ�ظ�״̬֡�SState_send_flag==1��ʾ��״̬֡Ҫ������֮��û��
							{
							
							//op_sim_message("wefcs3","defa");
							op_intrpt_schedule_self(timing+data_send_time,NT_send_state);
							}
						else
							{
							
							//op_sim_message("wefcs2","defa");
				            op_intrpt_schedule_self(timing+data_send_time,NT_send_data);
							}
						}
					else
						
						if(NTsend_destination_adress==0)
							{
						  // op_sim_message("wefcs1","defa");
						   Send_data_flag = 0;
					       op_intrpt_schedule_self(timing+data_send_time,NT_send_state);
						   }
						
					}
				
				
				
				
				
				
				
				
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (Send_data) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "Send_data", "fc_1553_NTprocess_7_1 [Send_data exit execs]")


			/** state (Send_data) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Send_data", "Idle", "tr_17", "fc_1553_NTprocess_7_1 [Send_data -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Result) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "Result", state4_enter_exec, "fc_1553_NTprocess_7_1 [Result enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTprocess_7_1 [Result enter execs]", state4_enter_exec)
				{
				if(self_adress==1)
					{
					printf("NT1 ����������ҵ������ʱ�ӣ�%f\n",period_ete_max_delay_1);
					printf("NT1 ����������ҵ�����Сʱ�ӣ�%f\n",period_ete_min_delay_1);
					}
				if(self_adress==2)
					{
					printf("NT2 ����������ҵ������ʱ�ӣ�%f\n",period_ete_max_delay_2);
					printf("NT2 ����������ҵ�����Сʱ�ӣ�%f\n",period_ete_min_delay_2);
				
					}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (9,"fc_1553_NTprocess_7_1")


			/** state (Result) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "Result", "fc_1553_NTprocess_7_1 [Result exit execs]")


			/** state (Result) transition processing **/
			FSM_TRANSIT_MISSING ("Result")
				/*---------------------------------------------------------*/



			/** state (Send_state) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "Send_state", state5_enter_exec, "fc_1553_NTprocess_7_1 [Send_state enter execs]")
				FSM_PROFILE_SECTION_IN ("fc_1553_NTprocess_7_1 [Send_state enter execs]", state5_enter_exec)
				{
				timing=op_sim_time();
				if(intrpt_code==NT_send_state)
					{
					state_pkptr = op_pk_create_fmt("fc_1553_Stateframe_format");
					op_pk_nfd_set (state_pkptr, "Frame_type", 2);
					op_pk_nfd_set (state_pkptr, "D_ID", state_destination_adress);
					op_pk_nfd_set (state_pkptr, "S_ID", self_adress);
				    state_size = op_pk_total_size_get(state_pkptr);
					state_send_time = state_size/channel_rate;//���㷢��һ������֡��ʱ��
					op_pk_send(state_pkptr,0);
					if(Send_data_flag==1)
						op_intrpt_schedule_self(timing+data_send_time,NT_send_data);
					}
				
				Send_state_flag=0;
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (Send_state) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "Send_state", "fc_1553_NTprocess_7_1 [Send_state exit execs]")


			/** state (Send_state) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Send_state", "Idle", "tr_20", "fc_1553_NTprocess_7_1 [Send_state -> Idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"fc_1553_NTprocess_7_1")
		}
	}




void
_op_fc_1553_NTprocess_7_1_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_fc_1553_NTprocess_7_1_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_fc_1553_NTprocess_7_1_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_fc_1553_NTprocess_7_1_svar function. */
#undef burst_delay_stahandle
#undef burst_send_total_stahandle
#undef burst_receive_total_stahandle
#undef period_delay_stahandle
#undef period_send_total_stahandle
#undef period_receive_total_stahandle
#undef tcbm_delay_stahandle
#undef tcbm_send_total_stahandle
#undef tcbm_receive_total_stahandle
#undef intrpt_code
#undef input_strm_index
#undef pkptr
#undef topo_ptr
#undef process_id
#undef node_num
#undef timing
#undef channel_rate
#undef DBA_period
#undef start_time
#undef node_id
#undef subnet_id
#undef node_user_id
#undef received_frame_type
#undef data_size_NTsend
#undef NTsend_destination_adress
#undef NC_adress
#undef self_adress
#undef state_destination_adress
#undef Send_data_flag
#undef Send_state_flag
#undef pre_period_receive_time5
#undef period_receive_max_interval5
#undef pre_period_receive_time10
#undef period_receive_max_interval10
#undef pre_period_receive_time15
#undef period_receive_max_interval15

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_fc_1553_NTprocess_7_1_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_fc_1553_NTprocess_7_1_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (fc_1553_NTprocess_7_1)",
		sizeof (fc_1553_NTprocess_7_1_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_fc_1553_NTprocess_7_1_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	fc_1553_NTprocess_7_1_state * ptr;
	FIN_MT (_op_fc_1553_NTprocess_7_1_alloc (obtype))

	ptr = (fc_1553_NTprocess_7_1_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "fc_1553_NTprocess_7_1 [Init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_fc_1553_NTprocess_7_1_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	fc_1553_NTprocess_7_1_state		*prs_ptr;

	FIN_MT (_op_fc_1553_NTprocess_7_1_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (fc_1553_NTprocess_7_1_state *)gen_ptr;

	if (strcmp ("burst_delay_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->burst_delay_stahandle);
		FOUT
		}
	if (strcmp ("burst_send_total_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->burst_send_total_stahandle);
		FOUT
		}
	if (strcmp ("burst_receive_total_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->burst_receive_total_stahandle);
		FOUT
		}
	if (strcmp ("period_delay_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_delay_stahandle);
		FOUT
		}
	if (strcmp ("period_send_total_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_send_total_stahandle);
		FOUT
		}
	if (strcmp ("period_receive_total_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_receive_total_stahandle);
		FOUT
		}
	if (strcmp ("tcbm_delay_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tcbm_delay_stahandle);
		FOUT
		}
	if (strcmp ("tcbm_send_total_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tcbm_send_total_stahandle);
		FOUT
		}
	if (strcmp ("tcbm_receive_total_stahandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tcbm_receive_total_stahandle);
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
	if (strcmp ("pkptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkptr);
		FOUT
		}
	if (strcmp ("topo_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->topo_ptr);
		FOUT
		}
	if (strcmp ("process_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->process_id);
		FOUT
		}
	if (strcmp ("node_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_num);
		FOUT
		}
	if (strcmp ("timing" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->timing);
		FOUT
		}
	if (strcmp ("channel_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->channel_rate);
		FOUT
		}
	if (strcmp ("DBA_period" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->DBA_period);
		FOUT
		}
	if (strcmp ("start_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->start_time);
		FOUT
		}
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
	if (strcmp ("node_user_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_user_id);
		FOUT
		}
	if (strcmp ("received_frame_type" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->received_frame_type);
		FOUT
		}
	if (strcmp ("data_size_NTsend" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->data_size_NTsend);
		FOUT
		}
	if (strcmp ("NTsend_destination_adress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NTsend_destination_adress);
		FOUT
		}
	if (strcmp ("NC_adress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->NC_adress);
		FOUT
		}
	if (strcmp ("self_adress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->self_adress);
		FOUT
		}
	if (strcmp ("state_destination_adress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->state_destination_adress);
		FOUT
		}
	if (strcmp ("Send_data_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Send_data_flag);
		FOUT
		}
	if (strcmp ("Send_state_flag" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Send_state_flag);
		FOUT
		}
	if (strcmp ("pre_period_receive_time5" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pre_period_receive_time5);
		FOUT
		}
	if (strcmp ("period_receive_max_interval5" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_receive_max_interval5);
		FOUT
		}
	if (strcmp ("pre_period_receive_time10" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pre_period_receive_time10);
		FOUT
		}
	if (strcmp ("period_receive_max_interval10" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_receive_max_interval10);
		FOUT
		}
	if (strcmp ("pre_period_receive_time15" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pre_period_receive_time15);
		FOUT
		}
	if (strcmp ("period_receive_max_interval15" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_receive_max_interval15);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

