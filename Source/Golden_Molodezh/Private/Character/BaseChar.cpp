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
	ThrowingDistance = 75;
	WeaponSlots.SetNum(2 * WeaponSlotsCount);
	WeaponIndex = 0;
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

AWeaponBase* ABaseChar::ActiveWeaponRight() const
{
	return WeaponSlots[2 * WeaponIndex];
}

AWeaponBase* ABaseChar::ActiveWeaponLeft() const
{
	return WeaponSlots[2 * WeaponIndex + 1];
}

void ABaseChar::SwitchWeaponNext()
{
	WeaponIndex = (WeaponIndex + 1) % WeaponSlotsCount;
}

void ABaseChar::SwitchWeapon(int Index)
{
	if (Index > 0 && Index < WeaponSlotsCount)
		WeaponIndex = Index;
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
	for (auto item : ArmorBySlotName)
		if (item.Value != nullptr)
			item.Value->ShowFront();
	if (SecondBoot != nullptr)
		SecondBoot->ShowFront();
}

void ABaseChar::ShowBack()
{
	Super::ShowBack();
	Skelet->SetRelativeRotation(FRotator(0, 180, 70));
	for (auto item : ArmorBySlotName)
		if (item.Value != nullptr)
			item.Value->ShowBack();
	if (SecondBoot != nullptr)
		SecondBoot->ShowBack();
}

void ABaseChar::ShowRight()
{
	Super::ShowRight();
	Skelet->SetRelativeRotation(FRotator(-70, -90, 0));
	for (auto item : ArmorBySlotName)
		if (item.Value != nullptr)
			item.Value->ShowRight();

	if (SecondBoot != nullptr)
		SecondBoot->ShowRight();
}

void ABaseChar::ShowLeft()
{
	Super::ShowLeft();
	Skelet->SetRelativeRotation(FRotator(70, 90, 0));
	for (auto item : ArmorBySlotName)
		if (item.Value != nullptr)
			item.Value->ShowLeft();
	if (SecondBoot != nullptr)
		SecondBoot->ShowLeft();
}

ABaseItem* ABaseChar::Equip_Implementation(ABaseItem* Equipment)
{
	if (!CanEquip_Implementation(Equipment))
		return nullptr;

	bool ok = false;

	switch (Equipment->Category)
	{
	case(EItemCategoryEnum::ICE_WeaponBothHand):
	case(EItemCategoryEnum::ICE_WeaponTwoHand):
	case(EItemCategoryEnum::ICE_WeaponOnlyLeftHand):
	case(EItemCategoryEnum::ICE_WeaponOnlyRightHand)://same function
		ok = EquipWeapon(Equipment);
		if (ok && OnEquipmentSlotChanged.IsBound()) // refresh slots
			for (auto entry : SlotNeighborNameRight)
				OnEquipmentSlotChanged.Broadcast(entry.Key);
		break;
	case(EItemCategoryEnum::ICE_ArmorChest):
	case(EItemCategoryEnum::ICE_ArmorHelmet):
	case(EItemCategoryEnum::ICE_ArmorBoot):
	case(EItemCategoryEnum::ICE_ArmorNecklace):
	case(EItemCategoryEnum::ICE_ArmorAmulet):
		ok = EquipArmor(Equipment);
		break;
	default:
		break;
	}
	//remove from backpack if need
	if (ok)
		for (int i = 0; i < BackPack->GetSize(); i++)
			if (BackPack->GetItem(i) == Equipment)
				BackPack->PopItem(i);

	return nullptr;
}

