/* Process model C form file: NC_Processor.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char NC_Processor_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 641ABEA7 641ABEA7 1 LAPTOP-45T4L382 12085 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "fc_test_global.h"
/* Include files.					*/
#include	<oms_dist_support.h>

#define period_time 0
#define DBA_time 1
#define burst_time 2
#define strong_burst_time 3
#define calculate_assured_band_time 4

#define PERIOD_DATA_SEND (op_intrpt_type()==OPC_INTRPT_SELF&& op_intrpt_code () == period_time)
#define DBA_CALC_TIME (op_intrpt_type()==OPC_INTRPT_SELF&& op_intrpt_code () == DBA_time)
#define BURST_DATA_SEND (op_intrpt_type()==OPC_INTRPT_SELF&& op_intrpt_code () == burst_time)
#define STRONG_BURST_SEND (op_intrpt_type()==OPC_INTRPT_SELF&& op_intrpt_code () == strong_burst_time)
#define Inital_Assured_Band (op_intrpt_type()==OPC_INTRPT_SELF&& op_intrpt_code () == calculate_assured_band_time)


#define FRAME_RECEIVE (op_intrpt_type() == OPC_INTRPT_STRM)

#define SIM_END		 (op_intrpt_type() == OPC_INTRPT_ENDSIM)


#define start_time 0.000001
#define request_spacing 0.000020
#define DBA_calc_spacing 0.000005
#define strong_burst_spacing 0.0000051

//一个NT2NT交换传输一个数据帧所需的最短时间
#define NT2NT_at_least_time 0.0000262
#define NCNT_at_least_time 0.0000182



static void strong_burst();
static double burst_band(int frame_num);
static double burst_band_NT(int frame_num);
static int burst_change_frame_number(double burst_change_time);
static int  burst_change_NT2NT_frame_number(double burst_change_time);
//the next is stat declare

extern int pro_period_pks_send;
extern int pro_tcbm_pks_send;
extern int pro_burst_pks_send;
extern int pro_total_pks_send;

extern double period_ete_max_delay;
extern double tcbm_ete_max_delay;
extern double burst_ete_max_delay;

extern double period_ete_sum_delay;
extern double tcbm_ete_sum_delay;
extern double burst_ete_sum_delay;

extern double period_send_interval_time;
extern double period_max_interval_time;
extern double period_min_interval_time;

extern int period_pks_receive;
//extern int tcbm_pks_receive;
extern int burst_pks_receive;

extern double period_ete_aveg_delay;
extern double tcbm_ete_aveg_delay;
extern double burst_ete_aveg_delay;

//extern long ete_aveg_delay;
//extern double total_rec_pks;

extern double total_throughput;
extern double total_lossrate_pks;
extern double total_source_throughput;//统计纯业务数据吞吐量，不包含普通命令帧和开销
extern double throughput_different_link[32][32];//按连接统计吞吐量
;
extern double fairness_index;



//统计不同连接的突发业务的最大时延，平均时延

extern double burst_ete_max_delay_different_link[32][32];
extern double burst_ete_sum_delay_different_link[32][32];
extern double burst_ete_aveg_delay_different_link[32][32];
extern int burst_packet_number_different_link[32][32];

//Node_load[32]存储节点的负载计算各连接保证带宽
extern double Node_load[32];

extern int period_request_map[2][32];
extern double Burst_traffic_control_matrix[32][32];


extern int control_command_nonSensitive_pks_send;

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
	int	                    		burst_request_map[33][32]                       ;	/* 突发业务请求表 */
	Objid	                  		process_id                                      ;
	Objid	                  		node_id                                         ;
	Objid	                  		subnet_id                                       ;
	int	                    		p_num                                           ;	/* 周期性业务序号 */
	int	                    		p1_num                                          ;	/* NC到NT的周期性业务个数 */
	int	                    		p2_num                                          ;	/* NT到NC的周期性业务个数 */
	int	                    		req_num                                         ;	/* 突发业务请求回复的个数 */
	double	                 		NC_send_credit[32]                              ;	/* NC发送信用值，包含广播地址自己也有一个信用值 */
	double	                 		NT_re_credit[31][32]                            ;	/* 数组1表示针对NT1接收轴的30个信用值 */
	double	                 		b_band                                          ;	/* 保证带宽 */
	double	                 		NC_send_task_delay[32]                          ;
	int	                    		NC_send_task_num[32]                            ;
	double	                 		NC_re_credit[32]                                ;	/* NC接收信用值 */
	double	                 		NC_re_task_delay[32]                            ;
	int	                    		NC_re_task_num[32]                              ;
	double	                 		NT_re_task_delay[31][32]                        ;
	int	                    		NT_re_task_num[31][32]                          ;
	int	                    		node_num                                        ;	/* 节点个数 */
	int	                    		strong_cycle_num                                ;	/* 50us强时效状态进入的次数 */
	double	                 		sim_time                                        ;
	Stathandle	             		period_ete_delay                                ;
	Stathandle	             		tcbm_ete_delay                                  ;
	Stathandle	             		burst_ete_delay                                 ;
	int	                    		period_number                                   ;	/* 周期性业务总个数 */
	int	                    		NT_re_task_num2[31][32]                         ;
	double	                 		NT_re_task_delay2[31][32]                       ;
	int	                    		NT_re_task_num3[31][32]                         ;
	double	                 		NT_re_task_delay3[31][32]                       ;
	int	                    		NT_re_task_num1[31][32]                         ;
	double	                 		NT_re_task_delay1[31][32]                       ;
	double	                 		pre_period_pks_send_time                        ;
	double	                 		Assured_band[33][32]                            ;
	double	                 		DBA_long                                        ;
	int	                    		success_DBA_num                                 ;
	double	                 		Period_Interarrival_Time                        ;
	double	                 		strong_burst_limit_time                         ;
	double	                 		Total_burst_band                                ;
	OmsT_Dist_Handle	       		Period_traffic_pksize_dist_ptr                  ;
	double	                 		period_spacing                                  ;
	double	                 		Channel_Rate                                    ;
	} NC_Processor_state;

#define burst_request_map       		op_sv_ptr->burst_request_map
#define process_id              		op_sv_ptr->process_id
#define node_id                 		op_sv_ptr->node_id
#define subnet_id               		op_sv_ptr->subnet_id
#define p_num                   		op_sv_ptr->p_num
#define p1_num                  		op_sv_ptr->p1_num
#define p2_num                  		op_sv_ptr->p2_num
#define req_num                 		op_sv_ptr->req_num
#define NC_send_credit          		op_sv_ptr->NC_send_credit
#define NT_re_credit            		op_sv_ptr->NT_re_credit
#define b_band                  		op_sv_ptr->b_band
#define NC_send_task_delay      		op_sv_ptr->NC_send_task_delay
#define NC_send_task_num        		op_sv_ptr->NC_send_task_num
#define NC_re_credit            		op_sv_ptr->NC_re_credit
#define NC_re_task_delay        		op_sv_ptr->NC_re_task_delay
#define NC_re_task_num          		op_sv_ptr->NC_re_task_num
#define NT_re_task_delay        		op_sv_ptr->NT_re_task_delay
#define NT_re_task_num          		op_sv_ptr->NT_re_task_num
#define node_num                		op_sv_ptr->node_num
#define strong_cycle_num        		op_sv_ptr->strong_cycle_num
#define sim_time                		op_sv_ptr->sim_time
#define period_ete_delay        		op_sv_ptr->period_ete_delay
#define tcbm_ete_delay          		op_sv_ptr->tcbm_ete_delay
#define burst_ete_delay         		op_sv_ptr->burst_ete_delay
#define period_number           		op_sv_ptr->period_number
#define NT_re_task_num2         		op_sv_ptr->NT_re_task_num2
#define NT_re_task_delay2       		op_sv_ptr->NT_re_task_delay2
#define NT_re_task_num3         		op_sv_ptr->NT_re_task_num3
#define NT_re_task_delay3       		op_sv_ptr->NT_re_task_delay3
#define NT_re_task_num1         		op_sv_ptr->NT_re_task_num1
#define NT_re_task_delay1       		op_sv_ptr->NT_re_task_delay1
#define pre_period_pks_send_time		op_sv_ptr->pre_period_pks_send_time
#define Assured_band            		op_sv_ptr->Assured_band
#define DBA_long                		op_sv_ptr->DBA_long
#define success_DBA_num         		op_sv_ptr->success_DBA_num
#define Period_Interarrival_Time		op_sv_ptr->Period_Interarrival_Time
#define strong_burst_limit_time 		op_sv_ptr->strong_burst_limit_time
#define Total_burst_band        		op_sv_ptr->Total_burst_band
#define Period_traffic_pksize_dist_ptr		op_sv_ptr->Period_traffic_pksize_dist_ptr
#define period_spacing          		op_sv_ptr->period_spacing
#define Channel_Rate            		op_sv_ptr->Channel_Rate

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	NC_Processor_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((NC_Processor_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif


//检测队列模块中有没有强时效，有就发下来，流索引号是子队列号；没有就不处理
void strong_burst()
{
	int dest = 0;
	int pksize = 0;
	Ici	*iciptr;
	Packet	*data_ptr;
	Packet  *CommondFrame_ptr;
	double pk_create_time;
	
	FIN(strong_burst());
	
	iciptr = op_ici_create ("NC_subque_ICI");
	op_ici_attr_set (iciptr, "subque_num",64);//强时效的子队列号，0~31是周期性（包含广播地址），32~63是突发（包含广播地址），64是强时效
	op_ici_install (iciptr);
	op_strm_access(4);
	op_ici_install(OPC_NIL);
				
	if(op_strm_empty (4) == OPC_FALSE)
	{
		data_ptr=op_pk_get(4);
		pk_create_time = op_pk_creation_time_get(data_ptr);
		op_pk_nfd_get(data_ptr,"destination",&dest);
		pksize=op_pk_total_size_get(data_ptr);
		CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
		op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);
		op_pk_nfd_set (CommondFrame_ptr, "D_ID", dest);
		op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
		op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 1);//业务类型，0代表周期性，1代表强时效，2代表突发
		op_pk_nfd_set (CommondFrame_ptr, "Task_type", 1);////0代表NT到NC，1代表Nc到Nt，2代表NT到NT
		op_pk_nfd_set (CommondFrame_ptr, "data", op_pk_create(pksize));
		
		op_pk_creation_time_set(CommondFrame_ptr, pk_create_time);
		
		pro_tcbm_pks_send ++;
		//printf("NC send tcbm packets ---%d---\n",pro_tcbm_pks_send);
		//printf(" TCBM packet send from NC:%d---time:%f\n",pro_tcbm_pks_send,op_sim_time());

		op_pk_send(CommondFrame_ptr,0);
		//printf("TCBM send time -------%f--------\n",op_sim_time());
	}
	
	FOUT;
}


//NC到NT货NT到NC：突发业务带宽计算，输入突发业务帧个数，输出该交换时长
double burst_band(int frame_num)
{
	double request;
	FIN(burst_band(frame_num));
	if(frame_num ==0)
		request = 0;
	else
		request = 0.000008 + 0.0000051*(frame_num+1);
	FRET(request);
}

//NT到NT:突发业务带宽计算，输入突发业务帧个数，输出该交换时长
double burst_band_NT(int frame_num)
{
	double request;
	FIN(burst_band_NT(frame_num));
	if(frame_num ==0)
		request = 0;
	else
		request = 0.000016 + 0.0000051*(frame_num+1);
	FRET(request);
}


//NC到NT任务：突发业务交换帧个数计算，输入NC到NT突发业务交换的时长，输出该交换传输的突发业务帧个数
int  burst_change_frame_number(double burst_change_time)
	{
	int frame_num;
	FIN(burst_change_frame_number(burst_change_time));
		if(burst_change_time < NCNT_at_least_time)//突发业务传输一个帧，至少需要18.2us
			frame_num = 0;
	    else
			
			frame_num = (int)((burst_change_time - NCNT_at_least_time)/0.0000051) + 1;
	FRET(frame_num);
	}

