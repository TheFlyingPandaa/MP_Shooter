#include "PingBeaconHostObject.h"
#include "PingBeacon.h"

APingBeaconHostObject::APingBeaconHostObject(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	/** Set our actual actor client class this host object will handle */
	ClientBeaconActorClass = APingBeacon::StaticClass();
	/** Set the beacon type name **/
	BeaconTypeName = ClientBeaconActorClass->GetName();

	/** Make sure we can tick **/
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

bool APingBeaconHostObject::Init()
{
	//just returning true for now
	return true;
}

void APingBeaconHostObject::DisconnectClient(AOnlineBeaconClient * ClientActor)
{
	Super::DisconnectClient(ClientActor);


}

void APingBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient * LeavingClientActor)
{
	Super::NotifyClientDisconnected(LeavingClientActor);


}

void APingBeaconHostObject::Unregister()
{
	Super::Unregister();


}

void APingBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	//Call super
	Super::OnClientConnected(NewClientActor, ClientConnection);

	//Cast to our actual APingBeacon
	APingBeacon* BeaconClient = Cast<APingBeacon>(NewClientActor);
	if (BeaconClient != NULL)
	{
		//It's good, so lets rpc back to the client and tell it we are ready
		BeaconClient->Ready();
	}
}

AOnlineBeaconClient* APingBeaconHostObject::SpawnBeaconActor(UNetConnection* ClientConnection)
{
	//Just super for now, technically you can return NULL here as well to prevent spawning
	return Super::SpawnBeaconActor(ClientConnection);
}