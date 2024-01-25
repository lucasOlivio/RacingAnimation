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

		int oldKeyFrame;
		int keyFrame;
		int nextKeyFrame;
	};
}
