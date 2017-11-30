// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterGame.h"
#include "PS_Survival.h"
#include "GM_Survival.h"
#include "DB_Minion.h"
#include "MinionPathFollowingComponent.h"
#include "MinionCharacter.h"
#include "HiveGravityZone.h"
#include "CustomCharacterMovementComponent.h"
#include "math.h"



AMinionCharacter::AMinionCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
	PrimaryActorTick.bCanEverTick = true;


	DifficultPoints = 1;


	MaxHealt = 100;
	Health = MaxHealt;
	Speed = 50;
	MapPawnOrientations.SetNumUninitialized(2);

//
//	CollisionAvoiderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionAvoiderComp"));
}

//void AMinionCharacter::OnCollisionAvoidOverlap(UPrimitiveComponent* MyComp, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {
//	
//	AMinionCharacter* OverlapedMinion = Cast<AMinionCharacter>(OtherActor);
//	if (OverlapedMinion) {
//		float OverlapedMinionVelocity = OverlapedMinion->GetVelocity().Size();
//		bool isOverlapedMinionFacingMe = !OverlapedMinion->GetCapsuleComponent()->GetComponentRotation().Equals(GetCapsuleComponent()->GetComponentRotation(), 30.f);
//		if (isOverlapedMinionFacingMe || OverlapedMinionVelocity < 5.f) {
//			FVector PointA = GetActorLocation();
//			FVector PointB = GetActorLocation() + GetCapsuleComponent()->GetUpVector();
//			FVector PointToCheck = OverlapedMinion->GetActorLocation();
//			if (IsPointAtRightOfLine(PointA, PointB, PointToCheck)) {
//				FVector Impulse = (GetCapsuleComponent()->GetUpVector() * 10) + GetCustomCharacterMovement()->Velocity;
//				GetCharacterMovement()->AddImpulse(Impulse, true);
//			}
//		}
//	}
//}

bool AMinionCharacter::IsPointAtRightOfLine(FVector PointA, FVector PointB, FVector PointToCheck) {
	return (((PointB.Y - PointA.Y) * (PointToCheck.Z - PointA.Z)) - ((PointToCheck.Y - PointA.Y) * (PointB.Z - PointA.Z)) < 0);
}

void AMinionCharacter::FillMinionVariablesWithData(UPARAM(ref) FGenericMinionData& DataParam) {
	
		Health = DataParam.LifeMax;
		MaxHealt = DataParam.LifeMax;
		Armor = DataParam.Armor;
		Speed = DataParam.SpeedMax;
		JumpPower = DataParam.JumpPower;

		SetCharacterSpeed(Speed);
		SetCharacterJump(JumpPower);
}
 
float AMinionCharacter::SetCharacterSpeed(float SpeedParam){
	UCharacterMovementComponent *MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = SpeedParam;
	return Speed;
}

float AMinionCharacter::SetCharacterJump(float JumpParam){
	UCharacterMovementComponent *MovementComponent = GetCharacterMovement();
	JumpPower = JumpParam;
	MovementComponent->JumpZVelocity = JumpPower;

	return JumpPower;
}

void AMinionCharacter::BeginPlay() {
	Super::BeginPlay();
	UStaticFunctionLibrary::GetMinionDataFromSelected(MinionData, MinionData.MinionSelected);
	FillMinionVariablesWithData(MinionData);
	MyDamageBehavior = NewObject<UDB_Minion>();
	MyDamageBehavior->Initialize((AActor*)this);
	CanReceiveDamage = true;
	//CollisionAvoiderComponent->OnComponentBeginOverlap.AddDynamic(this, &AMinionCharacter::OnCollisionAvoidOverlap);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("BeginPlay")));
    GravityRotatorCalculator(1.0f);
}


