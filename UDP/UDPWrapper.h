
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>




class UDPSocket
{
public:
	typedef sockaddr_in AddressType;

	static bool generateSocket(int &sock)
	{
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		return sock != -1;
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
		if (bind(result->m_sock, (sockaddr *)&server_address, sizeof(server_address)) == -1)
		{
			delete result;
			return NULL;
		}

		return result;
	}

	static UDPSocket *generateClient()
	{
		
		int server_socket;
		if (!UDPSocket::generateSocket(server_socket))
		{
			return NULL;
		}



		return new UDPSocket(server_socket);
	}

	~UDPSocket()
	{

		close(m_sock);
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
		socklen_t actual_size = sizeof(UDPSocket::AddressType);
		

		auto recv_res = recvfrom(m_sock, (char *)buf, buf_size, 0, (sockaddr*)&addr, &actual_size);
		addr_len = (int)actual_size;
		
		if (recv_res < 0)
		{
			return false;
		}
		bytes_written = (unsigned int)recv_res;
		return true;
	}

protected:

	static void initAddressForServer(AddressType &server_address, unsigned short port)
	{
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons((u_short)port);
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	}

private:
	UDPSocket(int sock):m_sock(sock) {}

	int m_sock;

};

