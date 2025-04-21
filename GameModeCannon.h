#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeCannon.generated.h"

/**
 * 
 */
UCLASS()
class COC_API AGameModeCannon : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeCannon();
	
protected:
	virtual void BeginPlay() override;
	
};
