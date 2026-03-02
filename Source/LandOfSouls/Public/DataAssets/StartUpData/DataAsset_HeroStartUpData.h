// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "LOS Types/LOSStructTypes.h"
#include "DataAsset_HeroStartUpData.generated.h"

/**
 * 
 */

UCLASS()
class LANDOFSOULS_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(ULOSAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData" , meta = (TitleProperty = "InputTag"))
	TArray<FLOSHeroAbilitySet> HeroStartUpAbilitiesSets;
};
