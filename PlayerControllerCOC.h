#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerCOC.generated.h"

class ABarbarian;
/**
 * 
 */
UCLASS()
class COC_API APlayerControllerCOC : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> BarbarianClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> ElectroDragonClass;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void HandleLeftClick();
};