void AMinionCharacter::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

    GravityRotatorCalculator(DeltaTime);
	currentDeltaTime = DeltaTime;
	

	if (GetVelocity().Size() <= 1) {
		StuckTime += DeltaTime;
	}
	else {
		StuckTime = 0;
	}

	if (StuckTime >= 3) {
		FVector Impulse = (GetCapsuleComponent()->GetUpVector() * 50) + (GetCapsuleComponent()->GetForwardVector() * 50);
		GetCharacterMovement()->AddImpulse(Impulse, true);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("time stuck: %f"), StuckTime));
	
    if(bIsDead){
        FVector Force = GetCapsuleComponent()->GetUpVector()*(-980.f);
        GetMesh()->AddForceToAllBodiesBelow(Force, "root", true, true);
        GetMesh()->AddForceToAllBodiesBelow(Force, "chest", true, true);
    }

}

void AMinionCharacter::Jump(){
	Super::Jump();
}

void AMinionCharacter::NavigationJump(FNavPathPoint StartPoint, FNavPathPoint EndPoint){
	
	JumpFromTo(StartPoint.Location, EndPoint.Location);
	return;

	FVector StartVector = FVector(StartPoint.Location.X, StartPoint.Location.Y, StartPoint.Location.Z);
	FVector EndVector = FVector(EndPoint.Location.X, EndPoint.Location.Y, EndPoint.Location.Z);
	FVector DirectionVector = EndVector - StartVector;
	float XDistance = EndVector.Y - StartVector.Y;
	float YDistance = EndVector.Z - StartVector.Z;

	float AngleBetweenPoints = atan2(DirectionVector.Z, DirectionVector.Y) * 180 / PI;

	float m = (EndVector.Z - StartVector.Z) / (EndVector.Y - StartVector.Y);
	float angle = FMath::Atan(m) * 180 / PI;
	angle = angle + angle / 10;
	if (angle < 0) {
		angle += 360;
	}
	//angle = AngleBetweenPoints;
	angle = FMath::DegreesToRadians(AngleBetweenPoints);
	
	float initialVelocity = (1 / FMath::Cos(angle)) * FMath::Sqrt((0.5f * GetCustomCharacterMovement()->GetGravityMagnitude() * FMath::Pow(XDistance, 2)) / (XDistance * FMath::Tan(angle) + YDistance));

	FVector velocity = FVector(0, initialVelocity * FMath::Cos(angle), initialVelocity * FMath::Sin(angle));
	

	GetCustomCharacterMovement()->AddImpulse(-velocity, true);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + velocity, FColor::Red, false, 20.f, 0, 10.f);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 33.0f, 20.0f, FColor::White, false, 15.0f, 0, 10.f);
	DrawDebugSphere(GetWorld(), GetActorLocation() + velocity, 33.0f, 20.0f, FColor::Blue, false, 10.0f, 0, 15.f);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("DirVector: %s"), *CurrentGravity.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Gravity: %s"), *GetCustomCharacterMovement()->GetGravityDirection().ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, FString::Printf(TEXT("StartVector: %s"), *StartVector.ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, FString::Printf(TEXT("EndVector: %s"), *EndVector.ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AngleBetweenPoints %f"), AngleBetweenPoints));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("angle %f"), angle));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("velocity %s"), *velocity.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("DotProduct %f"), PlanarDifference));
	//float angle = 

}

