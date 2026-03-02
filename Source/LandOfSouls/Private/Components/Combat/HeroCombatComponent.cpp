// Noctifuge All Rights Reserved


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/LOSHeroWeapon.h"
#include"AbilitySystemBlueprintLibrary.h"
#include "LOSGameplayTags.h"
#include "LOSDebugHelper.h"

ALOSHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<ALOSHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

ALOSHeroWeapon* UHeroCombatComponent::GetHeroCurrentEquippedWeapon() const
{
	return Cast<ALOSHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroCombatComponent::GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const
{
	return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		LOSGameplayTags::Shared_Event_MeleeHit,
		Data
		);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		LOSGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	);
}

void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		LOSGameplayTags::Player_Event_HitPause,
		FGameplayEventData()
	); // need to Refactor this code :: Cause is C&P (Line 47)
}