ABaseItem* ABaseChar::EquipAtSlot_Implementation(ABaseItem* Equipment, FName SlotName)
{
	if (!CanEquipAtSlot_Implementation(Equipment, SlotName))
		return Equipment;

	ABaseItem* itemToReturn = nullptr;
	//right hand weapon
	if (SlotNeighborNameRight.Contains(SlotName))
	{
		AWeaponBase* newWeapon = Cast<AWeaponBase>(Equipment);
		if (newWeapon == nullptr)
			return Equipment;

		itemToReturn = EquipWeaponRightHand(newWeapon, SlotIndexByName[SlotName]);
		if (OnEquipmentSlotChanged.IsBound()) OnEquipmentSlotChanged.Broadcast(SlotNeighborNameRight[SlotName]);
	}
	// left hand weapon
	else if (SlotNeighborNameLeft.Contains(SlotName))
	{
		AWeaponBase* newWeapon = Cast<AWeaponBase>(Equipment);
		if (newWeapon == nullptr)
			return Equipment;

		itemToReturn = EquipWeaponLeftHand(newWeapon, SlotIndexByName[SlotName]);
		if (OnEquipmentSlotChanged.IsBound()) OnEquipmentSlotChanged.Broadcast(SlotNeighborNameLeft[SlotName]);
	}
	else if (ArmorBySlotName.Contains(SlotName))
	{
		AArmorBase* newArmor = Cast<AArmorBase>(Equipment);
		if (newArmor == nullptr)
			return Equipment;

		itemToReturn = EquipArmorAtSlot(newArmor, SlotName);
	}

	if (OnEquipmentSlotChanged.IsBound()) OnEquipmentSlotChanged.Broadcast(SlotName);
	return itemToReturn;
}

bool ABaseChar::CanEquip_Implementation(ABaseItem* Equipment)
{
	if (Equipment == nullptr)
		return false;

	if (Equipment->Category == EItemCategoryEnum::ICE_WeaponBothHand
		|| Equipment->Category == EItemCategoryEnum::ICE_WeaponTwoHand
		|| Equipment->Category == EItemCategoryEnum::ICE_WeaponOnlyLeftHand
		|| Equipment->Category == EItemCategoryEnum::ICE_WeaponOnlyRightHand
		|| Equipment->Category == EItemCategoryEnum::ICE_ArmorChest
		|| Equipment->Category == EItemCategoryEnum::ICE_ArmorHelmet
		|| Equipment->Category == EItemCategoryEnum::ICE_ArmorAmulet
		|| Equipment->Category == EItemCategoryEnum::ICE_ArmorBoot
		|| Equipment->Category == EItemCategoryEnum::ICE_ArmorNecklace)
		return true;

	return false;
}

bool ABaseChar::CanEquipAtSlot_Implementation(ABaseItem* Equipment, FName SlotName)
{
	if (Equipment == nullptr)
		return false;

	if (SlotNeighborNameRight.Contains(SlotName) || SlotNeighborNameLeft.Contains(SlotName)
		&& (Equipment->Category == EItemCategoryEnum::ICE_WeaponBothHand
			|| Equipment->Category == EItemCategoryEnum::ICE_WeaponTwoHand
			|| Equipment->Category == EItemCategoryEnum::ICE_WeaponOnlyLeftHand
			|| Equipment->Category == EItemCategoryEnum::ICE_WeaponOnlyRightHand))
		return true;
	else if (SlotName == TEXT("Helmet") && Equipment->Category == EItemCategoryEnum::ICE_ArmorHelmet
		|| SlotName == TEXT("Chest") && Equipment->Category == EItemCategoryEnum::ICE_ArmorChest
		|| SlotName == TEXT("Boot") && Equipment->Category == EItemCategoryEnum::ICE_ArmorBoot
		|| SlotName == TEXT("Necklace") && Equipment->Category == EItemCategoryEnum::ICE_ArmorNecklace
		|| SlotName == TEXT("Amulet1") && Equipment->Category == EItemCategoryEnum::ICE_ArmorAmulet
		|| SlotName == TEXT("Amulet2") && Equipment->Category == EItemCategoryEnum::ICE_ArmorAmulet)
		return true;

	return false;

}

