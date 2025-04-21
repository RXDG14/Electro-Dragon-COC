#include "AttributesComponent.h"

#include "Barbarian.h"
#include "HealthBar.h"
#include "Components/WidgetComponent.h"


UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this,&UAttributesComponent::DamageTaken);

	if (UWidgetComponent* WidgetComp = GetOwner()->FindComponentByClass<UWidgetComponent>())
	{
		if (UUserWidget* Widget = WidgetComp->GetUserWidgetObject())
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);	
        }	
	}
}

void UAttributesComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, class AActor* DamageCauser)
{
	if (Damage <= 0.f) return;
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	ShowHealthWidget();
	UpdateHealthWidget();
	
	GetWorld()->GetTimerManager().SetTimer(HideHealthWidgetTimerHandle, this, &UAttributesComponent::HideHealthWidget, 2.f, false);
	
	if (IsDead())
	{
		GetOwner()->Destroy();
	}
}

void UAttributesComponent::UpdateHealthWidget()
{
	if (UWidgetComponent* WidgetComp = GetOwner()->FindComponentByClass<UWidgetComponent>())
	{
		if (UUserWidget* Widget = WidgetComp->GetUserWidgetObject())
		{
			if (UHealthBar* HealthBarWidget = Cast<UHealthBar>(Widget))
			{
				float Percent = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
				HealthBarWidget->SetHealthPercent(Percent);
			}
		}
	}
}

void UAttributesComponent::ShowHealthWidget()
{
	if (UWidgetComponent* WidgetComp = GetOwner()->FindComponentByClass<UWidgetComponent>())
	{
		WidgetComp->SetVisibility(true);
		if (UUserWidget* Widget = WidgetComp->GetUserWidgetObject())
		{
			Widget->SetVisibility(ESlateVisibility::Visible);	
		}
	}	
}

void UAttributesComponent::HideHealthWidget()
{
	if (UWidgetComponent* WidgetComp = GetOwner()->FindComponentByClass<UWidgetComponent>())
	{
		if (UUserWidget* Widget = WidgetComp->GetUserWidgetObject())
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);	
		}
	}	
}

bool UAttributesComponent::IsDead()
{
	return CurrentHealth <= 0;	
}