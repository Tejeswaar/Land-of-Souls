// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "LOSInputComponent.generated.h"


/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ULOSInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserObject , typename CallbackFunc>
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig , const FGameplayTag& InInputTag , ETriggerEvent TriggerEvent , UserObject* ContextObject, CallbackFunc Func);

	template<class UserObject , typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig ,UserObject* ContextObject , CallbackFunc InputPressedFunc , CallbackFunc InputReleasedFunc);

};

template<class UserObject, typename CallbackFunc>
inline void ULOSInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	checkf(InInputConfig, TEXT("Input Config Data Asset is null Cannot proceed with Binging "))

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template <class UserObject, typename CallbackFunc>
void ULOSInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject,CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	checkf(InInputConfig, TEXT("Input Config Data Asset is null Cannot proceed with Binging "))

	for (const FLOSInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid()) continue;

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started ,ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed ,ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}
}
