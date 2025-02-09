// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "View/MVVMViewModelContextResolver.h"
#include "CrimInteractSystemViewModelResolver.generated.h"

class UCrimVM_Interactor;

/**
 * Gets the interactor component from the Widgets owning player.
 */
UCLASS()
class CRIMINTERACTSYSTEM_API UCrimInteractSystemViewModelResolver : public UMVVMViewModelContextResolver
{
	GENERATED_BODY()

public:

	virtual UObject* CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const override;
	UCrimVM_Interactor* GetCrimInteractorViewModel(const UUserWidget* UserWidget) const;
};
