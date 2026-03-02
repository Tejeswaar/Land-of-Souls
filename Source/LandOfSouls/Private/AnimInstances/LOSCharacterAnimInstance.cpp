// Noctifuge All Rights Reserved


#include "AnimInstances/LOSCharacterAnimInstance.h"
#include "Characters/LOSBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULOSCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ALOSBaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}
void ULOSCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	 bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D()>0.0f;
}
