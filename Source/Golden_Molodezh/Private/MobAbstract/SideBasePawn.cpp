// Fill out your copyright notice in the Description page of Project Settings.


#include "SideBasePawn.h"

// Sets default values
ASideBasePawn::ASideBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootBoxCollision->SetCollisionProfileName(TEXT("Pawn"));
	RootBoxCollision->SetBoxExtent(FVector(16));
	RootComponent = RootBoxCollision;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetCollisionProfileName(TEXT("OverlapHitBox"));
	HitBox->SetBoxExtent(FVector(16));
	HitBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MoveComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMoveComp"));
	MoveComponent->UpdatedComponent = RootBoxCollision;
	MovementVector.Z = -1;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

TArray<UPrimitiveComponent*>& ASideBasePawn::GetComponentArray(uint8 side)
{
	if (side == LEFT_SIDE) return LeftComponents;
	else if (side == RIGHT_SIDE) return RightComponents;
	else if (side == BACK_SIDE) return BackComponents;
	else return FrontComponents;
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
	//Health = Health >= MaxHealth ? MaxHealth : MaxHealth + HealthRegeneration / 1000;
}

// Called to bind functionality to input
void ASideBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Event Hendlers
}

void ASideBasePawn::ShowFront()
{
	if (PresentSide == FRONT_SIDE)
		return;	

	TArray<UPrimitiveComponent*> PastComponents = GetComponentArray(PresentSide);
	PresentSide = FRONT_SIDE;

	for (auto comp : PastComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : FrontComponents)
		comp->SetHiddenInGame(false);

}

void ASideBasePawn::ShowBack()
{
	if (PresentSide == BACK_SIDE)
		return;

	TArray<UPrimitiveComponent*> PastComponents = GetComponentArray(PresentSide);
	PresentSide = BACK_SIDE;

	for (auto comp : PastComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : BackComponents)
		comp->SetHiddenInGame(false);
}

void ASideBasePawn::ShowRight()
{
	if (PresentSide == RIGHT_SIDE)
		return;	

	TArray<UPrimitiveComponent*> PastComponents = GetComponentArray(PresentSide);
	PresentSide = RIGHT_SIDE;

	for (auto comp : PastComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : RightComponents)
		comp->SetHiddenInGame(false);

}

void ASideBasePawn::ShowLeft()
{
	if (PresentSide == LEFT_SIDE)
		return;

	TArray<UPrimitiveComponent*> PastComponents = GetComponentArray(PresentSide);
	PresentSide = LEFT_SIDE;

	for (auto comp : PastComponents)
		comp->SetHiddenInGame(true);

	for (auto comp : LeftComponents)
		comp->SetHiddenInGame(false);
}


void ASideBasePawn::CalculateAnimationAndPosition()
{
	
	if (FMath::IsNearlyZero(MoveComponent->Velocity.X) && FMath::IsNearlyZero(MoveComponent->Velocity.Y))
	{
		if (AnimState != EAnimStateEnum::ASE_Idle)
			ChangeAnimState(EAnimStateEnum::ASE_Idle);
		return;
	}

	if (AnimState != EAnimStateEnum::ASE_Run)
		ChangeAnimState(EAnimStateEnum::ASE_Run);

	if (MoveComponent->Velocity.Y > 0)
	{ 
		// Front
		if (MoveComponent->Velocity.Y > (MoveComponent->Velocity.X > 0 ? MoveComponent->Velocity.X : -MoveComponent->Velocity.X))
			ShowFront();
		else if (MoveComponent->Velocity.X > 0)
			ShowRight();
		else
			ShowLeft();
	}
	else
	{
		//Back
		if (-MoveComponent->Velocity.Y > (MoveComponent->Velocity.X > 0 ? MoveComponent->Velocity.X : -MoveComponent->Velocity.X))
			ShowBack();
		else if (MoveComponent->Velocity.X > 0)
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

void ASideBasePawn::ChangeAnimState(EAnimStateEnum NewAnimState)
{
	AnimState = NewAnimState;
	ChangeAnimStateImplemetnation(NewAnimState);
}
