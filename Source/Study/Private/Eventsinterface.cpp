// Fill out your copyright notice in the Description page of Project Settings.


#include "Eventsinterface.h"

// Sets default values for this component's properties
UEventsinterface::UEventsinterface()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEventsinterface::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEventsinterface::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEventsinterface::Collisioninterface()
{
	UE_LOG(LogTemp, Warning, TEXT("1"));
	AActor* My = GetOwner();

	FVector start;
	FRotator send;
	My->GetActorEyesViewPoint(start, send);
	FVector end = start + (send.Vector() * 1000);
	FCollisionObjectQueryParams objects;//��ײ��������
	objects.AddObjectTypesToQuery(ECC_WorldDynamic);
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, start, end, objects);//����λ�ü�����
	AActor* Hitactor = Hit.GetActor();//���ж���
	if(Hitactor)
	{
		UE_LOG(LogTemp, Warning, TEXT("test"));
	}
	if (Hitactor)
	{
		if (Hitactor->Implements<UMyInterface>())//�������ж����Ƿ��нӿں���
		{
			APawn* my = Cast<APawn>(My);
			IMyInterface::Execute_inter(Hitactor,my);
		}
	}
}

