// Noctifuge All Rights Reserved


#include "LOSFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/LOSAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"

ULOSAbilitySystemComponent* ULOSFunctionLibrary::NativeGetLOSASCFromActor(AActor* InActor)
{
	check(InActor);
	
	return CastChecked<ULOSAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
	
}

void ULOSFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToHold)
{
	ULOSAbilitySystemComponent* ASC = NativeGetLOSASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToHold))
	{
		ASC->AddLooseGameplayTag(TagToHold);
	}
}

void ULOSFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	ULOSAbilitySystemComponent* ASC = NativeGetLOSASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool ULOSFunctionLibrary::NativeDoesActorHasTag(AActor* InActor, FGameplayTag TagToCheck)
{
	ULOSAbilitySystemComponent* ASC = NativeGetLOSASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);

}

void ULOSFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, ELOSConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHasTag(InActor, TagToCheck) ? ELOSConfirmType::Yes : ELOSConfirmType::No;
}

UPawnCombatComponent* ULOSFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	/*UE_LOG(LogTemp, Error,
	TEXT("CALLED | InActor=%s | IsCDO=%d | WorldType=%d"),
	InActor ? *InActor->GetName() : TEXT("NULL"),
	InActor && InActor->HasAnyFlags(RF_ClassDefaultObject),
	InActor && InActor->GetWorld() ? (int32)InActor->GetWorld()->WorldType : -1); */

	if (!IsValid(InActor))
	{
		return nullptr;
	}
	
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface -> GetCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* ULOSFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,ELOSValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	
	OutValidType = CombatComponent? ELOSValidType::Valid : ELOSValidType::Invalid;

	return CombatComponent;
	
}

