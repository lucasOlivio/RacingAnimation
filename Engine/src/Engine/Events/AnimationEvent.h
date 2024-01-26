#pragma once

#include "Event.hpp"

#include "Engine/Graphics/Animations/AnimationProperties.h"

namespace MyEngine
{
	enum eAnimationEvents
	{
		POSITION_KEYFRAME,
		ROTATION_KEYFRAME,
		SCALE_KEYFRAME
	};

	class PositionKeyFrameEvent : public Event<eAnimationEvents>
	{
	public:
		PositionKeyFrameEvent() :
			Event<eAnimationEvents>(eAnimationEvents::POSITION_KEYFRAME, "PositionKeyFrameEvent") {};
		virtual ~PositionKeyFrameEvent() {};

		Entity entityId;
		Scene* pScene;

		int oldKeyFrame;
		int keyFrame;
		int nextKeyFrame;
	};

	class RotationKeyFrameEvent : public Event<eAnimationEvents>
	{
	public:
		RotationKeyFrameEvent() :
			Event<eAnimationEvents>(eAnimationEvents::ROTATION_KEYFRAME, "RotationKeyFrameEvent") {};
		virtual ~RotationKeyFrameEvent() {};

		Entity entityId;
		Scene* pScene;

		int oldKeyFrame;
		int keyFrame;
		int nextKeyFrame;
	};

	class ScaleKeyFrameEvent : public Event<eAnimationEvents>
	{
	public:
		ScaleKeyFrameEvent() :
			Event<eAnimationEvents>(eAnimationEvents::SCALE_KEYFRAME, "ScaleKeyFrameEvent") {};
		virtual ~ScaleKeyFrameEvent() {};

		Entity entityId;
		Scene* pScene;

		int oldKeyFrame;
		int keyFrame;
		int nextKeyFrame;
	};
}
