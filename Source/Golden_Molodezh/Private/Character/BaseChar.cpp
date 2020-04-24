// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChar.h"

ABaseChar::ABaseChar()
{

}

ABaseChar::ABaseChar(const FObjectInitializer& ObjectInitializer)
{
	BuildComponentHierarchy();
	InitSprites();
	ShowFront();
}

ABaseChar::~ABaseChar()
{

}

void ABaseChar::CreateBackPack()
{
	BackPackSize = 16;
	FActorSpawnParameters Parameters;
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BackPack = this->GetWorld()->SpawnActor<ABackPack>(FVector::ZeroVector, FRotator::ZeroRotator, Parameters);
	BackPack->SetActorRelativeLocation(FVector::ZeroVector);
	BackPack->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	BackPack->SetSize(BackPackSize);
}

void ABaseChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Event Hendlers
	PlayerInputComponent->BindAxis("MoveRight", this, &ASideBasePawn::MoveX);
	PlayerInputComponent->BindAxis("MoveUp", this, &ASideBasePawn::MoveY);
}

void ABaseChar::ShowFront()
{
	Super::ShowFront();
	Skelet->SetRelativeRotation(FRotator(0, 0, -70));
}

void ABaseChar::ShowBack()
{
	Super::ShowBack();
	Skelet->SetRelativeRotation(FRotator(0, 180, 70));
}

void ABaseChar::ShowRight()
{
	Super::ShowRight();
	Skelet->SetRelativeRotation(FRotator(-70, -90, 0));
}

void ABaseChar::ShowLeft()
{
	Super::ShowLeft();
	Skelet->SetRelativeRotation(FRotator(70, 90, 0));
}

