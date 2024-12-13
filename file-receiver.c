#include "packet-format.h"
#include <arpa/inet.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  char *file_name = argv[1];
  int port = atoi(argv[2]);
  int max_window_size = atoi(argv[3]);
  int window_position = 0;
  // last_received = 0;

  FILE *file = fopen(file_name, "w");
  if (!file)
  {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  // Prepare server socket.
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // Allow address reuse so we can rebind to the same port,
  // after restarting the server.
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
      0)
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in srv_addr = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(INADDR_ANY),
      .sin_port = htons(port),
  };
  if (bind(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)))
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "Receiving on port: %d\n", port);

  ack_pkt_t ack_pkt = {
      .seq_num = 0,
      .selective_acks = 0,
  };
  struct timeval tv;
  tv.tv_sec = 4;
  tv.tv_usec = 4;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  ssize_t len;
  int last_pack = 0;
  struct sockaddr_in src_addr = {0};
  struct sockaddr_in src_addr_current = {0};
  //  int last_pack;
  do
  { // Iterate over segments, until last the segment is detected.
    // Receive segment.
    data_pkt_t data_pkt;

    if ((len =
             recvfrom(sockfd, &data_pkt, sizeof(data_pkt), 0,
                      (struct sockaddr *)&src_addr, &(socklen_t){sizeof(src_addr)})) < 0)
    {
      if (last_pack)
      {
        printf("EXIT SUCESS\n");
        exit(EXIT_SUCCESS);
      }
      else
      {
        printf("EXIT FAILURE\n");
        exit(EXIT_FAILURE);
      }
    }
    if (src_addr_current.sin_addr.s_addr == 0) // First call
    {
      src_addr_current.sin_addr.s_addr = src_addr.sin_addr.s_addr;
      src_addr_current.sin_port = src_addr.sin_port;
    }
    if (src_addr_current.sin_addr.s_addr != src_addr.sin_addr.s_addr)
    {
      printf("NEW SOURCE IGNORING\n");
      continue;
    }

    if (len == sizeof(data_pkt_t))
    {
      last_pack = 1;
    }
    // printf("R: Received segment %d, size %ld.\n", ntohl(data_pkt.seq_num), len);
    // printf("TESTE, Ack anterior: %d, ack atual: %d\n", ntohl(ack_pkt.seq_num), ntohl(data_pkt.seq_num));
    if ((ntohl(data_pkt.seq_num) > ntohl(ack_pkt.seq_num))) // DUPACK
    {
      if (ntohl(data_pkt.seq_num) != window_position + max_window_size) // checking if in window
      {
        // printf("Window posistion %d GOT seq %d\n", window_position, ntohl(data_pkt.seq_num));
        int position = ntohl(data_pkt.seq_num) - window_position - 1;
        ack_pkt.selective_acks |= htonl((1 << position));

        // printf("RECEIVER: got seq %d waiting in %d\n", ntohl(data_pkt.seq_num), ntohl(ack_pkt.seq_num));
        // printf("RECEIVER: position %d got ack_selective %d\n", position, ntohl(ack_pkt.selective_acks));
        fseek(file, ntohl(data_pkt.seq_num) * MAX_CHUNK_SIZE, 0); // Move file pointer to the offset
        fwrite(data_pkt.data, 1, len - offsetof(data_pkt_t, data), file);
        //  printf("R: First Zero is at: %d\n", find_first_zero(ntohl(ack_pkt.selective_acks), max_window_size - 1));
      }

      sendto(sockfd, &ack_pkt, sizeof(ack_pkt_t), 0,
             (struct sockaddr *)&src_addr, sizeof(src_addr));
      // printf("R: Sending DUPACK %d.\n", ntohl(ack_pkt.seq_num));
    }
    else // RECEIVED HAS EXPECTED SEQ
    {
      fseek(file, ntohl(data_pkt.seq_num) * MAX_CHUNK_SIZE, 0); // Move file pointer to the offset
      fwrite(data_pkt.data, 1, len - offsetof(data_pkt_t, data), file);
      //  Send acknowledgment.
      // ack_pkt.seq_num = htonl(ntohl(data_pkt.seq_num) + 1);
      // ack_pkt.seq_num += htonl(1);
      //  Checking bits of Selective Ack
      // if (ack_pkt.selective_acks)
      //{
      int advances = 1;
      // printf("RECEIVER: GOT WHAT EXPECTED %d - With slecive ack  %d\n", ntohl(ack_pkt.seq_num), ntohl(ack_pkt.selective_acks));
      for (int i = 0; i < max_window_size - 1; i++)
      {
        int bit = (ntohl(ack_pkt.selective_acks) >> i) & 1;
        // window_position++;
        // ack_pkt.seq_num = htonl(ntohl(data_pkt.seq_num) + 1);
        // printf("RECEIVER: ADVANCING Bit %d: %d \n", i, bit);
        // ack_pkt.selective_acks = htonl(ntohl(ack_pkt.selective_acks) >> 1);
        if (bit == 0)
        {
          // printf("RECEIVER: FOUND A 0 BREAKING\n");
          break;
        }
        advances++;
      }

      // printf("RECEIVER: ADVANCES %d\n", advances);
      ack_pkt.seq_num = htonl(ntohl(data_pkt.seq_num) + advances);
      ack_pkt.selective_acks = htonl(ntohl(ack_pkt.selective_acks) >> advances);
      window_position += advances;
      // printf("RECEIVER: NEW WINDOW POSITION %d\n", window_position);
      // printf("RECEIVER: NEW ACK %d --> %d\n", ntohl(ack_pkt.seq_num), ntohl(ack_pkt.selective_acks));
      // }
      sendto(sockfd, &ack_pkt, sizeof(ack_pkt_t), 0,
             (struct sockaddr *)&src_addr, sizeof(src_addr));
      // printf("R: Sending acknowledgment %d.\n", ntohl(ack_pkt.seq_num));
    }
  } while (true);

  // Clean up and exit.
  close(sockfd);
  fclose(file);

  return EXIT_SUCCESS;
}
