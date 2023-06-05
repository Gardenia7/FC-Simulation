# FC仿真

## 代码结构

- `fc1553_periodSource.pr.c`：实现了NC节点的周期性业务源，按照设置的间隔按周期生成发往NT的数据包。
- `fc1553_periodSource_NT.pr.c`：实现了NT节点的周期性业务源，按照设置的间隔按周期生成发往NC的数据包。
- `fc1553_burstSource_config_link_traffic.pr.c`：实现了一般突发性业务源，用ON/OFF源模拟，且各连接的数据量按设置的比例模拟。NC节点和NT节点均有。
- `fc1553_tcbmSource.pr.c`：实现了强时效性业务源。仅NC节点有。
- `NC_Queue.pr.c`：实现了NC节点的队列模块，NC节点所有业务源产生的数据包均在队列模块中排队。
- `NC_Processor.pr.c`：实现了NC节点的处理模块，完成了接收数据包（包括请求收集）、带宽信用值计算、全网业务的排布、按需发送各类数据包的功能。
- `fc_1553_NTqueue_report1destination_1DBA.pr.c`：实现了NT节点的队列模块，NT节点所有业务源产生的数据包均在队列模块中排队。
- `fc_1553_NTprocess_7_1.pr.c`：实现了NT节点的处理模块，完成了接收数据包、按需发送各类数据包（包括请求上报）的功能。
- `init_pksw_hub_process_drop_stat_rscn.pr.c`：实现了交换机的功能，将接收到的数据包按照目的地址转发至对应端口。