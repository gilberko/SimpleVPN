#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")



class UDPSocket
{
public:
	typedef sockaddr_in AddressType;

	static bool init()
	{
		WSADATA wsaData = { 0 };
		auto init_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (init_result != 0) {
			return false;
		}
		return true;
	}


	static bool generateSocket(SOCKET &sock)
	{
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		return sock != INVALID_SOCKET;
	}


	static bool initTargetAddress(AddressType &server_address, const char *addr_string, unsigned short port_num)
	{
		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons((u_short)port_num);
		return (inet_pton(AF_INET, addr_string, &server_address.sin_addr) > 0);
	}

	static UDPSocket *generateServer(unsigned int port_num)
	{
		UDPSocket *result = generateClient();
		if (!result)
		{
			return NULL;
		}
		UDPSocket::AddressType server_address;
		UDPSocket::initAddressForServer(server_address, (unsigned short)port_num);
		if (bind(result->m_sock, (sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR)
		{
			delete result;
			return NULL;
		}

		return result;
	}

	static UDPSocket *generateClient()
	{
		if (!UDPSocket::init())
		{
			return NULL;
		}

		SOCKET server_socket;
		if (!UDPSocket::generateSocket(server_socket))
		{
			UDPSocket::deinit();
			return NULL;
		}



		return new UDPSocket(server_socket);
	}

	~UDPSocket()
	{

		closesocket(m_sock);
		
		UDPSocket::deinit();
	}

	bool write(UDPSocket::AddressType &addr, void *buf, unsigned int buf_size, unsigned int &bytes_written)
	{
		auto send_res = sendto(m_sock, (const char *)buf, buf_size, 0, (const sockaddr *)&addr, sizeof(UDPSocket::AddressType));
		if (send_res < 0)
		{
			return false;
		}
		bytes_written = (unsigned int)send_res;
		return true;
	}

	bool read(UDPSocket::AddressType &addr, int &addr_len, void *buf, unsigned int buf_size, unsigned int &bytes_written)
	{
		addr_len = sizeof(UDPSocket::AddressType);

		auto recv_res = recvfrom(m_sock, (char *)buf, buf_size, 0, (sockaddr*)&addr, &addr_len);
		if (recv_res < 0)
		{
			return false;
		}
		bytes_written = (unsigned int)recv_res;
		return true;
	}

protected:
	static void deinit()
	{
		WSACleanup();
	}

	static void initAddressForServer(AddressType &server_address, unsigned short port)
	{
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons((u_short)port);
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	}

private:
	UDPSocket(SOCKET sock):m_sock(sock) {}

	SOCKET m_sock;

};





int main()
{
	UDPSocket *test = UDPSocket::generateServer(5588);
	UDPSocket *client = UDPSocket::generateClient();
	UDPSocket::AddressType target;
	bool res = UDPSocket::initTargetAddress(target, "127.0.0.1", 5588);
	char buf1[] = "Hello World";
	unsigned int written = 0;
	res = client->write(target, buf1, sizeof(buf1), written);
	char buf2[100];
	UDPSocket::AddressType received_from;
	written = 0;
	int addr_len = sizeof(received_from);
	res = test->read(received_from, addr_len, buf2, 100, written);
	printf("UDP Server is %x.\n", test);

	delete test;

	return 0;



}