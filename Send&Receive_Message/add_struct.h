#define MAXMSG 1387 //1400-1-4-4-4

//campuri din lib.h dar astea vor fi folosite
//structura va fi incarcata in payload din msg
typedef struct{
	int no_pkt; //numarul pachetului
	int len;
	int no_max_pkt;
	char payload[MAXMSG];
	char checksum;
}add_struct;

char calcul_sum(char* data, int len);
