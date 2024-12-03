// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/TCP/NetworkTCPServer.h"

#include "Network/NetworkManager.h"

void OnNewClientConnectCallback(uv_stream_t* InUVServer, int InStatus)
{
	if (InStatus < 0)
	{
		LOG_ERROR("New connection error: " << uv_strerror(InStatus));
	}
	else
	{
		FNetworkManager* NetworkManager = GEngine->GetNetworkManager();
		NetworkManager->OnNewClientConnectToTCP(InUVServer);
	}
}

FNetworkTCPServer::FNetworkTCPServer(FNetworkManager* InNetworkManager)
	: FNetworkServerBase(InNetworkManager)
	, UVServer(nullptr)
{
}

FNetworkTCPServer::~FNetworkTCPServer() = default;

void FNetworkTCPServer::Initialize()
{
	Super::Initialize();

	if (NetworkManager != nullptr)
	{
		const FNetworkConnectionData& DefaultConnectionData = NetworkManager->GetDefaultNetworkConnectionData();

		uv_tcp_t TCP_Server;
		uv_tcp_init(LoopForUV, &TCP_Server);

		sockaddr_in addr;
		uv_ip4_addr(DefaultConnectionData.GetNetworkAddressIP().c_str(), DefaultConnectionData.GetNetworkPort(), &addr);

		uv_tcp_bind(&TCP_Server, reinterpret_cast<const sockaddr*>(&addr), 0);

		UVServer = reinterpret_cast<uv_stream_t*>(&TCP_Server);
		UVServer->data = new FCustomNetworkInstanceData(ThreadName);

		const int ListenResult = uv_listen(UVServer, DefaultConnectionData.GetNetworkDefaultBackLog(), OnNewClientConnectCallback);
		if (ListenResult)
		{
			LOG_ERROR("Listen error: " << uv_strerror(ListenResult));
		}
		else
		{
			LOG_INFO("Listening (TCP Server) at: " << DefaultConnectionData.GetNetworkAddressIP() << '@' << DefaultConnectionData.GetNetworkPort());
		}
	}
}

void FNetworkTCPServer::DeInitialize()
{
	Super::DeInitialize();

	if (UVServer != nullptr)
	{
		FCustomNetworkInstanceData* CustomNetworkInstanceData = static_cast<FCustomNetworkInstanceData*>(UVServer->data);
		delete CustomNetworkInstanceData;
	}
}

void FNetworkTCPServer::AsyncWork()
{
	uv_run(LoopForUV, UV_RUN_NOWAIT);
}

void echo_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	if (nread > 0)
	{
		// Create a std::string from the buffer
		std::string data(buf->base, nread);

		// Successfully read data, process it
		LOG_INFO("Received: " << data);
	}
	else if (nread < 0)
	{
		if (nread == UV_EOF)
		{
			// End of file (stream closed)
			LOG_INFO("Connection closed.");
		}
		else
		{
			// An error occurred
			LOG_ERROR("Read error: " << uv_strerror(nread));
		}

		uv_close(reinterpret_cast<uv_handle_t*>(stream), NULL);
	}

	// Free the buffer allocated in alloc_buffer
	free(buf->base);
}

void FNetworkTCPServer::OnNewClientConnected(uv_stream_t* InUVServer)
{
	if (InUVServer->data != nullptr)
	{
		FCustomNetworkInstanceData* CustomData = static_cast<FCustomNetworkInstanceData*>(InUVServer->data);
		if (CustomData != nullptr)
		{
			if (CustomData->ServerThreadName == ThreadName)
			{
				uv_tcp_t* client = static_cast<uv_tcp_t*>(malloc(sizeof(uv_tcp_t)));
				uv_tcp_init(LoopForUV, client);
				if (uv_accept(InUVServer, reinterpret_cast<uv_stream_t*>(client)) == 0)
				{
					uv_read_start(reinterpret_cast<uv_stream_t*>(client), alloc_buffer, echo_read);
				}
			}
		}
	}
}

#endif