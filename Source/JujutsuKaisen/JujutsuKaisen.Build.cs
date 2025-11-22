// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JujutsuKaisen : ModuleRules
{
	public JujutsuKaisen(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "NiagaraCore", "UMG"});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"GameplayCameras", // Required for camera animation sequences and gameplay camera systems
			"Sequencer",       // For core Sequencer functionality
			"MovieScene",      // For the underlying movie scene system
			"TemplateSequence" // Specifically for Camera Animation Sequence assets
		});
	}
}