//NT到NT任务：突发业务交换帧个数计算，输入NT到NT突发业务交换的时长，输出该交换传输的突发业务帧个数

int  burst_change_NT2NT_frame_number(double burst_change_time)
	{
	int frame_num;
	FIN(burst_change_frame_number(burst_change_time));
		if(burst_change_time < NT2NT_at_least_time)//突发业务传输一个数据帧帧，至少需要29.8us
			frame_num = 0;
	    else
			frame_num = (int)((burst_change_time - NT2NT_at_least_time)/0.0000051) + 1;
	FRET(frame_num);
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
	void NC_Processor (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_NC_Processor_init (int * init_block_ptr);
	void _op_NC_Processor_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_NC_Processor_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_NC_Processor_alloc (VosT_Obtype, int);
	void _op_NC_Processor_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
NC_Processor (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (NC_Processor ());

		{
		/* Temporary Variables */
		int i;
		int j;
		int k;
		int input_strm_index;
		int frame_type;
		int traffic_type;
		//int DBA_num;
		
		
		int pksize;
		int dest;
		int source;
		
		int request[32];
		
		Ici	*iciptr;
		
		Packet	*ptr;
		Packet	*data_ptr;
		
		Packet  *CommondFrame_ptr;
		Packet  *DataFrame_ptr;
		Packet  *Dataunit_ptr;
		
		double per_delay;
		double burst_delay;
		double pk_create_time;
		double load_sum;
		double Time_in_current_DBA;
		
		char	Period_traffic_size_str [128];
		double Period_traffic_pksize;
		
		
		int burst_packet_big_delay_number=0;
		int strong_burst_check_number=15;
		/* End of Temporary Variables */


		FSM_ENTER ("NC_Processor")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (Init) enter executives **/
			FSM_STATE_ENTER_FORCED_NOLABEL (0, "Init", "NC_Processor [Init enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Processor [Init enter execs]", state0_enter_exec)
				{
				 //静态拓扑初始化
				process_id=op_id_self();
				node_id=op_topo_parent(op_id_self());
				subnet_id=op_topo_parent(node_id);
				
				
				//printf("the node numble is %d\n",node_num);
				
				
				
				
				op_ima_obj_attr_get(op_topo_parent(op_id_self()),"Node Number",&node_num);
				op_ima_obj_attr_get(op_topo_parent(op_id_self()),"DBA period",&DBA_long);
				op_ima_obj_attr_get(op_topo_parent(op_id_self()),"Traffic Load(0-1)",&Node_load[0]);
				op_ima_obj_attr_get (op_topo_parent(op_id_self ()),"Period Traffic Interarrival Time",&Period_Interarrival_Time);
				 op_ima_obj_attr_get (op_topo_parent(op_id_self ()),"Channel rate(Mbits/s)",&Channel_Rate);
				
				//获取周期性业务的数据量大小
				op_ima_obj_attr_get (op_topo_parent(op_id_self ()), "Period Data Size", Period_traffic_size_str);
				Period_traffic_pksize_dist_ptr    = oms_dist_load_from_string (Period_traffic_size_str);
				Period_traffic_pksize = (double) ceil (oms_dist_outcome (Period_traffic_pksize_dist_ptr));
				
				 
				//计算执行一个周期性业务的时长
				Channel_Rate = Channel_Rate * 1000000 ;//链路速率的单位换算
				period_spacing = 2*Period_traffic_pksize/Channel_Rate + 0.000006 + 0.0000006 + 0.000001;//0.000006为6us传播时延，0.0000006为0.6us命令帧状态帧时长，0.000001为1us响应时长
				
				 
				 if(DBA_long>Period_Interarrival_Time)//如果DBA周期大于周期性业务的周期，改变DBA周期的大小
					 {
					 DBA_long = Period_Interarrival_Time;
					 }
				 
				
				//初始化突发业务请求表
				for(i=0;i<33;i++)
				{
					for(j=0;j<32;j++)
					{
						burst_request_map[i][j]=0;
					}
				}
				
				
				//初始化周期性业务的业务个数参量
				p_num=0;
				p1_num=0;
				p2_num=0;
				
				
				
				//初始化信用值
				for(i=0;i<32;i++)
				{
					NC_send_credit[i]=0.0;
					NC_re_credit[i]=0.0;
				}
				
				for(i=0;i<31;i++)
					for(j=0;j<32;j++)
						NT_re_credit[i][j]=0.0;
				
				//初始化任务列表数组
				
				
				//产生自中断
				op_intrpt_schedule_self(op_sim_time()+0, calculate_assured_band_time);//0时刻进入calculate_assured_band状态计算各连接的保证带宽
				op_intrpt_schedule_self(op_sim_time()+start_time, period_time);//1us后为DBA的起始时刻
				
				
				//the next is stat init
				
				
				
				pro_period_pks_send = 0;
				pro_tcbm_pks_send = 0;
				pro_burst_pks_send = 0;
				pro_total_pks_send = 0;
				
				period_ete_max_delay = 0.0;
				tcbm_ete_max_delay = 0.0;
				burst_ete_max_delay = 0.0;
				
				period_ete_sum_delay = 0.0;
				tcbm_ete_sum_delay = 0.0;
				burst_ete_sum_delay = 0.0;
				
				period_pks_receive = 0;
				
				burst_pks_receive = 0;
				
				period_ete_aveg_delay = 0.0;
				tcbm_ete_aveg_delay = 0.0;
				burst_ete_aveg_delay = 0.0;
				
				
				
				total_throughput = 0;
				total_lossrate_pks = 0.0;
				period_min_interval_time=0.1;
				
				
				//统计量初始化
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
						throughput_different_link[i][j]=0.0;//按连接统计吞吐量
						}
					
					}
				fairness_index=0.0;
				total_source_throughput=0.0;//统计纯业务数据吞吐量，不包含普通命令帧和开销
				
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** state (Init) exit executives **/
			FSM_STATE_EXIT_FORCED (0, "Init", "NC_Processor [Init exit execs]")


			/** state (Init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Init", "Idle", "tr_0", "NC_Processor [Init -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "Idle", state1_enter_exec, "NC_Processor [Idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"NC_Processor")


			/** state (Idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "Idle", "NC_Processor [Idle exit execs]")


			/** state (Idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("NC_Processor [Idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (FRAME_RECEIVE)
			FSM_TEST_COND (PERIOD_DATA_SEND)
			FSM_TEST_COND (DBA_CALC_TIME)
			FSM_TEST_COND (BURST_DATA_SEND)
			FSM_TEST_COND (SIM_END)
			FSM_TEST_COND (STRONG_BURST_SEND)
			FSM_TEST_COND (Inital_Assured_Band)
			FSM_TEST_LOGIC ("Idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 3, state3_enter_exec, ;, "FRAME_RECEIVE", "", "Idle", "Frame_Pro", "tr_3", "NC_Processor [Idle -> Frame_Pro : FRAME_RECEIVE / ]")
				FSM_CASE_TRANSIT (1, 4, state4_enter_exec, ;, "PERIOD_DATA_SEND", "", "Idle", "Control_Period", "tr_5", "NC_Processor [Idle -> Control_Period : PERIOD_DATA_SEND / ]")
				FSM_CASE_TRANSIT (2, 2, state2_enter_exec, ;, "DBA_CALC_TIME", "", "Idle", "DBA_Process", "tr_9", "NC_Processor [Idle -> DBA_Process : DBA_CALC_TIME / ]")
				FSM_CASE_TRANSIT (3, 5, state5_enter_exec, ;, "BURST_DATA_SEND", "", "Idle", "Burst_Process", "tr_11", "NC_Processor [Idle -> Burst_Process : BURST_DATA_SEND / ]")
				FSM_CASE_TRANSIT (4, 6, state6_enter_exec, ;, "SIM_END", "", "Idle", "result", "tr_13", "NC_Processor [Idle -> result : SIM_END / ]")
				FSM_CASE_TRANSIT (5, 7, state7_enter_exec, ;, "STRONG_BURST_SEND", "", "Idle", "Strong_Burst_Pro", "tr_15", "NC_Processor [Idle -> Strong_Burst_Pro : STRONG_BURST_SEND / ]")
				FSM_CASE_TRANSIT (6, 8, state8_enter_exec, ;, "Inital_Assured_Band", "", "Idle", "calculate_assured_band", "tr_16", "NC_Processor [Idle -> calculate_assured_band : Inital_Assured_Band / ]")
				}
				/*---------------------------------------------------------*/



			/** state (DBA_Process) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "DBA_Process", state2_enter_exec, "NC_Processor [DBA_Process enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Processor [DBA_Process enter execs]", state2_enter_exec)
				{
				
				
				int request_frame=0;
				double request=0.0;//总请求大小
				
				
				double NC_DownLink_band=0.7*Total_burst_band;
				double band_NT=0.0;
				
				double temp[32];//存信用值
				int px[32];//存储原序号
				
				double vMax;
				int iMax;
				
				double m;
				int n;
				
				double rest;
				int w1,w2,w3;
				//int z;
				int h;
				
				
				double delay1,delay2;
				int num1,num2;
				
				//double burst_request_time;
				double remain_time_part_one,remain_time_part_two,remain_time_part_three;
				double burst_part_one,burst_part_two,burst_part_three;//
				int task_has_cut_off=0;
				int task_has_cut_off1=0;
				
				//printf("----%f----\n",band);
				//输出本DBA周期的请求列表
				
				//printf("this is ---%d----DBA period\n",(int)(op_sim_time()/0.0006)+1);//打印当前是第几个DBA周期
				
				//printf("burst_request_map[0][1]:%d%%%%%%%%%%%%%%%%%%%\n",burst_request_map[0][1]);
				if(op_sim_time()>2340)
					{
				printf("this is ---%d----DBA period\n",(int)(op_sim_time()/DBA_long)+1);//打印当前是第几个DBA周期
				for(i=0;i<33;i++)//i=0表示NC发送轴的请求，i=1表示NC接收轴的请求，i=2~32表示31个NT的接收轴的请求
					{
					
					for(j=0;j<32;j++)
						{
						
					
						if(i==0)
							{
							if(burst_request_map[i][j]!=0)
						         printf("Request:NC send to.......%d............%d(frame number)\n",j+1,burst_request_map[i][j]);
							}
						/*
						if(i==1)
							{
							if(burst_request_map[i][j]!=0)
						         printf("Request:NC receive .......%d............%d(frame number)\n",j,burst_request_map[i][j]);
							}
						if(i>1)
							{
							if(burst_request_map[i][j]!=0)
						         printf("Request:NT%d......receive......NT%d........%d(frame number)\n",i-1,j,burst_request_map[i][j]);
							}
						*/
						
						}
					}
				
					
				
				//打印NC发送链接的信用值
				/*
				for(i=0;i<9;i++)
				{
				if(i==8)
				  printf("----NC-NT%d:%f\n",i+1,NC_send_credit[i]);
				else
				  printf("----NC-NT%d:%f",i+1,NC_send_credit[i]);
				}
				*/
				}
				
				//安排NC到NT的任务
				for(i=0;i<32;i++)
				{
					if(burst_request_map[0][i] != 0)
					{
						request_frame = request_frame + burst_request_map[0][i];
						request = request + burst_band(burst_request_map[0][i]);//计算总请求带宽，用时长来衡量
					}
				}
				
				
				
				
				
				
				//printf("request_frame:%d---------\n",request_frame);
				if(request_frame != 0)
				{
					
				
				//按px[i]的顺序安排NC到NT的业务:包含了广播地址
						rest=0.0;
						w1=0;
						for(w1=0;w1<31;w1++)
						{
						    
							if(burst_request_map[0][delay_num[w1]] != 0)
							{
								
								if(rest+burst_band(burst_request_map[0][delay_num[w1]]) <= NC_DownLink_band)
									{
									NC_send_task_delay[delay_num[w1]] = rest;
									NC_send_task_num[delay_num[w1]] = burst_request_map[0][delay_num[w1]];
				
									//更新信用值
									NC_send_credit[delay_num[w1]] = NC_send_credit[delay_num[w1]] - burst_band(NC_send_task_num[delay_num[w1]]);//
									rest = rest + burst_band(burst_request_map[0][delay_num[w1]]);
									}
								else
									{
									remain_time_part_one = NC_DownLink_band - rest;
									if(remain_time_part_one >= NCNT_at_least_time)
										{
										NC_send_task_delay[delay_num[w1]] = rest;
										NC_send_task_num[delay_num[w1]] = burst_change_frame_number(remain_time_part_one);
				
										//更新信用值
										NC_send_credit[delay_num[w1]] = NC_send_credit[delay_num[w1]]  - burst_band(NC_send_task_num[delay_num[w1]]);//
										}
									break;
									}
							}
							
						}
						//printf("NC_send_credit[0]:%f-----1-----Assured_band[0][px[w1]]:\n");
				
				}
				
				
				//安排NT到NC的任务
				request_frame=0;//重新初始化
				request=0.0;//总请求大小
				for(i=0;i<32;i++)
				{
					if(burst_request_map[1][i] != 0)
					{
						request_frame = request_frame + burst_request_map[1][i];
						request = request + burst_band(burst_request_map[1][i]);//计算总请求带宽，用时长来衡量
					}
				}
				
				if(request_frame != 0)
				{
					//if(request <= band)//总请求小于等于总剩余带宽：时长来衡量
					//{
						//信用值排序，主要记录信用值序号的排列顺序:自己到自己也有一个信用值，这个值始终为0
						for(i=0;i<32;i++)
						{
							temp[i] = NC_re_credit[i];
							px[i] = i;
						}
					
						for(i=0;i<32;i++)
						{
							iMax=i;
							vMax=temp[i];
							
							for(j=i;j<32;j++)
							{
								if(temp[j] > vMax)
								{
									iMax = j;
									vMax = temp[j];
								}
							}
							
							m = temp[i];
							temp[i] = temp[iMax];
							temp[iMax] = m;
							
							n = px[i];  //px[i]就是输出序号按顺序存入的数组
							px[i] = px[iMax];
							px[iMax] = n;
						}
						
						
						//按px[i]的顺序安排NT到NC的业务
						rest=0.0;
						w1=0;
						
						for(w1=0;w1<32;w1++)
						{
							if(burst_request_map[1][px[w1]] != 0)//在此处自己到自己的业务为0，不会被安排
							{
								if(rest+ burst_band(burst_request_map[1][px[w1]]) <= Total_burst_band)
									{
									NC_re_task_delay[px[w1]] = rest;
									NC_re_task_num[px[w1]] = burst_request_map[1][px[w1]];
									//更新信用值
									NC_re_credit[px[w1]] = NC_re_credit[px[w1]] - burst_band(NC_re_task_num[px[w1]]);//
									rest =rest + burst_band(burst_request_map[1][px[w1]]);
									}
								else
									{
									remain_time_part_one = Total_burst_band - rest;
									if(remain_time_part_one >= NCNT_at_least_time)
										{
										NC_re_task_delay[px[w1]] = rest;
										NC_re_task_num[px[w1]] = burst_change_frame_number(remain_time_part_one);
										//更新信用值
										NC_re_credit[px[w1]] = NC_re_credit[px[w1]] - burst_band(NC_re_task_num[px[w1]]);// 
										}
									break;
									}
							}
				
						}
				
				}
				
				
				//安排NT到NT的任务:NT1~NT31的接收任务
				for(h=0;h<31;h++)
				{
				
				
				//安排NT到NT的任务:NTi的接收任务
				int request_frame=0;//重新初始化
				double request=0.0;//总请求大小
				for(i=0;i<32;i++)
				{
					if(burst_request_map[h+2][i] != 0)
					{
						request_frame = request_frame + burst_request_map[h+2][i];
						request = request + burst_band_NT(burst_request_map[h+2][i]);//计算总请求带宽，用时长来衡量
					}
				}
				
				
				if(request_frame != 0)
				{
					
				
						//信用值排序，主要记录信用值序号的排列顺序:自己到自己也有一个信用值，这个值始终为0
						for(i=0;i<32;i++)
						{
							temp[i] = NT_re_credit[h][i];
							px[i] = i;
						}
					
						for(i=0;i<32;i++)
						{
							iMax=i;
							vMax=temp[i];
							
							for(j=i;j<32;j++)
							{
								if(temp[j] > vMax)
								{
									iMax = j;
									vMax = temp[j];
								}
							}
							
							m = temp[i];
							temp[i] = temp[iMax];
							temp[iMax] = m;
							
							n = px[i];  //px[i]就是输出序号按顺序存入的数组
							px[i] = px[iMax];
							px[iMax] = n;
						}
						
					
						
						
						//没有NC到NT1的任务，也没有广播任务的情况下：按px[i]的顺序安排NT到NT1的业务
						if(NC_send_task_num[h] ==0 && NC_send_task_num[31] ==0)
						{
							rest=0.0;
						    
							for(k=0;k<32;k++)
							{
								if(burst_request_map[h+2][px[k]] != 0)//在此处自己到自己的业务为0，不会被安排
								{
									
									if(rest+burst_band_NT(burst_request_map[h+2][px[k]]) <= Total_burst_band)
									{
										NT_re_task_delay[h][px[k]] = rest;
										NT_re_task_num[h][px[k]] = burst_request_map[h+2][px[k]];
				
										//更新信用值
										NT_re_credit[h][px[k]] = NT_re_credit[h][px[k]]  - burst_band_NT(NT_re_task_num[h][px[k]]);// 
										rest = rest+burst_band_NT(burst_request_map[h+2][px[k]]);
									}
									else
									{
									    remain_time_part_one = Total_burst_band -rest;
										if(remain_time_part_one >= NT2NT_at_least_time)
											{
											NT_re_task_delay[h][px[k]] = rest;
										    NT_re_task_num[h][px[k]] = burst_change_NT2NT_frame_number(remain_time_part_one);
				
										    //更新信用值
										    NT_re_credit[h][px[k]] = NT_re_credit[h][px[k]]  - burst_band_NT(NT_re_task_num[h][px[k]]);//
											}
										break;
									 }
								}
							}
						}
						
						
						
						
						//有NC到NT1的任务,但没有广播任务的情况下：按px[i]的顺序安排NT到NT1的业务
						if(NC_send_task_num[h] !=0 && NC_send_task_num[31] ==0)
						{
							//按信用值的顺序计算NC到NT1的任务前面能安排几个请求:能安排前w个任务
							rest = 0.0;
							w1=0;
							burst_part_one = NC_send_task_delay[h];
							task_has_cut_off = 0;
							//计算0~NC_send_task_delay[h]的时间段能够安排的任务个数
							if(burst_part_one >= NT2NT_at_least_time)//0~NC_send_task_delay[h]的时间段至少能安排传输一个数据帧的任务,计算能在第一个时间段安排的任务个数w
							 {
								for(w1=0;w1<32;w1++)
							    {
								   if(burst_request_map[h+2][px[w1]] != 0)
								   {	
								      if(  (rest + burst_band_NT(burst_request_map[h+2][px[w1]])) <= burst_part_one  )//如果0~NC_send_task_delay[h]的时间段能够安排下第W个任务
									   {
									    NT_re_task_delay[h][px[w1]] = rest;//第w1个任务的时间起点
									    NT_re_task_num[h][px[w1]] = burst_request_map[h+2][px[w1]];//第w1个任务的数据帧个数
									 
									    rest = rest + burst_band_NT(burst_request_map[h+2][px[w1]]);
									    //更新信用值
									    NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]]  - burst_band_NT(NT_re_task_num[h][px[w1]]);//
									   }
								      else//剩余带宽不能完整的安排第w1个请求，能够安排的传输的请求个数为burst_change_NT2NT_frame_number(burst_remain_time)个
									   {
									    remain_time_part_one = burst_part_one - rest;
									    if(remain_time_part_one >= NT2NT_at_least_time)
										    {
										    NT_re_task_delay[h][px[w1]] = rest;//计算该任务的时间起点
										    NT_re_task_num[h][px[w1]] = burst_change_NT2NT_frame_number(remain_time_part_one);//计算该任务能够发送的数据帧数
										 
										    burst_request_map[h+2][px[w1]] = burst_request_map[h+2][px[w1]] - NT_re_task_num[h][px[w1]];//更新该NT的还未安排的请求
										    //更新信用值
									        NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]]   - burst_band_NT(NT_re_task_num[h][px[w1]]);//
										    task_has_cut_off = 1 ;//标记该任务已经被分割
										    }
									    break;
									   }
								   }
								
							    }
							 }
							else//0~NC_send_task_delay[h]的时间段不能安排一个任务，因此需要从NC_send_task_delay[h]+burst_band(NC_send_task_num[h])时间点开始安排任务
							{
							w1=0; 
							}
							
							//计算NC_send_task_delay[h]+burst_band(NC_send_task_num[h])到后面的时间段能够安排的任务个数
							rest=0.0;
							w2=0;
							
							
							burst_part_two = Total_burst_band - NC_send_task_delay[h] - burst_band(NC_send_task_num[h]);//计算突发业务时间段被打断的第二个时间段的长度
							if(burst_part_two >= NT2NT_at_least_time)
							{
							  for(w2=w1;w2<32;w2++)
							  {
							      if(burst_request_map[h+2][px[w2]] != 0)
								  {			
								     if(rest+burst_band_NT(burst_request_map[h+2][px[w2]]) < burst_part_two )
								     {
									  
									     if(w2 == w1)
									     {
										     if(task_has_cut_off==1)//判断0~NC_send_task_delay[h]时间段是否分割了一个请求
											  {
											  NT_re_task_delay1[h][px[w1]] = rest+NC_send_task_delay[h] + burst_band(NC_send_task_num[h]);//计算该任务的时间起点
											  NT_re_task_num1[h][px[w1]] = burst_request_map[h+2][px[w1]];
											  
											  //更新信用值
									          NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]] - burst_band_NT(NT_re_task_num1[h][px[w1]]);
											  task_has_cut_off =0;//task_has_cut_off置0
																								  
											  }
										     else
											  {
											  NT_re_task_delay[h][px[w1]] = rest+NC_send_task_delay[h] + burst_band(NC_send_task_num[h]);//计算该任务的时间起点
											  NT_re_task_num[h][px[w1]] = burst_request_map[h+2][px[w1]];
											  
											  //更新信用值
									          NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]]  - burst_band_NT(NT_re_task_num[h][px[w1]]);// 
											  }
									     }
									     else
									     {
										   NT_re_task_delay[h][px[w2]] = rest+NC_send_task_delay[h] + burst_band(NC_send_task_num[h]);//计算该任务的时间起点
										   NT_re_task_num[h][px[w2]] = burst_request_map[h+2][px[w2]];
											  
										   //更新信用值
									       NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]]  - burst_band_NT(NT_re_task_num[h][px[w2]]);// 
									     }
									    rest = rest+burst_band_NT(burst_request_map[h+2][px[w2]]);
								     }
								     else
								     {
								  	     remain_time_part_two = burst_part_two - rest;
									     if(remain_time_part_two >= NT2NT_at_least_time)
									     {
								  	        if(w2 == w1)
									        {
									           if(task_has_cut_off==1)
											   {
											    NT_re_task_delay1[h][px[w2]] = rest+NC_send_task_delay[h] + burst_band(NC_send_task_num[h]);//计算该任务的时间起点
												NT_re_task_num1[h][px[w2]] = burst_change_NT2NT_frame_number(remain_time_part_two);
												//更新信用值
									            NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]]  - burst_band_NT(NT_re_task_num1[h][px[w2]]);
											    task_has_cut_off =0;
											   }
											   else
											   {
											    NT_re_task_delay[h][px[w2]] = rest+NC_send_task_delay[h] + burst_band(NC_send_task_num[h]);//计算该任务的时间起点
												NT_re_task_num[h][px[w2]] = burst_change_NT2NT_frame_number(remain_time_part_two);
												//更新信用值
									            NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]] - burst_band_NT(NT_re_task_num[h][px[w2]]);//
											   }
									        }
										    else
										    {
										    NT_re_task_delay[h][px[w2]] = rest+NC_send_task_delay[h] + burst_band(NC_send_task_num[h]);//计算该任务的时间起点
										    NT_re_task_num[h][px[w2]] = burst_change_NT2NT_frame_number(remain_time_part_two);
										    //更新信用值
									        NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]]  - burst_band_NT(NT_re_task_num[h][px[w2]]);//
										    }
									     }
									     break;
									  }
								   }
							
							  }
							}
											
						}
				
				
					
						//没有NC到NT1的任务,但有广播任务的情况下：按px[i]的顺序安排NT到NT1的业务
						if(NC_send_task_num[h] ==0 && NC_send_task_num[31] !=0)
						{
							//按信用值的顺序计算NC到NT1的任务前面能安排几个请求:能安排前w个任务
							rest = 0.0;
							w1=0;
							burst_part_one = NC_send_task_delay[31];
							//安排0~NC_send_task_delay[31]的时间段的任务
							if(burst_part_one >= NT2NT_at_least_time)//0~NC_send_task_delay[31]的时间段至少能安排传输一个数据帧的任务,计算能在第一个时间段安排的任务个数w
							 {
								for(w1=0;w1<32;w1++)
							    {
									if(burst_request_map[h+2][px[w1]] !=0)
									{
					
										if(  (rest + burst_band_NT(burst_request_map[h+2][px[w1]])) <= burst_part_one  )//如果0~NC_send_task_delay[31]的时间段能够安排下第W个任务
											{
											NT_re_task_delay[h][px[w1]] = rest;//第w1个任务的时间起点
											NT_re_task_num[h][px[w1]] = burst_request_map[h+2][px[w1]];//第w1个任务的数据帧个数
									 
											rest = rest + burst_band_NT(burst_request_map[h+2][px[w1]]);
											//更新信用值
											NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]]  - burst_band_NT(NT_re_task_num[h][px[w1]]);//
									 
											}
										else//剩余带宽不能完整的安排第w1个请求，能够安排的传输的请求个数为burst_change_NT2NT_frame_number(burst_remain_time)个
											{
											remain_time_part_one = NC_send_task_delay[31]-rest;
											if(remain_time_part_one >= NT2NT_at_least_time)
												{
												NT_re_task_delay[h][px[w1]] = rest;//计算该任务的时间起点
												NT_re_task_num[h][px[w1]] = burst_change_NT2NT_frame_number(remain_time_part_one);//计算该任务能够发送的数据帧数
										 
												burst_request_map[h+2][px[w1]] = burst_request_map[h+2][px[w1]] - NT_re_task_num[h][px[w1]];//更新该NT的还未安排的请求
												//更新信用值
												NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]]  - burst_band_NT(NT_re_task_num[h][px[w1]]);//
												task_has_cut_off = 1 ;//标记该任务已经被分割
												}
											break;
											}
									}	
							    }
							 }
							else//0~NC_send_task_delay[31]的时间段不能安排一个任务，因此需要从NC_send_task_delay[31]+burst_band(NC_send_task_num[31])时间点开始安排任务
							{
							w1=0; 
							}
							
							//安排NC_send_task_delay[31]+burst_band(NC_send_task_num[31])到后面的时间段的任务
							rest=0.0;
							w2=0;
							
							
							burst_part_two = Total_burst_band - NC_send_task_delay[31] - burst_band(NC_send_task_num[31]);//计算突发业务时间段被打断的第二个时间段的长度
							if(burst_part_two >= NT2NT_at_least_time)
							{
							  for(w2=w1;w2<32;w2++)
							  {
							  	if(burst_request_map[h+2][px[w2]] != 0)
								{
									if(rest+burst_band_NT(burst_request_map[h+2][px[w2]]) < burst_part_two )
									{
									  
										if(w2 == w1)
										{
											if(task_has_cut_off==1)//判断0~NC_send_task_delay[31]时间段是否分割了一个请求
												{
												NT_re_task_delay2[h][px[w1]] = rest+NC_send_task_delay[31] + burst_band(NC_send_task_num[31]);//计算该任务的时间起点
												NT_re_task_num2[h][px[w1]] = burst_request_map[h+2][px[w1]];
											  
												//更新信用值
												NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]] - burst_band_NT(NT_re_task_num2[h][px[w1]]);
												task_has_cut_off =0;//task_has_cut_off置0
																								  
												}
											else
												{
												NT_re_task_delay[h][px[w1]] = rest+NC_send_task_delay[31] + burst_band(NC_send_task_num[31]);//计算该任务的时间起点
												NT_re_task_num[h][px[w1]] = burst_request_map[h+2][px[w1]];
											  
												//更新信用值
												NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]]  - burst_band_NT(NT_re_task_num[h][px[w1]]);//
												}
										}
										else
										{
										NT_re_task_delay[h][px[w2]] = rest+NC_send_task_delay[31] + burst_band(NC_send_task_num[31]);//计算该任务的时间起点
										NT_re_task_num[h][px[w2]] = burst_request_map[h+2][px[w2]];
											  
										//更新信用值
										NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]] - burst_band_NT(NT_re_task_num[h][px[w2]]);//  
										}
										rest = rest+burst_band_NT(burst_request_map[h+2][px[w2]]);
									}
									else
									{
									remain_time_part_two = burst_part_two - rest;
									if(remain_time_part_two >= NT2NT_at_least_time)
										{
										if(w2 == w1)
											{
									        if(task_has_cut_off==1)
												{
											    NT_re_task_delay2[h][px[w2]] = rest+NC_send_task_delay[31] + burst_band(NC_send_task_num[31]);//计算该任务的时间起点
												NT_re_task_num2[h][px[w2]] = burst_change_NT2NT_frame_number(remain_time_part_two);
												//更新信用值
									            NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]]  - burst_band_NT(NT_re_task_num2[h][px[w2]]);
												task_has_cut_off =0;
												}
											else
											   {
											    NT_re_task_delay[h][px[w2]] = rest+NC_send_task_delay[31] + burst_band(NC_send_task_num[31]);//计算该任务的时间起点
												NT_re_task_num[h][px[w2]] = burst_change_NT2NT_frame_number(remain_time_part_two);
												//更新信用值
									            NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]]  - burst_band_NT(NT_re_task_num[h][px[w2]]);// 
											   }
											}
										else
										{
										 NT_re_task_delay[h][px[w2]] = rest+NC_send_task_delay[31] + burst_band(NC_send_task_num[31]);//计算该任务的时间起点
										 NT_re_task_num[h][px[w2]] = burst_change_NT2NT_frame_number(remain_time_part_two);
										 //更新信用值
									     NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]]  - burst_band_NT(NT_re_task_num[h][px[w2]]);//
										}
									}
									break;
								  }
								}
							
							  }
							}		
							
						}
				
						
						
						
						//有NC到NT1的任务,也有广播任务的情况下：按px[i]的顺序安排NT到NT1的业务
						if(NC_send_task_num[h] !=0 && NC_send_task_num[31] !=0)
						{	
							//判断两个delay的大小关系
							if(NC_send_task_delay[h] < NC_send_task_delay[31])
							{
								delay1 = NC_send_task_delay[h];
								num1 = NC_send_task_num[h];
								delay2 =NC_send_task_delay[31];
								num2 =NC_send_task_num[31];
							}
							else
							{
								delay1 = NC_send_task_delay[31];
								num1 = NC_send_task_num[31];
								delay2 =NC_send_task_delay[h];
								num2 =NC_send_task_num[h];
							}
							
							
							
							
							rest = 0.0;
							w1=0;
							burst_part_one = delay1;
							//安排0~delay1时间段的任务
							if(burst_part_one >= NT2NT_at_least_time)//0~delay1的时间段至少能安排传输一个数据帧的任务,
							 {
								for(w1=0;w1<32;w1++)
							    {
									if(burst_request_map[h+2][px[w1]] != 0)
									{
									//if(px[w1]==9)
										//{printf("***********%d**************NT9**********1111*************\n",h+1);}
										
										if(  (rest + burst_band_NT(burst_request_map[h+2][px[w1]])) <= burst_part_one  )//如果0~delay1的时间段能够安排下第W个任务
											{
											NT_re_task_delay[h][px[w1]] = rest;//第w1个任务的时间起点
											NT_re_task_num[h][px[w1]] = burst_request_map[h+2][px[w1]];//第w1个任务的数据帧个数
									 
											rest = rest + burst_band_NT(burst_request_map[h+2][px[w1]]);
											//更新信用值
											NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]] - burst_band_NT(NT_re_task_num[h][px[w1]]);//  
											}
										else//剩余带宽不能完整的安排第w1个请求，能够安排的传输的请求个数为burst_change_NT2NT_frame_number(remain_time_part_one)个
											{
											remain_time_part_one = burst_part_one - rest;
											if(remain_time_part_one >= NT2NT_at_least_time)
												{
												NT_re_task_delay[h][px[w1]] = rest;//计算该任务的时间起点
												NT_re_task_num[h][px[w1]] = burst_change_NT2NT_frame_number(remain_time_part_one);//计算该任务能够发送的数据帧数
										 
												burst_request_map[h+2][px[w1]] = burst_request_map[h+2][px[w1]] - NT_re_task_num[h][px[w1]];//更新该NT的还未安排的请求
												//更新信用值
												NT_re_credit[h][px[w1]] = NT_re_credit[h][px[w1]]  - burst_band_NT(NT_re_task_num[h][px[w1]]);// 第w1个任务被分割，下一次再给px[w1]分配带宽时，信用值的计算会不一样
												task_has_cut_off = 1 ;//标记该任务已经被分割
												}
											break;
											}
									}
								
							    }
							 }
							else//0~delay1的时间段不能安排一个任务，因此需要从delay1+burst_band(num1)时间点开始安排任务
							{
							w1=0; 
							}
							
							
							//安排delay1+burst_band(num1)到delay2的时间段的任务
							rest=0.0;
							w2=0;
							burst_part_two = delay2 - delay1 - burst_band(num1);//计算突发业务时间段被打断的第二个时间段的长度
							
							if(burst_part_two >= NT2NT_at_least_time)
							{
							  for(w2=w1;w2<32;w2++)
							  {
							  	if(burst_request_map[h+2][px[w2]] != 0)
								{
									//if(px[w2]==9)
										//{printf("**********%d***************NT9**************2222********\n",h+1);}
									if(rest+burst_band_NT(burst_request_map[h+2][px[w2]]) <= burst_part_two )
										{
									  
										if(w2 == w1 && task_has_cut_off==1)  //判断0~delay1时间段是否分割了一个请求
											{
											NT_re_task_delay1[h][px[w2]] = rest+delay1 + burst_band(num1);//计算该任务的时间起点
											NT_re_task_num1[h][px[w2]] = burst_request_map[h+2][px[w2]];
											  
											//更新信用值
											NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]] - burst_band_NT(NT_re_task_num1[h][px[w2]]);
											task_has_cut_off =0;//task_has_cut_off置0													  
											}
										else
											{
											NT_re_task_delay[h][px[w2]] = rest+delay1 + burst_band(num1);//计算该任务的时间起点
											NT_re_task_num[h][px[w2]] = burst_request_map[h+2][px[w2]];
											  
											//更新信用值
											NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]]   - burst_band_NT(NT_re_task_num[h][px[w2]]);	//				 
											}
										rest = rest+burst_band_NT(burst_request_map[h+2][px[w2]]);
										}
									else
										{
										remain_time_part_two = burst_part_two - rest;
										if(w2 == w1 && task_has_cut_off==1)
										{
											if(remain_time_part_two >= NT2NT_at_least_time)
												{
												NT_re_task_delay1[h][px[w2]] = rest+delay1 + burst_band(num1);//计算该任务的时间起点
												NT_re_task_num1[h][px[w2]] = burst_change_NT2NT_frame_number(remain_time_part_two);
												burst_request_map[h+2][px[w2]] = burst_request_map[h+2][px[w2]] - NT_re_task_num1[h][px[w2]];//更新该NT的还未安排的请求 
												//更新信用值
												NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]] - burst_band_NT(NT_re_task_num1[h][px[w2]]);
												task_has_cut_off1=1;//说明一个任务被同时打断了两次
												}
											
											}
											else
											{
									    
											  if(remain_time_part_two >= NT2NT_at_least_time)
												{
												NT_re_task_delay[h][px[w2]] = rest+delay1 + burst_band(num1);//计算该任务的时间起点
												NT_re_task_num[h][px[w2]] = burst_change_NT2NT_frame_number(remain_time_part_two);
												burst_request_map[h+2][px[w2]] = burst_request_map[h+2][px[w2]] - NT_re_task_num[h][px[w2]];//更新该NT的还未安排的请求	  
												//更新信用值
												NT_re_credit[h][px[w2]] = NT_re_credit[h][px[w2]]  - burst_band_NT(NT_re_task_num[h][px[w2]]);//
												task_has_cut_off = 1;
												}
										
											}
											break;
										}
								}
							
							  }
							}
							else
							{
							w2=w1;
							}
							
							
							
							//安排delay2+burst_band(num2)到band的时间段的任务
							rest=0.0;
							w3=0;
							burst_part_three = Total_burst_band - delay2 - burst_band(num2);//计算突发业务时间段被打断的第三个时间段的长度
							
							if(burst_part_three >= NT2NT_at_least_time)
							{
							  for(w3=w2;w3<32;w3++)
							  {
							  		if(burst_request_map[h+2][px[w3]] != 0)
									{
									//if(px[w3]==9)
										//{printf("************%d*************NT9************3333***********\n",h+1);}
										if(rest+burst_band_NT(burst_request_map[h+2][px[w3]]) <=  burst_part_three )
											{
											if(w3 == w2 && task_has_cut_off == 1) //判断delay1+burst_band(num2)~delay2时间段是否分割了一个请求,一个任务被打断一次
												{
												NT_re_task_delay2[h][px[w3]] = rest+delay2 + burst_band(num2);//计算该任务的时间起点
												NT_re_task_num2[h][px[w3]] = burst_request_map[h+2][px[w3]];
											  
												//更新信用值
												NT_re_credit[h][px[w3]] = NT_re_credit[h][px[w3]] - burst_band_NT(NT_re_task_num2[h][px[w3]]);
												task_has_cut_off = 0;//task_has_cut_off置0	
												}
											else 
												{
												NT_re_task_delay[h][px[w3]] = rest+delay2 + burst_band(num2);//计算该任务的时间起点
												NT_re_task_num[h][px[w3]] = burst_request_map[h+2][px[w3]];
											  
												//更新信用值
												NT_re_credit[h][px[w3]] = NT_re_credit[h][px[w3]]   - burst_band_NT(NT_re_task_num[h][px[w3]]);	//				 
												}
											rest = rest+burst_band_NT(burst_request_map[h+2][px[w3]]);
											}
										else
											{
											remain_time_part_three = burst_part_three - rest;
											if(w3 == w2 && task_has_cut_off == 1) //判断delay1+burst_band(num2)~delay2时间段是否分割了一个请求,一个任务被打断一次
												{
												NT_re_task_delay2[h][px[w3]] = rest+delay2 + burst_band(num2);//计算该任务的时间起点
												NT_re_task_num2[h][px[w3]] = burst_change_NT2NT_frame_number(remain_time_part_three);
											  
												//更新信用值
												NT_re_credit[h][px[w3]] = NT_re_credit[h][px[w3]] - burst_band_NT(NT_re_task_num2[h][px[w3]]);
												task_has_cut_off = 0;//task_has_cut_off置0	
												}
											else 
												{
												NT_re_task_delay[h][px[w3]] = rest+delay2 + burst_band(num2);//计算该任务的时间起点
												NT_re_task_num[h][px[w3]] = burst_change_NT2NT_frame_number(remain_time_part_three);
											  
												//更新信用值
												NT_re_credit[h][px[w3]] = NT_re_credit[h][px[w3]]  - burst_band_NT(NT_re_task_num[h][px[w3]]);	// 				 
												}
											break;
											}
									}
									
							  }
							}
					
						}	
				}
				
				
				}//这个大括号是NT1~NT31 for循环的大括号
				
				
				//更新信用值
				for(i=0;i<32;i++)
				{
					NC_send_credit[i]=NC_send_credit[i]+Assured_band[0][i+1];
					NC_re_credit[i]=NC_re_credit[i] +Assured_band[1][i] ;
				}
				
				for(i=0;i<31;i++)
					for(j=0;j<32;j++)
						NT_re_credit[i][j]=NT_re_credit[i][j]+Assured_band[i+2][j];
				
				
				
				
				//输出本DBA周期的任务列表
				if(op_sim_time()>24320)
					{
				
				for(i=0;i<32;i++)
					{
					if(NC_send_task_num[i] !=0)
					printf("NC send list___________%d__________%f___________%d\n",i+1,NC_send_task_delay[i],NC_send_task_num[i]);
					}
				
				/*
				for(i=0;i<32;i++)
					{
					if(NC_re_task_num[i] !=0)
					printf("NC re list___________%d__________%f___________%d\n",i,NC_re_task_delay[i],NC_re_task_num[i]);
					}
				
				for(i=0;i<31;i++)
					{
					for(j=0;j<32;j++)
						{
						if(NT_re_task_num[i][j] !=0)
						  printf("NT-NT list ___________%d___________%d____________%f___________%d\n",i+1,j,NT_re_task_delay[i][j],NT_re_task_num[i][j]);
						if(NT_re_task_num1[i][j] !=0)
						  printf("NT-NT 时间段1被打断 list ___________%d___________%d____________%f___________%d\n",i+1,j,NT_re_task_delay1[i][j],NT_re_task_num1[i][j]);
						if(NT_re_task_num2[i][j] !=0)
						  printf("NT-NT 时间段1被打断 list ___________%d___________%d____________%f___________%d\n",i+1,j,NT_re_task_delay2[i][j],NT_re_task_num2[i][j]);
						if(NT_re_task_num3[i][j] !=0)
						  printf("NT-NT 一个任务被打断两次 list ___________%d___________%d____________%f___________%d\n",i+1,j,NT_re_task_delay3[i][j],NT_re_task_num3[i][j]);
						}
					}
				*/
				}
				
				//突发业务发送时间到（自中断）
				op_intrpt_schedule_self(op_sim_time() + DBA_calc_spacing, burst_time);
				
				
				/*
				DBA_num=(int)(op_sim_time()/DBA_long);
				if((DBA_num%100)==0)
					{
					for(i=0;i<32;i++)
					{
					NC_send_credit[i]=0.0;
					
					}
				
					
					}
				*/
				//op_stat_write (Link_credit_stathandle, 	0.0);NC_traffic_request[64]
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (DBA_Process) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "DBA_Process", "NC_Processor [DBA_Process exit execs]")


			/** state (DBA_Process) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "DBA_Process", "Idle", "tr_10", "NC_Processor [DBA_Process -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Frame_Pro) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "Frame_Pro", state3_enter_exec, "NC_Processor [Frame_Pro enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Processor [Frame_Pro enter execs]", state3_enter_exec)
				{
				
				char *p[]={"Request_1","Request_2","Request_3","Request_4","Request_5","Request_6","Request_7","Request_8","Request_9","Request_10","Request_11","Request_12","Request_13","Request_14","Request_15","Request_16","Request_17","Request_18","Request_19","Request_20","Request_21","Request_22","Request_23","Request_24","Request_25","Request_26","Request_27","Request_28","Request_29","Request_30","Request_31","Request_32"};
				
				
				input_strm_index=op_intrpt_strm();
				ptr=op_pk_get(input_strm_index);
				op_pk_nfd_get(ptr,"Frame_type",&frame_type);
				total_throughput = total_throughput + op_pk_total_size_get(ptr);
				op_pk_nfd_get(ptr,"S_ID",&source);
				throughput_different_link[source][0]=throughput_different_link[source][0]+op_pk_total_size_get(ptr);
				
				//frame_type:0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
				
				
				//total_rec_pks ++;
				//数据帧操作：销毁
				if(frame_type==1)
				{
					
					op_pk_nfd_get(ptr,"Traffic_type",&traffic_type);
					op_pk_nfd_get(ptr,"S_ID",&source);
					op_pk_nfd_get(ptr,"data",&Dataunit_ptr);
					op_pk_destroy(Dataunit_ptr);
					
					if(traffic_type == 2)//突发业务返回
					  {
					  total_source_throughput=total_source_throughput+2112*8;//统计纯业务数据的吞吐量
					  burst_delay = op_sim_time () - op_pk_creation_time_get (ptr);
					  burst_ete_sum_delay = burst_ete_sum_delay + burst_delay ;
					 if (burst_delay > burst_ete_max_delay)
							{
							burst_ete_max_delay = burst_delay;
							}
					  burst_pks_receive ++;
					  
					  //统计不同连接的最大时延、平均时延
					  if(burst_delay > burst_ete_max_delay_different_link[source][0])
						{
						burst_ete_max_delay_different_link[source][0]=burst_delay;
						}
					  burst_ete_sum_delay_different_link[source][0]=burst_ete_sum_delay_different_link[source][0]+burst_delay;
					  burst_packet_number_different_link[source][0]++;
				
					  
					  }  
					op_pk_destroy(ptr);
				}
				
				//状态帧操作：统计一个DBA周期内状态帧的个数，销毁
				else if(frame_type==2)
				{
					op_pk_nfd_get(ptr,"Traffic_type",&traffic_type);
					
					
					
					if (traffic_type == 0)//周期性消息跟随状态帧返回
					  {
					 
					  pksize=op_pk_total_size_get(ptr);
					  //接收到NT返回带周期性消息的状态帧
					  if(pksize > 4000)
						  {
						  total_source_throughput=total_source_throughput+4096;//统计纯业务数据的吞吐量
						   op_pk_nfd_get(ptr,"data",&Dataunit_ptr);
						   op_pk_destroy(Dataunit_ptr);
						  per_delay = op_sim_time () - op_pk_creation_time_get (ptr);
						  period_ete_sum_delay = period_ete_sum_delay + per_delay ;
						  if (per_delay > period_ete_max_delay)
							  {
							  period_ete_max_delay = per_delay;
							  }
				
						  /*op_stat_write (ete_gsh, ete_delay);*/ 
						  period_pks_receive ++;
						  }
					  
					  }
					op_pk_destroy(ptr);
				}
				
				//请求回复操作：生成带宽请求列表，销毁
				else if(frame_type==4)
				{	
					req_num++;//统计回复请求的节点个数，每个DBA周期重新初始化
					op_pk_nfd_get(ptr,"S_ID",&source);//源地址
					if(req_num==9)
						{
						success_DBA_num++;
						}
					burst_delay = op_sim_time () - op_pk_creation_time_get (ptr);
					if(burst_delay>0.00002)
						{
						printf("request_delay:%f-------------------NT:%d---------------------\n",burst_delay,source);
						}
					
					
					
					
					for(i=0;i<32;i++)
					{
						request[i]=0;
					}
					
					
					
					//请求大小是帧个数
					for(i=0;i<32;i++)
						{
						op_pk_nfd_get(ptr,p[i],&request[i]);
						}
					
						
					
					
					
					
					for(i=0;i<32;i++)
					{
						burst_request_map[i+1][source]=request[i];
						
					}
					
				
					
					op_pk_destroy(ptr);
				}
					
				//RSCN帧操作：统计时延，销毁
				else if(frame_type==5)
				{
					op_pk_destroy(ptr);
				}
				
				
				
				
				
				
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (Frame_Pro) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "Frame_Pro", "NC_Processor [Frame_Pro exit execs]")


			/** state (Frame_Pro) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Frame_Pro", "Idle", "tr_4", "NC_Processor [Frame_Pro -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Control_Period) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "Control_Period", state4_enter_exec, "NC_Processor [Control_Period enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Processor [Control_Period enter execs]", state4_enter_exec)
				{
				
				double period_interval_time=0.0;
				//周期性业务请求表在初始化已制定，为period_request_map[2][31]
				
				
				
				
				//先发送NC到NT的周期性消息
				if(p_num<32)
				{
					if(period_request_map[0][p1_num]==1)//该周期性消息存在，则取数据
					{
						
						iciptr = op_ici_create ("NC_subque_ICI");
						op_ici_attr_set (iciptr, "subque_num",p1_num);//子队列号，与流号对应，但目的地址不是p1_num，
						op_ici_install (iciptr);
						op_strm_access(3);
						op_ici_install(OPC_NIL);
						
						if (op_strm_empty (3) == OPC_FALSE)
						{
							
							data_ptr=op_pk_get(3);
							pk_create_time = op_pk_creation_time_get(data_ptr);
							op_pk_nfd_get(data_ptr,"destination",&dest);
							pksize=op_pk_total_size_get(data_ptr);
						
							//生成命令帧+数据
							CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
							op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
							op_pk_nfd_set (CommondFrame_ptr, "D_ID", dest);//32是广播地址
							op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
							op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 0);
							op_pk_nfd_set (CommondFrame_ptr, "Task_type", 1);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT
							op_pk_nfd_set (CommondFrame_ptr, "data", op_pk_create(pksize));
							
							op_pk_creation_time_set(CommondFrame_ptr, pk_create_time);
							
							//period task send stat
							/*
							pro_period_pks_send ++;
							if(p_num==10)
								{
								if(pre_period_pks_send_time==0)
									{
									pre_period_pks_send_time = op_sim_time();
									}
								else
									{
									period_interval_time =op_sim_time() - pre_period_pks_send_time;
									if(period_interval_time > period_max_interval_time)
										{
										period_max_interval_time = period_interval_time;
										}
									pre_period_pks_send_time = op_sim_time();
									}
								period_send_interval_time = period_send_interval_time+period_interval_time;
								
								}
							*/
				
							op_pk_send(CommondFrame_ptr,0);
						
							//检测队列模块中有没有强时效，有就发下来，流索引号是子队列号；没有就不处理
							for(i=0;i<strong_burst_check_number;i++)
								{
								strong_burst();
								}
							
							//产生下一周期性业务的自中断
							op_intrpt_schedule_self(op_sim_time()+period_spacing, period_time);
						}
					}else
					{
						
						op_intrpt_schedule_self(op_sim_time() + 0.0, period_time);
					}
					
					if(p1_num==31)
					{
						p1_num=0;
					}else
					{
						p1_num++;
					}
					
					p_num++;
					//printf("p1_num %d\n",p1_num);
					//printf("p_num %d\n",p_num);
					
				}
				
				
				//再发送NT到NC的周期性消息
				else if(p_num>=32 && p_num<63)//不要求NT发广播，所以少一个地址
				{
				//printf("**************16******************\n");
				    
					if(period_request_map[1][p2_num]==1)//该周期性消息存在，则发送命令帧
					{
					
						//生成命令帧
						CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
						op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);
						op_pk_nfd_set (CommondFrame_ptr, "D_ID", p2_num+1);
						op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
						op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 0);
						op_pk_nfd_set (CommondFrame_ptr, "Task_type", 0);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT
					
					
						op_pk_send(CommondFrame_ptr,0);
					for(i=0;i<strong_burst_check_number;i++)
						{
						strong_burst();
						}
					
						//产生下一周期性业务的自中断
						op_intrpt_schedule_self(op_sim_time()+period_spacing, period_time);
					}
					else
					{
						
						op_intrpt_schedule_self(op_sim_time() + 0.0, period_time);
					}
					
					
					if(p2_num==30)
					{
						p2_num=0;
					}else
					{
						p2_num++;
					}
				
					p_num++;
					//printf("p_num %d\n",p_num);
					
				
				}
				
				//收集请求
				else if(p_num==63)
				{
					
					
					//收集NC自身突发请求，是32~63的子队列
					//首先重置突发业务请求表数组
					for(i=0;i<33;i++)
					{
						for(j=0;j<32;j++)
						{
							burst_request_map[i][j]=0;
						}
					}
					
					iciptr = op_ici_create ("NC_self_request_ICI");
					
					op_ici_install (iciptr);
					
					op_strm_access(3);
					
					op_ici_install(OPC_NIL);
					
					if(op_strm_connected (OPC_STRM_IN, 3) == OPC_TRUE)
					{
							
						if (op_strm_empty (3) == OPC_FALSE)
						{
							//若缓存模块的队列不为0，则读取其中的请求大小
							ptr=op_pk_get(3);
							
							
							for(i=0;i<32;i++)
							{
								request[i]=0;
							}
							
							//请求大小是帧个数
							op_pk_nfd_get(ptr,"Request_1",&request[0]);//目的地址是NT1的请求大小
							op_pk_nfd_get(ptr,"Request_2",&request[1]);//目的地址是NT2的请求大小
							op_pk_nfd_get(ptr,"Request_3",&request[2]);//目的地址是NT3的请求大小
							op_pk_nfd_get(ptr,"Request_4",&request[3]);
							op_pk_nfd_get(ptr,"Request_5",&request[4]);
							op_pk_nfd_get(ptr,"Request_6",&request[5]);
							op_pk_nfd_get(ptr,"Request_7",&request[6]);
							op_pk_nfd_get(ptr,"Request_8",&request[7]);
							op_pk_nfd_get(ptr,"Request_9",&request[8]);
							op_pk_nfd_get(ptr,"Request_10",&request[9]);
							op_pk_nfd_get(ptr,"Request_11",&request[10]);
							op_pk_nfd_get(ptr,"Request_12",&request[11]);
							op_pk_nfd_get(ptr,"Request_13",&request[12]);
							op_pk_nfd_get(ptr,"Request_14",&request[13]);
							op_pk_nfd_get(ptr,"Request_15",&request[14]);
							op_pk_nfd_get(ptr,"Request_16",&request[15]);
							op_pk_nfd_get(ptr,"Request_17",&request[16]);
							op_pk_nfd_get(ptr,"Request_18",&request[17]);
							op_pk_nfd_get(ptr,"Request_19",&request[18]);
							op_pk_nfd_get(ptr,"Request_20",&request[19]);
							op_pk_nfd_get(ptr,"Request_21",&request[20]);
							op_pk_nfd_get(ptr,"Request_22",&request[21]);
							op_pk_nfd_get(ptr,"Request_23",&request[22]);
							op_pk_nfd_get(ptr,"Request_24",&request[23]);
							op_pk_nfd_get(ptr,"Request_25",&request[24]);
							op_pk_nfd_get(ptr,"Request_26",&request[25]);
							op_pk_nfd_get(ptr,"Request_27",&request[26]);
							op_pk_nfd_get(ptr,"Request_28",&request[27]);
							op_pk_nfd_get(ptr,"Request_29",&request[28]);
							op_pk_nfd_get(ptr,"Request_30",&request[29]);
							op_pk_nfd_get(ptr,"Request_31",&request[30]);
							op_pk_nfd_get(ptr,"Request_32",&request[31]);//广播请求
							
							
							for(i=0;i<32;i++)
							{
								burst_request_map[0][i]=request[i];
							}
							
							
							//op_pk_destroy(data_ptr);
							//op_pk_destroy(ptr);
						}else
						{
							op_sim_end("NC self requests have not been recieved!","","","");
						}
						
					}else
					{
						op_sim_end("NC self requests collect failed!","","","");
					}
					
					
					//发送广播请求命令帧收集各NT请求
					req_num=0;//初始化突发业务请求回复的个数
					CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
					op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 3);
					op_pk_nfd_set (CommondFrame_ptr, "D_ID", 32);
					op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
					op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 2);//业务类型，0代表周期性，1代表强时效，2代表突发
					op_pk_nfd_set (CommondFrame_ptr, "Task_type", 1);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT
					//op_pk_nfd_set (CommondFrame_ptr, "data", op_pk_create(0));//inherited表明该字段的大小取决于创建包的大小
					op_pk_send(CommondFrame_ptr,0);
					//检测有没有强时效
					for(i=0;i<strong_burst_check_number;i++)
						{
						strong_burst();
						}
					
				
					k=0;//有多少个周期性业务
					for(i=0;i<2;i++)
					{	
						for(j=0;j<32;j++)
						{
							if(period_request_map[i][j]==1)
								k++;
						}	
					}
					
					
					
					//下个DBA周期自中断
					if(Period_Interarrival_Time==DBA_long)//周期性业务周期等于DBA周期时，下一个周期一定存在周期性业务
						{
						 op_intrpt_schedule_self(op_sim_time() + DBA_long - period_spacing*k, period_time);
						 strong_burst_limit_time=DBA_long -period_spacing*period_number;
						 Total_burst_band=DBA_long -period_spacing*period_number-request_spacing - DBA_calc_spacing;
						 p_num=0;//为下个DBA周期使用初始化 
						}
					else
						{
						Time_in_current_DBA = op_sim_time() - (int)(op_sim_time()/Period_Interarrival_Time)*Period_Interarrival_Time;//计算当前时间在周期性业务一个周期内的时间点
						if(Time_in_current_DBA < (Period_Interarrival_Time-DBA_long) && Time_in_current_DBA>=DBA_long)//判断下一个DBA周期是否含有周期性业务
							{
							op_intrpt_schedule_self(op_sim_time() + DBA_long, period_time);
							strong_burst_limit_time=DBA_long;
							Total_burst_band = DBA_long - request_spacing - DBA_calc_spacing;
							p_num=63;//下个DBA周期直接从请求收集开始
							}
						  else
							  if(Time_in_current_DBA <DBA_long )
								  {
								  op_intrpt_schedule_self(op_sim_time() + DBA_long - period_spacing*k, period_time);
								  strong_burst_limit_time=DBA_long - period_spacing*period_number;
								  Total_burst_band = DBA_long -period_spacing*period_number-request_spacing-DBA_calc_spacing;
								  p_num=63;//下个DBA周期直接从请求收集开始,无周期性业务
								  }
						  	else
								{
								op_intrpt_schedule_self(op_sim_time() + DBA_long, period_time);
								strong_burst_limit_time=DBA_long;
								Total_burst_band = DBA_long - request_spacing - DBA_calc_spacing;
								p_num=0;//下个DBA周期存在周期性业务
								}
						}
					
					
					
					
					
					
					
					//每个DBA需要重新置0的变量在这里设置！！！！！！！！！！
					for(i=0;i<32;i++)
					{
						NC_send_task_delay[i]=0.0;
						NC_send_task_num[i]=0;
					}
					
					for(i=0;i<32;i++)
					{	
						NC_re_task_delay[i]=0.0;
						NC_re_task_num[i]=0;
					}
					
					for(i=0;i<31;i++)
					{
						for(j=0;j<32;j++)
						{
							NT_re_task_delay[i][j]=0.0;
							NT_re_task_num[i][j]=0;
							NT_re_task_delay1[i][j]=0.0;
							NT_re_task_num1[i][j]=0;
							NT_re_task_delay2[i][j]=0.0;
							NT_re_task_num2[i][j]=0;
							NT_re_task_delay2[i][j]=0.0;
							NT_re_task_num2[i][j]=0;
						}
					}
					
					strong_cycle_num=0;
					
					//DBA计算时间到（自中断）
					op_intrpt_schedule_self(op_sim_time() + request_spacing, DBA_time);
					
					//强时效突发业务发送时间到（自中断）
					op_intrpt_schedule_self(op_sim_time() + 0.0, strong_burst_time);
					//printf("p_num %d\n",p_num);
				}
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (Control_Period) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "Control_Period", "NC_Processor [Control_Period exit execs]")


			/** state (Control_Period) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Control_Period", "Idle", "tr_6", "NC_Processor [Control_Period -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (Burst_Process) enter executives **/
			FSM_STATE_ENTER_FORCED (5, "Burst_Process", state5_enter_exec, "NC_Processor [Burst_Process enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Processor [Burst_Process enter execs]", state5_enter_exec)
				{
				
				
				//发送NC到NT的突发业务
				for(i=0;i<32;i++)
				{
				
				
					if(NC_send_task_num[i] !=0)
					{
						//先发命令帧出去
						CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
						op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
						op_pk_nfd_set (CommondFrame_ptr, "D_ID", i+1);//32是广播地址
						op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
						op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 2);//业务类型，0代表周期性，1代表强时效，2代表突发
						op_pk_nfd_set (CommondFrame_ptr, "Task_type", 1);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT		
						
						op_pk_send_delayed(CommondFrame_ptr,0,NC_send_task_delay[i]);
					
					
						//再发接下来的几个数据帧
						iciptr = op_ici_create ("NC_subque_ICI");
						op_ici_attr_set (iciptr, "subque_num",i+32);//子队列号
						op_ici_attr_set (iciptr, "frame_num",NC_send_task_num[i]);//要发的帧个数
						op_ici_install (iciptr);
						op_strm_access(5);
						op_ici_install(OPC_NIL);
						
						
						
						
						
						for(j=0;j<NC_send_task_num[i];j++)
						{
							if (op_strm_empty (5) == OPC_FALSE)
							{
								data_ptr=op_pk_get(5);
								pk_create_time = op_pk_creation_time_get(data_ptr);
								op_pk_nfd_get(data_ptr,"destination",&dest);
								pksize=op_pk_total_size_get(data_ptr);
								
								//生成数据帧
								DataFrame_ptr = op_pk_create_fmt ("fc_1553_Dataframe_format");
								op_pk_nfd_set (DataFrame_ptr, "Frame_type", 1);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
								op_pk_nfd_set (DataFrame_ptr, "D_ID", dest);//32是广播地址
								op_pk_nfd_set (DataFrame_ptr, "S_ID", 0);
								op_pk_nfd_set (DataFrame_ptr, "Traffic_type", 2);
								//op_pk_nfd_set (DataFrame_ptr, "Task_type", 1);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT
								
								if(j==NC_send_task_num[i]-1)//判断是不是最后一个数据帧
									op_pk_nfd_set (DataFrame_ptr, "Data_frame_location", 1);//1代表是最后一个数据帧
								else
									op_pk_nfd_set (DataFrame_ptr, "Data_frame_location", 0);
								
								op_pk_nfd_set (DataFrame_ptr, "data", op_pk_create(pksize));
								
								op_pk_creation_time_set(DataFrame_ptr, pk_create_time);
								
								pro_burst_pks_send ++;
								
								op_pk_send_delayed(DataFrame_ptr,0,NC_send_task_delay[i] + 66.0*8.0/3400000000.0 + (2154.0*8.0/3400000000.0)*j);//一个数据帧的长度为2154字节,一个命令帧长为66字节
							
								//printf("**************************** %0.8f *****************************\n",NC_send_task_delay[i]+66.0*8.0/3400000000.0 + (2154.0*8.0/3400000000.0)*j);
							}	
						}
					}
				}
				
				
				
				
				
				//发送NT到NC的突发业务命令帧
				for(i=0;i<32;i++)
				{
				
					if(NC_re_task_num[i] !=0)
					{
						//发命令帧出去
						CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
						op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
						op_pk_nfd_set (CommondFrame_ptr, "D_ID", i);
						op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
						op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 2);//业务类型，0代表周期性，1代表强时效，2代表突发
						op_pk_nfd_set (CommondFrame_ptr, "Task_type", 0);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT		
						op_pk_nfd_set (CommondFrame_ptr, "Data_bytes_count", NC_re_task_num[i]);//帧个数
						
						op_pk_send_delayed(CommondFrame_ptr,0,NC_re_task_delay[i]);
					}
					
				}
				
				
				//发送NT到NT的突发业务命令帧
				for(i=0;i<31;i++)
				{
					for(j=0;j<32;j++)
					{
						if(NT_re_task_num[i][j] !=0)
						{
							//发命令帧
							CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
							op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
							op_pk_nfd_set (CommondFrame_ptr, "D_ID", j);//此处目的地址是NT到NT里面的发送NT
							op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
							op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 2);//业务类型，0代表周期性，1代表强时效，2代表突发
							op_pk_nfd_set (CommondFrame_ptr, "Task_type", 2);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT		
							op_pk_nfd_set (CommondFrame_ptr, "Data_bytes_count", NT_re_task_num[i][j]);//帧个数
							op_pk_nfd_set (CommondFrame_ptr, "ReceiveNT_adress", i+1);//此处的地址是NT到NT里面的接收NT
						
							op_pk_send_delayed(CommondFrame_ptr,0,NT_re_task_delay[i][j]);
						}
						
						
						if(NT_re_task_num1[i][j] !=0)
						{
							//发命令帧
							CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
							op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
							op_pk_nfd_set (CommondFrame_ptr, "D_ID", j);//此处目的地址是NT到NT里面的发送NT
							op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
							op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 2);//业务类型，0代表周期性，1代表强时效，2代表突发
							op_pk_nfd_set (CommondFrame_ptr, "Task_type", 2);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT		
							op_pk_nfd_set (CommondFrame_ptr, "Data_bytes_count", NT_re_task_num1[i][j]);//帧个数
							op_pk_nfd_set (CommondFrame_ptr, "ReceiveNT_adress", i+1);//此处的地址是NT到NT里面的接收NT
						
							op_pk_send_delayed(CommondFrame_ptr,0,NT_re_task_delay1[i][j]);
						}
						
						if(NT_re_task_num2[i][j] !=0)
						{
							//发命令帧
							CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
							op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
							op_pk_nfd_set (CommondFrame_ptr, "D_ID", j);//此处目的地址是NT到NT里面的发送NT
							op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
							op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 2);//业务类型，0代表周期性，1代表强时效，2代表突发
							op_pk_nfd_set (CommondFrame_ptr, "Task_type", 2);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT		
							op_pk_nfd_set (CommondFrame_ptr, "Data_bytes_count", NT_re_task_num2[i][j]);//帧个数
							op_pk_nfd_set (CommondFrame_ptr, "ReceiveNT_adress", i+1);//此处的地址是NT到NT里面的接收NT
						
							op_pk_send_delayed(CommondFrame_ptr,0,NT_re_task_delay2[i][j]);
						}
						if(NT_re_task_num3[i][j] !=0)
						{
							//发命令帧
							CommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");
							op_pk_nfd_set (CommondFrame_ptr, "Frame_type", 0);//0是命令帧，1是数据帧，2是状态帧，3是请求命令帧，4是请求回复，5是RSCN帧
							op_pk_nfd_set (CommondFrame_ptr, "D_ID", j);//此处目的地址是NT到NT里面的发送NT
							op_pk_nfd_set (CommondFrame_ptr, "S_ID", 0);
							op_pk_nfd_set (CommondFrame_ptr, "Traffic_type", 2);//业务类型，0代表周期性，1代表强时效，2代表突发
							op_pk_nfd_set (CommondFrame_ptr, "Task_type", 2);//0代表NT到NC，1代表Nc到Nt，2代表NT到NT		
							op_pk_nfd_set (CommondFrame_ptr, "Data_bytes_count", NT_re_task_num3[i][j]);//帧个数
							op_pk_nfd_set (CommondFrame_ptr, "ReceiveNT_adress", i+1);//此处的地址是NT到NT里面的接收NT
						
							op_pk_send_delayed(CommondFrame_ptr,0,NT_re_task_delay3[i][j]);
						}
					}
				}
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state5_enter_exec)

			/** state (Burst_Process) exit executives **/
			FSM_STATE_EXIT_FORCED (5, "Burst_Process", "NC_Processor [Burst_Process exit execs]")


			/** state (Burst_Process) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Burst_Process", "Idle", "tr_12", "NC_Processor [Burst_Process -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (result) enter executives **/
			FSM_STATE_ENTER_UNFORCED (6, "result", state6_enter_exec, "NC_Processor [result enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Processor [result enter execs]", state6_enter_exec)
				{
				double thoughtout_sum=0.0;
				double thoughtout2_sum=0.0;
				double NC_send_throughput[32];
				double NC_re_throughput[32];
				double NT_re_throughput[32][32];
				double bbb1,ddd1,bbb2,ddd2,bbb3,ddd3;
				
				sim_time = op_sim_time();
				printf("success_DBA_num:%d\n",success_DBA_num);
				//各业务总发生包数 
				printf("the pro module pro_period_pks_send %d\n ",pro_period_pks_send);
				printf("the pro module pro_burst_pks_send %d\n ",pro_burst_pks_send);
				printf("the pro module pro_tcbm_pks_send %d\n ",pro_tcbm_pks_send);
				
				//各业务总接收包数
				printf("the pro module period_pks_receive %d\n ",period_pks_receive);
				printf("the pro module burst_pks_receive %d\n ",burst_pks_receive);
				printf("the pro module tcbm_pks_receive %d\n ",tcbm_pks_receive);
				
				//各业务最大时延
				printf("the pro module period_ete_max_delay %f\n ",period_ete_max_delay);
				printf("the pro module tcbm_ete_max_delay %f\n ",tcbm_ete_max_delay);
				printf("the pro module burst_ete_max_delay %f\n ",burst_ete_max_delay);
				
				printf("the pro module NC发送 burst_ete_max_delay %f\n ",NC_send_burst_ete_max_delay);
				
				//各业务平均时延
				printf("the pro module period_ete_aveg_delay %f\n ",period_ete_aveg_delay = (period_ete_sum_delay/period_pks_receive));
				printf("the pro module tcbm_ete_aveg_delay %f\n ",tcbm_ete_aveg_delay = (tcbm_ete_sum_delay/tcbm_pks_receive));
				printf("the pro module burst_ete_aveg_delay %f\n ",burst_ete_aveg_delay = (burst_ete_sum_delay/burst_pks_receive));
				
				//吞吐量
				printf("the pro module total_throughput %f\n ",total_throughput/op_sim_time());
				printf("the pro module total_source_throughput %f\n ",total_source_throughput/op_sim_time());
				period_ete_aveg_delay = period_ete_sum_delay/period_pks_receive;
				tcbm_ete_aveg_delay = tcbm_ete_sum_delay/tcbm_pks_receive;
				burst_ete_aveg_delay = burst_ete_sum_delay/burst_pks_receive;
				printf("%f	%f	%f	%f	%f	%f	%f	%f	%f",tcbm_ete_max_delay*1000000,tcbm_ete_aveg_delay*1000000,NC_send_burst_ete_max_delay*1000,burst_ete_max_delay*1000,burst_ete_aveg_delay*1000,(total_throughput/op_sim_time())/1000000000,(total_source_throughput/op_sim_time())/1000000000,control_command_nonSensitive_pks_ete_max_delay*1000,control_command_nonSensitive_pks_ete_sum_delay*1000/control_command_nonSensitive_pks_receive);
				
				//printf("%f	%f\n",control_command_nonSensitive_pks_ete_max_delay*1000,control_command_nonSensitive_pks_ete_sum_delay*1000/control_command_nonSensitive_pks_receive);
				
				
				
				
				//printf("period max interval time  %f-------min interval time %f\n",period_max_interval_time,period_min_interval_time);
				
				/*
				
				//不同连接的最大时延
				for(i=0;i<32;i++)
					{
					for(j=0;j<32;j++)
						{
						if(j==31)
							
							printf("%f\n",burst_ete_max_delay_different_link[i][j]);
						else
							
							printf("%f ",burst_ete_max_delay_different_link[i][j]);
						}
					}
				*/
				/*
				for(i=0;i<32;i++)
					{
					for(j=0;j<32;j++)
						{
						
						}
					}
				*/
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				
				/*
				//不同连接的平均时延
				for(i=0;i<32;i++)
					{
					for(j=0;j<32;j++)
						{
						if(j==31)
							printf("%f\n",burst_ete_sum_delay_different_link[i][j]/burst_packet_number_different_link[i][j]);
						else
							if(burst_ete_sum_delay_different_link[i][j]!=0 && burst_packet_number_different_link[i][j]!=0)
								printf("%f ",burst_ete_sum_delay_different_link[i][j]/burst_packet_number_different_link[i][j]);
						}
					}
				*/
				
				
				/*
				
				for(i=0;i<32;i++)
					{
					for(j=0;j<32;j++)
						{
						if(j==31)
							
							printf("%f\n",throughput_different_link[i][j]/op_sim_time());
						else
							if(throughput_different_link[i][j]!=0)
								printf("%f ",throughput_different_link[i][j]/op_sim_time());
						}
					}
				
				*/
				
				//计算公平性因子
				bbb1=0.0;
				ddd1=0.0;
				bbb2=0.0;
				ddd2=0.0;
				bbb3=0.0;
				ddd3=0.0;
				for(i=1;i<31;i++)
				{
					if(Assured_band[0][i]!=0)
						{
					NC_send_throughput[i] = throughput_different_link[0][i]/(Assured_band[0][i]*3400000000*250*op_sim_time());
					bbb1 = bbb1 + NC_send_throughput[i];
					ddd1 = ddd1 + NC_send_throughput[i]*NC_send_throughput[i];
					}
				}
				
				for(i=1;i<31;i++)
				{
					if(Assured_band[1][i]!=0)
						{
						NC_re_throughput[i] = throughput_different_link[i][0]/(Assured_band[1][i]*3400000000*250*op_sim_time());
						bbb2 = bbb2 + NC_re_throughput[i];
						ddd2 = ddd2 + NC_re_throughput[i]*NC_re_throughput[i];
						}
				}
				
				for(i=1;i<31;i++)
				{
					for(j=1;j<32;j++)
					{
						if(Assured_band[j+1][i]!=0)
						{
						
							
							NT_re_throughput[i][j] = throughput_different_link[i][j]/(Assured_band[j+1][i]*3400000000*250*op_sim_time());
							bbb3 = bbb3 + NT_re_throughput[i][j];
							ddd3 = ddd3 + NT_re_throughput[i][j]*NT_re_throughput[i][j];
						}
					}
				}
				
				
				fairness_index = ( (bbb1+bbb2+bbb3)*(bbb1+bbb2+bbb3) )/( 962*(ddd1+ddd2+ddd3));
				printf("bbb1:%f----bbb2:%f----bbb3:%f----\n",bbb1,bbb2,bbb3);
				printf("ddd1:%f----ddd2:%f----ddd3:%f----\n",ddd1,ddd2,ddd3);
				printf("公平性因子：%f\n",fairness_index);
				}
				FSM_PROFILE_SECTION_OUT (state6_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (13,"NC_Processor")


			/** state (result) exit executives **/
			FSM_STATE_EXIT_UNFORCED (6, "result", "NC_Processor [result exit execs]")


			/** state (result) transition processing **/
			FSM_TRANSIT_MISSING ("result")
				/*---------------------------------------------------------*/



			/** state (Strong_Burst_Pro) enter executives **/
			FSM_STATE_ENTER_FORCED (7, "Strong_Burst_Pro", state7_enter_exec, "NC_Processor [Strong_Burst_Pro enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Processor [Strong_Burst_Pro enter execs]", state7_enter_exec)
				{
				
				
				//检测队列模块中有没有强时效，有就发下来
				for(i=0;i<strong_burst_check_number;i++)
						{
						strong_burst();
						}
				strong_cycle_num++;
				if(strong_cycle_num*strong_burst_spacing < strong_burst_limit_time)
				{
					//产生下一强时效性性业务的自中断
					op_intrpt_schedule_self(op_sim_time()+strong_burst_spacing, strong_burst_time);
				}
				
				}
				FSM_PROFILE_SECTION_OUT (state7_enter_exec)

			/** state (Strong_Burst_Pro) exit executives **/
			FSM_STATE_EXIT_FORCED (7, "Strong_Burst_Pro", "NC_Processor [Strong_Burst_Pro exit execs]")


			/** state (Strong_Burst_Pro) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "Strong_Burst_Pro", "Idle", "tr_14", "NC_Processor [Strong_Burst_Pro -> Idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (calculate_assured_band) enter executives **/
			FSM_STATE_ENTER_FORCED (8, "calculate_assured_band", state8_enter_exec, "NC_Processor [calculate_assured_band enter execs]")
				FSM_PROFILE_SECTION_IN ("NC_Processor [calculate_assured_band enter execs]", state8_enter_exec)
				{
				
				period_number=0;//有多少个周期性业务
				for(i=0;i<2;i++)
				{	
					for(j=0;j<32;j++)
					{
						if(period_request_map[i][j]==1)
							period_number++;
					}
				
				}
				//printf("period_request_map[0][0]:%d********op_sim_time():%f********\n",period_request_map[0][0],op_sim_time());
				//printf("period_number:%d-----------\n",period_number);
				
				//period_request_map[0][31]=1;//NC下行广播周期性业务
				
				//初始化每个突发业务的保证带宽
				
				load_sum=0.0;
				for(j=1;j<node_num;j++)//初始化NC下行的保证带宽
					{
					for(i=1;i<node_num;i++)
						{
						
							load_sum=load_sum+Node_load[i];
						}
					Assured_band[0][j]=(DBA_long-period_number*period_spacing - request_spacing - DBA_calc_spacing)*(Node_load[j]/load_sum);
					load_sum=0.0;
					}
				
				load_sum=0.0;
				for(j=1;j<node_num;j++)//初始化NC接收轴的保证带宽
					{
					for(i=1;i<node_num;i++)
						{
						
							load_sum=load_sum+Node_load[i];
						}
					Assured_band[1][j]=(DBA_long-period_number*period_spacing - request_spacing - DBA_calc_spacing)*(Node_load[j]/load_sum);
					load_sum=0.0;
					}
				
				load_sum=0.0;
				
				for(i=2;i<node_num+1;i++)//NT接收轴的保证带宽
					{
				
					for(j=1;j<node_num;j++)
						{
						if(i-1==j) 
							{
							Assured_band[i][j]=0.0;
							}
						else
							{
							if(Burst_traffic_control_matrix[i-1][j]!=0)
								{
									for(k=1;k<node_num;k++)
										{
										if((k!=i-1)&&(Burst_traffic_control_matrix[i-1][k]!=0))
											load_sum=load_sum+Node_load[k];
										}
				
									Assured_band[i][j]=(DBA_long - period_number*period_spacing - request_spacing - DBA_calc_spacing-Assured_band[0][i-1])*(Node_load[j]/load_sum);
								}
							else
								{
								Assured_band[i][j]=0;
								}
							}
				
						load_sum=0.0;
						
						}	
				
					}
				
				
				for(i=0;i<node_num+1;i++)
					{
					for(j=0;j<node_num;j++)
						{
						if(j==node_num-1)
							printf(" %f \n",Assured_band[i][j]);
						else
							printf(" %f ",Assured_band[i][j]);
						}
					}
				}
				FSM_PROFILE_SECTION_OUT (state8_enter_exec)

			/** state (calculate_assured_band) exit executives **/
			FSM_STATE_EXIT_FORCED (8, "calculate_assured_band", "NC_Processor [calculate_assured_band exit execs]")


			/** state (calculate_assured_band) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "calculate_assured_band", "Idle", "tr_17", "NC_Processor [calculate_assured_band -> Idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"NC_Processor")
		}
	}




void
_op_NC_Processor_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_NC_Processor_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_NC_Processor_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_NC_Processor_svar function. */
#undef burst_request_map
#undef process_id
#undef node_id
#undef subnet_id
#undef p_num
#undef p1_num
#undef p2_num
#undef req_num
#undef NC_send_credit
#undef NT_re_credit
#undef b_band
#undef NC_send_task_delay
#undef NC_send_task_num
#undef NC_re_credit
#undef NC_re_task_delay
#undef NC_re_task_num
#undef NT_re_task_delay
#undef NT_re_task_num
#undef node_num
#undef strong_cycle_num
#undef sim_time
#undef period_ete_delay
#undef tcbm_ete_delay
#undef burst_ete_delay
#undef period_number
#undef NT_re_task_num2
#undef NT_re_task_delay2
#undef NT_re_task_num3
#undef NT_re_task_delay3
#undef NT_re_task_num1
#undef NT_re_task_delay1
#undef pre_period_pks_send_time
#undef Assured_band
#undef DBA_long
#undef success_DBA_num
#undef Period_Interarrival_Time
#undef strong_burst_limit_time
#undef Total_burst_band
#undef Period_traffic_pksize_dist_ptr
#undef period_spacing
#undef Channel_Rate

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_NC_Processor_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_NC_Processor_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (NC_Processor)",
		sizeof (NC_Processor_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_NC_Processor_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	NC_Processor_state * ptr;
	FIN_MT (_op_NC_Processor_alloc (obtype))

	ptr = (NC_Processor_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "NC_Processor [Init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_NC_Processor_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	NC_Processor_state		*prs_ptr;

	FIN_MT (_op_NC_Processor_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (NC_Processor_state *)gen_ptr;

	if (strcmp ("burst_request_map" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->burst_request_map);
		FOUT
		}
	if (strcmp ("process_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->process_id);
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
	if (strcmp ("p_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->p_num);
		FOUT
		}
	if (strcmp ("p1_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->p1_num);
		FOUT
		}
	if (strcmp ("p2_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->p2_num);
		FOUT
		}
	if (strcmp ("req_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->req_num);
		FOUT
		}
	if (strcmp ("NC_send_credit" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NC_send_credit);
		FOUT
		}
	if (strcmp ("NT_re_credit" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_credit);
		FOUT
		}
	if (strcmp ("b_band" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->b_band);
		FOUT
		}
	if (strcmp ("NC_send_task_delay" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NC_send_task_delay);
		FOUT
		}
	if (strcmp ("NC_send_task_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NC_send_task_num);
		FOUT
		}
	if (strcmp ("NC_re_credit" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NC_re_credit);
		FOUT
		}
	if (strcmp ("NC_re_task_delay" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NC_re_task_delay);
		FOUT
		}
	if (strcmp ("NC_re_task_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NC_re_task_num);
		FOUT
		}
	if (strcmp ("NT_re_task_delay" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_task_delay);
		FOUT
		}
	if (strcmp ("NT_re_task_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_task_num);
		FOUT
		}
	if (strcmp ("node_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_num);
		FOUT
		}
	if (strcmp ("strong_cycle_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->strong_cycle_num);
		FOUT
		}
	if (strcmp ("sim_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sim_time);
		FOUT
		}
	if (strcmp ("period_ete_delay" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_ete_delay);
		FOUT
		}
	if (strcmp ("tcbm_ete_delay" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tcbm_ete_delay);
		FOUT
		}
	if (strcmp ("burst_ete_delay" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->burst_ete_delay);
		FOUT
		}
	if (strcmp ("period_number" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_number);
		FOUT
		}
	if (strcmp ("NT_re_task_num2" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_task_num2);
		FOUT
		}
	if (strcmp ("NT_re_task_delay2" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_task_delay2);
		FOUT
		}
	if (strcmp ("NT_re_task_num3" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_task_num3);
		FOUT
		}
	if (strcmp ("NT_re_task_delay3" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_task_delay3);
		FOUT
		}
	if (strcmp ("NT_re_task_num1" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_task_num1);
		FOUT
		}
	if (strcmp ("NT_re_task_delay1" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->NT_re_task_delay1);
		FOUT
		}
	if (strcmp ("pre_period_pks_send_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pre_period_pks_send_time);
		FOUT
		}
	if (strcmp ("Assured_band" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->Assured_band);
		FOUT
		}
	if (strcmp ("DBA_long" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->DBA_long);
		FOUT
		}
	if (strcmp ("success_DBA_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->success_DBA_num);
		FOUT
		}
	if (strcmp ("Period_Interarrival_Time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Period_Interarrival_Time);
		FOUT
		}
	if (strcmp ("strong_burst_limit_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->strong_burst_limit_time);
		FOUT
		}
	if (strcmp ("Total_burst_band" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Total_burst_band);
		FOUT
		}
	if (strcmp ("Period_traffic_pksize_dist_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Period_traffic_pksize_dist_ptr);
		FOUT
		}
	if (strcmp ("period_spacing" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->period_spacing);
		FOUT
		}
	if (strcmp ("Channel_Rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Channel_Rate);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

