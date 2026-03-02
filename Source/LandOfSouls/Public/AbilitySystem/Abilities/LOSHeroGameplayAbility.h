// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LOSGameplayAbility.h"
#include "LOSHeroGameplayAbility.generated.h"

class ALOSHeroCharacter;
class ALOSPlayerController;
class UHeroCombatComponent;
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ULOSHeroGameplayAbility : public ULOSGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "LOS|Ability")
	ALOSHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "LOS|Ability")
	ALOSPlayerController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "LOS|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "LOS|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount );

	
private:
	TWeakObjectPtr<ALOSHeroCharacter> CachedLOSHeroCharacter;
	TWeakObjectPtr<ALOSPlayerController> CachedLOSPlayerController;
};
