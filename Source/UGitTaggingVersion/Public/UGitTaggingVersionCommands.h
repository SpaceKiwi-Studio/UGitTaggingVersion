// Copyright Space Kiwi Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UGitTaggingVersionStyle.h"

/**
 * @author Space Kiwi Studio
 */
class FUGitTaggingVersionCommands : public TCommands<FUGitTaggingVersionCommands>
{
public:

	FUGitTaggingVersionCommands()
		: TCommands<FUGitTaggingVersionCommands>(TEXT("UGitTaggingVersion"), NSLOCTEXT("Contexts", "UGitTaggingVersion", "UGitTaggingVersion Plugin"), NAME_None, FUGitTaggingVersionStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
