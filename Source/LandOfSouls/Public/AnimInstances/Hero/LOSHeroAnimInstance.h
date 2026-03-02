// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/LOSCharacterAnimInstance.h"
#include "LOSHeroAnimInstance.generated.h"


class ALOSHeroCharacter;
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ULOSHeroAnimInstance : public ULOSCharacterAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="AnimData|Refereances")
	ALOSHeroCharacter* OwningHeroCharacter;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="AnimData|LocomotionData")
	bool bShouldEnterRelaxState;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.0f;

	float IdleElapsedTime;
};
