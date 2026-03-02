// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LOS Types/LOSStructTypes.h"
#include "LOSAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ULOSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	void OnAbilityInputPressed(FGameplayTag InInputTag);
	void OnAbilityInputReleased(FGameplayTag InInputTag);

	UFUNCTION(BlueprintCallable, Category = "LOS|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbility(const TArray<FLOSHeroAbilitySet>& InDefaultWeaponAbility, int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "LOS|Ability")
	void RemoveGrantedHeroWeaponAbility(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);
};