ABaseItem* ABaseChar::GetItemAtSlot_Implementation(FName SlotName)
{

	if (SlotNeighborNameRight.Contains(SlotName))
	{
		return WeaponSlots[2 * SlotIndexByName[SlotName]];
	}
	else if (SlotNeighborNameLeft.Contains(SlotName))
	{
		return WeaponSlots[2 * SlotIndexByName[SlotName] + 1];
	}
	else if (ArmorBySlotName.Contains(SlotName))
	{
		return ArmorBySlotName[SlotName];
	}

	return nullptr;
}

ABaseItem* ABaseChar::PopItemAtSlot_Implementation(FName SlotName)
{
	ABaseItem* itemToReturn = nullptr;

	if (SlotNeighborNameRight.Contains(SlotName))
	{
		itemToReturn = DisEquipWeaponRightHand(SlotIndexByName[SlotName]);
		if (OnEquipmentSlotChanged.IsBound()) OnEquipmentSlotChanged.Broadcast(SlotNeighborNameRight[SlotName]);
	}
	else if (SlotNeighborNameLeft.Contains(SlotName))
	{
		itemToReturn = DisEquipWeaponLeftHand(SlotIndexByName[SlotName]);
		if (OnEquipmentSlotChanged.IsBound()) OnEquipmentSlotChanged.Broadcast(SlotNeighborNameLeft[SlotName]);
	}
	else if (ArmorBySlotName.Contains(SlotName))
	{
		itemToReturn = DisEquipArmor(SlotName);
	}


	/*
	else if (SlotName == TEXT("Helmet"))
	{
		itemToReturn = DisEquipHelmet();
	}
	else if (SlotName == TEXT("Chest"))
	{
		itemToReturn = DisEquipChest();
	}*/

	if (OnEquipmentSlotChanged.IsBound()) OnEquipmentSlotChanged.Broadcast(SlotName);
	return itemToReturn;
}

bool ABaseChar::EquipWeapon(ABaseItem* Equipment)
{
	AWeaponBase* NewWeapon = Cast<AWeaponBase>(Equipment);
	if (NewWeapon == nullptr)
		return false;

	ABaseItem* itemToPutInBackpak = nullptr;
	//find empty slot
	if (NewWeapon->Category == EItemCategoryEnum::ICE_WeaponTwoHand)
	{
		for (int i = 0; i < WeaponSlotsCount; i++)
			if (WeaponSlots[2 * i] == nullptr && WeaponSlots[2 * i + 1] == nullptr) {
				itemToPutInBackpak = EquipWeaponTwoHand(NewWeapon, i);
				return true;
			}
	}
	else {

		// index is actualy array index, not weapon index
		int startIndex = 0;
		int indexIncreace = 1;
		switch (NewWeapon->Category)
		{
		case(EItemCategoryEnum::ICE_WeaponBothHand):
			startIndex = 0;
			indexIncreace = 1;
			break;
		case(EItemCategoryEnum::ICE_WeaponOnlyLeftHand):
			startIndex = 1;
			indexIncreace = 2;
			break;
		case(EItemCategoryEnum::ICE_WeaponOnlyRightHand):
			startIndex = 0;
			indexIncreace = 2;
			break;
		default:
			break;
		}

		for (int i = startIndex; i < WeaponSlots.Num(); i += indexIncreace)
			if (WeaponSlots[i] == nullptr) {
				if (i % 2 == 0) // right hand
					itemToPutInBackpak = EquipWeaponRightHand(NewWeapon, i / 2);
				else // left hand
					itemToPutInBackpak = EquipWeaponLeftHand(NewWeapon, i / 2);
				return true;
			}
	}

	// not found
	if (NewWeapon->Category == EItemCategoryEnum::ICE_WeaponOnlyLeftHand)
		itemToPutInBackpak = EquipWeaponLeftHand(NewWeapon, 0);
	else
		itemToPutInBackpak = EquipWeaponRightHand(NewWeapon, 0);

	if (itemToPutInBackpak != nullptr)
		PutToBackPackOrThrowToWorld(itemToPutInBackpak);

	return true;
}

