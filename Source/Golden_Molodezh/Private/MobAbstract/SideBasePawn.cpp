// Fill out your copyright notice in the Description page of Project Settings.


#include "SideBasePawn.h"

// Sets default values
ASideBasePawn::ASideBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootBoxCollision->SetBoxExtent(FVector(16));
	RootComponent = RootBoxCollision;

	MoveComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMoveComp"));
	MoveComponent->UpdatedComponent = RootBoxCollision;
	MovementVector.Z = -1;
}

// Called when the game starts or when spawned
void ASideBasePawn::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ASideBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddMovementInput(MovementVector);
	CalculateAnimationAndPosition();
}

// Called to bind functionality to input
void ASideBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Event Hendlers
}

void ASideBasePawn::ShowFront()
{
	for (auto comp : FrontComponents)	
		comp->SetHiddenInGame(false);
	
	for (auto comp : LeftComponents)	
		comp->SetHiddenInGame(true);

}

void ASideBasePawn::ShowBack()
{
	for (auto comp : FrontComponents)
		comp->SetHiddenInGame(false);

	for (auto comp : LeftComponents)
		comp->SetHiddenInGame(true);
}

void ASideBasePawn::ShowRight()
{	
	for (auto comp : LeftComponents)
		comp->SetHiddenInGame(false);

	for (auto comp : FrontComponents)
		comp->SetHiddenInGame(true);

}

void ASideBasePawn::ShowLeft()
{
	for (auto comp : LeftComponents)
		comp->SetHiddenInGame(false);

	for (auto comp : FrontComponents)
		comp->SetHiddenInGame(true);
}

void ASideBasePawn::CalculateAnimationAndPosition()
{
	if (FMath::IsNearlyZero(MovementVector.X) && FMath::IsNearlyZero(MovementVector.Y))
	{
		AnimState = EAnimStateEnum::ASE_Idle;
		return;
	}

	AnimState = EAnimStateEnum::ASE_Run;

	if (MovementVector.Y > 0)
	{ 
		// Front
		if (MovementVector.Y > (MovementVector.X > 0 ? MovementVector.X : -MovementVector.X))
			ShowFront();
		else if (MovementVector.X > 0)
			ShowRight();
		else
			ShowLeft();
	}
	else
	{
		//Back
		if (-MovementVector.Y > (MovementVector.X > 0 ? MovementVector.X : -MovementVector.X))
			ShowBack();
		else if (MovementVector.X > 0)
			ShowRight();
		else
			ShowLeft();
	}

}

void ASideBasePawn::MoveX(float Value)
{
	MovementVector.X = Value;
}

void ASideBasePawn::MoveY(float Value)
{
	MovementVector.Y = Value;
}