void AMinionCharacter::JumpFromTo(FVector StartPoint, FVector EndPoint) {

	double incidenceAngle, minAngle, numerator, denominator, V, Vx,Vy;
	float gravity = GetCustomCharacterMovement()->GetGravityMagnitude();
	FVector gravityV = GetCustomCharacterMovement()->GetGravity();
	float gravityDisplacementAngle = FMath::Atan(gravityV.Z/ gravityV.Y);
	float EndPointAngle = FMath::Atan(EndPoint.Z/ EndPoint.Y);
	float StartPointAngle = FMath::Atan(StartPoint.Z/ StartPoint.Y);

	EndPoint = EndPoint.RotateAngleAxis(FMath::RadiansToDegrees(-gravityDisplacementAngle),FVector::ForwardVector);
	StartPoint = StartPoint.RotateAngleAxis(FMath::RadiansToDegrees(-gravityDisplacementAngle), FVector::ForwardVector);

	float DeltaY = FMath::Abs(EndPoint.Z - StartPoint.Z);
	float DeltaX = FMath::Abs(EndPoint.Y - StartPoint.Y);
	FVector velocity;
	FVector PositiveDirection = EndPoint - StartPoint;
	PositiveDirection.X = FMath::Abs(PositiveDirection.X);
	PositiveDirection.Y = FMath::Abs(PositiveDirection.Y);
	PositiveDirection.Z = FMath::Abs(PositiveDirection.Z);

	FVector velocitySign = (EndPoint - StartPoint) / PositiveDirection;

	this->GetCustomCharacterMovement()->StopMovementImmediately();

	//source http://www.sc.ehu.es/sbweb/fisica/cinematica/baloncesto/BALONCES.htm

	//Positions of this object and the target on the same plane
	//FVector planarTarget = FVector(0, EndPoint.Y, 0);
	//FVector planarPostion = FVector(0, StartPoint.Y, 0);
	//FVector YTarget = FVector(0, 0, EndPoint.Z);
	//FVector YPostion = FVector(0, 0, StartPoint.Z);


//#pragma region VelocityEquationBasketPageFormula
	incidenceAngle = tan(FMath::Atan2(DeltaY,DeltaX)/ 180.f * PI);
	minAngle = FMath::Atan(incidenceAngle +(2 * DeltaY / DeltaX));
	numerator = (gravity* DeltaX);
	denominator = 2* pow(cos(minAngle), 2)	* (tan(minAngle) - DeltaY / DeltaX);
	V = sqrtf(numerator / denominator);
	Vx = FMath::Abs(V * cos(minAngle));
	Vy = FMath::Abs(V * sin(minAngle));

	velocity = FVector(0, Vx*velocitySign.Y, Vy*velocitySign.Z) ;//FVector(0,-Vx,-Vy);	
//#pragma endregion

	
	//float VelocityMagnitude = velocity.Size();
	//velocity = FVector(0,VelocityMagnitude,FMath::Tan(-gravityDisplacementAngle)*VelocityMagnitude);
	velocity = velocity.RotateAngleAxis(FMath::RadiansToDegrees(gravityDisplacementAngle), FVector::ForwardVector);


	GetCustomCharacterMovement()->AddImpulse(velocity, true);
	//LOGS
	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Emerald, FString::Printf(TEXT("\tminA %f||\t IncidenceAngle %f||\t gravityAngle %f\t startAngle %f\t endAngle %f"),FMath::RadiansToDegrees(minAngle),FMath::RadiansToDegrees(incidenceAngle),
		FMath::RadiansToDegrees(gravityDisplacementAngle), FMath::RadiansToDegrees(StartPointAngle), FMath::RadiansToDegrees(EndPointAngle)));
	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Emerald, FString::Printf(TEXT("Vector Velocity: %s"), *velocity.ToString()));
	//LINES
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + velocity, FColor::Red, false, 20.f, 0, 10.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0,0,1)*10000.f, FColor::Blue, false, 20.f, 0, 10.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, -1)*10000.f, FColor::Blue, false, 20.f, 0, 10.f);

	DrawDebugLine(GetWorld(), FVector::ZeroVector, StartPoint, FColor::Cyan, false, 20.f, 0, 10.f);
	DrawDebugLine(GetWorld(), FVector::ZeroVector, EndPoint, FColor::Cyan, false, 20.f, 0, 10.f);


	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 1, 0)*10000.f, FColor::Blue, false, 20.f, 0, 10.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, -1, 0)*10000.f, FColor::Blue, false, 20.f, 0, 10.f);
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Green, false, 20.f, 0, 10.f);
	DrawDebugLine(GetWorld(), FVector(0,0,0), gravityV.GetSafeNormal()*10000.f, FColor::Magenta, false, 20.f, 0, 10.f);






