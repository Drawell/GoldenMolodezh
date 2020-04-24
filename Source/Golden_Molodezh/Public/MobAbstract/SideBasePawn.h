// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SideBasePawn.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAnimStateEnum : uint8
{
	ASE_Idle 	UMETA(DisplayName = "Idle"),
	ASE_Run 	UMETA(DisplayName = "Run")
};

UCLASS()
class GOLDEN_MOLODEZH_API ASideBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASideBasePawn();

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Collision)
		UBoxComponent* RootBoxCollision;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Collision)
		UBoxComponent* HitBox;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement)
		UFloatingPawnMovement* MoveComponent;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement)
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		EAnimStateEnum AnimState;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = State)
		TArray<UPrimitiveComponent*> LeftComponents;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = State)
		TArray<UPrimitiveComponent*> FrontComponents;

private:
	FVector MovementVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ShowFront();
	virtual void ShowBack();
	virtual void ShowRight();
	virtual void ShowLeft();

	virtual void CalculateAnimationAndPosition();

	virtual void MoveX(float Value);
	virtual void MoveY(float Value);
	
};
