// Todos los derechos reservados por Catness Game Studios

#include "ShooterGame.h"
#include "Engine/DemoNetDriver.h"
#include "AConceptDemoSpectator.h"



AAConceptDemoSpectator::AAConceptDemoSpectator(){
    
}

void AAConceptDemoSpectator::GoToTimeSeconds(float PlayBackTime){
    UDemoNetDriver* DemoDriver = GetWorld()->DemoNetDriver;
    
    PlayBackTime = FMath::Clamp(PlayBackTime, 0.0f, DemoDriver->DemoTotalTime);
    DemoDriver->GotoTimeInSeconds(PlayBackTime);
}

void AAConceptDemoSpectator::MoveAlongClip(float ForwardBackwardAmount){
    
    UDemoNetDriver* DemoDriver = GetWorld()->DemoNetDriver;
    
    GoToTimeSeconds(DemoDriver->DemoCurrentTime + ForwardBackwardAmount);
}
