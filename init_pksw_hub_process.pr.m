MIL_3_Tfile_Hdr_ 145A 140A modeler 9 5593ABF8 55D28C94 9 dell-PC dell 0 0 none none 0 0 none 5611F11 E66 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                     ��g�      @   D   H      	�        N  Z  ^  b  	�           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Stathandle	\ete1_gsh;              7#define PK_ARRVL (op_intrpt_type () == OPC_INTRPT_STRM)   B   static void route_pk(void)   		{   		int dest_address;   		int i;   		   		double ete1_delay;   		double pk_create_time;   		Packet * pkptr;   		Packet * cp_pkptr;   		int received_frame_type;   $		//int received_frame_traffic_type;   		   		   		FIN(route_pk());   (		/* receive a frame from upper layer */   '		pkptr = op_pk_get(op_intrpt_strm ());   2		pk_create_time = op_pk_creation_time_get(pkptr);   		   4		op_pk_nfd_get_int32 (pkptr, "D_ID",&dest_address);   ;		//op_pk_nfd_get(pkptr,"Frame_type",&received_frame_type);   		       		/*   		if(received_frame_type==0)   			{   D			op_pk_nfd_get(pkptr,"Traffic_type",&received_frame_traffic_type);   %			if(received_frame_traffic_type==1)   9				printf("SW recv tcbm ----%f-------\n",op_sim_time());   			}   		*/   		   		   		   4		if(dest_address ==32 )//���յ���ΪNC���͵Ĺ㲥֡��   		    		     (         /* copy and send to all port */   9         for (i = 1; i < 32; i++)//�ֱ��Ʒ��͸�1~31��NT   		       {   #		   cp_pkptr = op_pk_copy (pkptr);   7		   op_pk_creation_time_set(cp_pkptr, pk_create_time);   C		   ete1_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   *		   op_stat_write (ete1_gsh, ete1_delay);   		   op_pk_send (cp_pkptr, i);   		   }   		   		    else   		{   
             B		  ete1_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   *		  op_stat_write (ete1_gsh, ete1_delay);	   $		  op_pk_send (pkptr,dest_address);   		}   		     W		 /*��η���һ�����ǲ����Եģ������ȸ��ƣ����ܷ��ͣ�Ҳ����˵һ����ָ��ֻ������һ�η���   		   		    		  op_pk_send (pkptr, 0);   !		  pkptr2 = op_pk_copy (pkptr1);   		  op_pk_send (pkptr1, 1);   !		  pkptr3 = op_pk_copy (pkptr2);   		  op_pk_send (pkptr2, 2);   		  op_pk_send (pkptr3, 3);*/   		   		FOUT;   		}                                          �   �          
   idle   
       
      Lete1_gsh = op_stat_reg ("ETE1 Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   
                         ����             pr_state                         �   �      �   �   ~   �  '   �   �   �          
   tr_0   
       
   PK_ARRVL   
       
   
route_pk()   
       
    ����   
          ����                       pr_transition                 �   =      �   �   u   8  1   ;   �   �          
   tr_1   
       
   default   
       ����          
    ����   
          ����                       pr_transition                       
ETE1 Delay        ������������        ԲI�%��}                        