void ABaseChar::BuildComponentHierarchy()
{
	RootBoxCollision->SetBoxExtent(FVector(6, 14, 16));
	HitBox->SetBoxExtent(FVector(9, 20, 16));
	HitBox->SetRelativeLocation(FVector(0, -3, 0));

	Skelet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skelet"));
	Skelet->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		skelet_asset(TEXT("SkeletalMesh'/Game/Characters/BaseChar/Resources/baseCharArm_run.baseCharArm_run'"));
	Skelet->SetSkeletalMesh(skelet_asset.Object);
	Skelet->SetWorldTransform(FTransform(FRotator(0., 0., -70.),
		FVector(0.000000, 16.828100, -9.317459), FVector(0.17, 0.17, 0.17)));


	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> FoundAnimBP(TEXT("AnimBlueprint'/Game/Characters/BaseChar/Resources/bc_AnimBlueprint.bc_AnimBlueprint'"));
	UAnimBlueprintGeneratedClass* animBP = FoundAnimBP.Object->GetAnimBlueprintGeneratedClass();
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> skeletMaterialAsset(TEXT("Material'/Game/Material/M_TransparentMaterial.M_TransparentMaterial'"));
	Skelet->SetMaterial(0, skeletMaterialAsset.Object);

	Skelet->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	Skelet->SetAnimInstanceClass(animBP);


	HandSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("HandSprite"));
	HandSprite->SetRelativeRotation(FRotator(0, 0, -90));
	HandSprite->SetRelativeLocation(FVector(0, 1, 0));

	HeadSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("HeadSprite"));
	HeadSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "headSocket");
	HeadSprite->SetRelativeLocation(FVector(0.000000, -0.000000, -3.921569));
	FrontComponents.Add(HeadSprite);

	LeftEyeSprite = UObject::CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftEyeSprite"));
	LeftEyeSprite->AttachToComponent(HeadSprite, FAttachmentTransformRules::KeepRelativeTransform);
	LeftEyeSprite->SetRelativeLocation(FVector(-4, 1, 1));
	FrontComponents.Add(LeftEyeSprite);

	RightEyeSprite = UObject::CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightEyeSprite"));
	RightEyeSprite->AttachToComponent(HeadSprite, FAttachmentTransformRules::KeepRelativeTransform);
	RightEyeSprite->SetRelativeLocation(FVector(4, 1, 1));
	FrontComponents.Add(RightEyeSprite);

	BodySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BodySprite"));
	BodySprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "backSocket");
	BodySprite->SetRelativeLocation(FVector(0.000000, 0.000000, -4.901960));
	FrontComponents.Add(BodySprite);

	LeftLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftLegSprite"));
	LeftLegSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "leftLegSocket");
	LeftLegSprite->SetRelativeLocation(FVector(1, 0.000000, 6));
	FrontComponents.Add(LeftLegSprite);

	RightLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightLegSprite"));
	RightLegSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "rightLegSocket");
	RightLegSprite->SetRelativeLocation(FVector(0.000000, 0.000000, 6));
	FrontComponents.Add(RightLegSprite);

	SideHeadSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideHeadSprite"));
	SideHeadSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "headSocket");
	SideHeadSprite->SetRelativeLocationAndRotation(FVector(0, 0, -4), FRotator(0, 90, 0));
	LeftComponents.Add(SideHeadSprite);

	SideEyeLeftSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideEyeLeftSprite"));
	SideEyeLeftSprite->AttachToComponent(SideHeadSprite, FAttachmentTransformRules::KeepRelativeTransform);
	SideEyeLeftSprite->SetRelativeLocation(FVector(4, -1, 1));
	LeftComponents.Add(SideEyeLeftSprite);

	SideEyeRightSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideEyeRightSprite"));
	SideEyeRightSprite->AttachToComponent(SideHeadSprite, FAttachmentTransformRules::KeepRelativeTransform);
	SideEyeRightSprite->SetRelativeLocation(FVector(4, 1, 1));
	LeftComponents.Add(SideEyeRightSprite);

	SideBodySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideBodySprite"));
	SideBodySprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "backSocket");
	SideBodySprite->SetRelativeLocationAndRotation(FVector(0, 0, -5), FRotator(0, 90, 0));
	LeftComponents.Add(SideBodySprite);

	SideLeftLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideLeftLegSprite"));
	SideLeftLegSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "leftLegSocket");
	SideLeftLegSprite->SetRelativeLocationAndRotation(FVector(0, 0, 6), FRotator(0, 90, 0));
	LeftComponents.Add(SideLeftLegSprite);

	SideRightLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideRightLegSprite"));
	SideRightLegSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "rightLegSocket");
	SideRightLegSprite->SetRelativeLocationAndRotation(FVector(0, 0, 6), FRotator(0, 90, 0));
	LeftComponents.Add(SideRightLegSprite);



	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->TargetArmLength = 300;
	CameraBoom->SetRelativeRotation(FRotator(290, -90, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	Camera->SetFieldOfView(90);



	//LeftLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftLegSprite"));

	//RightLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightLegSprite"));



	const int sprites_count = 13;
	UPaperSpriteComponent* sprites[sprites_count] =
	{
		HandSprite,
		HeadSprite,
		LeftEyeSprite,
		RightEyeSprite,
		BodySprite,
		LeftLegSprite,
		RightLegSprite,
		SideHeadSprite,
		SideEyeLeftSprite,
		SideEyeRightSprite,
		SideBodySprite,
		SideLeftLegSprite,
		SideRightLegSprite,
	};


	static ConstructorHelpers::FObjectFinder<UMaterial>
		material_asset(TEXT("/Game/Material/MyDefaultSpriteMaterial.MyDefaultSpriteMaterial"));

	for (int i = 0; i < sprites_count; i++)
	{
		sprites[i]->SetGenerateOverlapEvents(false);
		sprites[i]->SetMaterial(0, material_asset.Object);
		sprites[i]->CastShadow = true;
		sprites[i]->SetCollisionProfileName("NoCollision");
	}

}

void ABaseChar::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseChar::InitSprites()
{
	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		asset1(TEXT("/Game/Characters/BaseChar/Resources/Sprites/bc_head_Sprite.bc_head_Sprite"));
	HeadSprite->SetSprite(asset1.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		asset2(TEXT("PaperSprite'/Game/Characters/BaseChar/Resources/Sprites/bc_eye_Sprite.bc_eye_Sprite'"));
	LeftEyeSprite->SetSprite(asset2.Object);
	RightEyeSprite->SetSprite(asset2.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		asset3(TEXT("PaperSprite'/Game/Characters/BaseChar/Resources/Sprites/bc_body_Sprite.bc_body_Sprite'"));
	BodySprite->SetSprite(asset3.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		asset4(TEXT("PaperSprite'/Game/Characters/BaseChar/Resources/Sprites/bc_left_leg_Sprite.bc_left_leg_Sprite'"));
	LeftLegSprite->SetSprite(asset4.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		asset5(TEXT("PaperSprite'/Game/Characters/BaseChar/Resources/Sprites/bc_right_leg_Sprite.bc_right_leg_Sprite'"));
	RightLegSprite->SetSprite(asset5.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		asset6(TEXT("PaperSprite'/Game/Characters/BaseChar/Resources/Sprites/bc_head_Sprite.bc_head_Sprite'"));
	SideHeadSprite->SetSprite(asset6.Object);

	//static ConstructorHelpers::FObjectFinder<UPaperSprite>
	//	asset7(TEXT(""));	
	SideEyeLeftSprite->SetSprite(asset2.Object);

	//static ConstructorHelpers::FObjectFinder<UPaperSprite>
	//	asset8(TEXT(""));
	SideEyeRightSprite->SetSprite(asset2.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		asset9(TEXT("PaperSprite'/Game/Characters/BaseChar/Resources/Sprites/bc_side_body_Sprite.bc_side_body_Sprite'"));
	SideBodySprite->SetSprite(asset9.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite>
		asset10(TEXT("PaperSprite'/Game/Characters/BaseChar/Resources/Sprites/bc_side_leg_Sprite.bc_side_leg_Sprite'"));
	SideLeftLegSprite->SetSprite(asset10.Object);
	SideRightLegSprite->SetSprite(asset10.Object);
}
