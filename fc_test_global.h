// the next is que stat
int period_pks_generate;
int tcbm_pks_generate;
int burst_pks_generate;


int period_loss_queue;
int burst_loss_queue;
int tcbm_loss_queue;
int pks_loss_queue;

int total_pks_generate;

//the next is pro stat
int pro_period_pks_send;
int pro_tcbm_pks_send;
int pro_burst_pks_send;
int pro_total_pks_send;


int period_pks_receive;
int tcbm_pks_receive;
int burst_pks_receive;
int rscn_receive;
double rscn_max_delay;

double period_ete_sum_delay;
double period_ete_max_delay;

double period_ete_max_delay_1;
double period_ete_max_delay_2;
double period_ete_min_delay_1;
double period_ete_min_delay_2;

double period_ete_sum_delay_1;
double period_ete_sum_delay_2;

double tcbm_ete_max_delay;
double tcbm_ete_sum_delay;
double tcbm_ete_aveg_delay;



double burst_ete_max_delay;//所有突发业务连接的最大时延
double burst_ete_sum_delay;
double NC_send_burst_ete_max_delay;
double burst_ete_aveg_delay;
double burst_ete_max_delay_different_link[32][32];//统计不同连接的最大时延
double burst_ete_sum_delay_different_link[32][32];//统计不同连接的时延总和，用于计算不同连接的平均时延
double burst_ete_aveg_delay_different_link[32][32];//统计不同连接的平均时延
int burst_packet_number_different_link[32][32];//统计不同连接收到的突发业务数据包数


double total_throughput;//统计的吞吐量为所有帧
double total_source_throughput;//统计纯业务数据的吞吐量
double throughput_different_link[32][32];//按连接统计吞吐量

double fairness_index;

double period_send_interval_time;
double period_max_interval_time;
double period_min_interval_time;



double period_ete_aveg_delay;

double ete_aveg_delay;
double total_rec_bits;


double total_lossrate_pks;
int rscn_num;

int request_number[10];
int delay_num[32];
double delay_sort[32];
double delay_value[32];
double Node_load[32];
int period_request_map[2][32];
double Burst_traffic_control_matrix[32][32];



//时延不敏感的控制指令
int control_command_nonSensitive_pks_generate;
int control_command_nonSensitive_pks_loss_queue;
int control_command_nonSensitive_pks_request[32];
int control_command_nonSensitive_pks_request[32];
int control_command_nonSensitive_pks_send;
int control_command_nonSensitive_pks_receive;
double control_command_nonSensitive_pks_ete_max_delay;
double control_command_nonSensitive_pks_ete_sum_delay;







