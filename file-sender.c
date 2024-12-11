#include "packet-format.h"
#include <limits.h>
#include <netdb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  char *file_name = argv[1];
  char *host = argv[2];
  int port = atoi(argv[3]);
  int max_window_size = atoi(argv[4]);
  int window_position = 0;
  int sent = 0;
  int dupack_counter = 0;
  int received_ack = 0;

  FILE *file = fopen(file_name, "r");
  if (!file)
  {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  // Prepare server host address.
  struct hostent *he;
  if (!(he = gethostbyname(host)))
  {
    perror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in srv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr = *((struct in_addr *)he->h_addr),
  };

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  uint32_t seq_num = 0;
  data_pkt_t data_pkt;
  size_t data_len;
  int retry = 0;
  int num_timeouts = 0;

  data_pkt_t ack_pkt;
  ack_pkt.seq_num = htonl(0);
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  // ssize_t len;
  while (!(feof(file) && data_len < sizeof(data_pkt.data)) || retry > 0)
  { // Generate segments from file, until the the end of the file.
    // Prepare data segment.
    if (num_timeouts >= 3)
    {
      printf("S: Too many timeouts, exiting\n");
      exit(EXIT_FAILURE);
    }
    if (retry)
    {
      // setting to last chunk
      printf("resetting cursor, to %d\n", ntohl(ack_pkt.seq_num));
      // printf("S: RESENDING SEQ NUM %d\n", received_ack);
      fseek(file, ntohl(ack_pkt.seq_num) * MAX_CHUNK_SIZE, 0);
      // fseek(file, received_ack * MAX_CHUNK_SIZE, 0);
      sent = received_ack;
      seq_num = received_ack;
      retry = 0;
    }

    // Load data from file.

    // Send segment.
    printf("TESTE: Window position: %d, SENT: %d\n", window_position, sent);
    while (sent < window_position + max_window_size)
    {
      data_pkt.seq_num = htonl(seq_num++);
      data_len = fread(data_pkt.data, 1, sizeof(data_pkt.data), file);
      ssize_t sent_len =
          sendto(sockfd, &data_pkt, offsetof(data_pkt_t, data) + data_len, 0,
                 (struct sockaddr *)&srv_addr, sizeof(srv_addr));
      // printf("S: SENDING SEG,  %d\n", data_pkt.seq_num);
      printf("S: Sending segment %d, size %ld.\n", ntohl(data_pkt.seq_num),
             offsetof(data_pkt_t, data) + data_len);
      sent++;
      if (sent_len != offsetof(data_pkt_t, data) + data_len)
      {
        fprintf(stderr, "Truncated packet.\n");
        exit(EXIT_FAILURE);
      }
    }
    struct sockaddr_in src_addr;
    if (recvfrom(sockfd, &ack_pkt, sizeof(ack_pkt), 0,
                 (struct sockaddr *)&src_addr, &(socklen_t){sizeof(src_addr)}) < 0)
    {
      printf("S: Timeout\n");
      num_timeouts += 1;
      retry = 1;
    }
    else
    {
      printf("SENDER: ACK RECEIVED %d SEQ %d\n", ntohl(ack_pkt.seq_num), ntohl(data_pkt.seq_num));
      if (ntohl(ack_pkt.seq_num) == received_ack)
      {
        dupack_counter++;
        printf("S: DUPACK, number %d\n", dupack_counter);
        if (dupack_counter >= 3)
        {
          printf("S: Fast retransmit.\n");
          retry = 1;
          continue;
        }
        continue;
      }
      else
      {
        received_ack = ntohl(ack_pkt.seq_num);
        dupack_counter = 0;
        window_position = ntohl(ack_pkt.seq_num);
        printf("S: Window position: %d\n", window_position);
        printf("S: Received ack %d.\n", ntohl(ack_pkt.seq_num));
      }
    }
  }
  // Clean up and exit.
  close(sockfd);
  fclose(file);

  return EXIT_SUCCESS;
}