//#pragma region OTHER

	//float gravity = 4.9f; //GetCustomCharacterMovement()->GetGravityMagnitude();
	//// Selected angle in radians
	//float m = (EndPoint.Z - StartPoint.Z) / (EndPoint.Y - StartPoint.Y);
	//float angle = (FMath::Atan(m) * 180/PI ) + 10;
	//angle = FMath::DegreesToRadians(angle);
	//float Xdistance = EndPoint.Y - StartPoint.Y;
	//float Ydistance = EndPoint.Z - StartPoint.Z; //transform.position.y - p.y;

	//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Emerald, FString::Printf(TEXT("Minion Velocity: %s"),
	//	*v.ToString()));
	//GetCustomCharacterMovement()->AddImpulse(velocity, false);
	//	v = GetCustomCharacterMovement()->Velocity;

	//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Emerald, FString::Printf(TEXT("Minion Velocity: %s"),
	//	*v.ToString()));

	/*GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::White, FString::Printf(TEXT("velocity: %s"), *velocity.ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::White, FString::Printf(TEXT("angle: %f"), angle*180/PI));
	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::White, FString::Printf(TEXT("Impulse: %s"), *Impulse.ToString()));*/
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::Printf(TEXT("angle %f"), AngleBetweenPoints));

	//GetCustomCharacterMovement()->AddImpulse(velocity, false);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Vector: " + velocity.ToString()));

	// Rotate our velocity to match the direction between the two objects
	//float angleBetweenObjects = Vector3.Angle(Vector3.forward, planarTarget - planarPostion);
	//float angleBetweenObjects = Vector3.Angle(Vector3.forward, planarTarget - planarPostion) * (p.x > transform.position.x ? 1 : -1);
	//Vector3 finalVelocity = Quaternion.AngleAxis(angleBetweenObjects, Vector3.up) * velocity;

	// Fire!
	//rigid.velocity = finalVelocity;

	// Alternative way:
	//rigid.AddForce(finalVelocity * rigid.mass, ForceMode.Impulse);

//#pragma endregion
//#pragma region DavixeEquation
	/*float initialVelocity = (1 / FMath::Cos(minAngle)) * FMath::Sqrt((0.5f * gravity * FMath::Pow(DeltaX, 2)) /
		(DeltaX * FMath::Tan(minAngle) + DeltaY));

	FVector velocity = FVector(0, -initialVelocity * FMath::Cos(minAngle), initialVelocity * FMath::Sin(minAngle));

	float V = FMath::Sqrt(gravity*FMath::Sqrt(pow(DeltaX, 2) + pow(DeltaY, 2)) + DeltaY);
	float V = (1 / FMath::Cos(minAngle)) * FMath::Sqrt((0.5f * gravity * FMath::Pow(DeltaX, 2)) / (DeltaX * FMath::Tan(minAngle) + DeltaY));
	*/
//#pragma endregion


}

void AMinionCharacter::JumpFromToWithFixedHeight(FVector StartPoint, FVector EndPoint, float JumpTime)
{
	this->GetCustomCharacterMovement()->StopMovementImmediately();
	FVector gravityV = GetCustomCharacterMovement()->GetGravity();
	FVector velocity;
	float maxHeight = EndPoint.Z + GetComponentsBoundingBox(true).GetSize().Z *2;
	float timeUp = FMath::Sqrt(-2 * maxHeight / gravityV.Z);
	float timeDown = FMath::Sqrt((2 * (EndPoint.Z - maxHeight)) / gravityV.Z);
	float totalTime = JumpTime;//timeUp + timeDown;
	GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Emerald, FString::Printf(TEXT("Vector Velocity: %f "),maxHeight));
	velocity = ((EndPoint - GetActorLocation() ) - 0.5f*gravityV*totalTime*totalTime) / totalTime;
	GetCustomCharacterMovement()->AddImpulse(velocity, true);



}

