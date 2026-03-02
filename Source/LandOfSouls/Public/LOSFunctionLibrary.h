// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LOS Types/LOSEnumtypes.h"
#include "LOSFunctionLibrary.generated.h"

class UPawnCombatComponent;
class ULOSAbilitySystemComponent;


/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ULOSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	static ULOSAbilitySystemComponent* NativeGetLOSASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "LOS|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToHold);

	UFUNCTION(BlueprintCallable, Category = "LOS|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHasTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "LOS|FunctionLibrary", meta=(DisplayName="Does Actor Have Tag", ExpandEnumAsExecs="OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, ELOSConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "LOS|FunctionLibrary", meta=(DisplayName="Get Pawn Combat Component From Actor", ExpandEnumAsExecs="OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, ELOSValidType& OutValidType);
};
