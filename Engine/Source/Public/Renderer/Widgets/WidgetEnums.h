#pragma once

/** Visibility and hit control enum */
enum class EWidgetVisibility : Uint8
{
	None = 0,				// Should never happen
	Visible,				// Visible
	Hidden,					// Not visible
};

/** Visibility and hit control enum */
enum class EWidgetInteraction : Uint8
{
	None = 0,				// Should never happen
	Interactive,			// Interactive
	NotInteractive,			// Not interactive
};

/** Alignment of child to parent widget */
enum class EAnchor : Uint8
{
	None = 0,				// Anchor system not used at all
	Center,					// Center of parent widget
	LeftTop,				// Left top corner
	LeftBottom,				// left bottom corner
	RightTop,				// Right top corner
	RightBottom,			// Right bottom corner
	TopCenter,				// Center of top edge
	LeftCenter,				// Center of left edge
	BottomCenter,			// Center of bottom edge
	RightCenter				// Center of right edge
};

/** What should happen if widget is too big? */
enum class EClipping : Uint8
{
	None = 0,				// Do not clip
	Resize,					// Use SDL to resize if available
	Cut,					// Cut if too big
};

/** Widget orientation of Location to parent, can be relative to window (absolute) or to parent widget (relative) */
enum class EWidgetOrientation : Uint8
{
	Absolute = 0,			// Relative to window so it will be like absolute location
	Relative,				// Relative to parent widget so it will try to align to be inside parent widget.
};

/** Widget size type */
enum class EWidgetSizeType : Uint8
{
	Pixels = 0,				// Size in pixels
	ParentPercentage,		// Size in screen percentage
};

inline const char* WidgetVisibilityToString(const EWidgetVisibility InWidgetVisibility)
{
	const char* OutValue = "Unknown visibility";

	switch (InWidgetVisibility)
	{
	case EWidgetVisibility::None:
		OutValue = "None";
		break;
	case EWidgetVisibility::Visible:
		OutValue = "Visible";
		break;
	case EWidgetVisibility::Hidden:
		OutValue = "Hidden";
		break;
	}

	return OutValue;
}

inline const char* WidgetInteractionToString(const EWidgetInteraction WidgetInteraction)
{
	const char* OutValue = "Unknown interaction";

	switch (WidgetInteraction)
	{
	case EWidgetInteraction::None:
		OutValue = "None";
		break;
	case EWidgetInteraction::Interactive:
		OutValue = "Interactive";
		break;
	case EWidgetInteraction::NotInteractive:
		OutValue = "NotInteractive";
		break;
	}

	return OutValue;
}
