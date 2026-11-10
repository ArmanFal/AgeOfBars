// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#if WITH_EDITORONLY_DATA
#include "MasterEditorUtilityWidget.h"

#include "LevelEditor.h"
#include "Styling/SlateStyleRegistry.h"


void UMasterEditorUtilityWidget::ApplyIcon(FName TabID)
{
	if (!TabIcon.IsValid())
	{
		return;
	}
	
	FLevelEditorModule& LevelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	TSharedPtr<FTabManager> TabManager = LevelEditor.GetLevelEditorTabManager();
	
	if (!TabManager.IsValid())
	{
		return;
	}

	TSharedPtr<SDockTab> Tab =TabManager->FindExistingLiveTab(FTabId(TabID));
	
	if (!Tab.IsValid())
	{
		return;
	}

	const ISlateStyle* Style = FSlateStyleRegistry::FindSlateStyle(TabStyle);
	if (!Style)
	{
		return;
	}
	
	const FSlateBrush* Icon = Style->GetOptionalBrush(TabIcon);

	if (Icon)
	{
		Tab->SetTabIcon(Icon);
	}
}
#endif
