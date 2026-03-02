// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/LOSWeaponBase.h"
#include "LOS Types/LOSStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "LOSHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ALOSHeroWeapon : public ALOSWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FLOSHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandle(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle>GetGrantedAbilitySpecHandles() const;
private:
	

	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
