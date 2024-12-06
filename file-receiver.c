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
  // int last_pack;
  do
  { // Iterate over segments, until last the segment is detected.
    // Receive segment.
    struct sockaddr_in src_addr;
    data_pkt_t data_pkt;
    if ((len =
             recvfrom(sockfd, &data_pkt, sizeof(data_pkt), 0,
                      (struct sockaddr *)&src_addr, &(socklen_t){sizeof(src_addr)})) < 0)
    {
      printf("RECEIVER TIMOUT\n");
      break;
    }
    printf("Received segment %d, size %ld.\n", ntohl(data_pkt.seq_num), len);

    if (ntohl(data_pkt.seq_num) > ntohl(ack_pkt.seq_num) + 1) // DUPACK
    {
      sendto(sockfd, &ack_pkt, sizeof(ack_pkt_t), 0,
             (struct sockaddr *)&src_addr, sizeof(src_addr));
    }
    else
    {
      fwrite(data_pkt.data, 1, len - offsetof(data_pkt_t, data), file);
      //  Send acknowledgment.
      ack_pkt.seq_num = data_pkt.seq_num;
      ack_pkt.seq_num += htonl(1);
      sendto(sockfd, &ack_pkt, sizeof(ack_pkt_t), 0,
             (struct sockaddr *)&src_addr, sizeof(src_addr));
      printf("Sending acknowledgment %d.\n", ntohl(ack_pkt.seq_num));
    }

  } while (true);

  // Clean up and exit.
  close(sockfd);
  fclose(file);

  return EXIT_SUCCESS;
}
