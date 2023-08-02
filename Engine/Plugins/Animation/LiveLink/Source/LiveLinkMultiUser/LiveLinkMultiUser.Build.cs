// Copyright Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class LiveLinkMultiUser : ModuleRules
	{
		public LiveLinkMultiUser(ReadOnlyTargetRules Target) : base(Target)
		{
			PublicDependencyModuleNames.AddRange(
			new string[]
			{
			});

			PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ConcertSyncClient", // Dependancy to register our transaction filters. Needs to enable that plugin. Would be better to have modular feature interface functionality
				"Core",
				"CoreUObject",
				"LiveLinkComponents",
			});
		}
	}
}
