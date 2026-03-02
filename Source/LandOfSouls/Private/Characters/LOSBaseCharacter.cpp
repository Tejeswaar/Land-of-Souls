// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/LOSBaseCharacter.h"
#include "AbilitySystem/LOSAbilitySystemComponent.h"
#include "AbilitySystem/LOSAttributeSet.h"

// Sets default values
ALOSBaseCharacter::ALOSBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->SetReceivesDecals(false);
	
	LOSAbilitySystemComponent = CreateDefaultSubobject<ULOSAbilitySystemComponent>(TEXT("LOSAbilitySystemComponent"));
	LOSAttributeSet = CreateDefaultSubobject<ULOSAttributeSet>(TEXT("LOSAttributeSet"));
	
}



void ALOSBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (LOSAbilitySystemComponent)
	{
		LOSAbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("CharacterStartUpData is invalid! Assign The Start Up Data"), *GetName());
	}
}

UAbilitySystemComponent* ALOSBaseCharacter::GetAbilitySystemComponent() const
{
	return GetLOSAbilitySystemComponent();
}

UPawnCombatComponent* ALOSBaseCharacter::GetCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* ALOSBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

