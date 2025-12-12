#include "UDPWrapper.h"


int main()
{
	UDPSocket *test = UDPSocket::generateServer(5588);
	UDPSocket *client = UDPSocket::generateClient();
	UDPSocket::AddressType target;
	bool res = UDPSocket::initTargetAddress(target, "127.0.0.1", 5588);
	char buf1[] = "Hello World";
	unsigned int written = 0;
	res = client->write(target, buf1, sizeof(buf1), written);
	printf("Sent %d bytes\n",written);
	char buf2[100] = {0};

	UDPSocket::AddressType received_from;
	written = 0;
	int addr_len = sizeof(received_from);
	res = test->read(received_from, addr_len, buf2, 100, written);
	printf("UDP Server is %x.\n", test);
	printf("Received %d bytes - %s.\n", written, buf2);



	delete test;

	return 0;



}