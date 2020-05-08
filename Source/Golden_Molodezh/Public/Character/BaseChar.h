// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseItem.h"
#include "BackPack.h"
#include "WeaponBase.h"
#include "ArmorBase.h"
#include "EquippedMobInterface.h"
#include "MobAbstract/SideBasePawn.h"

#include "Engine.h"
#include "CoreMinimal.h"

#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/CollisionProfile.h"
#include "BaseChar.generated.h"


/**
 *
 */
UCLASS()
class GOLDEN_MOLODEZH_API ABaseChar : public ASideBasePawn, public IEquippedMobInterface
{
	GENERATED_BODY()

public:
	ABaseChar();
	ABaseChar(const FObjectInitializer& ObjectInitializer);
	~ABaseChar();

	//*Property block
public:

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* HandSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* HeadSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* LeftEyeSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* RightEyeSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* BodySprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* LeftLegSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* RightLegSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* SideHeadSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* SideEyeLeftSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* SideEyeRightSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* SideBodySprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* SideLeftLegSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		UPaperSpriteComponent* SideRightLegSprite;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Paper)
		USkeletalMeshComponent* Skelet;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		ABackPack* BackPack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
		int BackPackSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
		int ThrowingDistance;

	UPROPERTY(BlueprintAssignable, Category = Equipment)
		FEquipmentSlotChangeEvent OnEquipmentSlotChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
		int WeaponSlotsCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
		AArmorBase* Helmet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment)
		AArmorBase* Chest;

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void CreateBackPack();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		AWeaponBase* ActiveWeaponRight() const;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		AWeaponBase* ActiveWeaponLeft() const;

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void SwitchWeaponNext();

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void SwitchWeapon(int Index);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//SideBasePawn overrides
	virtual void ShowFront() override;
	virtual void ShowBack() override;
	virtual void ShowRight() override;
	virtual void ShowLeft() override;

	//EquippedMob implementation
	ABaseItem* Equip_Implementation(ABaseItem* Equipment) override;
	ABaseItem* EquipAtSlot_Implementation(ABaseItem* Equipment, FName SlotName) override;
	bool CanEquip_Implementation(ABaseItem* Equipment) override;
	bool CanEquipAtSlot_Implementation(ABaseItem* Equipment, FName SlotName) override;
	ABaseItem* GetItemAtSlot_Implementation(FName SlotName) override;
	ABaseItem* PopItemAtSlot_Implementation(FName SlotName) override;

	//Equip functions

	//calls when equip by left click
	bool EquipWeapon(ABaseItem* Equipment);
	ABaseItem* EquipWeaponTwoHand(AWeaponBase* NewWeapon, int Index);
	ABaseItem* EquipWeaponRightHand(AWeaponBase* NewWeapon, int Index);
	ABaseItem* EquipWeaponLeftHand(AWeaponBase* NewWeapon, int Index);
	AWeaponBase* DisEquipWeaponRightHand(int Index);
	AWeaponBase* DisEquipWeaponLeftHand(int Index);

	//calls when equip by left click
	bool EquipArmor(ABaseItem* Equipment);
	AArmorBase* EquipArmorAtSlot(AArmorBase* Armor, FName SlotName);
	AArmorBase* DisEquipArmor(FName SlotName);

	void PutToBackPackOrThrowToWorld(ABaseItem* Item);
	

private:
	//weapon
	int WeaponIndex;
	TArray<AWeaponBase*> WeaponSlots;

	const TMap<FName, FName> SlotNeighborNameRight =
	{
		{TEXT("WeaponRight1"), TEXT("WeaponLeft1")},
		{TEXT("WeaponRight2"), TEXT("WeaponLeft2")},
		{TEXT("WeaponRight3"), TEXT("WeaponLeft3")}
	};

	const TMap<FName, FName> SlotNeighborNameLeft =
	{
		{TEXT("WeaponLeft1"), TEXT("WeaponRight1")},
		{TEXT("WeaponLeft2"), TEXT("WeaponRight2")},
		{TEXT("WeaponLeft3"), TEXT("WeaponRight3")}
	};

	const TMap<FName, int> SlotIndexByName =
	{
		{TEXT("WeaponRight1"), 0},
		{TEXT("WeaponLeft1"),  0},
		{TEXT("WeaponRight2"), 1},
		{TEXT("WeaponLeft2"),  1},
		{TEXT("WeaponRight3"), 2},
		{TEXT("WeaponLeft3"),  2}
	};

	//equipment
	TMap<FName, AArmorBase*> ArmorBySlotName =
	{
		{TEXT("Helmet"), nullptr},
		{TEXT("Chest"), nullptr},
		{TEXT("Boot"), nullptr},
		{TEXT("Necklace"), nullptr},
		{TEXT("Amulet1"), nullptr},
		{TEXT("Amulet2"), nullptr},
	};

	AArmorBase* SecondBoot = nullptr;
	const FName SecondBootBoneSocketName = TEXT("leftLegSocket");

	const TMap<FName, FName> ArmorBoneSlotNameBySlotName =
	{
		{TEXT("Helmet"), TEXT("headSocket")},
		{TEXT("Chest"), TEXT("backSocket")},
		{TEXT("Boot"), TEXT("rightLegSocket")}
	};
	

private:
	void BuildComponentHierarchy();

protected:
	virtual void BeginPlay() override;
	virtual void InitSprites();


};
