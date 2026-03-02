// Fill out your copyright notice in the Description page of Project Settings.


#include "Battle/System/UBGameStateBase.h"

void AUBGameStateBase::SetGameStart()
{
		bIsGameStart = true;
		if(bAllowPlayerUI == true)
		OnGameStart.Broadcast();
	
}
