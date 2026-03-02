// Noctifuge All Rights Reserved


#include "AnimInstances/Hero/LOSHeroAnimInstance.h"

#include "LOSDebugHelper.h"
#include "Characters/LOSHeroCharacter.h"

void ULOSHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningHeroCharacter = Cast<ALOSHeroCharacter>(OwningCharacter);
	}
}

void ULOSHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapsedTime >= EnterRelaxStateThreshold);
	}
}