void AMinionCharacter::SpawnParticle_Implementation(FVector PointToSpawn, FRotator Rotation, FVector Size, UParticleSystem* ParticleClass, FName Socket){

	UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleClass, GetMesh()->GetSocketLocation(Socket), Rotation, true);
	//Particle->SetWorldScale3D(Size);
}


void AMinionCharacter::Attack_Implementation(FVector StartLocation, FRotator StartRotation, TSubclassOf<class AProjectile> ProjectileClass){
	ShotReplicated(StartLocation, StartRotation, ProjectileClass);
	SpawnParticle(StartLocation, StartRotation, FVector(3,3,3), MuzzleClass, "Muzzle");
	DrawDebugSphere(GetWorld(), FVector(GetActorLocation() + FVector(0, 0, 100)), 10.0f, 10.0f, FColor::Green);
}

void AMinionCharacter::ShotReplicated_Implementation(FVector StartLocation, FRotator StartRotation, TSubclassOf<class AProjectile> ProjectileClass){
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;
	
	AProjectile* ProjectileSpawned = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetMesh()->GetSocketLocation("Muzzle"), StartRotation, SpawnParams);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ShotReplicated_Implementation")));
}


AProjectile* AMinionCharacter::SpawnProjectile(FVector StartLocation, FRotator StartRotation, TSubclassOf<class AProjectile> ProjectileClass){

	return NULL;
}

float AMinionCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	if (Health <= 0 || !CanReceiveDamage) { return 0.0f; }

	float fDamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//ASpeeder_ML* speederML = Cast<ASpeeder_ML>(this->GetController());

	//if (speederML) {
	//	speederML->Damaged();
	//}
	

	AConceptPlayerController* Controller = Cast<AConceptPlayerController>(EventInstigator);
	if (Controller){
		AConceptCharacter* Character = Cast<AConceptCharacter>(Controller->GetPawn());
        if(Controller){
            Character->OtherHasReceivedDamage();
            
            if (HasAuthority()){
                TSubclassOf<UDamageType> ClaseSinCastear = DamageEvent.DamageTypeClass;
                class UConceptDamageType* ClaseCasteado = Cast<UConceptDamageType>(ClaseSinCastear.GetDefaultObject());
                
                FAbilityData Data;
                AProjectile* Projectile = Cast<AProjectile>(DamageCauser);
                AAbility* Ability = Cast<AAbility>(DamageCauser);
                if (Projectile) {
                    Data = Projectile->AbilityData;
                }
                else if (Ability) {
                    Data = Ability->AbilityData;
                }
                ClaseCasteado->HandleDamage(DamageAmount, EventInstigator, GetController(), MyDamageBehavior, (AConceptGameMode*)GetWorld()->GetAuthGameMode(), Data);
            }
        }
	}
	return DamageAmount;
}

void AMinionCharacter::OnHover(){
	AConceptCharacter* CurrentCharacter = Cast<AConceptCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (CurrentCharacter){
		CurrentCharacter->SetTarget((AActor*)this);
	}
	GetMesh()->SetRenderCustomDepth(true);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("OnHover")));
}

void AMinionCharacter::OnExitHover(){
	AConceptCharacter* CurrentCharacter = Cast<AConceptCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (CurrentCharacter){
		CurrentCharacter->SetTarget(NULL);
		CurrentCharacter->SelectedTarget = NULL;
	}
	GetMesh()->SetRenderCustomDepth(false);
}

void AMinionCharacter::Death_Implementation() {
    
    FTimerHandle Handle;
    FTimerDelegate delegate = FTimerDelegate::CreateUObject(this, &AMinionCharacter::DissolveMaterialEffect);
    GetWorldTimerManager().SetTimer(Handle, delegate, 5.0f, false);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Death_Implementation")));
    SpawnParticle(FVector(0, 0, 0), FRotator(0, 0, 0), FVector(1.5, 1.5, 1.5), ExplosionClass, "Center");
    DissolveMaterialEffect();
    
    
    EnablePhysics();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    AMinionController* MinionController = Cast<AMinionController>(GetController());
    if(MinionController){
        MinionController->KillingMinion();
    }
    
    //Destroy();
}