ABaseItem* ABaseChar::EquipWeaponTwoHand(AWeaponBase* NewWeapon, int Index)
{
	ABaseItem* itemToReturn = nullptr;
	if (NewWeapon == nullptr || NewWeapon->Category != EItemCategoryEnum::ICE_WeaponTwoHand)
		return NewWeapon;

	if (WeaponSlots[2 * Index] != nullptr)
	{
		itemToReturn = WeaponSlots[2 * Index];
	}

	if (WeaponSlots[2 * Index + 1] != nullptr)
	{
		//two weapons in both hand 
		if (itemToReturn != nullptr && itemToReturn->Category != EItemCategoryEnum::ICE_WeaponTwoHand)
		{
			PutToBackPackOrThrowToWorld(WeaponSlots[2 * Index + 1]);
		}
		else
		{
			itemToReturn = WeaponSlots[2 * Index + 1];
		}
	}
	WeaponSlots[2 * Index] = NewWeapon;
	WeaponSlots[2 * Index + 1] = NewWeapon;
	return itemToReturn;
}

ABaseItem* ABaseChar::EquipWeaponRightHand(AWeaponBase* NewWeapon, int Index)
{
	if (NewWeapon == nullptr)
		return nullptr;

	ABaseItem* itemToReturn = nullptr;
	//Equip two hand weapon
	if (NewWeapon->Category == EItemCategoryEnum::ICE_WeaponTwoHand)
		return EquipWeaponTwoHand(NewWeapon, Index);

	if (NewWeapon->Category == EItemCategoryEnum::ICE_WeaponOnlyLeftHand)
		return NewWeapon;

	//Equip one hand weapon
	if (WeaponSlots[2 * Index] != nullptr && WeaponSlots[2 * Index]->Category == EItemCategoryEnum::ICE_WeaponTwoHand)
		WeaponSlots[2 * Index + 1] = nullptr;

	itemToReturn = WeaponSlots[2 * Index];
	WeaponSlots[2 * Index] = NewWeapon;
	return itemToReturn;
}

ABaseItem* ABaseChar::EquipWeaponLeftHand(AWeaponBase* NewWeapon, int Index)
{
	if (NewWeapon == nullptr)
		return nullptr;

	ABaseItem* itemToReturn = nullptr;
	//Equip two hand weapon
	if (NewWeapon->Category == EItemCategoryEnum::ICE_WeaponTwoHand)
		return EquipWeaponTwoHand(NewWeapon, Index);

	if (NewWeapon->Category == EItemCategoryEnum::ICE_WeaponOnlyRightHand)
		return NewWeapon;

	//Equip one hand weapon	
	if (WeaponSlots[2 * Index + 1] != nullptr && WeaponSlots[2 * Index + 1]->Category == EItemCategoryEnum::ICE_WeaponTwoHand)
		WeaponSlots[2 * Index] = nullptr;

	itemToReturn = WeaponSlots[2 * Index + 1];
	WeaponSlots[2 * Index + 1] = NewWeapon;
	return itemToReturn;
}

AWeaponBase* ABaseChar::DisEquipWeaponRightHand(int Index)
{
	AWeaponBase* itemToReturn = WeaponSlots[2 * Index];
	if (WeaponSlots[2 * Index] != nullptr && WeaponSlots[2 * Index]->Category == EItemCategoryEnum::ICE_WeaponTwoHand)
		WeaponSlots[2 * Index + 1] = nullptr;

	WeaponSlots[2 * Index] = nullptr;
	return itemToReturn;
}

AWeaponBase* ABaseChar::DisEquipWeaponLeftHand(int Index)
{
	AWeaponBase* itemToReturn = WeaponSlots[2 * Index + 1];
	if (WeaponSlots[2 * Index + 1] != nullptr && WeaponSlots[2 * Index + 1]->Category == EItemCategoryEnum::ICE_WeaponTwoHand)
		WeaponSlots[2 * Index] = nullptr;

	WeaponSlots[2 * Index + 1] = nullptr;
	return itemToReturn;
}

