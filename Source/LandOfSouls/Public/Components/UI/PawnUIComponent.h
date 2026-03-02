// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercenteChaingedDelegate,float,NewPercent);
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API UPawnUIComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercenteChaingedDelegate OnCurrentHealthChanged;
};