void AMinionCharacter::EnablePhysics() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("EnablePhysics")));
    bIsDead = true;
	GetMovementComponent()->SetComponentTickEnabled(false);
    GetMesh()->SetAllBodiesBelowSimulatePhysics("root", true);
	GetMesh()->SetAllBodiesBelowSimulatePhysics("chest", true);

}

void AMinionCharacter::DissolveMaterialEffect_Implementation() {
	//IMPLEMENTED IN BLUEPRINT
}

bool AMinionCharacter::SpawnParticle_Validate(FVector PointToSpawn, FRotator Rotation, FVector Size, UParticleSystem* ParticleClass, FName Socket){ return true; }
bool AMinionCharacter::Attack_Validate(FVector StartLocation, FRotator StartRotation, TSubclassOf<class AProjectile> ProjectileClass){ return true; }


void AMinionCharacter::GravityRotatorCalculator(float DeltaSeconds) {

	TArray<FRotator> RightOrientations;
	TArray<FRotator> LeftOrientations;
	TArray<FRotator> FloorOrientations;
	TArray<FVector> Gravitys;
	TArray<AActor*> TriangleArea;
	TArray<AHiveGravityZone*> TriangleAreaCasted;

	GetCapsuleComponent()->GetOverlappingActors(TriangleArea, TSubclassOf<AHiveGravityZone>());
	GetCustomCharacterMovement()->UpdateComponentRotation();

	for (int32 i = 0; i != TriangleArea.Num(); ++i) {
		AHiveGravityZone* GravityArea = Cast<AHiveGravityZone>(TriangleArea[i]);
		if (GravityArea) {
			TriangleAreaCasted.Add(GravityArea);
		}
	}

	if (TriangleAreaCasted.Num() == 1 && !isLerpingGravity) {
		return;
	}
	
	isLerpingGravity = true;

	for (int32 i = 0; i != TriangleAreaCasted.Num(); ++i) {
		AHiveGravityZone* GravityArea = TriangleAreaCasted[i];
		if (GravityArea) {
			Gravitys.Add(GravityArea->GetActorUpVector() * (-1));
			LeftOrientations.Add(GravityArea->Left);
			RightOrientations.Add(GravityArea->Right);
			FloorOrientations.Add(GravityArea->GetActorRotation());
		}
	}

	for (int32 i = 0; i != Gravitys.Num(); ++i) {
		if (i == 0) {
			Gravity = Gravitys[i];
			LeftOrientation = LeftOrientations[i];
			RightOrientation = RightOrientations[i];
			FloorOrientation = FRotator(FloorOrientations[i].Pitch, FloorOrientations[i].Yaw, FloorOrientations[i].Roll*-1);
		}
		else {
			Gravity = FMath::Lerp(Gravity, Gravitys[i], 0.3f);
			LeftOrientation = RotationLerp(LeftOrientation, LeftOrientations[i], 0.5f);
			RightOrientation = RotationLerp(RightOrientation, RightOrientations[i], 0.5f);
			FloorOrientation = RotationLerp(FloorOrientation, (FRotator(FloorOrientations[i].Pitch, FloorOrientations[i].Yaw, FloorOrientations[i].Roll*-1)), 0.5f);
		}
	}

	RotationSpeed = 6.5f;
	Gravity = FMath::Lerp(GetCustomCharacterMovement()->GetGravityDirection(), Gravity, RotationSpeed * DeltaSeconds);
	LeftOrientation = RotationLerp(MapPawnOrientations[1], LeftOrientation, RotationSpeed * DeltaSeconds);
	RightOrientation = RotationLerp(MapPawnOrientations[0], RightOrientation, RotationSpeed * DeltaSeconds);
	FloorOrientation = RotationLerp(CurrentFloorOrientation, FloorOrientation, RotationSpeed * DeltaSeconds);
	
	if (MapPawnOrientations[0] == RightOrientation) {
		isLerpingGravity = false;
		return;
	}


	GetCustomCharacterMovement()->SetGravityDirection(Gravity);


	MapPawnOrientations[0] = RightOrientation;
	MapPawnOrientations[1] = LeftOrientation;
	CurrentFloorOrientation = FloorOrientation;


}

