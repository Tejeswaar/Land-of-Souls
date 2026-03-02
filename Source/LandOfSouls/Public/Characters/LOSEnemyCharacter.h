// Noctifuge All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/LOSBaseCharacter.h"
#include "LOSEnemyCharacter.generated.h"

class UEnemyUIComponent;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class LANDOFSOULS_API ALOSEnemyCharacter : public ALOSBaseCharacter
{
	GENERATED_BODY()
	
public:
	ALOSEnemyCharacter();

	//~ Begin IPawnCombatInterface Interface 
	virtual UPawnCombatComponent* GetCombatComponent() const override;
	//~ End Pawn IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface 
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	//~ End Pawn IPawnUIInterface Interface

protected:
	//~ Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyUIComponent* EnemyUIComponent;
	

private:
	void InItEnemyStartUpData();

public:

	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent;}
	
};
