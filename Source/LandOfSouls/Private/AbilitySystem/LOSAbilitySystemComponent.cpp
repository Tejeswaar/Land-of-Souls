// Noctifuge All Rights Reserved


#include "AbilitySystem/LOSAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/LOSGameplayAbility.h"
#include "AbilitySystem/Abilities/LOSHeroGameplayAbility.h"


void ULOSAbilitySystemComponent::OnAbilityInputPressed(FGameplayTag InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;
		TryActivateAbility(AbilitySpec.Handle); 
	}

}

void ULOSAbilitySystemComponent::OnAbilityInputReleased(FGameplayTag InInputTag)
{
}

void ULOSAbilitySystemComponent::GrantHeroWeaponAbility(const TArray<FLOSHeroAbilitySet>& InDefaultWeaponAbility,int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbility.IsEmpty())
	{
		return;
	}
	for (const FLOSHeroAbilitySet& AbilitySet : InDefaultWeaponAbility)
	{
		if (!AbilitySet.IsValid())
		{
			continue;
		}
		
		TSubclassOf<ULOSHeroGameplayAbility> AbilityClass = AbilitySet.AbilityToGrant;
		FGameplayAbilitySpec AbilitySpec(
			AbilityClass,                // Ability class
			ApplyLevel,                  // Ability level (unchanged)
			INDEX_NONE,                  // Input ID (unchanged)
			GetAvatarActor()             // Source object (unchanged)
		);
		
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
	// for (const FLOSHeroAbilitySet& AbilitySet : InDefaultWeaponAbility)
	// {
	// 	if (!AbilitySet.IsValid()) continue;
	//
	// 	FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
	// 	AbilitySpec.SourceObject = GetAvatarActor();
	// 	AbilitySpec.Level = ApplyLevel;
	// 	AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
	// 	OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	// 	
	// }
}

void ULOSAbilitySystemComponent::RemoveGrantedHeroWeaponAbility(TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}
	InSpecHandlesToRemove.Empty();
}