FRotator AMinionCharacter::RotationLerp(FRotator InitRotation, FRotator FinalRotation, float alpha) {

	FQuat InitQuat(InitRotation);
	FQuat FinalQuat(FinalRotation);
	FQuat LerpRotation = FQuat::FastLerp(InitQuat, FinalQuat, alpha);
	LerpRotation.Normalize();
	return LerpRotation.Rotator();

}



void AMinionCharacter::ApplyDamageMomentum(float DamageTaken, const FDamageEvent& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	const UDamageType* DmgTypeCDO = DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>();
	const float ImpulseScale = DmgTypeCDO->DamageImpulse;

	UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
	if (ImpulseScale > 3.0f && CharacterMovement != NULL)
	{
		FHitResult HitInfo;
		FVector ImpulseDir;
		DamageEvent.GetBestHitInfo(this, PawnInstigator, HitInfo, ImpulseDir);

		FVector Impulse = ImpulseDir * ImpulseScale;
		const bool bMassIndependentImpulse = !DmgTypeCDO->bScaleMomentumByMass;

		// Limit Z momentum added if already going up faster than jump (to avoid blowing character way up into the sky).
		{
			FVector MassScaledImpulse = Impulse;
			if (!bMassIndependentImpulse && CharacterMovement->Mass > SMALL_NUMBER)
			{
				MassScaledImpulse = MassScaledImpulse / CharacterMovement->Mass;
			}

			const FVector AxisZ = GetActorQuat().GetAxisZ();
			if ((CharacterMovement->Velocity | AxisZ) > GetDefault<UCharacterMovementComponent>(CharacterMovement->GetClass())->JumpZVelocity && (MassScaledImpulse | AxisZ) > 0.0f)
			{
				Impulse = FVector::VectorPlaneProject(Impulse, AxisZ) + AxisZ * ((Impulse | AxisZ) * 0.5f);
			}
		}

		CharacterMovement->AddImpulse(Impulse, bMassIndependentImpulse);
	}
}

FVector AMinionCharacter::GetPawnViewLocation() const
{
	return GetActorLocation() + GetActorQuat().GetAxisZ() * BaseEyeHeight;
}

void AMinionCharacter::PostNetReceiveLocationAndRotation()
{
	// Always consider Location as changed if we were spawned this tick as in that case our replicated Location was set as part of spawning, before PreNetReceive().
	if (ReplicatedMovement.Location == GetActorLocation() && ReplicatedMovement.Rotation == GetActorRotation() && CreationTime != GetWorld()->TimeSeconds)
	{
		return;
	}

	if (Role == ROLE_SimulatedProxy)
	{
		const FVector OldLocation = GetActorLocation();
		const FQuat OldRotation = GetActorQuat();
		const FQuat NewRotation = ReplicatedMovement.Rotation.Quaternion();

		// Correction to make sure pawn doesn't penetrate floor after replication rounding.
		ReplicatedMovement.Location += NewRotation.GetAxisZ() * 0.01f;

		SetActorLocationAndRotation(ReplicatedMovement.Location, ReplicatedMovement.Rotation, /*bSweep=*/ false);

		INetworkPredictionInterface* PredictionInterface = Cast<INetworkPredictionInterface>(GetMovementComponent());
		if (PredictionInterface)
		{
			PredictionInterface->SmoothCorrection(OldLocation, OldRotation, ReplicatedMovement.Location, NewRotation);
		}
	}
}

