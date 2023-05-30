// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNode_AzureKinectPose.h"
#include "Animation/AnimInstanceProxy.h"
#include "AnimationRuntime.h"
#include "k4abttypes.h"

DEFINE_LOG_CATEGORY(AzureKinectAnimNodeLog);

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
	
	BoneTransforms.Reset(K4ABT_JOINT_COUNT);

	for (int i = 0; i < Skeleton.Joints.Num(); i++)
	{
		EKinectBodyJoint JointIndex = static_cast<EKinectBodyJoint>(i);
		if (BonesToModify.Contains(JointIndex))
		{
			int32 BoneIndex = SkelMesh->GetBoneIndex(BonesToModify[JointIndex].BoneName);
			if (BoneIndex != INDEX_NONE)
			{
				FCompactPoseBoneIndex CompactBoneIndex(BoneIndex);
				BoneTransforms.Emplace(CompactBoneIndex, Skeleton.Joints[i]);
			}
		}
	}
	
}

void FAnimNode_AzureKinectPose::EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateComponentSpace_AnyThread)
	Output.ResetToRefPose();

	for (const FBoneTransform& BoneTransform : BoneTransforms)
	{	
		FTransform Transform = Output.Pose.GetComponentSpaceTransform(BoneTransform.BoneIndex);
		FVector3d rot = FVector3d(90.0, 0.0, 0.0);

		//FVector3d rota = BoneTransform.Transform;
		FRotator3d rotator = BoneTransform.Transform.Rotator();
		rotator.Add(90.0, 0.0, 0.0);
		if (BoneTransform.BoneIndex == 3) {
			rotator.Add(-180.0, 0.0, 0.0);
		}
		//FRotator3d rotated2 = rotated.ToOrientationRotator();
		FQuat4d quaternion = rotator.Quaternion();
		Transform.SetRotation(quaternion);
		if (BoneTransform.BoneIndex == 1) {
			Transform.SetTranslation(BoneTransform.Transform.GetTranslation());
		}
		Output.Pose.SetComponentSpaceTransform(BoneTransform.BoneIndex, Transform);
	}
}
