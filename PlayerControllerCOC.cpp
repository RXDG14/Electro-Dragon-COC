#include "PlayerControllerCOC.h"
#include "Barbarian.h"
#include "GameMode_ElectroDragon.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void APlayerControllerCOC::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void APlayerControllerCOC::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &APlayerControllerCOC::HandleLeftClick);
}

void APlayerControllerCOC::HandleLeftClick() // ortho camera
{
	float MouseX, MouseY;
	if (!GetMousePosition(MouseX, MouseY))
	{
		UE_LOG(LogTemp, Warning, TEXT("Mouse not over viewport"));
		return;
	}

	FHitResult Hit;
	if (GetHitResultAtScreenPosition(FVector2D(MouseX, MouseY),ECC_Visibility,true,Hit))
	{
		FVector BarbarianSpawnLocation = Hit.ImpactPoint + FVector(0, 0, 100);
		FVector ElectroDragonSpawnLocation = Hit.ImpactPoint + FVector(0, 0, 200);
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
		
		if (GameMode && GameMode->IsA(AGameMode_ElectroDragon::StaticClass()))
		{
			if (ElectroDragonClass)
			{
				GetWorld()->SpawnActor<ACharacter>(ElectroDragonClass, ElectroDragonSpawnLocation, FRotator::ZeroRotator);
			}
		}
		else
		{
			if (BarbarianClass)
			{
				GetWorld()->SpawnActor<ACharacter>(BarbarianClass,BarbarianSpawnLocation,FRotator::ZeroRotator);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing under cursor"));
	}
}


