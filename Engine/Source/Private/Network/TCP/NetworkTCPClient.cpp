// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/TCP/NetworkTCPClient.h"

#include "Network/NetworkManager.h"

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	if (nread > 0)
	{
		printf("Received: %.*s\n", static_cast<int>(nread), buf->base); // Process received data
	}
	else if (nread < 0)
	{
		if (nread == UV_EOF)
		{
			LOG_INFO("Connection closed by server\n");
		}
		else
		{
			LOG_ERROR("Read error: " << uv_strerror(nread));
		}

		uv_close(reinterpret_cast<uv_handle_t*>(stream), nullptr); // Close the stream on error or EOF
	}

	if (buf->base)
	{
		free(buf->base); // Free the allocated buffer
	}
}

void on_write(uv_write_t* req, int status)
{
	if (status < 0)
	{
		LOG_ERROR("Write error: " << uv_strerror(status));
	}
	else
	{
		LOG_INFO("Message sent successfully\n");
	}

	free(req); // Free the write request
}

void write_message(uv_tcp_t* socket, const char* message)
{
	uv_buf_t buf = uv_buf_init((char*)message, strlen(message)); // Initialize buffer
	uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));   // Allocate write request

	int ret = uv_write(req, (uv_stream_t*)socket, &buf, 1, on_write);
	if (ret < 0)
	{
		LOG_ERROR("Write failed: " << uv_strerror(ret));

		free(req); // Free request if write fails
	}
}

void on_connect(uv_connect_t* req, int status)
{
	if (status < 0)
	{
		LOG_ERROR("Connection error: " << uv_strerror(status));
	}
	else
	{
		LOG_INFO("Successfully connected to the server!");

		// You can now use req->handle to interact with the server (e.g., read/write data).

		// Read
		uv_read_start(req->handle, alloc_buffer, on_read);

		// Write
		write_message(reinterpret_cast<uv_tcp_t*>(req->handle), "Hello, server!");
	}

	// Cleanup the connect request
	free(req);
}

FNetworkTCPClient::FNetworkTCPClient(FNetworkManager* InNetworkManager)
	: FNetworkClientBase(InNetworkManager)
	, TCP_Socket(nullptr)
	, Connection(nullptr)
	, DestinationSocket(nullptr)
{
}

void FNetworkTCPClient::Initialize()
{
	FNetworkClientBase::Initialize();

	const FNetworkConnectionData& DefaultNetworkConnectionData = NetworkManager->GetDefaultNetworkConnectionData();

	TCP_Socket = static_cast<uv_tcp_t*>(malloc(sizeof(uv_tcp_t)));
	uv_tcp_init(LoopForUV, TCP_Socket);

	Connection = static_cast<uv_connect_t*>(malloc(sizeof(uv_connect_t)));

	DestinationSocket = new sockaddr_in();
	uv_ip4_addr(DefaultNetworkConnectionData.GetNetworkAddressIP().c_str(), DefaultNetworkConnectionData.GetNetworkPort(), DestinationSocket);
}

void FNetworkTCPClient::DeInitialize()
{
	delete TCP_Socket;
	delete Connection;
	delete DestinationSocket;

	FNetworkClientBase::DeInitialize();
}

void FNetworkTCPClient::AsyncWork()
{
	FNetworkClientBase::AsyncWork();

	uv_tcp_connect(Connection, TCP_Socket, reinterpret_cast<sockaddr*>(DestinationSocket), on_connect);
}

#endif