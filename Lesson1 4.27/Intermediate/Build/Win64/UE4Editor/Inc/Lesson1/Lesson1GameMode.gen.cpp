// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Lesson1/Lesson1GameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLesson1GameMode() {}
// Cross Module References
	LESSON1_API UClass* Z_Construct_UClass_ALesson1GameMode_NoRegister();
	LESSON1_API UClass* Z_Construct_UClass_ALesson1GameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Lesson1();
// End Cross Module References
	void ALesson1GameMode::StaticRegisterNativesALesson1GameMode()
	{
	}
	UClass* Z_Construct_UClass_ALesson1GameMode_NoRegister()
	{
		return ALesson1GameMode::StaticClass();
	}
	struct Z_Construct_UClass_ALesson1GameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ALesson1GameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Lesson1,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ALesson1GameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "Lesson1GameMode.h" },
		{ "ModuleRelativePath", "Lesson1GameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ALesson1GameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ALesson1GameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ALesson1GameMode_Statics::ClassParams = {
		&ALesson1GameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ALesson1GameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ALesson1GameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ALesson1GameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ALesson1GameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ALesson1GameMode, 1843379365);
	template<> LESSON1_API UClass* StaticClass<ALesson1GameMode>()
	{
		return ALesson1GameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ALesson1GameMode(Z_Construct_UClass_ALesson1GameMode, &ALesson1GameMode::StaticClass, TEXT("/Script/Lesson1"), TEXT("ALesson1GameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ALesson1GameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
