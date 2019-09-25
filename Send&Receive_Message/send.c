#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"
#include "add_struct.h"

#define BITS_NO 8
#define HOST "127.0.0.1"
#define PORT 10000
//buna dimineata
int main(int argc,char** argv){
  init(HOST,PORT);
  int nr_ack;
  int no_recv_pkt = 0;
  long i, res;
  int delay = atoi(argv[3]);
  msg t;
  add_struct s;
  struct stat status;
  int miner_sursa = open(argv[1], O_RDONLY);  
  /* ALOCARE BUFFER DE CITIRE */
  fstat(miner_sursa, &status);
  int* buffer_ack;
  int* buffer_len;
  char** buffer_send;
  long buffer_size = ((status.st_size) / MAXMSG) + 2;

  buffer_send =  malloc ( sizeof(char*) *  buffer_size);
  buffer_ack = malloc ( sizeof(int) * buffer_size);
  buffer_len = malloc ( sizeof(int) * buffer_size);
  for(i = 0; i < buffer_size; i++){
    buffer_send[i] = malloc(sizeof(char) * MAXMSG);
    buffer_ack[i] = 0;
    buffer_len[i] = -1;
  }

  //printf("[SEND]dim fisier: %ld \n", status.st_size);
  printf("[SEND]dim buffer: %ld \n",buffer_size);
  /*TRIMITERE PRIM PACHET*/
  buffer_send[0] = argv[1];
  memcpy(&s.payload, buffer_send[0], MAXMSG);
  s.no_pkt = 0;
  s.no_max_pkt = buffer_size;
  s.len = buffer_size;
  buffer_len[0] = s.len;
  s.checksum = 0;
  s.checksum = calcul_sum(s.payload, strlen(s.payload));

  memcpy(t.payload, &s, sizeof(s));
  t.len = sizeof(s);
  send_message(&t);
  
  /*START FEREASTRA*/
  long BDP = atoi(argv[2]) * atoi(argv[3]) * 1000;

  long wnd = BDP / (MSGSIZE * BITS_NO);
  printf("[SEND] BDP = %ld; wnd = %ld\n", BDP, wnd);
  long limita_superioara = (wnd < buffer_size) ? wnd : buffer_size;
  
  for(i = 1; i < limita_superioara; i++){
    //trimitere pachet
    s.len = read(miner_sursa,buffer_send[i],MAXMSG);
    memcpy(&s.payload, buffer_send[i], MAXMSG);
    buffer_len[i] = s.len;
    s.no_pkt = i;
    s.no_max_pkt = buffer_size;
    s.checksum = 0;
    s.checksum = calcul_sum(s.payload, s.len);

    memcpy(t.payload, &s, sizeof(s));
    t.len = sizeof(s);

    res = send_message(&t);
    if(res < 0){
      perror("[SENDER] Send error.\n");
      return -1;
    }
  }

  while(no_recv_pkt < buffer_size){
    for(i = wnd; i < buffer_size; i++){
      res = recv_message_timeout(&t, 2*delay);
      if(res < 0){
        perror("Timeout 1.\n");
        return -1;
      }
      else{
          nr_ack = atoi(t.payload);
          buffer_ack[nr_ack] = 1;
          no_recv_pkt++;
      }
      if(buffer_len[i] == -1){
        s.len = read(miner_sursa,buffer_send[i],MAXMSG);
        memcpy(&s.payload, buffer_send[i], MAXMSG);
        buffer_len[i] = s.len;
      }
      else{
        ;//TODO
      }
      s.no_pkt = i;
      s.no_max_pkt = buffer_size;
      s.checksum = 0;
      s.checksum = calcul_sum(s.payload, s.len);
      
      memcpy(t.payload, &s, sizeof(s));
      t.len = sizeof(s);

      res = send_message(&t);
      if(res < 0){
        perror("[SENDER] Send error.\n");
        return -1;
      }
    }
    for(i = 0; i < limita_superioara; i++){
      res = recv_message_timeout(&t, 2*delay);
        if (res < 0) {
          perror("Timeout 2\n");
          return -1;
        }
        else{
          nr_ack = atoi(t.payload);
          buffer_ack[nr_ack] = 1;
          no_recv_pkt++;
        }
      }
    }
  /*for(i = 0; i < buffer_size; i++){
    printf("buffer_ack[%ld] = %d\n", i, buffer_ack[i]);
  }*/
  
  res = recv_message(&t);
  if (res < 0) {
    perror("[SENDER] Send error. Exiting.\n");
    return -1;
  }
  printf("[SEND]Am terminat\n");
  close(miner_sursa);
  return 0;
}
/*

*/
