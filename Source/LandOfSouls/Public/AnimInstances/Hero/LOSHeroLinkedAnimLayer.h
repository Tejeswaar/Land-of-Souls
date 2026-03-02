// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/LOSBaseAnimInstance.h"
#include "LOSHeroLinkedAnimLayer.generated.h"

class ULOSHeroAnimInstance;
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ULOSHeroLinkedAnimLayer : public ULOSBaseAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	ULOSHeroAnimInstance* GetHeroAnimInstance() const;
};
