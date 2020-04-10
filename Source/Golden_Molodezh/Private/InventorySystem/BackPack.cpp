// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPack.h"

// Sets default values
ABackPack::ABackPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USphereComponent* Shpere = CreateDefaultSubobject<USphereComponent>("RootSphere");
	Shpere->SetSphereRadius(32);
	//Shpere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Shpere->SetCollisionProfileName(TEXT("OverlapItem"));
	RootComponent = Shpere;
}

// Called when the game starts or when spawned
void ABackPack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABackPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

