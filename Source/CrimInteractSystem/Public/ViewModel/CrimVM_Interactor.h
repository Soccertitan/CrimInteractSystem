// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "CrimVM_Interactor.generated.h"

class UCrimInteractableComponent;
class UCrimInteractorComponent;

/**
 * 
 */
UCLASS()
class CRIMINTERACTSYSTEM_API UCrimVM_Interactor : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	
	void SetInteractor(UCrimInteractorComponent* Interactor);

	FText GetInteractableNameText() const { return InteractableNameText; }
	void SetInteractableNameText(FText Text);
	FText GetInteractableActionText() const { return InteractableActionText; }
	void SetInteractableActionText(FText Text);
	bool GetbIsInstantInteract() const { return bIsInstantInteract; }
	void SetbIsInstantInteract(bool bInIsInstantInteract);
	void SetbIsInteracting(bool Value);
	bool GetbIsInteracting() const { return bIsInteracting; }

	UFUNCTION(BlueprintPure)
	float GetInteractPercentage() const;
	UFUNCTION(BlueprintPure)
	float GetRemainingInteractTime() const;
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	FText InteractableNameText = FText();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	FText InteractableActionText = FText();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	bool bIsInstantInteract = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	bool bIsInteracting = false;

	UPROPERTY()
	TObjectPtr<UCrimInteractorComponent> InteractorComponent;
	UPROPERTY()
	TObjectPtr<UCrimInteractableComponent> CurrentInteractable;

	UFUNCTION()
	void OnBeginInteract(UCrimInteractableComponent* Interactable);
	UFUNCTION()
	void OnEndInteract(UCrimInteractableComponent* Interactable);
	UFUNCTION()
	void OnInteractableUpdated(UCrimInteractableComponent* Interactable);
};
