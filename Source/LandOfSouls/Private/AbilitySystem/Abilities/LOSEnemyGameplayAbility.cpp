// Noctifuge All Rights Reserved


#include "AbilitySystem/Abilities/LOSEnemyGameplayAbility.h"
#include "Characters/LOSEnemyCharacter.h"

ALOSEnemyCharacter* ULOSEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedLOSEnemyCharacter.IsValid())
	{
		CachedLOSEnemyCharacter = Cast<ALOSEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedLOSEnemyCharacter.IsValid()? CachedLOSEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* ULOSEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
