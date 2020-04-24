// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackPack.h"

#include "Engine.h"
#include "CoreMinimal.h"
#include "MobAbstract/SideBasePawn.h"

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

class UPaperSpriteComponent;
/**
 * 
 */
UCLASS()
class GOLDEN_MOLODEZH_API ABaseChar : public ASideBasePawn
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
		USkeletalMeshComponent * Skelet;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		ABackPack* BackPack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
		int BackPackSize;

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CreateBackPack();


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void ShowFront() override;
	virtual void ShowBack() override;
	virtual void ShowRight() override;
	virtual void ShowLeft() override;

private:
	void BuildComponentHierarchy();

protected:
	virtual void BeginPlay() override;
	virtual void InitSprites();


};
