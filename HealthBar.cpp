#include "HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	SetHealthPercent(1.0f);
}

void UHealthBar::SetHealthPercent(float Percent)
{
	UE_LOG(LogTemp, Warning, TEXT("SetHealthPercent called: %f"), Percent);
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HealthBar is nullptr"));
	}
}


