// Copyright Epic Games, Inc. All Rights Reserved.

#include "UGitTaggingVersion.h"
#include "UGitTaggingVersionStyle.h"
#include "UGitTaggingVersionCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName UGitTaggingVersionTabName("UGitTaggingVersion");

#define LOCTEXT_NAMESPACE "FUGitTaggingVersionModule"

void FUGitTaggingVersionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FUGitTaggingVersionStyle::Initialize();
	FUGitTaggingVersionStyle::ReloadTextures();

	FUGitTaggingVersionCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUGitTaggingVersionCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FUGitTaggingVersionModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUGitTaggingVersionModule::RegisterMenus));
}

void FUGitTaggingVersionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FUGitTaggingVersionStyle::Shutdown();

	FUGitTaggingVersionCommands::Unregister();
}

void FUGitTaggingVersionModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FUGitTaggingVersionModule::PluginButtonClicked()")),
							FText::FromString(TEXT("UGitTaggingVersion.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FUGitTaggingVersionModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FUGitTaggingVersionCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FUGitTaggingVersionCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUGitTaggingVersionModule, UGitTaggingVersion)