bool ABaseChar::EquipArmor(ABaseItem* Equipment)
{
	if (Equipment == nullptr)
		return false;

	AArmorBase* newArmor = Cast<AArmorBase>(Equipment);
	if (newArmor == nullptr)
		return false;

	AArmorBase* oldArmor = nullptr;
	FName slotName = TEXT("No");
	if (newArmor->Category == EItemCategoryEnum::ICE_ArmorHelmet)
		slotName = TEXT("Helmet");
	else if (newArmor->Category == EItemCategoryEnum::ICE_ArmorChest)
		slotName = TEXT("Chest");
	else if (newArmor->Category == EItemCategoryEnum::ICE_ArmorBoot)
		slotName = TEXT("Boot");
	else if (newArmor->Category == EItemCategoryEnum::ICE_ArmorNecklace)
		slotName = TEXT("Necklace");
	else if (newArmor->Category == EItemCategoryEnum::ICE_ArmorAmulet)
	{
		if (ArmorBySlotName[TEXT("Amulet1")] != nullptr && ArmorBySlotName[TEXT("Amulet2")] == nullptr)
			slotName = TEXT("Amulet2");
		else
			slotName = TEXT("Amulet1");
	}

	if (slotName == TEXT("No"))
		return false;

	oldArmor = EquipArmorAtSlot(newArmor, slotName);
	PutToBackPackOrThrowToWorld(oldArmor);
	if (OnEquipmentSlotChanged.IsBound()) OnEquipmentSlotChanged.Broadcast(slotName);
	return true;
}

AArmorBase* ABaseChar::EquipArmorAtSlot(AArmorBase* Armor, FName SlotName)
{
	if (Armor == nullptr || !ArmorBySlotName.Contains(SlotName))
		return Armor;

	AArmorBase* oldArmor = DisEquipArmor(SlotName);
	ArmorBySlotName[SlotName] = Armor;

	if (ArmorBoneSlotNameBySlotName.Contains(SlotName))
	{
		Armor->SetActorRelativeRotation(FRotator::ZeroRotator);
		Armor->SetActorRelativeLocation(Armor->LockalOffset);
		Armor->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, ArmorBoneSlotNameBySlotName[SlotName]);
		Armor->SetActorHiddenInGame(false);	
		Armor->ShowSide(PresentSide);

		//create second boot
		if (Armor->Category == EItemCategoryEnum::ICE_ArmorBoot)
		{
			if (SecondBoot != nullptr)
				SecondBoot->Destroy();

			FActorSpawnParameters Parameters;
			Parameters.Template = Armor;
			Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SecondBoot = GetWorld()->SpawnActor<AArmorBase>(Armor->GetClass(), Parameters);
			SecondBoot->SetActorRelativeRotation(FRotator::ZeroRotator);
			SecondBoot->SetActorRelativeLocation(Armor->LockalOffset);
			SecondBoot->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, SecondBootBoneSocketName);
			SecondBoot->SetHidden(false);
			SecondBoot->ShowSide(PresentSide);
		}
	}

	return oldArmor;
}

AArmorBase* ABaseChar::DisEquipArmor(FName SlotName)
{
	AArmorBase* itemToReturn = nullptr;

	if (ArmorBySlotName.Contains(SlotName))
	{
		itemToReturn = ArmorBySlotName[SlotName];
		ArmorBySlotName[SlotName] = nullptr;

		if (itemToReturn != nullptr)
		{
			itemToReturn->SetActorHiddenInGame(true);
			itemToReturn->AttachToActor(BackPack, FAttachmentTransformRules::KeepRelativeTransform);

			if (itemToReturn->Category == EItemCategoryEnum::ICE_ArmorBoot)
			{
				SecondBoot->Destroy();
				SecondBoot = nullptr;
			}
		}

	}

	return itemToReturn;
}

