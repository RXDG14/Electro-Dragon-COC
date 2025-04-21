#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"


class UWidgetComponent;
class UHealthBar;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COC_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributesComponent();
	void UpdateHealthWidget();
	void HideHealthWidget();

	UFUNCTION()
	bool IsDead();

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle HideHealthWidgetTimerHandle;
	
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage,const UDamageType* DamageType, class AController* Instigator, class AActor* DamageCauser);
	void ShowHealthWidget();

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100.f;
};
