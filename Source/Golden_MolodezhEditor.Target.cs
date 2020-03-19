// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Golden_MolodezhEditorTarget : TargetRules
{
	public Golden_MolodezhEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Golden_Molodezh" } );
	}
}