void ABaseChar::PutToBackPackOrThrowToWorld(ABaseItem* Item)
{
	if (Item == nullptr)
		return;

	if (!BackPack->AddItem(Item)) // throw to world if not added
		Item->ThrowToWorld(FVector(0, ThrowingDistance, 0));
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
	BackComponents.Add(HeadSprite);

	LeftEyeSprite = UObject::CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftEyeSprite"));
	LeftEyeSprite->AttachToComponent(HeadSprite, FAttachmentTransformRules::KeepRelativeTransform);
	LeftEyeSprite->SetRelativeLocation(FVector(-4, 1, 1));
	FrontComponents.Add(LeftEyeSprite);
	BackComponents.Add(LeftEyeSprite);

	RightEyeSprite = UObject::CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightEyeSprite"));
	RightEyeSprite->AttachToComponent(HeadSprite, FAttachmentTransformRules::KeepRelativeTransform);
	RightEyeSprite->SetRelativeLocation(FVector(4, 1, 1));
	FrontComponents.Add(RightEyeSprite);
	BackComponents.Add(RightEyeSprite);

	BodySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BodySprite"));
	BodySprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "backSocket");
	BodySprite->SetRelativeLocation(FVector(0.000000, 0.000000, -5));
	FrontComponents.Add(BodySprite);
	BackComponents.Add(BodySprite);

	LeftLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftLegSprite"));
	LeftLegSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "leftLegSocket");
	LeftLegSprite->SetRelativeLocation(FVector(0, 0.000000, 6));
	FrontComponents.Add(LeftLegSprite);
	BackComponents.Add(LeftLegSprite);

	RightLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightLegSprite"));
	RightLegSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "rightLegSocket");
	RightLegSprite->SetRelativeLocation(FVector(0.000000, 0.000000, 6));
	FrontComponents.Add(RightLegSprite);
	BackComponents.Add(RightLegSprite);

	SideHeadSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideHeadSprite"));
	SideHeadSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "headSocket");
	SideHeadSprite->SetRelativeLocationAndRotation(FVector(0, 0, -4), FRotator(0, 90, 0));
	LeftComponents.Add(SideHeadSprite);
	RightComponents.Add(SideHeadSprite);

	SideEyeLeftSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideEyeLeftSprite"));
	SideEyeLeftSprite->AttachToComponent(SideHeadSprite, FAttachmentTransformRules::KeepRelativeTransform);
	SideEyeLeftSprite->SetRelativeLocation(FVector(4, -1, 1));
	LeftComponents.Add(SideEyeLeftSprite);
	RightComponents.Add(SideEyeLeftSprite);

	SideEyeRightSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideEyeRightSprite"));
	SideEyeRightSprite->AttachToComponent(SideHeadSprite, FAttachmentTransformRules::KeepRelativeTransform);
	SideEyeRightSprite->SetRelativeLocation(FVector(4, 1, 1));
	LeftComponents.Add(SideEyeRightSprite);
	RightComponents.Add(SideEyeRightSprite);

	SideBodySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideBodySprite"));
	SideBodySprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "backSocket");
	SideBodySprite->SetRelativeLocationAndRotation(FVector(0, 0, -5), FRotator(0, 90, 0));
	LeftComponents.Add(SideBodySprite);
	RightComponents.Add(SideBodySprite);

	SideLeftLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideLeftLegSprite"));
	SideLeftLegSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "leftLegSocket");
	SideLeftLegSprite->SetRelativeLocationAndRotation(FVector(0, 0, 6), FRotator(0, 90, 0));
	LeftComponents.Add(SideLeftLegSprite);
	RightComponents.Add(SideLeftLegSprite);

	SideRightLegSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SideRightLegSprite"));
	SideRightLegSprite->AttachToComponent(Skelet, FAttachmentTransformRules::KeepRelativeTransform, "rightLegSocket");
	SideRightLegSprite->SetRelativeLocationAndRotation(FVector(0, 0, 6), FRotator(0, 90, 0));
	LeftComponents.Add(SideRightLegSprite);
	RightComponents.Add(SideRightLegSprite);


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
