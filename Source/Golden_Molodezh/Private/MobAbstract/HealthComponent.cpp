// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
	:Super()
{

}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
		return;

	Amount = FMath::Clamp(Amount - Damage, 0.f, MaxAmount);

}
