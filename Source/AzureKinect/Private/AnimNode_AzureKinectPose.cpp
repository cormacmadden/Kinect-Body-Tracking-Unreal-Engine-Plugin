// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNode_AzureKinectPose.h"
#include "Animation/AnimInstanceProxy.h"
#include "AnimationRuntime.h"
#include "k4abttypes.h"

DEFINE_LOG_CATEGORY(AzureKinectAnimNodeLog);
//General Log
//DECLARE_LOG_CATEGORY_EXTERN(AzureKinectAnimNodeLog, Log, All);

FAnimNode_AzureKinectPose::FAnimNode_AzureKinectPose()
{
	BonesToModify.Reserve(K4ABT_JOINT_COUNT);
	for (int i = 0; i < K4ABT_JOINT_COUNT; i++)
	{
		BonesToModify.Add(static_cast<EKinectBodyJoint>(i), FBoneReference());
	}
}

void FAnimNode_AzureKinectPose::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread);
	
	GetEvaluateGraphExposedInputs().Execute(Context);

	USkeletalMeshComponent* SkelMesh = Context.AnimInstanceProxy->GetSkelMeshComponent();
	USkeleton* mySkeleton = Context.AnimInstanceProxy->GetSkeleton();
	BoneTransforms.Reset(K4ABT_JOINT_COUNT);

	//for each UE Skeleton joint, add the corresponding Kinect Joint to a BoneTransforms Array
	for (int i = 0; i < Skeleton.Joints.Num(); i++)
	{
		EKinectBodyJoint JointIndex = static_cast<EKinectBodyJoint>(i);
		if (BonesToModify.Contains(JointIndex))
		{
			//UE_LOG(AzureKinectAnimNodeLog, Warning, TEXT("Azure Joint Index: %d"), JointIndex);
			int32 BoneIndex = SkelMesh->GetBoneIndex(BonesToModify[JointIndex].BoneName);
			//UE_LOG(AzureKinectAnimNodeLog, Warning, TEXT("UE Bone: %s"), *BonesToModify[JointIndex].BoneName.ToString());
			if (BoneIndex != INDEX_NONE)
			{
				FCompactPoseBoneIndex CompactBoneIndex(BoneIndex);
				BoneTransforms.Emplace(CompactBoneIndex, Skeleton.Joints[i]);
			}
			if (BoneIndex == INDEX_NONE)
			{
				//UE_LOG(AzureKinectAnimNodeLog, Warning, TEXT("Bone Index is NONE"));
			}
		}
	}
	
}

void FAnimNode_AzureKinectPose::EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateComponentSpace_AnyThread)
	Output.ResetToRefPose();

	//for each kinect bone set the output pose
	for (const FBoneTransform& BoneTransform : BoneTransforms)
	{	
		//transform = transform of UE bone
		FTransform Transform = Output.Pose.GetComponentSpaceTransform(BoneTransform.BoneIndex);

		FRotator3d rotator = BoneTransform.Transform.Rotator();
		if (BoneTransform.BoneIndex == 1) {
			float xtrans = BoneTransform.Transform.GetTranslation().X;
			float ytrans = BoneTransform.Transform.GetTranslation().Y;
			float ztrans = BoneTransform.Transform.GetTranslation().Z;
			
			FVector3d fixedTranslation = FVector3d(ytrans, -ztrans, -xtrans);
			Transform.SetTranslation(fixedTranslation);
		}
		FQuat4d quaternion = rotator.Quaternion();
		Transform.SetRotation(quaternion);
		Output.Pose.SetComponentSpaceTransform(BoneTransform.BoneIndex, Transform);
	}
}
