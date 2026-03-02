// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUpData.generated.h"


class ULOSEnemyGameplayAbility;
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
 	virtual void GiveToAbilitySystemComponent(ULOSAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<ULOSEnemyGameplayAbility>> EnemyCombatAbilities;
	
};
