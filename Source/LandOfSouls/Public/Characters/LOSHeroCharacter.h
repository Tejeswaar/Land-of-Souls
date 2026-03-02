// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/LOSBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "LOSHeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
class UHeroCombatComponent;
class UHeroUIComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ALOSHeroCharacter : public ALOSBaseCharacter
{
	GENERATED_BODY()
public:
	ALOSHeroCharacter();

	//~ Begin IPawnCombatInterface Interface 
	virtual UPawnCombatComponent* GetCombatComponent() const override;
	//~ End Pawn IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface 
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UHeroUIComponent* GetHeroUIComponent() const override;
	//~ End Pawn IPawnUIInterface Interface

	
protected:

	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camara", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camara", meta=(AllowPrivateAccess="true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	UHeroCombatComponent* HeroCombatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	UHeroUIComponent* HeroUIComponent;
	
#pragma endregion
	
//-------------------------------------------------------------------------------------------------------------------
	
#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	
#pragma endregion


public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }

};

