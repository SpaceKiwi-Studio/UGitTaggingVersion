// Copyright Space Kiwi Studio. All Rights Reserved.


#include "UGitTaggingVersion/Classes/UGitTaggingVersionActionBase.h"
#include "UGitTaggingVersionSettings.h"
#include "Misc/CommandLine.h"
#include "Misc/MessageDialog.h"
#include "HAL/PlatformProcess.h"
#include "GeneralProjectSettings.h"

#define LOCTEXT_NAMESPACE "FGitAutoTaggingGameModule"

void CommandMessageError(const int32 ReturnCode, const FString& StdErr)
{
	FFormatOrderedArguments Args;
	Args.Add(FText::FromString(StdErr));
	Args.Add(FText::AsNumber(ReturnCode));

	const FText ErrorDialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "{0} with return code {1}"),
		Args);
	FMessageDialog::Open(EAppMsgType::Ok, ErrorDialogText);
}

FString ExecuteGitCommand(const FString& GitParams, const FString& GitDir, const FString& ProjectDir, FString& StdErr)
{
	const UGitTaggingVersionSettings* Settings = GetDefault<UGitTaggingVersionSettings>();

	FString StdOut = "";
	int32 ReturnCode = -1;

	// Execute the command
	// TODO Change URL and WorkingDir with Unreal Settings Variable
	FPlatformProcess::ExecProcess(*GitDir, *GitParams, &ReturnCode, &StdOut, &StdErr, *ProjectDir, true);

	// Check if the command successful
	if (ReturnCode != 0)
	{
		CommandMessageError(ReturnCode, StdErr);
	}

	StdOut.ReplaceInline(TEXT("\n"), TEXT(""));

	return StdOut;
}

FString GetFormattedDate()
{
	const FDateTime Now = FDateTime::Now();
	FString FormattedDate = Now.ToString(TEXT("%Y-%m-%d"));
	return FormattedDate;
}

bool UGitTaggingVersionActionBase::OnActionTrigger(FString& StdErr)
{
	bool bResult = false;
	const UGitTaggingVersionSettings* Settings = GetDefault<UGitTaggingVersionSettings>();

	// Init string variable
	const FString BranchName = ExecuteGitCommand(
		TEXT("rev-parse --abbrev-ref HEAD"), *Settings->GitPath,
		*Settings->RepositoryPath, StdErr); // git rev-parse --abbrev-ref HEAD;
	const FString CommitCount = ExecuteGitCommand(
		TEXT("rev-list --count HEAD"), *Settings->GitPath, *Settings->RepositoryPath, StdErr); // git rev-list --count HEAD;
	const FString CommitHash = ExecuteGitCommand(
		TEXT("rev-parse --short HEAD"), *Settings->GitPath, *Settings->RepositoryPath, StdErr); // git rev-parse HEAD;
	const FString DateTime = GetFormattedDate();

	FString Result = FString::Printf(TEXT("%s_%s_%s_%s"), *BranchName, *CommitCount, *CommitHash, *DateTime);

	FFormatOrderedArguments Args;
	Args.Add(FText::FromString(Result));

	if (ExecuteGitCommand(TEXT("status --porcelain"), *Settings->GitPath, *Settings->RepositoryPath, StdErr) != "")
	{
		FMessageDialog::Open(EAppMsgType::Ok,
		                     LOCTEXT("PluginButtonDialogText",
		                             "You need to commit or statch your change before tagging"));
	}
	else
	{
		const FText DialogText = FText::Format(
			LOCTEXT("PluginButtonDialogText", "You will tag and change your game version with: \n "
			        "{0} \n "
			        "Do you want to continue?"),
			Args);

		UGeneralProjectSettings* ProjectSettings;
		FString Version = "";

		switch (EAppReturnType::Type DialogResult = FMessageDialog::Open(EAppMsgType::YesNo, DialogText))
		{
		case EAppReturnType::Yes:
			ProjectSettings = GetMutableDefault<UGeneralProjectSettings>();
			ProjectSettings->ProjectVersion = Result;

			if (GConfig)
			{
				GConfig->SetString(
					TEXT("/Script/EngineSettings.GeneralProjectSettings"),
					TEXT("ProjectVersion"),
					*Result,
					Settings->GameConfigFile);
				GConfig->Flush(false, Settings->GameConfigFile);

				ExecuteGitCommand(FString::Printf(TEXT("tag %s"), *Result), *Settings->GitPath,
				                  *Settings->RepositoryPath, StdErr);

				bResult = true;
			}
			break;
		case EAppReturnType::No:
			bResult = true;
			break;
		default:
			break;
		}
	}

	return bResult;
}
