// Noctifuge All Rights Reserved


#include "AbilitySystem/Abilities/LOSHeroGameplayAbility.h"

#include "LOSGameplayTags.h"
#include "AbilitySystem/LOSAbilitySystemComponent.h"
#include "Characters/LOSHeroCharacter.h"
#include "Controllers/LOSPlayerController.h"

ALOSHeroCharacter* ULOSHeroGameplayAbility::GetHeroCharacterFromActorInfo() 
{
	if (!CachedLOSHeroCharacter.IsValid())
	{
		CachedLOSHeroCharacter =  Cast<ALOSHeroCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedLOSHeroCharacter.IsValid() ? CachedLOSHeroCharacter.Get() : nullptr;
}

ALOSPlayerController* ULOSHeroGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedLOSPlayerController.IsValid())
	{
		CachedLOSPlayerController = Cast<ALOSPlayerController>(CurrentActorInfo->PlayerController);
	}
	
	return CachedLOSPlayerController.IsValid() ? CachedLOSPlayerController.Get() : nullptr;
}

UHeroCombatComponent* ULOSHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

FGameplayEffectSpecHandle ULOSHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass)

	FGameplayEffectContextHandle ContextHandle = GetLOSAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	 FGameplayEffectSpecHandle EffectSpecHandle = GetLOSAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
		);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		LOSGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponBaseDamage
		);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag,InUsedComboCount);
	}

	return EffectSpecHandle;
}
