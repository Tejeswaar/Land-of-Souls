// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "LOSBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class ULOSAbilitySystemComponent;
class ULOSAttributeSet;	
UCLASS()
class LANDOFSOULS_API ALOSBaseCharacter : public ACharacter , public IAbilitySystemInterface, public IPawnCombatInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALOSBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

	
	//~ Begin IPawnCombatInterface Interface 
	virtual UPawnCombatComponent* GetCombatComponent() const override;
	//~ End Pawn IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface 
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End Pawn IPawnUIInterface Interface
	
protected:
	
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	ULOSAbilitySystemComponent* LOSAbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	ULOSAttributeSet* LOSAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
	FORCEINLINE ULOSAbilitySystemComponent* GetLOSAbilitySystemComponent() const { return LOSAbilitySystemComponent; }
	FORCEINLINE ULOSAttributeSet* GetLOSAttributeSet() const { return LOSAttributeSet; }
};
