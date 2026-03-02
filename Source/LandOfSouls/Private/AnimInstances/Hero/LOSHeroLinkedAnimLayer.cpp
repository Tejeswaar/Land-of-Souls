// Noctifuge All Rights Reserved


#include "AnimInstances/Hero/LOSHeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/LOSHeroAnimInstance.h"

ULOSHeroAnimInstance* ULOSHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<ULOSHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
