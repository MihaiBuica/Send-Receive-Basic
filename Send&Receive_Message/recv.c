#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"
#include "add_struct.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc,char** argv){
  msg r,t;
  init(HOST,PORT);
  add_struct s;
  long buffer_size = 10;
  long i, res;
  int j;
  long dim_last_pkg;
  int setare_dim = 0;
  char** buffer_send;
 
  for (i = 0; i < buffer_size; i++) {
    /* wait for message */
    res = recv_message(&r);
    if (res < 0) {
      perror("[RECEIVER] Receive error. Exiting.\n");
      return -1;
    }
    else{
    }

    memcpy(&s, r.payload, sizeof(s));
    if(!setare_dim){
      /*BUFFER SCRIERE*/
      buffer_size = s.no_max_pkt;
      buffer_send =  malloc ( sizeof(char*) *  buffer_size);
      for(j = 0; j < buffer_size; j++){
        buffer_send[j] = malloc(sizeof(char) * MAXMSG);
      }
      setare_dim = 1;
      //printf("[RECV]Dimensiune buffer: %ld\n", buffer_size);
    /*END*/
    }
    //if( s.checksum == calcul_sum(s.payload, s.len) ){
      memcpy(buffer_send[s.no_pkt], s.payload, s.len);
      dim_last_pkg = s.len;
      sprintf(t.payload,"%d", s.no_pkt);
      t.len = strlen(t.payload+1);
      if(send_message(&t) < 0){
        perror("[RECEIVER] Send ACK error. Exiting.\n");
        return -1;
      }
    //}
  }


  /*SCRIERE FISIER RECV*/
  /* REDENUMIRE FISIER:*/
  char* prefix = "recv_";        
  char *file_name = (char *) malloc(1 + strlen(prefix)+ strlen(buffer_send[0]) );
  strcpy(file_name, prefix);
  strcat(file_name, buffer_send[0]); 
  //printf("[RECV] fisierul de scriere: %s\n", file_name);
  /*FINISH*/

  int miner_destinatie = open(file_name, O_WRONLY | O_CREAT, 0644);
  
  for(i = 1; i < buffer_size - 1; i++){
    write(miner_destinatie, buffer_send[i], MAXMSG);
  }
  write(miner_destinatie, buffer_send[buffer_size - 1], dim_last_pkg);
  
  
  printf("[RECV]Am terminat\n");
  close(miner_destinatie);

  
  sprintf(t.payload,"FINISH");
  t.len = strlen(t.payload+1);
  if(send_message(&t) < 0){
    perror("[RECEIVER] Send ACK error. Exiting.\n");
    return -1;
  }
  
  return 0;
}
