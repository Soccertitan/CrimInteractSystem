// Copyright Soccertitan


#include "ViewModel/CrimInteractSystemViewModelResolver.h"

#include "CrimInteractSystemUISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "ViewModel/CrimVM_Interactor.h"

UObject* UCrimInteractSystemViewModelResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget,
                                                              const UMVVMView* View) const
{
	return GetCrimInteractorViewModel(UserWidget);
}

UCrimVM_Interactor* UCrimInteractSystemViewModelResolver::GetCrimInteractorViewModel(const UUserWidget* UserWidget) const
{
	UCrimInteractSystemUISubsystem* Subsystem = UserWidget->GetWorld()->GetSubsystem<UCrimInteractSystemUISubsystem>();
	return Subsystem->GetOrCreateInteractorViewModel(UserWidget->GetOwningPlayer());
}
