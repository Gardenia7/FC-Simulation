MIL_3_Tfile_Hdr_ 145A 140A modeler 9 55D68C61 55EFF7A9 D PC-201208291525 Administrator 0 0 none none 0 0 none 4FD2FDE4 17FA 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                  ��g�      @   D   H      =  �  �  �  �  �  �  �  1           	   begsim intrpt         
   ����   
   doc file            	nd_module      endsim intrpt         
   ����   
   failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Stathandle	\ete1_gsh;       double	\sim_time;       int	\transmit_drop;              #include "oms_pr.h"   #include "oms_tan.h"   #include "oms_bgutil.h"   #include "oms_protocol.h"   #include "oms_pipeline.h"   "#include "oms_auto_addr_support.h"   #include "oms_pkt_analyzer.h"   //#include "oms_vlan_support.h"   #include "oms_data_def.h"   #include "oms_rr.h"       #include <hsrp.h>       #define TRANSMIT_INSTAT			0   7#define PK_ARRVL (op_intrpt_type () == OPC_INTRPT_STRM)       5#define XMIT_UP	(op_intrpt_type() == OPC_INTRPT_STAT)   							        9#define SIM_END		 (op_intrpt_type() == OPC_INTRPT_ENDSIM)       n//extern int trans_drop;op_intrpt_stat () == TRANSMIT_INSTAT && \ op_stat_local_read (TRANSMIT_INSTAT) == 1.0)       X   static void route_pk(void)   		{   		int dest_address;   		int i;   		   		double ete1_delay;   		double pk_create_time;   		Packet * pkptr;   		Packet * cp_pkptr;   //		int received_frame_type;   $		//int received_frame_traffic_type;   		   		   		FIN(route_pk());   (		/* receive a frame from upper layer */   '		pkptr = op_pk_get(op_intrpt_strm ());   2		pk_create_time = op_pk_creation_time_get(pkptr);   		   4		op_pk_nfd_get_int32 (pkptr, "D_ID",&dest_address);   ;		//op_pk_nfd_get(pkptr,"Frame_type",&received_frame_type);   		       		/*   		if(received_frame_type==0)   			{   D			op_pk_nfd_get(pkptr,"Traffic_type",&received_frame_traffic_type);   %			if(received_frame_traffic_type==1)   9				printf("SW recv tcbm ----%f-------\n",op_sim_time());   			}   		*/   		   		   		   4		if(dest_address ==32 )//���յ���ΪNC���͵Ĺ㲥֡��   		    		 {   (         /* copy and send to all port */   :         	for (i = 1; i < 32; i++)//�ֱ��Ʒ��͸�1~31��NT   				{   "				cp_pkptr = op_pk_copy (pkptr);   6				op_pk_creation_time_set(cp_pkptr, pk_create_time);   B				ete1_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   )				op_stat_write (ete1_gsh, ete1_delay);   				op_pk_send (cp_pkptr, i);   		      				}   			op_pk_destroy(pkptr);   		 }   		   		    else   		{   
             B		  ete1_delay = op_sim_time () - op_pk_creation_time_get (pkptr);   *		  op_stat_write (ete1_gsh, ete1_delay);	   $		  op_pk_send (pkptr,dest_address);   		}   		     W		 /*��η���һ�����ǲ����Եģ������ȸ��ƣ����ܷ��ͣ�Ҳ����˵һ����ָ��ֻ������һ�η���   		   		    		  op_pk_send (pkptr, 0);   !		  pkptr2 = op_pk_copy (pkptr1);   		  op_pk_send (pkptr1, 1);   !		  pkptr3 = op_pk_copy (pkptr2);   		  op_pk_send (pkptr2, 2);   		  op_pk_send (pkptr3, 3);*/   		   		FOUT;   		}           /*static void xmit_drop(void)   	{   	   	FIN(xmit_drop());   	   		if (XMIT_UP)   			{   			transmit_drop ++;   -			op_stat_write (trans_drop, transmit_drop);   			}   	FOUT;   	}*/                                                             �   �          
   idle   
       
       
       
       
           ����             pr_state        v  J          
   result   
       
      sim_time = op_sim_time();   -printf("the sw drop pks %d\n",transmit_drop);   (printf("the sim time is %f\n",sim_time);   
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
      Lete1_gsh = op_stat_reg ("ETE1_Delay", OPC_STAT_INDEX_NONE, OPC_STAT_GLOBAL);   O//trans_drop = op_stat_reg ("TRANS_DROP", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);       transmit_drop = 0;   
                     
   ����   
          pr_state                       �  /     �   �  n  4    2  �   �          
   tr_0   
       
   PK_ARRVL   
       
   
route_pk()   
       
    ����   
          ����                       pr_transition                �   y     �   �  e   t  !   w  �   �          
   tr_1   
       
   default   
       ����          
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
          ����                       pr_transition         	              
ETE1_Delay        ������������        ԲI�%��}                        