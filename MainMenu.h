// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MainMenu.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class COC_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION()
	void OnCannonButtonClicked();
	
	UPROPERTY(meta = (BindWidget))
	UButton* CannonButton;

	UFUNCTION()
    void OnElectroDragonButtonClicked();
    	
    UPROPERTY(meta = (BindWidget))
    UButton* EdragButton;
};
