MIL_3_Tfile_Hdr_ 145A 140A modeler 9 5647E743 641ABE6E 7A LAPTOP-45T4L382 12085 0 0 none none 0 0 none 2558F2C 2060 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                          ��g�      @   D   H      i  
      H  T  X  \  ]           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt         
   ����   
   failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����          
   Stathandle	\ete1_gsh;       double	\sim_time;       int	\transmit_drop;       Distribution *	\add_frame;       Distribution *	\judge_frame;          Packet  *CommondFrame_ptr;   Packet  *copy1;   //Packet  *copy2;       int dest = 0;       double time= 0.0;   //double rscn_delay;       int Logi_node_num ;   int i;   $   #include "oms_pr.h"   #include "oms_tan.h"   #include "oms_bgutil.h"   #include "oms_protocol.h"   #include "oms_pipeline.h"   "#include "oms_auto_addr_support.h"   #include "oms_pkt_analyzer.h"   //#include "oms_vlan_support.h"   #include "oms_data_def.h"   #include "oms_rr.h"       #include <hsrp.h>   #include <fc_test_global.h>              #define TRANSMIT_INSTAT			0       #define RSCN_TIME     1        7#define PK_ARRVL (op_intrpt_type () == OPC_INTRPT_STRM)       5#define XMIT_UP	(op_intrpt_type() == OPC_INTRPT_STAT)   							        9#define SIM_END		 (op_intrpt_type() == OPC_INTRPT_ENDSIM)       W#define RSCN_SEND  (op_intrpt_type()==OPC_INTRPT_SELF&& op_intrpt_code () == RSCN_TIME)           #define gen_time 0           extern int rscn_num;               V   static void route_pk(void)   		{   		int dest_address;   		int i;   		int received_frame_type;   		   		double ete1_delay;   		double pk_create_time;   		double rscn_delay;   		Packet * pkptr;   		Packet * cp_pkptr;   		//int received_frame_type;   $		//int received_frame_traffic_type;   		   		   		FIN(route_pk());   (		/* receive a frame from upper layer */   '		pkptr = op_pk_get(op_intrpt_strm ());   2		pk_create_time = op_pk_creation_time_get(pkptr);   		   4		op_pk_nfd_get_int32 (pkptr, "D_ID",&dest_address);   9		op_pk_nfd_get(pkptr,"Frame_type",&received_frame_type);   		       		   		if(received_frame_type==5)   			{   A			rscn_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   #			if (rscn_delay > rscn_max_delay)   				{    				rscn_max_delay = rscn_delay;   				}   		   			rscn_receive++;   			op_pk_destroy (pkptr);   			}   		else   			{   			   5			if(dest_address ==32 )//���յ���ΪNC���͵Ĺ㲥֡��   		    				{   #				/* copy and send to all port */   4				for (i = 1; i < 32; i++)//�ֱ��Ʒ��͸�1~31��NT   					{   #					cp_pkptr = op_pk_copy (pkptr);   7					op_pk_creation_time_set(cp_pkptr, pk_create_time);   C					ete1_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   *					op_stat_write (ete1_gsh, ete1_delay);   					op_pk_send (cp_pkptr, i);   		      					}   				op_pk_destroy(pkptr);   				}   		   			else   				{   
             B				ete1_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   *				op_stat_write (ete1_gsh, ete1_delay);	   $				op_pk_send (pkptr,dest_address);   				}   			   			}   		   		   		   		   		     W		 /*��η���һ�����ǲ����Եģ������ȸ��ƣ����ܷ��ͣ�Ҳ����˵һ����ָ��ֻ������һ�η���   		   		    		  op_pk_send (pkptr, 0);   !		  pkptr2 = op_pk_copy (pkptr1);   		  op_pk_send (pkptr1, 1);   !		  pkptr3 = op_pk_copy (pkptr2);   		  op_pk_send (pkptr2, 2);   		  op_pk_send (pkptr3, 3);*/   		   		FOUT;   		}                                                             �   �          
   idle   
       
      /*   time = op_sim_time();       value = (int)(time);       #//printf("the time is %d\n",value);       if(time == 10 )   	{   "		printf("this is rscn ------\n");   =		op_intrpt_schedule_self(op_sim_time()+gen_time, RSCN_TIME);   	}   */   
       
       
           ����             pr_state        v  J          
   result   
       
      sim_time = op_sim_time();   -printf("the sw drop pks %d\n",transmit_drop);   (printf("the sim time is %f\n",sim_time);       (printf("the rscn num is %d\n",rscn_num);   .printf("the RSCN delay :%f\n",rscn_max_delay);   
                     
    ����   
          pr_state        v   �          
   	drop_stat   
       
      J//printf("######## %f\n #########",op_stat_local_read (op_intrpt_stat()));   0if(op_stat_local_read (op_intrpt_stat()) == 1.0)   {   Bprintf(" the SW lost packet in --------%f------\n",op_sim_time());   transmit_drop ++;   }	   
                     
   ����   
          pr_state           �          
   init   
       
      Lete1_gsh = op_stat_reg ("ETE1_Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   O//trans_drop = op_stat_reg ("TRANS_DROP", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       transmit_drop = 0;       rscn_num = 0;       4op_intrpt_schedule_self(op_sim_time()+0, RSCN_TIME);   
                     
   ����   
          pr_state        v   Z          
   RSCN_PRO   
       J      .add_frame = op_dist_load ("uniform_int", 1,5);       1Logi_node_num = (int)op_dist_outcome (add_frame);       DCommondFrame_ptr = op_pk_create_fmt ("fc_1553_Commandframe_format");   zop_pk_nfd_set (CommondFrame_ptr, "Frame_type", 5);//0������֡��1������֡��2��״̬֡��3����������֡��4������ظ���5��RSCN֡   1op_pk_nfd_set (CommondFrame_ptr, "D_ID", dest);//   %//copy1=op_pk_copy(CommondFrame_ptr);   %//copy2=op_pk_copy(CommondFrame_ptr);   for(i=0;i<Logi_node_num;i++)   {   	for(dest=0;dest<10;dest++)   		{   %		copy1=op_pk_copy(CommondFrame_ptr);   		op_pk_send (copy1, dest);   		   		}   	   }   !op_pk_destroy (CommondFrame_ptr);   //op_pk_send (copy1, dest);   //op_pk_send (copy2, dest);               ,//printf("the next rscn time is %f\n",time);   time=op_dist_exponential(0.1);   9op_intrpt_schedule_self(op_sim_time()+time, RSCN_TIME);//   %//printf("RSCN time:%f-----\n",time);   rscn_num++;       J                     
   ����   
          pr_state                       �  /     �   �  h      
  �   �          
   tr_0   
       
   PK_ARRVL   
       
   
route_pk()   
       
    ����   
          ����                       pr_transition               9       �   �  \  .          
   tr_3   
       
   SIM_END   
       ����          
    ����   
          ����                       pr_transition               !   �     �   �  ~   �          
   tr_6   
       
   XMIT_UP   
       ����          
    ����   
          ����                       pr_transition                   �     n   �  �   �          
   tr_7   
       ����          ����          
    ����   
          ����                       pr_transition               h   �        �  �   �          
   tr_8   
       ����          ����          
    ����   
          ����                       pr_transition      	            �     �   �  s   b          
   tr_9   
       
   	RSCN_SEND   
       ����          
    ����   
          ����                       pr_transition                �   j     �   �  D   �  ]   }  �   {  �   �          
   tr_12   
       
   default   
       ����          
    ����   
          ����                       pr_transition               F   �     z   V  #   �  �   �          
   tr_13   
       ����          ����          
    ����   
          ����                       pr_transition                       
ETE1_Delay        ������������        ԲI�%��}                        