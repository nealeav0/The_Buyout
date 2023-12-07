// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffsWidget.h"
#include "EntityBase.h"
#include "BuffStat.h"
#include "Components/HorizontalBox.h"

UBuffsWidget::UBuffsWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<UBuffStat> BuffStatBPClass(TEXT("/Game/GUI/BP_BuffStat"));
    if (BuffStatBPClass.Class)
        BuffStatClass = BuffStatBPClass.Class;
}

void UBuffsWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UBuffsWidget::InitializeUI(FEntityStruct Entity)
{
     if (BuffsContainer) {
        // make sure previous buffs were wiped first
        if (BuffsContainer->GetAllChildren().Num() > 0) {
            BuffsContainer->ClearChildren();
        }

        // now add buffs
        if (Entity.AttackBuff > 0.f) {
            UBuffStat* BuffStat = CreateWidget<UBuffStat>(this, BuffStatClass);
            BuffStat->InitializeUI(FString(TEXT("ATK")), FMath::RoundToInt(Entity.AttackBuff * 100), true);
            BuffsContainer->AddChild(BuffStat);
        }

        if (Entity.DefenseBuff > 0.f) {
            UBuffStat* BuffStat = CreateWidget<UBuffStat>(this, BuffStatClass);
            BuffStat->InitializeUI(FString(TEXT("DEF")), FMath::RoundToInt(Entity.DefenseBuff * 100), true);
            BuffsContainer->AddChild(BuffStat);
        }

        if (Entity.AccuracyBuff > 0.f) {
            UBuffStat* BuffStat = CreateWidget<UBuffStat>(this, BuffStatClass);
            BuffStat->InitializeUI(FString(TEXT("ACC")), FMath::RoundToInt(Entity.AccuracyBuff * 100), true);
            BuffsContainer->AddChild(BuffStat);
        }

        if (Entity.EvasionBuff > 0.f) {
            UBuffStat* BuffStat = CreateWidget<UBuffStat>(this, BuffStatClass);
            BuffStat->InitializeUI(FString(TEXT("EVA")), FMath::RoundToInt(Entity.EvasionBuff * 100), true);
            BuffsContainer->AddChild(BuffStat);
        }

        if (Entity.BurnStacks > 0) {
            UBuffStat* BuffStat = CreateWidget<UBuffStat>(this, BuffStatClass);
            BuffStat->InitializeUI(FString(TEXT("BURN")), Entity.BurnStacks, false);
            BuffsContainer->AddChild(BuffStat);
        }

        if (Entity.ChillStacks > 0.f) {
            UBuffStat* BuffStat = CreateWidget<UBuffStat>(this, BuffStatClass);
            BuffStat->InitializeUI(FString(TEXT("CHILL")), Entity.ChillStacks, false);
            BuffsContainer->AddChild(BuffStat);
        }

        if (Entity.StunStacks > 0.f) {
            UBuffStat* BuffStat = CreateWidget<UBuffStat>(this, BuffStatClass);
            BuffStat->InitializeUI(FString(TEXT("STUN")), Entity.StunStacks, false);
            BuffsContainer->AddChild(BuffStat);
        }

        if (Entity.PoisonStacks > 0.f) {
            UBuffStat* BuffStat = CreateWidget<UBuffStat>(this, BuffStatClass);
            BuffStat->InitializeUI(FString(TEXT("POISON")), Entity.PoisonStacks, false);
            BuffsContainer->AddChild(BuffStat);
        }
    }
}


