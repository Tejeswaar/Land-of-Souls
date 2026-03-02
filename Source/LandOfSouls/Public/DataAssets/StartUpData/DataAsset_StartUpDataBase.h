// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UGameplayEffect;
class ULOSGameplayAbility;
class ULOSAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	
	virtual void GiveToAbilitySystemComponent(ULOSAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<ULOSGameplayAbility> > ActivateOnGivenAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf<ULOSGameplayAbility> > ReActiveAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>>StartUpGameplayEffects;

	void GrantAbilities(const TArray< TSubclassOf<ULOSGameplayAbility> >& InAbilitiesToGive, ULOSAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
