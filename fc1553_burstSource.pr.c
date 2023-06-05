/* Process model C form file: fc1553_burstSource.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char fc1553_burstSource_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 565EA3CD 565EA3CD 1 PC-201208291525 Administrator 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                            ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

/* Include files. */
#include "oms_dist_support.h"
#include <math.h>

/* Define constants used in the process model.	*/
#define	OFF_TO_ON		10
#define	ON_TO_ON		20
#define	ON_TO_OFF		30


/* State transition macro definitions.	*/
#define	INACTIVE_TO_ACTIVE	(intrpt_type == OPC_INTRPT_SELF && intrpt_code == OFF_TO_ON)
#define	REMAIN_ACTIVE		(intrpt_type == OPC_INTRPT_SELF && intrpt_code == ON_TO_ON)
#define	ACTIVE_TO_INACTIVE	(intrpt_type == OPC_INTRPT_SELF && intrpt_code == ON_TO_OFF)

/* 宏定义 */
//#define ONOFF_NUM	1			//每一组ONOFF源的组成数
#define UNIT_CONVERT		1E6

/* Function Declarations.	*/
extern double 	pk_generate_bits;
extern double 	interval_cal=0;
extern double	interval_num=0;
static void		bursty_source_sv_init ();
extern double Node_load[32];


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
	char	                   		pid_string [64]                                 ;	/* Process ID display string */
	Boolean	                		debug_mode                                      ;	/* Determines whether the simulation is in debug mode */
	double	                 		stop_time                                       ;	/* Stop time for traffic generation */
	double	                 		off_state_start_time                            ;	/* Time at which the process will enter the OFF state */
	Stathandle	             		pksize_stathandle                               ;	/* Statistic handle to the "Packet Generation Status" statistic */
	Distribution *	         		on_state_dist_handle                            ;	/* On state distribution handle */
	Distribution *	         		off_state_dist_handle                           ;	/* Off state distribution handle */
	OmsT_Dist_Handle	       		intarrival_time_dist_handle                     ;	/* Interarrival time distribution handle */
	Distribution *	         		packet_size_dist_handle                         ;	/* Packet size distribution handle */
	double	                 		start_time                                      ;	/* Start time for traffic generation */
	Stathandle	             		bits_sent_stathandle                            ;
	Stathandle	             		bitssec_sent_stathandle                         ;
	Stathandle	             		pkts_sent_stathandle                            ;
	Stathandle	             		pktssec_sent_stathandle                         ;
	Stathandle	             		bits_sent_gstathandle                           ;
	Stathandle	             		bitssec_sent_gstathandle                        ;
	Stathandle	             		pkts_sent_gstathandle                           ;
	Stathandle	             		pktssec_sent_gstathandle                        ;
	int	                    		segmentation_size                               ;	/* Size using which segmentation will occur. */
	Sbhandle	               		segmentation_buf_handle                         ;
	double	                 		load_net                                        ;
	Stathandle	             		interval_gstathandle                            ;
	int	                    		user_id                                         ;
	double	                 		time_test                                       ;
	double	                 		channel_rate                                    ;
	double	                 		onoff_peak_rate                                 ;
	double	                 		on_period_bytes                                 ;
	Distribution *	         		destdist                                        ;
	int	                    		node_num                                        ;
	int	                    		dest_id                                         ;
	Objid	                  		node_id                                         ;
	int*	                   		node_user_id                                    ;
	Objid	                  		subnet_id                                       ;
	int*	                   		sum_num                                         ;
	int	                    		sum                                             ;
	int	                    		packet_size                                     ;
	Stathandle	             		Packet_size_stathandle                          ;
	int	                    		Burst_destination                               ;
	Stathandle	             		packet_destination_handle                       ;
	int	                    		EN_burst                                        ;
	int	                    		EN_burst_num                                    ;
	double	                 		Link_traffic_load                               ;
	int	                    		self_adress                                     ;
	} fc1553_burstSource_state;

