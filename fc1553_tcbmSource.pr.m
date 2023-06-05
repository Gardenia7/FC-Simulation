MIL_3_Tfile_Hdr_ 145A 140A modeler 9 557E42E7 64212D6A 89 LAPTOP-45T4L382 12085 0 0 none none 0 0 none 338B02E3 53C2 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      @  l  p  8  <  *�  I�  I�  Mq  Mu  M�  Q�  Q�  *�      Packet Interarrival Time   �������      seconds   �      constant (1.0)      ����      ����         bernoulli (mean)      bernoulli (mean)      $binomial (num_samples, success_prob)      $binomial (num_samples, success_prob)      chi_square (mean)      chi_square (mean)      constant (mean)      constant (mean)      erlang (scale, shape)      erlang (scale, shape)      exponential (mean)      exponential (mean)      extreme (location, scale)      extreme (location, scale)      fast_normal (mean, variance)      fast_normal (mean, variance)      gamma (scale, shape)      gamma (scale, shape)      geometric (success_prob)      geometric (success_prob)      laplace (mean, scale)      laplace (mean, scale)      logistic (mean, scale)      logistic (mean, scale)      lognormal (mean, variance)      lognormal (mean, variance)      normal (mean, variance)      normal (mean, variance)      pareto (location, shape)      pareto (location, shape)      poisson (mean)      poisson (mean)      power function (shape, scale)      power function (shape, scale)      rayleigh (mean)      rayleigh (mean)      triangular (min, max)      triangular (min, max)      uniform (min, max)      uniform (min, max)      uniform_int (min, max)      uniform_int (min, max)      weibull (shape, scale)      weibull (shape, scale)      scripted (filename)      scripted (filename)         �Specifies the distribution name and arguments to be used for generating random outcomes for times between successive packet generations.       �While selecting a distribution, replace the arguments within parenthesis (e.g., mean, variance, location, etc.) with the desired numerical values.       �For the special "scripted" distribution, specify a filename (*.csv or *.gdf) containing the values required as outcomes. Values will be picked from this file in cyclic order.   oms_dist_configure    oms_dist_conf_dbox_click_handler   $oms_dist_conf_dbox_new_value_handler���������Z             Packet Size   �������      Byte   �      constant (100)      ����      ����         bernoulli (mean)      bernoulli (mean)      $binomial (num_samples, success_prob)      $binomial (num_samples, success_prob)      chi_square (mean)      chi_square (mean)      constant (mean)      constant (mean)      erlang (scale, shape)      erlang (scale, shape)      exponential (mean)      exponential (mean)      extreme (location, scale)      extreme (location, scale)      fast_normal (mean, variance)      fast_normal (mean, variance)      gamma (scale, shape)      gamma (scale, shape)      geometric (success_prob)      geometric (success_prob)      laplace (mean, scale)      laplace (mean, scale)      logistic (mean, scale)      logistic (mean, scale)      lognormal (mean, variance)      lognormal (mean, variance)      normal (mean, variance)      normal (mean, variance)      pareto (location, shape)      pareto (location, shape)      poisson (mean)      poisson (mean)      power function (shape, scale)      power function (shape, scale)      rayleigh (mean)      rayleigh (mean)      triangular (min, max)      triangular (min, max)      uniform (min, max)      uniform (min, max)      uniform_int (min, max)      uniform_int (min, max)      weibull (shape, scale)      weibull (shape, scale)      scripted (filename)      scripted (filename)      None      None         �Specifies the distribution name and arguments to be used for generating random outcomes for the size of generated packets (specified in bits).       �While selecting a distribution, replace the arguments within parenthesis (e.g., mean, variance, location, etc.) with the desired numerical values.       �For the special "scripted" distribution, specify a filename (*.csv or *.gdf) containing the values required as outcomes. Values will be picked from this file in cyclic order.   oms_dist_configure    oms_dist_conf_dbox_click_handler   $oms_dist_conf_dbox_new_value_handler���������Z             Packet Format   �������   ����   �      NONE      ����      ����         NONE      NONE         �Specifies the format of the packets that will be generated by the source. Setting the attribute's value to "NONE" will make the source generate unformatted packets.       �Even if the generated packets are formatted packets, the final size of the packet will be set based on the value of the sibling attribute "Packet Size".    #simple_source_active_attrib_handler   -simple_source_packet_format_get_click_handler�������������Z             
Start Time   �������      seconds              ����              ����              ����         10.0   @$      ����      Infinity   ��      ����         �Specifies the simulation time in seconds when the source will start its traffic generation. Setting the value to "Infinity" will simply disable the source. �Z             	Stop Time   �������      seconds      ��         Infinity              ����              ����         Infinity   ��      ����         �Specifies the simulation time in seconds when the source will stop its traffic generation. Setting the value to "Infinity" will make the source generate traffic until the end of the simulation.�Z                 	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue         
            count    ���   
   ����   
      list   	���   
          
   
   super priority             ����          9   */* Object ID of the surrounding module. */   Objid	\own_id;       5/* Format of the packets generated by this source. */   char	\format_str [64];       </* Time when this source will start its packet generation */   </* activities.                                            */   double	\start_time;       ;/* Time when this source will stop its packet generation */   ;/* activities.                                           */   double	\stop_time;       5/* PDF used to determine the interarrival times of */   5/* generated packets.                              */   (OmsT_Dist_Handle	\interarrival_dist_ptr;       ;/* PDF used to determine the sizes of generated packets. */   "OmsT_Dist_Handle	\pksize_dist_ptr;       :/* Flag that indicates whether the source will generate */   :/* unformatted or formatted packets.                    */   Boolean	\generate_unformatted;       2/* Event handle for the arrival of next packet. */   Evhandle	\next_pk_evh;       </* Time between the generation of the last packet and the */   </* next packet.                                           */   double	\next_intarr_time;       ?/* Statistic handle for "Traffic Sent (bits/sec)" statistic. */   Stathandle	\bits_sent_hndl;       B/* Statistic handle for "Traffic Sent (packets/sec)" statistic. */   Stathandle	\packets_sent_hndl;       :/* Statistic handle for "Packet Size (bits)" statistic. */   Stathandle	\packet_size_hndl;       </* Statistic handle for "Packet Interaarival Time (secs)" */   </* statistic.                                             */   Stathandle	\interarrivals_hndl;       int	\dest_id;       int	\node_num;       Distribution *	\destdist;       Objid	\node_id;       Objid	\subnet_id;       *Stathandle	\packet_destination_stathandle;       
   */* Variables used in the "init" state.		*/   char		interarrival_str [128];   char		size_str [128];   Prg_List*	pk_format_names_lptr;   char*		found_format_str;   Boolean		format_found;   int			i;       +/* Variables used in state transitions.		*/   int			intrpt_code;      /* Include files.					*/   #include	<oms_dist_support.h>        /* Special attribute values.		*/    #define		SSC_INFINITE_TIME		-1.0       /* Interrupt code values.			*/   #define		SSC_START				0   #define		SSC_GENERATE			1   #define		SSC_STOP				2       #/* Node configuration constants.	*/   #define		SSC_STRM_TO_LOW			0       "/* Macro definitions for state		*/   /* transitions.						*/   ,#define		START				(intrpt_code == SSC_START)   -#define		DISABLED			(intrpt_code == SSC_STOP)   *#define		STOP				(intrpt_code == SSC_STOP)   7#define		PACKET_GENERATE		(intrpt_code == SSC_GENERATE)       /* Function prototypes.				*/   (static void			ss_packet_generate (void);    static int      dest_generate();   K   static void   ss_packet_generate (void)   	{   	Packet*				pkptr;   	double				pksize;   	int i;   	int j;   G	/** This function creates a packet based on the packet generation		**/   L	/** specifications of the source model and sends it to the lower layer.	**/   	FIN (ss_packet_generate ());   	for(j=0;j<130;j++)   	{       *	/* Generate a packet size outcome.					*/   =	pksize = (double) ceil (oms_dist_outcome (pksize_dist_ptr));   	   4	/* Create a packet of specified format and size.	*/   &	if (generate_unformatted == OPC_TRUE)   		{   3		/* We produce unformatted packets. Create one.	*/    		pkptr = op_pk_create (pksize);   		}   	else   		{   2		/* Create a packet with the specified format.	*/   		//���ɵ�ַ   		for(i=0;i<1;i++)   			{   I			dest_id = dest_generate();                    //change by wj 2015-6-23   			while(dest_id==0)   				{   				dest_id=32;   				}   )			pkptr = op_pk_create_fmt (format_str);   (			op_pk_total_size_set (pkptr, pksize);   L			op_pk_nfd_set (pkptr, "destination", dest_id);   //change by wj 2015-6-23   ;			/* Send the packet via the stream to the lower layer.	*/   '			op_pk_send (pkptr, SSC_STRM_TO_LOW);   			}   	       		}         //printf(   " %d",dest_id);   1	/* Update the packet generation statistics.			*/   (	op_stat_write (packets_sent_hndl, 1.0);   (	op_stat_write (packets_sent_hndl, 0.0);   1	op_stat_write (bits_sent_hndl, (double) pksize);   %	op_stat_write (bits_sent_hndl, 0.0);   3	op_stat_write (packet_size_hndl, (double) pksize);   6	op_stat_write (interarrivals_hndl, next_intarr_time);       	   P	op_stat_write (packet_destination_stathandle, dest_id);//change by wj 2-15-6-23       	}   <    //printf("TCBM generate ------%f-----\n",op_sim_time());   	FOUT;   	}	       5/* ����һ�����ȷֲ��ĵ�ַ   change by wj 2015-6-23	*/   int dest_generate()   	{   	int temp_dest;   	FIN(dest_generate());   	if(node_num>1)   		{   '		 temp_dest=op_dist_outcome(destdist);   		 if(temp_dest==0)   			 temp_dest=32;   >		 //op_stat_write (packet_destination_stathandle, temp_dest);   		 }   	else   		temp_dest=node_num-1;   	   	FRET(temp_dest);   	}                                         \   Y          
   init   
       
   �   D/* At this initial state, we read the values of source attributes	*/   E/* and schedule a selt interrupt that will indicate our start time	*/   &/* for packet generation.											*/       9/* Obtain the object id of the surrounding module.					*/   own_id = op_id_self ();       C/* Read the values of the packet generation parameters, i.e. the	*/   6/* attribute values of the surrounding module.						*/   Kop_ima_obj_attr_get (own_id, "Packet Interarrival Time", interarrival_str);   Cop_ima_obj_attr_get (own_id, "Packet Size",              size_str);   Eop_ima_obj_attr_get (own_id, "Packet Format",            format_str);   Fop_ima_obj_attr_get (own_id, "Start Time",               &start_time);   Eop_ima_obj_attr_get (own_id, "Stop Time",                &stop_time);               @/* ��ȡ�������ɵ�ַ������ڵ�����	 change by wj 2-15-6-23					*/   *    node_id=op_topo_parent(op_id_self ());   #	subnet_id=op_topo_parent(node_id);   9	//node_num = op_topo_object_count(OPC_OBJTYPE_NODE_FIX);   Lop_ima_obj_attr_get (op_topo_parent(op_id_self ()),"Node Number",&node_num);       ?/* Load the PDFs that will be used in computing the packet			*/   1/* interarrival times and packet sizes.								*/   Einterarrival_dist_ptr = oms_dist_load_from_string (interarrival_str);   =pksize_dist_ptr       = oms_dist_load_from_string (size_str);       @/* Verify the existence of the packet format to be used for			*/   #/* generated packets.												*/   %if (strcmp (format_str, "NONE") == 0)   	{   <	/* We will generate unformatted packets. Set the flag.			*/   !	generate_unformatted = OPC_TRUE;   	}   else   	{   >	/* We will generate formatted packets. Turn off the flag.		*/   "	generate_unformatted = OPC_FALSE;       7	/* Get the list of all available packet formats.				*/   P	pk_format_names_lptr = prg_tfile_name_list_get (PrgC_Tfile_Type_Packet_Format);       :	/* Search the list for the requested packet format.				*/   	format_found = OPC_FALSE;   ^	for (i = prg_list_size (pk_format_names_lptr); ((format_found == OPC_FALSE) && (i > 0)); i--)   		{   ?		/* Access the next format name and compare with requested 	*/   		/* format name.												*/   M		found_format_str = (char *) prg_list_access (pk_format_names_lptr, i - 1);    1		if (strcmp (found_format_str, format_str) == 0)   			format_found = OPC_TRUE;   		}   	   ;	/*  ���ɾ��ȷֲ��ľ��destdist    change by wj 2-15-6-23*/   3	destdist=op_dist_load("uniform_int",1,node_num-1);       	   "    if (format_found == OPC_FALSE)   		{   8		/* The requested format does not exist. Generate 			*/   %		/* unformatted packets.										*/   "		generate_unformatted = OPC_TRUE;   	   -		/* Display an appropriate warning.							*/   Y		op_prg_odb_print_major ("Warning from simple packet generator model (simple_source):",    2								"The specified packet format", format_str,   J								"is not found. Generating unformatted packets instead.", OPC_NIL);   		}       @	/* Destroy the lits and its elements since we don't need it		*/   	/* anymore.														*/   &	prg_list_free (pk_format_names_lptr);   &	prg_mem_free  (pk_format_names_lptr);   	}	       	    E/* Make sure we have valid start and stop times, i.e. stop time is	*/   +/* not earlier than start time.										*/   Bif ((stop_time <= start_time) && (stop_time != SSC_INFINITE_TIME))   	{   @	/* Stop time is earlier than start time. Disable the source.	*/    	start_time = SSC_INFINITE_TIME;       -	/* Display an appropriate warning.								*/   X	op_prg_odb_print_major ("Warning from simple packet generator model (simple_source):",    W							"Although the generator is not disabled (start time is set to a finite value),",   I							"a stop time that is not later than the start time is specified.",   ,							"Disabling the generator.", OPC_NIL);   	}       E/* Schedule a self interrupt that will indicate our start time for	*/   @/* packet generation activities. If the source is disabled,			*/   C/* schedule it at current time with the appropriate code value.		*/   $if (start_time == SSC_INFINITE_TIME)   	{   4	op_intrpt_schedule_self (op_sim_time (), SSC_STOP);   	}   else   	{   1	op_intrpt_schedule_self (start_time, SSC_START);       B	/* In this case, also schedule the interrupt when we will stop	*/   @	/* generating packets, unless we are configured to run until	*/   )	/* the end of the simulation.									*/   $	if (stop_time != SSC_INFINITE_TIME)   		{   0		op_intrpt_schedule_self (stop_time, SSC_STOP);   		}   	   =	next_intarr_time = oms_dist_outcome (interarrival_dist_ptr);       H	/* Make sure that interarrival time is not negative. In that case it */   $	/* will be set to 0.												 */   	if (next_intarr_time <0)   		{   		next_intarr_time = 0.0;   		}       	}       D/* Register the statistics that will be maintained by this model.	*/   tbits_sent_hndl     	= op_stat_reg ("tcbm_Generator.Traffic Sent (bits/sec)",			OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   vpackets_sent_hndl   = op_stat_reg ("tcbm_Generator.Traffic Sent (packets/sec)",		OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   zpacket_size_hndl    = op_stat_reg ("tcbm_Generator.Packet Size (bits)",              OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);   zinterarrivals_hndl  = op_stat_reg ("tcbm_Generator.Packet Interarrival Time (secs)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       j/*ע��һ������ͳ��ÿ������Ŀ�ĵ�ַ��ͳ��������֤ÿ�β�����Ŀ�ĵ�ַ�Ƿ�Ϊ���ȷֲ�  change by wj 2-15-6-24*/   xpacket_destination_stathandle  = op_stat_reg ("tcbm_Generator.packet destination", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);           
       
      F/* Determine the code of the interrupt, which is used in evaluating	*/   +/* state transition conditions.										*/    intrpt_code = op_intrpt_code ();   
           ����             pr_state        J   Z          
   generate   
       
      A/* At the enter execs of the "generate" state we schedule the		*/   */* arrival of the next packet.										*/   <next_intarr_time = oms_dist_outcome (interarrival_dist_ptr);       G/* Make sure that interarrival time is not negative. In that case it */   #/* will be set to 0.												 */   if (next_intarr_time <0)   	{   	next_intarr_time = 0;   	}       ]next_pk_evh      = op_intrpt_schedule_self (op_sim_time () + next_intarr_time, SSC_GENERATE);       
       
      F/* Determine the code of the interrupt, which is used in evaluating	*/   +/* state transition conditions.										*/    intrpt_code = op_intrpt_code ();       
           ����             pr_state        J            
   stop   
       
   	   C/* When we enter into the "stop" state, it is the time for us to	*/   E/* stop generating traffic. We simply cancel the generation of the	*/   F/* next packet and go into a silent mode by not scheduling anything	*/   /* else.															*/   *if (op_ev_valid (next_pk_evh) == OPC_TRUE)   	{   	op_ev_cancel (next_pk_evh);   	}       
                         ����             pr_state                        �   O      x   X  .   Y          
   tr_0   
       
   START   
       
   ss_packet_generate()   
       
   ����   
          ����                       pr_transition                �   �      o   m  5   �          
   tr_1   
       
   DISABLED   
       ����          
   ����   
          ����                       pr_transition              d   �     I   u  I   �          
   tr_2   
       
   STOP   
       ����          
   ����   
          ����                       pr_transition              �   X     `   k  y   �  y   (  ^   E          
   tr_3   
       
   PACKET_GENERATE   
       
   ss_packet_generate()   
       
   ����   
          ����                       pr_transition                   Packet Interarrival Time (secs)          &The time interval in seconds that is     "separating successive arrivals of    !packets produced by this source.        tcbm_Generator    bucket/default total/sample mean   linear        ԲI�%��}   Packet Size (bits)          'Size of the packets that are generated    by this source.       tcbm_Generator    bucket/default total/sample mean   linear        ԲI�%��}   Traffic Sent (bits/sec)          'Total size of packets in bits that are    %generated and sent to lower layer by    this source in one second.       tcbm_Generator   bucket/default total/sum_time   linear        ԲI�%��}   Traffic Sent (packets/sec)          (Total number of packets per second that    &are generated and sent to lower layer    by this source.        tcbm_Generator   bucket/default total/sum_time   linear        ԲI�%��}   packet destination           tcbm_Generator    bucket/default total/sample mean   linear        ԲI�%��}          oms_dist_support   oms_string_support              General Process Description:    ----------------------------       �The simple_source process model is implemented to replace "ideal source" module type, which is not supported starting with OPNET 7.0 release. The model simply generates packets and forwards them to lower layer.  Interarrival time, size and format of the packets are determined based on the model attribute values. Similarly simulation times when the process will start and stop its packet generation activities are also determined by user specifications via model attributes.               Packet Formats:   ---------------       =Unformatted or formatted packets based on user configuration.                ICI Interfaces:    ---------------       3The simple_source model does not use any ICI model.               Restrictions:   -------------        �* Connection with the lower layer is hardcoded as "Stream 0" in the header block under the constant definition for SSC_STRM_TO_LOW.       =* This model is not implemented to handle any packet arrival.        