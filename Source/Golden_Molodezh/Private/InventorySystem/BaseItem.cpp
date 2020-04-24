// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "Inventory.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//bCanBeUsed = false;
	bPickedUp = false;
	MaxStackSize = 1;
	Amount = 1;
	Shpere = CreateDefaultSubobject<USphereComponent>("RootSphere");
	Shpere->SetSphereRadius(16);
	Shpere->SetCollisionProfileName(TEXT("OverlapInventory"));
	Shpere->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::BeginOverlap);

	RootComponent = Shpere;

}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseItem::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bPickedUp)
		return;

	if (OtherActor == nullptr)
		return;

	AInventory* inventory = Cast<AInventory>(OtherActor);
	if (inventory == nullptr)
		return;

	inventory->AddItem(this);

}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::PickUp()
{
	bPickedUp = true;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorRelativeLocation(FVector::ZeroVector);

}


void ABaseItem::ThrowToWorld(FVector WorldOffset)
{
	SetActorRelativeLocation(FVector::ZeroVector);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AddActorWorldOffset(WorldOffset, false, nullptr, ETeleportType::TeleportPhysics);
	bPickedUp = false;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
}

void ABaseItem::Use(AActor* User)
{
	UseBluePrintImplemetnation(User);
}


