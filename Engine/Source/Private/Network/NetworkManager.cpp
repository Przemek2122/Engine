// See https://github.com/Przemek2122/GameEngine

#include "CoreEngine.h"

#include "Assets/IniReader/IniManager.h"
#include "Assets/IniReader/IniObject.h"
#include "Network/TCP/NetworkTCPClient.h"
#include "Network/TCP/NetworkTCPServer.h"

#if ENGINE_NETWORK_LIB_ENABLED
#include "Network/NetworkManager.h"

static const std::string NETWORK_CONFIG_INI_NAME = "NetworkSettings";
static const std::string NETWORK_CONFIG_IP = "Default_AddressIP";
static const std::string NETWORK_CONFIG_PORT = "Default_Port";
static const std::string NETWORK_CONFIG_BACKLOG = "Default_BackLog";

FNetworkConnectionData::FNetworkConnectionData(const std::string& InNetworkAddressIP, const int32 InNetworkPort, const int32 InNetworkDefaultBackLog)
	: NetworkAddressIP(InNetworkAddressIP)
	, NetworkPort(InNetworkPort)
	, NetworkDefaultBackLog(InNetworkDefaultBackLog)
{
}

void FNetworkConnectionData::SetNetworkAddressIP(const std::string& NewNetworkAddressIP)
{
	// @TODO add network address validate
	NetworkAddressIP = NewNetworkAddressIP;
}

void FNetworkConnectionData::SetNetworkPort(const int32 NewPort)
{
	NetworkPort = FMath::Clamp(NewPort, 1, 65535);
}

void FNetworkConnectionData::SetNetworkDefaultBackLog(const int32 NewBackLogSize)
{
	NetworkDefaultBackLog = FMath::Clamp(NewBackLogSize, 0, 65535);
}

FNetworkManager::FNetworkManager(FEngine* InEngine, const bool bInIsServer)
	: Engine(InEngine)
	, bIsServer(bInIsServer)
	, DefaultNetworkConnectionData("0.0.0.0", 47529, 128)
{
	FNetworkConnectorServerData NetworkConnectorTCPServerData;
	NetworkConnectorTCPServerData.NetworkServerClass.Set<FNetworkTCPServer>();
	NetworkServerConnectorsMap.Emplace(ENetworkConnectionMethod::TCP, NetworkConnectorTCPServerData);

	FNetworkConnectorClientData NetworkConnectorClientData;
	NetworkConnectorClientData.NetworkServerClass.Set<FNetworkTCPClient>();
	NetworkClientConnectorsMap.Emplace(ENetworkConnectionMethod::TCP, NetworkConnectorClientData);
}

void FNetworkManager::Initialize()
{
	LoadConfig();

	if (bIsServer)
	{
		if (NetworkServerConnectorsMap.ContainsKey(ENetworkConnectionMethod::TCP))
		{
			FNetworkConnectorServerData& NetworkConnectorServerData = NetworkServerConnectorsMap[ENetworkConnectionMethod::TCP];
			FNetworkServerBase* ServerConnector = NetworkConnectorServerData.NetworkServerClass.Allocate(this);
			if (ServerConnector != nullptr)
			{
				ServerConnector->Initialize();

				NetworkServerInstances.Push(ServerConnector);
			}
		}
	}
	else
	{
		if (NetworkClientConnectorsMap.ContainsKey(ENetworkConnectionMethod::TCP))
		{
			FNetworkConnectorClientData& NetworkConnectorClientData = NetworkClientConnectorsMap[ENetworkConnectionMethod::TCP];
			FNetworkClientBase* ClientConnector = NetworkConnectorClientData.NetworkServerClass.Allocate(this);
			if (ClientConnector != nullptr)
			{
				ClientConnector->Initialize();

				NetworkClientInstances.Push(ClientConnector);
			}
		}
	}
}

void FNetworkManager::DeInitialize()
{
	for (FNetworkServerBase* NetworkServerInstance : NetworkServerInstances)
	{
		NetworkServerInstance->DeInitialize();
	}

	for (FNetworkClientBase* NetworkClientInstance : NetworkClientInstances)
	{
		NetworkClientInstance->DeInitialize();
	}
}

void FNetworkManager::LoadConfig()
{
	FIniManager* IniManager = GEngine->GetAssetsManager()->GetIniManager();
	if (IniManager != nullptr)
	{
		std::shared_ptr<FIniObject> IniObject = IniManager->GetIniObject(NETWORK_CONFIG_INI_NAME);
		if (IniObject->DoesIniExist())
		{
			IniObject->LoadIni();

			FIniField IPField = IniObject->FindFieldByName(NETWORK_CONFIG_IP);
			if (IPField.IsValid())
			{
				DefaultNetworkConnectionData.SetNetworkAddressIP(IPField.GetValueAsString());
			}

			FIniField PortField = IniObject->FindFieldByName(NETWORK_CONFIG_PORT);
			if (PortField.IsValid())
			{
				DefaultNetworkConnectionData.SetNetworkPort(PortField.GetValueAsInt());
			}

			FIniField BackLogField = IniObject->FindFieldByName(NETWORK_CONFIG_BACKLOG);
			if (BackLogField.IsValid())
			{
				// @TODO What should be backlog max?
				// Change if needed, 65535 is random number.
				DefaultNetworkConnectionData.SetNetworkDefaultBackLog(BackLogField.GetValueAsInt());
			}
		}
	}
}

void FNetworkManager::InitializeServer()
{
}


void FNetworkManager::InitializeClient()
{
}

void FNetworkManager::OnNewClientConnectToTCP(uv_stream_t* InUVServer)
{
	for (FNetworkServerBase* NetworkServerInstance : NetworkServerInstances)
	{
		if (FNetworkTCPServer* NetworkTcpServer = dynamic_cast<FNetworkTCPServer*>(NetworkServerInstance))
		{
			NetworkTcpServer->OnNewClientConnected(InUVServer);
		}
	}
}

#endif