void AMinionCharacter::LaunchCharacterRotated(FVector LaunchVelocity, bool bHorizontalOverride, bool bVerticalOverride)
{

	UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
	if (CharacterMovement)
	{
		if (!bHorizontalOverride && !bVerticalOverride)
		{
			CharacterMovement->Launch(GetVelocity() + LaunchVelocity);
		}
		else if (bHorizontalOverride && bVerticalOverride)
		{
			CharacterMovement->Launch(LaunchVelocity);
		}
		else
		{
			FVector FinalVel;
			const FVector Velocity = GetVelocity();
			const FVector AxisZ = GetActorQuat().GetAxisZ();

			if (bHorizontalOverride)
			{
				FinalVel = FVector::VectorPlaneProject(LaunchVelocity, AxisZ) + AxisZ * (Velocity | AxisZ);
			}
			else // if (bVerticalOverride)
			{
				FinalVel = FVector::VectorPlaneProject(Velocity, AxisZ) + AxisZ * (LaunchVelocity | AxisZ);
			}

			CharacterMovement->Launch(FinalVel);
		}

		OnLaunched(LaunchVelocity, bHorizontalOverride, bVerticalOverride);
	}
}

FORCEINLINE class UCustomCharacterMovementComponent* AMinionCharacter::GetCustomCharacterMovement() const
{
	return Cast<UCustomCharacterMovementComponent>(GetMovementComponent());
}

void AMinionCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		AddMovementInput(FVector::VectorPlaneProject(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X),
			GetActorQuat().GetAxisZ()).GetSafeNormal(), Value);
	}
}

void AMinionCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FVector AxisZ = GetActorQuat().GetAxisZ();

		AddMovementInput(AxisZ ^ FVector::VectorPlaneProject(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X),
			AxisZ).GetSafeNormal(), Value);
	}
}

void AMinionCharacter::OnMinionDestroyed() {
	SpawnAltenumCollectable();

	if (GetWorld()) {
		AGM_Survival* LocalGameMode = Cast<AGM_Survival>(GetWorld()->GetAuthGameMode());
		if (LocalGameMode) {

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("OnMinionDeleted Gamemode")));
			LocalGameMode->Enemies.Remove(this);
			if (UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("OnMinionDeleted Controller")));
				APS_Survival* LocalPlayerState = Cast<APS_Survival>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
				if (LocalPlayerState) {
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("OnMinionDeleted Local")));
					if (LocalPlayerState->RoundEnemiesKilledClasses.Contains(this->GetClass())) {
						int32 Value = *LocalPlayerState->RoundEnemiesKilledClasses.Find(this->GetClass());
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("OnMinionDeleted Contains Minion: %d"), Value));
						LocalPlayerState->RoundEnemiesKilledClasses.Add(this->GetClass(), Value + 1);
					}
					else {
						LocalPlayerState->RoundEnemiesKilledClasses.Add(this->GetClass(), 1);
					}

					if (LocalPlayerState->TotalEnemiesKilledClasses.Contains(this->GetClass())) {
						int32 Value = *LocalPlayerState->TotalEnemiesKilledClasses.Find(this->GetClass());
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("OnMinionDeleted Contains Total Minion: %d"), Value));
						LocalPlayerState->TotalEnemiesKilledClasses.Add(this->GetClass(), Value + 1);
					}
					else {
						LocalPlayerState->TotalEnemiesKilledClasses.Add(this->GetClass(), 1);
					}
				}


				
			}

			LocalGameMode->ContinueSpawnEnemies();
		}
	}
	
}

void AMinionCharacter::SpawnAltenumCollectable() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = this;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Altenum = GetWorld()->SpawnActor<AActor>(AltenumClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	int32 Difficult = (int32)(DifficultPoints*2);
	UFunction* function = Altenum->FindFunction("SetAltenumQuantity");
	Altenum->ProcessEvent(function, &Difficult);
}

void AMinionCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMinionCharacter, Health);
	DOREPLIFETIME(AMinionCharacter, TeamNumber);
	DOREPLIFETIME(AMinionCharacter, Target_C);
}
