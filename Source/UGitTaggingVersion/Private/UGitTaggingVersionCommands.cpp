// Copyright Space Kiwi Studio. All Rights Reserved.

#include "UGitTaggingVersionCommands.h"

#define LOCTEXT_NAMESPACE "FUGitTaggingVersionModule"

void FUGitTaggingVersionCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "UGitTaggingVersion", "Execute UGitTaggingVersion action",
	           EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