#define pid_string              		op_sv_ptr->pid_string
#define debug_mode              		op_sv_ptr->debug_mode
#define stop_time               		op_sv_ptr->stop_time
#define off_state_start_time    		op_sv_ptr->off_state_start_time
#define pksize_stathandle       		op_sv_ptr->pksize_stathandle
#define on_state_dist_handle    		op_sv_ptr->on_state_dist_handle
#define off_state_dist_handle   		op_sv_ptr->off_state_dist_handle
#define intarrival_time_dist_handle		op_sv_ptr->intarrival_time_dist_handle
#define packet_size_dist_handle 		op_sv_ptr->packet_size_dist_handle
#define start_time              		op_sv_ptr->start_time
#define bits_sent_stathandle    		op_sv_ptr->bits_sent_stathandle
#define bitssec_sent_stathandle 		op_sv_ptr->bitssec_sent_stathandle
#define pkts_sent_stathandle    		op_sv_ptr->pkts_sent_stathandle
#define pktssec_sent_stathandle 		op_sv_ptr->pktssec_sent_stathandle
#define bits_sent_gstathandle   		op_sv_ptr->bits_sent_gstathandle
#define bitssec_sent_gstathandle		op_sv_ptr->bitssec_sent_gstathandle
#define pkts_sent_gstathandle   		op_sv_ptr->pkts_sent_gstathandle
#define pktssec_sent_gstathandle		op_sv_ptr->pktssec_sent_gstathandle
#define segmentation_size       		op_sv_ptr->segmentation_size
#define segmentation_buf_handle 		op_sv_ptr->segmentation_buf_handle
#define load_net                		op_sv_ptr->load_net
#define interval_gstathandle    		op_sv_ptr->interval_gstathandle
#define user_id                 		op_sv_ptr->user_id
#define time_test               		op_sv_ptr->time_test
#define channel_rate            		op_sv_ptr->channel_rate
#define onoff_peak_rate         		op_sv_ptr->onoff_peak_rate
#define on_period_bytes         		op_sv_ptr->on_period_bytes
#define destdist                		op_sv_ptr->destdist
#define node_num                		op_sv_ptr->node_num
#define dest_id                 		op_sv_ptr->dest_id
#define node_id                 		op_sv_ptr->node_id
#define node_user_id            		op_sv_ptr->node_user_id
#define subnet_id               		op_sv_ptr->subnet_id
#define sum_num                 		op_sv_ptr->sum_num
#define sum                     		op_sv_ptr->sum
#define packet_size             		op_sv_ptr->packet_size
#define Packet_size_stathandle  		op_sv_ptr->Packet_size_stathandle
#define Burst_destination       		op_sv_ptr->Burst_destination
#define packet_destination_handle		op_sv_ptr->packet_destination_handle
#define EN_burst                		op_sv_ptr->EN_burst
#define EN_burst_num            		op_sv_ptr->EN_burst_num
#define Link_traffic_load       		op_sv_ptr->Link_traffic_load
#define self_adress             		op_sv_ptr->self_adress

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	fc1553_burstSource_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((fc1553_burstSource_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif



static void
	bursty_source_sv_init ()
	{
	Prohandle			my_prohandle;
	int					my_pro_id;
	Objid				my_id;
	//Objid				traf_gen_comp_attr_objid, traf_conf_objid;
	//Objid				pkt_gen_comp_attr_objid, pkt_gen_args_objid;
	//char				start_time_string [128];
	double				para_H;
	double				traf_link_rate;
	double				load_traf;
	double				on_location_para;
	double				on_shape_para;
	double				off_location_para;
	double				off_shape_para;
//	OmsT_Dist_Handle	start_time_dist_handle;

	/*	 Initializes state variables associated with this process model.	*/
	FIN (bursty_source_sv_init ());

	/*	Determine the prohandle of this process as well as	*/
	/*	the object IDs of the containing module and node.*/
		
	my_prohandle = op_pro_self ();
	my_pro_id = op_pro_id (my_prohandle);
	my_id = op_id_self ();
	node_id=op_topo_parent(op_id_self ()); 
	subnet_id=op_topo_parent(node_id);
	//node_num = op_topo_object_count(OPC_OBJTYPE_NODE_FIX);
	//node_user_id=(int*)op_prg_mem_alloc(sizeof(int)*node_num);
	
	/*	Determine the process ID display string.	*/
	sprintf (pid_string, "bursty_source PID (%d)", my_pro_id);

	/*	Determine whether or not the simulation is in debug	mode.  						*/
	/*Trace statement are only enabled when the	simulation is in debug mode.	*/
	debug_mode = op_sim_debug ();

	/*从模块属性中读取相关属性值 change by wj 2015-6-24*/
	
	op_ima_obj_attr_get (my_id, "EN burst",            &EN_burst  );//本数据源是否使能的属性
	op_ima_obj_attr_get (my_id, "Parameter H",         &para_H  );//数据源的H参数
	op_ima_obj_attr_get (my_id, "Link Rate",           &traf_link_rate);//数据源的峰值速率
	op_ima_obj_attr_get (my_id, "Packet Size",         &packet_size);//数据源产生的包长度
	
	
	
	
	/*从节点属性中读取相关属性值
	/* Read the traffic generation parameters.	*/
	op_ima_obj_attr_get (op_topo_parent(op_id_self ()), "Traffic Load(0-1)", &load_net);//节点负载
	
	op_ima_obj_attr_get (op_topo_parent(op_id_self ()), "Channel rate(Mbits/s)", &channel_rate);//信道速率
	op_ima_obj_attr_get (op_topo_parent(op_id_self ()), "Start Time", &start_time);//开始时间
	op_ima_obj_attr_get (op_topo_parent(op_id_self ()), "Stop Time", &stop_time);//结束时间
    op_ima_obj_attr_get (op_topo_parent(op_id_self ()), "EN burst number",     &EN_burst_num  );//数据源使能的个数
    op_ima_obj_attr_get (op_topo_parent(op_id_self ()), "Node Number",     &node_num  );//节点个数
    op_ima_obj_attr_get (op_topo_parent(op_id_self ()),"user id",&self_adress);

	
   
	

	packet_size=2112;
	Node_load[self_adress] = load_net;//每个节点读取负载，存在Node_load计算保证带宽	
		
	onoff_peak_rate = traf_link_rate/EN_burst_num;
	onoff_peak_rate = onoff_peak_rate *UNIT_CONVERT;
	channel_rate= channel_rate*UNIT_CONVERT;
	/*
	if (strcmp (start_time_string, "Never") != 0)
		{
		//start_time_dist_handle = oms_dist_load_from_string (start_time_string);
		start_time = 0.0;//oms_dist_outcome (start_time_dist_handle);
		}
	else	// traffic generation is disabled.
		{
		start_time = 0.0;
		}
    */
	start_time=0.0;
	
	if (stop_time == -1.0)
		stop_time = OPC_DBL_INFINITY;

	/* If the start time is set to "Infinity", then there is no need to schedule an interrupt 	*/
	/* as this node has been set not to generate any traffic.														*/
	/* Obtain the neccessary information of IP traffic to be generated.*/
	if ((start_time >= 0.0) && (stop_time > start_time)) 
		{

		
		
		/*负载*/
		load_traf = load_net;//*channel_rate/onoff_peak_rate;
		
		/* Load the distribution used to determine the time	*/
		/* for which the process stays in the "ON" state.	*/
		on_shape_para = 3 - 2*para_H;
		on_location_para = (double)packet_size*8/onoff_peak_rate;//
		on_state_dist_handle = op_dist_load ("pareto", on_location_para, on_shape_para);
		
		/* Load the distribution used to determine the time	*/
		/* for which the process stays in the "OFF" state.	*/
		off_shape_para = on_shape_para;
		off_location_para = on_location_para*(1-load_traf)/load_traf;
		off_state_dist_handle = op_dist_load ("pareto", off_location_para, off_shape_para);

		/*	Initilaize the packet generation status statistic	*/
		/*	to indicate that currently there are no packets 	*/
		/*	being generated.									*/
		pksize_stathandle = op_stat_reg ("Traffic Source.Packet Generation Status", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		op_stat_write (pksize_stathandle, (double) OPC_FALSE);
						
		destdist=op_dist_load("uniform_int",0,node_num-1);
		//printf("%d\n",node_num);

		/* Initilaize the statistic handles to keep	*/
		/* track of traffic Sourceed by this process.	*/
		//pk_generate_bits=0;
		bits_sent_stathandle 		= op_stat_reg ("Traffic Source.Traffic Sent (bits)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		bitssec_sent_stathandle 	= op_stat_reg ("Traffic Source.Traffic Sent (bits/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		pkts_sent_stathandle 		= op_stat_reg ("Traffic Source.Traffic Sent (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		pktssec_sent_stathandle 	= op_stat_reg ("Traffic Source.Traffic Sent (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
			
		bits_sent_gstathandle 		= op_stat_reg ("Traffic Source.Traffic Sent (bits)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		bitssec_sent_gstathandle 	= op_stat_reg ("Traffic Source.Traffic Sent (bits/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		pkts_sent_gstathandle 		= op_stat_reg ("Traffic Source.Traffic Sent (packets)",		OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		pktssec_sent_gstathandle 	= op_stat_reg ("Traffic Source.Traffic Sent (packets/sec)",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		interval_gstathandle 		= op_stat_reg ("Interval Test",	OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);
		
		/*注册用于统计每个包包长的统计量。  */
		Packet_size_stathandle 		= op_stat_reg ("Traffic Source.Packet Size (Bytes)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		
		packet_destination_handle 		= op_stat_reg ("Traffic Source.Packet destionation",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
		
		
		
		
		}	
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
	void fc1553_burstSource (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_fc1553_burstSource_init (int * init_block_ptr);
	void _op_fc1553_burstSource_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_fc1553_burstSource_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_fc1553_burstSource_alloc (VosT_Obtype, int);
	void _op_fc1553_burstSource_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
fc1553_burstSource (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (fc1553_burstSource ());

		{
		/* Temporary Variables */
		int					intrpt_type;
		int					intrpt_code;
		
		Packet*				pkptr;
		int					pksize;
		double				dval0 = 0.0;
		double				dval1 = 0.0;
		double				on_period;
		double				off_period;
		double				intime;
		double				next_packet_arrival_time;
		/* End of Temporary Variables */


		FSM_ENTER ("fc1553_burstSource")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED_NOLABEL (0, "init", "fc1553_burstSource [init enter execs]")
				FSM_PROFILE_SECTION_IN ("fc1553_burstSource [init enter execs]", state0_enter_exec)
				{
				/* Initialize the traffic generation parameters.	*/
				bursty_source_sv_init ();
				
				/* Schedule the first OFF-period scheduling by setting	 a self-interrupt for the start time.	*/
				/* If the start time is set to "Infinity", then there is no need to	 schedule an interrupt as    	*/
				/* this node has been set	not to generate any traffic.																	*/
				if ((start_time >= 0.0)&&(EN_burst==1))   //EN_burst==1时突发业务数据源使能
					{
					
					op_intrpt_schedule_self (op_sim_time () + start_time, 0);
					}
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"fc1553_burstSource")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "fc1553_burstSource [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "off", "tr_0", "fc1553_burstSource [init -> off : default / ]")
				/*---------------------------------------------------------*/



			/** state (off) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "off", state1_enter_exec, "fc1553_burstSource [off enter execs]")
				FSM_PROFILE_SECTION_IN ("fc1553_burstSource [off enter execs]", state1_enter_exec)
				{
				/*	Determine the time for which the process remains in	 the "OFF" (inactive) state. 	*/
				/* This is determined by	the distribution loaded to characterize the "OFF" state 		*/
				/* duration (i.e., time for which the source does not generate any packets.)					*/
				time_test=op_sim_time();
				off_period = op_dist_outcome  (off_state_dist_handle);
				
				/*	Schedule a self-interrupt to transit to the "ON"	*/
				/*	when the "OFF" state duration expires.				*/
				if (time_test + off_period < stop_time)
					{
					//off_period = (int)(off_period+0.5);
					op_intrpt_schedule_self (time_test + off_period, OFF_TO_ON);
					
					}
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"fc1553_burstSource")


			/** state (off) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "off", "fc1553_burstSource [off exit execs]")
				FSM_PROFILE_SECTION_IN ("fc1553_burstSource [off exit execs]", state1_exit_exec)
				{
				/* Determine the type of interrupt.		*/
				intrpt_type = op_intrpt_type ();
				intrpt_code = op_intrpt_code ();
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (off) transition processing **/
			FSM_TRANSIT_ONLY ((INACTIVE_TO_ACTIVE), 2, state2_enter_exec, ;, off, "INACTIVE_TO_ACTIVE", "", "off", "on", "tr_1", "fc1553_burstSource [off -> on : INACTIVE_TO_ACTIVE / ]")
				/*---------------------------------------------------------*/



			/** state (on) enter executives **/
			FSM_STATE_ENTER_UNFORCED (2, "on", state2_enter_exec, "fc1553_burstSource [on enter execs]")
				FSM_PROFILE_SECTION_IN ("fc1553_burstSource [on enter execs]", state2_enter_exec)
				{
				time_test=op_sim_time();
				
				if (op_intrpt_code () == OFF_TO_ON)
					{
					/*	Determine the time at which this process will	*/
					/*	enter the next "OFF" state.								*/
					on_period = op_dist_outcome  (on_state_dist_handle);
					/*
					while(on_period>0.001)
						{
						on_period = op_dist_outcome  (on_state_dist_handle);
						}
					*/
					Burst_destination = op_dist_outcome  (destdist);
					while(Burst_destination==self_adress)
						{
						Burst_destination = op_dist_outcome  (destdist);
						}
					off_state_start_time = time_test + on_period;
					
					//计算ON周期内的数据量
					on_period_bytes = floor(on_period * onoff_peak_rate / 8);
					
					/*	Schedule a self-interrupt to transit to "OFF"	*/
					/*	state when the "ON" state duration expires.	*/
					
					op_intrpt_schedule_self (off_state_start_time, ON_TO_OFF);
					}
				
				/*	Create a packet using the outcome of the loaded	distribution.	*/
				/* OBTN中的ON/OFF源产生变长的数据，将其改为定长的数据  change by wj 2015-6-23*/
				if(on_period_bytes>packet_size/2)            //(on_period_bytes > packet_min_size/2)
					{
					pksize = packet_size;
					on_period_bytes -= pksize;
					
					pksize *= 8;
					pkptr = op_pk_create_fmt ("fc1553_sourceFormat");
					op_pk_nfd_set (pkptr, "destination", Burst_destination);
					op_pk_total_size_set (pkptr, pksize);
					op_pk_send (pkptr, 0);
					/*	Record statistics to indicate that a packet 	*/
					/*	was generated at the current simulation time.	*/
					op_stat_write (pksize_stathandle, (double) OPC_TRUE);
					//pk_generate_bits += pksize;
				
					/* Update global statistics.				*/
					op_stat_write (bits_sent_gstathandle, 		pksize);
					op_stat_write (pkts_sent_gstathandle, 		1.0);
				
					op_stat_write (bitssec_sent_gstathandle, 	pksize);
					op_stat_write (bitssec_sent_gstathandle, 	0.0);
					op_stat_write (pktssec_sent_gstathandle, 	1.0);
					op_stat_write (pktssec_sent_gstathandle, 	0.0);
					
						/* Update local  statistics.				*///change by wj 2015-6-17
					op_stat_write (bits_sent_stathandle, 		pksize);
					op_stat_write (pkts_sent_stathandle, 		1.0);
				
					op_stat_write (bitssec_sent_stathandle, 	pksize);
					op_stat_write (bitssec_sent_stathandle, 	0.0);
					
					op_stat_write (pktssec_sent_stathandle, 	1.0);
					op_stat_write (pktssec_sent_stathandle, 	0.0);
					
					op_stat_write (Packet_size_stathandle, 		(double)pksize/8);    //统计每个包的长度
					op_stat_write (packet_destination_handle, 		Burst_destination);    //统计每个包的地址
					
					/*	Compute the transmission time of  the next possible packets based on the inter-arrival-time distribution. */
					intime = (double)pksize/onoff_peak_rate;
					next_packet_arrival_time = time_test + intime;
					
					/*	Check if the next packet arrival time is within the	time in which 	*/
					/* the process remains in "ON" (active)	state or not.							*/
					if (next_packet_arrival_time < off_state_start_time && next_packet_arrival_time < stop_time)
						{	
						/*	Schedule the next packet arrival.				*/
						op_intrpt_schedule_self (next_packet_arrival_time, ON_TO_ON);
						}
					}
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (5,"fc1553_burstSource")


			/** state (on) exit executives **/
			FSM_STATE_EXIT_UNFORCED (2, "on", "fc1553_burstSource [on exit execs]")
				FSM_PROFILE_SECTION_IN ("fc1553_burstSource [on exit execs]", state2_exit_exec)
				{
				/* Determine the type of interrupt.		*/
				intrpt_type = op_intrpt_type ();
				intrpt_code = op_intrpt_code ();
				}
				FSM_PROFILE_SECTION_OUT (state2_exit_exec)


			/** state (on) transition processing **/
			FSM_PROFILE_SECTION_IN ("fc1553_burstSource [on trans conditions]", state2_trans_conds)
			FSM_INIT_COND (ACTIVE_TO_INACTIVE)
			FSM_TEST_COND (REMAIN_ACTIVE)
			FSM_TEST_LOGIC ("on")
			FSM_PROFILE_SECTION_OUT (state2_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 1, state1_enter_exec, ;, "ACTIVE_TO_INACTIVE", "", "on", "off", "tr_3", "fc1553_burstSource [on -> off : ACTIVE_TO_INACTIVE / ]")
				FSM_CASE_TRANSIT (1, 2, state2_enter_exec, ;, "REMAIN_ACTIVE", "", "on", "on", "tr_4", "fc1553_burstSource [on -> on : REMAIN_ACTIVE / ]")
				}
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"fc1553_burstSource")
		}
	}




void
_op_fc1553_burstSource_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_fc1553_burstSource_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_fc1553_burstSource_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_fc1553_burstSource_svar function. */
#undef pid_string
#undef debug_mode
#undef stop_time
#undef off_state_start_time
#undef pksize_stathandle
#undef on_state_dist_handle
#undef off_state_dist_handle
#undef intarrival_time_dist_handle
#undef packet_size_dist_handle
#undef start_time
#undef bits_sent_stathandle
#undef bitssec_sent_stathandle
#undef pkts_sent_stathandle
#undef pktssec_sent_stathandle
#undef bits_sent_gstathandle
#undef bitssec_sent_gstathandle
#undef pkts_sent_gstathandle
#undef pktssec_sent_gstathandle
#undef segmentation_size
#undef segmentation_buf_handle
#undef load_net
#undef interval_gstathandle
#undef user_id
#undef time_test
#undef channel_rate
#undef onoff_peak_rate
#undef on_period_bytes
#undef destdist
#undef node_num
#undef dest_id
#undef node_id
#undef node_user_id
#undef subnet_id
#undef sum_num
#undef sum
#undef packet_size
#undef Packet_size_stathandle
#undef Burst_destination
#undef packet_destination_handle
#undef EN_burst
#undef EN_burst_num
#undef Link_traffic_load
#undef self_adress

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_fc1553_burstSource_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_fc1553_burstSource_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (fc1553_burstSource)",
		sizeof (fc1553_burstSource_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_fc1553_burstSource_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	fc1553_burstSource_state * ptr;
	FIN_MT (_op_fc1553_burstSource_alloc (obtype))

	ptr = (fc1553_burstSource_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "fc1553_burstSource [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_fc1553_burstSource_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	fc1553_burstSource_state		*prs_ptr;

	FIN_MT (_op_fc1553_burstSource_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (fc1553_burstSource_state *)gen_ptr;

	if (strcmp ("pid_string" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->pid_string);
		FOUT
		}
	if (strcmp ("debug_mode" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->debug_mode);
		FOUT
		}
	if (strcmp ("stop_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->stop_time);
		FOUT
		}
	if (strcmp ("off_state_start_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->off_state_start_time);
		FOUT
		}
	if (strcmp ("pksize_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pksize_stathandle);
		FOUT
		}
	if (strcmp ("on_state_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->on_state_dist_handle);
		FOUT
		}
	if (strcmp ("off_state_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->off_state_dist_handle);
		FOUT
		}
	if (strcmp ("intarrival_time_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->intarrival_time_dist_handle);
		FOUT
		}
	if (strcmp ("packet_size_dist_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packet_size_dist_handle);
		FOUT
		}
	if (strcmp ("start_time" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->start_time);
		FOUT
		}
	if (strcmp ("bits_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_sent_stathandle);
		FOUT
		}
	if (strcmp ("bitssec_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bitssec_sent_stathandle);
		FOUT
		}
	if (strcmp ("pkts_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkts_sent_stathandle);
		FOUT
		}
	if (strcmp ("pktssec_sent_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pktssec_sent_stathandle);
		FOUT
		}
	if (strcmp ("bits_sent_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bits_sent_gstathandle);
		FOUT
		}
	if (strcmp ("bitssec_sent_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->bitssec_sent_gstathandle);
		FOUT
		}
	if (strcmp ("pkts_sent_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pkts_sent_gstathandle);
		FOUT
		}
	if (strcmp ("pktssec_sent_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->pktssec_sent_gstathandle);
		FOUT
		}
	if (strcmp ("segmentation_size" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->segmentation_size);
		FOUT
		}
	if (strcmp ("segmentation_buf_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->segmentation_buf_handle);
		FOUT
		}
	if (strcmp ("load_net" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->load_net);
		FOUT
		}
	if (strcmp ("interval_gstathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->interval_gstathandle);
		FOUT
		}
	if (strcmp ("user_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->user_id);
		FOUT
		}
	if (strcmp ("time_test" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->time_test);
		FOUT
		}
	if (strcmp ("channel_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->channel_rate);
		FOUT
		}
	if (strcmp ("onoff_peak_rate" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->onoff_peak_rate);
		FOUT
		}
	if (strcmp ("on_period_bytes" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->on_period_bytes);
		FOUT
		}
	if (strcmp ("destdist" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->destdist);
		FOUT
		}
	if (strcmp ("node_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_num);
		FOUT
		}
	if (strcmp ("dest_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->dest_id);
		FOUT
		}
	if (strcmp ("node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_id);
		FOUT
		}
	if (strcmp ("node_user_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->node_user_id);
		FOUT
		}
	if (strcmp ("subnet_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->subnet_id);
		FOUT
		}
	if (strcmp ("sum_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sum_num);
		FOUT
		}
	if (strcmp ("sum" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sum);
		FOUT
		}
	if (strcmp ("packet_size" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packet_size);
		FOUT
		}
	if (strcmp ("Packet_size_stathandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Packet_size_stathandle);
		FOUT
		}
	if (strcmp ("Burst_destination" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Burst_destination);
		FOUT
		}
	if (strcmp ("packet_destination_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->packet_destination_handle);
		FOUT
		}
	if (strcmp ("EN_burst" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->EN_burst);
		FOUT
		}
	if (strcmp ("EN_burst_num" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->EN_burst_num);
		FOUT
		}
	if (strcmp ("Link_traffic_load" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->Link_traffic_load);
		FOUT
		}
	if (strcmp ("self_adress" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->self_adress);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

