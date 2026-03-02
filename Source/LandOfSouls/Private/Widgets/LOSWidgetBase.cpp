// Noctifuge All Rights Reserved


#include "Widgets/LOSWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void ULOSWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if(IPawnUIInterface* PawnUIInterface =  Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitialized(HeroUIComponent);
		}
	}
	
}

void ULOSWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface =  Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent =  PawnUIInterface->GetEnemyUIComponent();
		
		checkf(EnemyUIComponent, TEXT("Failed to extract EnemyUIComponent from %s"), *OwningEnemyActor->GetActorNameOrLabel());
		
		BP_OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
}
