// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/LOSBaseAnimInstance.h"
#include "LOSCharacterAnimInstance.generated.h"

class ALOSBaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ULOSCharacterAnimInstance : public ULOSBaseAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY()
	ALOSBaseCharacter* OwningCharacter;
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimData|LocomotionData")
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimData|LocomotionData")
	bool bHasAcceleration;
	
};
