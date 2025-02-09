// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/TCP/NetworkTCPServer.h"
#include "Network/NetworkManager.h"

FNetworkTCPServer::FNetworkTCPServer(FNetworkManager* InNetworkManager)
	: FNetworkServerBase(InNetworkManager)
{
}

FNetworkTCPServer::~FNetworkTCPServer() = default;

void FNetworkTCPServer::Initialize()
{
    FNetworkServerBase::Initialize();

    LoopPtr = uvw::loop::get_default();

    tcp = LoopPtr->resource<uvw::tcp_handle>();

    tcp->on<uvw::listen_event>([](const uvw::listen_event&, uvw::tcp_handle& srv) {
        std::shared_ptr<uvw::tcp_handle> client = srv.parent().resource<uvw::tcp_handle>();

        client->on<uvw::close_event>([ptr = srv.shared_from_this()](const uvw::close_event&, uvw::tcp_handle&) { ptr->close(); });
        client->on<uvw::end_event>([](const uvw::end_event&, uvw::tcp_handle& client) { client.close(); });

        srv.accept(*client);
        client->read();
    });

    tcp->bind("127.0.0.1", 4242);
    tcp->listen();
}

void FNetworkTCPServer::DeInitialize()
{
    FNetworkServerBase::DeInitialize();


}

void FNetworkTCPServer::AsyncWork()
{
}

void FNetworkTCPServer::OnNewClientConnected(uv_stream_t* InUVServer)
{
}

#endif