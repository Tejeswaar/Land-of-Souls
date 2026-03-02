// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LANDOFSOULS_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()
protected:
	template<class T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T,APawn>::Value, "'T' Template Parameter Get GetPawn Must Be Driven From APawn");
		return CastChecked<T>(GetOwner());
	}
	
	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T,AController>::Value, "'T' Template Parameter to Get GetController Must Be Driven From AController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
	

	
};
