// Noctifuge All Rights Reserved


#include "LOS Types/LOSStructTypes.h"
#include "AbilitySystem/Abilities/LOSHeroGameplayAbility.h"

bool FLOSHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
