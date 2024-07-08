// Copyright Space Kiwi Studio. All Rights Reserved.

#include "UGitTaggingVersion.h"

#include "ISettingsModule.h"
#include "UGitTaggingVersion/Classes/UGitTaggingVersionActionBase.h"
#include "UGitTaggingVersionStyle.h"
#include "UGitTaggingVersionCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "UGitTaggingVersionSettings.h"

static const FName UGitTaggingVersionTabName("UGitTaggingVersion");

#define LOCTEXT_NAMESPACE "FUGitTaggingVersionModule"

void FUGitTaggingVersionModule::StartupModule()
{
	FUGitTaggingVersionStyle::Initialize();
	FUGitTaggingVersionStyle::ReloadTextures();
	FUGitTaggingVersionCommands::Register();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"Git Tagging Version",
			LOCTEXT("RuntimeSettingsName", "Git Tagging Version"),
			LOCTEXT("RuntimeSettingsDescriptor", "Git Tagging Version Configuration"),
			GetMutableDefault<UGitTaggingVersionSettings>());
	}
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUGitTaggingVersionCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FUGitTaggingVersionModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUGitTaggingVersionModule::RegisterMenus));
}

void FUGitTaggingVersionModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(
			"Project",
			"Plugins",
			"Git Tagging Version");
	}
	
	FUGitTaggingVersionStyle::Shutdown();
	FUGitTaggingVersionCommands::Unregister();
}

void FUGitTaggingVersionModule::PluginButtonClicked()
{
	bool bResult = false;
	FString StdErr = "";

	if (const UGitTaggingVersionSettings* Settings = GetMutableDefault<UGitTaggingVersionSettings>(); Settings !=
		nullptr)
	{
		// Check if ActionToTrigger is valid
		if (Settings->ActionToTrigger != nullptr)
		{
			UGitTaggingVersionActionBase* Action = NewObject<UGitTaggingVersionActionBase>(Settings->ActionToTrigger);
			// Call the OnActionTrigger() function and store the result
			bResult = Action->OnActionTrigger(StdErr);
		}
	}

	if (!bResult)
	{
		FFormatOrderedArguments Args;
		Args.Add(FText::FromString(StdErr));

		FMessageDialog::Open(
			EAppMsgType::Ok,
			FText::Format(
				LOCTEXT("PluginButtonDialogText", "GitTaggingVersionAction return false. \n Please check : {0}"),
				Args));
	}
}

void FUGitTaggingVersionModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
	{
		FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
		Section.AddMenuEntryWithCommandList(FUGitTaggingVersionCommands::Get().PluginAction, PluginCommands);
	}

	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	{
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
		{
			FToolMenuEntry& Entry = Section.AddEntry(
				FToolMenuEntry::InitToolBarButton(FUGitTaggingVersionCommands::Get().PluginAction));
			Entry.SetCommandList(PluginCommands);
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUGitTaggingVersionModule, UGitTaggingVersion)
