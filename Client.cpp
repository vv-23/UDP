/*
demo-udp-01: create a UDP/IP socket

usage:	demo-udp-01

create a socket. Bind it to any available port.
Then use getsockname to print the port.
That's it.
Paul Krzyzanowski
*/

#include <stdlib.h>	/* defines system calls */
#include <iostream>
#include <stdio.h>
#include <cstring>	/* needed for memset */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>	/* needed for sockaddr_in */

main(int argc, char **argv)
{
	struct sockaddr_in myaddr;	/* our address */
	int fd;	/* our socket */
	unsigned int alen;	/* length of address (for getsockname) */

						/* create a udp/ip socket */
						/* request the Internet address protocol */
						/* and a reliable 2-way byte stream (TCP/IP) */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}

	printf("created socket: descriptor = %d\n", fd);

	/* bind to an arbitrary return address */
	/* in this case, assume it's the client side, so we won't
	/* care about the port number since no application will initiate service here */
	/* INADDR_ANY is the IP address and 0 is the socket */
	/* htonl converts a long integer (e.g. address) to a network */
	/* representation (IP-standard byte ordering) */

	/* if you want to use a specific port number (such as myport), use    */
	/*	 	myaddr.sin_port = htons(myport)    */
	/* instead of	myaddr.sin_port = htons(0);    */

	memset((void *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(591);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	alen = sizeof(myaddr);
	if (getsockname(fd, (struct sockaddr *)&myaddr, &alen) < 0) {
		perror("getsockname failed");
		return 0;
	}

	printf("bind complete. Port number = %d\n", ntohs(myaddr.sin_port));
	
	struct hostent *hp;     /* host information */
	struct sockaddr_in servaddr;    /* server address */
	char *my_messsage = "this is a test message";

	/* fill in the server's address and data */
	memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);

	/* look up the address of the server given its name */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "could not obtain address of %s\n", host);
		return 0;
	}

	/* put the host's address into the server address structure */
	memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);

	/* send a message to the server */
	if (sendto(fd, my_message, strlen(my_message), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("sendto failed");
		return 0;
	}
}
