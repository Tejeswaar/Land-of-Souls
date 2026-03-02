// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LOSGameplayAbility.h"
#include "LOSEnemyGameplayAbility.generated.h"

class ALOSEnemyCharacter;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ULOSEnemyGameplayAbility : public ULOSGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "LOS|Ability")
	ALOSEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "LOS|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:

	TWeakObjectPtr<ALOSEnemyCharacter> CachedLOSEnemyCharacter;
};
