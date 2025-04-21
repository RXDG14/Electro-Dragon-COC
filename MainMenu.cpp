#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GetOwningPlayer()->bShowMouseCursor = true;
	
	if (CannonButton)
	{
		CannonButton->OnClicked.AddDynamic(this,&UMainMenu::OnCannonButtonClicked);
	}
	if (EdragButton)
    {
    	EdragButton->OnClicked.AddDynamic(this,&UMainMenu::OnElectroDragonButtonClicked);
    }
}


void UMainMenu::OnCannonButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Map_Cannon", true);
}

void UMainMenu::OnElectroDragonButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Map_ElectroDragon", true);
}
