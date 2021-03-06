// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORSTYLE_H
#define	MOAIVECTORSTYLE_H

//================================================================//
// MOAIVectorStyle
//================================================================//
class MOAIVectorStyle {
public:

	static const u32 DEFAULT_CIRCLE_RESOLUTION = 32;

	enum {
		LINE_NONE,
		LINE_VECTOR,
	};
	
	enum {
		STROKE_NONE,
		STROKE_CENTER,
		STROKE_INTERIOR,
		STROKE_EXTERIOR,
	};
	
	enum {
		FILL_NONE,
		FILL_SOLID,
	};

	enum {
		JOIN_BEVEL,
		JOIN_MITER,
		JOIN_ROUND,
	};
	
	enum {
		CAP_BUTT,
		CAP_POINTY,
		CAP_ROUND,
		CAP_SQUARE,
	};

	u8					mFillStyle;
	ZLColorVec			mFillColor;
	
	u8					mLineStyle;
	ZLColorVec			mLineColor;
	float				mLineWidth;
	
	u8					mStrokeStyle;
	ZLColorVec			mStrokeColor;
	float				mStrokeWidth;
	float				mStrokeDepthBias;
	u8					mJoinStyle;
	u8					mCapStyle;
	float				mMiterLimit;
	
	u8					mWindingRule;
	u16					mCircleResolution;
	
	float				mExtrude;
	float				mZOffset;
	
	ZLVec3D				mLightVec;
	
	ZLColorVec			mLightColor;
	u8					mLightCurve;
	
	ZLColorVec			mShadowColor;
	u8					mShadowCurve;
	
	ZLAffine2D			mDrawingToWorld;
	ZLAffine2D			mWorldToDrawing;
	
	u32					mFillExtraID;
	u32					mStrokeExtraID;
	
	float				mMergeNormals; // in degrees - 0 to 180; 0 will not be merged
	
	GET_SET_CONST ( u8, FillStyle, mFillStyle )
	GET_SET_CONST ( ZLColorVec&, FillColor, mFillColor )
	
	GET_SET_CONST ( u8, LineStyle, mLineStyle )
	GET_SET_CONST ( ZLColorVec&, LineColor, mLineColor )
	GET_SET_CONST ( float, LineWidth, mLineWidth )
	
	GET_SET_CONST ( u8, StrokeStyle, mStrokeStyle )
	GET_SET_CONST ( ZLColorVec&, StrokeColor, mStrokeColor )
	GET_SET_CONST ( float, StrokeWidth, mStrokeWidth )
	GET_SET_CONST ( float, StrokeDepthBias, mStrokeDepthBias )
	GET_SET_CONST ( u8, JoinStyle, mJoinStyle )
	GET_SET_CONST ( u8, CapStyle, mCapStyle )
	GET_SET_CONST ( float, MiterLimit, mMiterLimit )
	
	GET_SET_CONST ( u8, WindingRule, mWindingRule )
	GET_SET_CONST ( u16, CircleResolution, mCircleResolution )
	
	GET_SET_CONST ( float, Extrude, mExtrude )
	GET_SET_CONST ( float, ZOffset, mZOffset )
	
	GET_SET_CONST ( ZLVec3D&, LightVec, mLightVec )
	
	GET_SET_CONST ( ZLColorVec&, LightColor, mLightColor )
	GET_SET_CONST ( u8, LightCurve, mLightCurve );
	
	GET_SET_CONST ( ZLColorVec&, ShadowColor, mShadowColor )
	GET_SET_CONST ( u8, ShadowCurve, mShadowCurve )
	
	GET_SET_CONST ( ZLAffine2D&, DrawingToWorld, mDrawingToWorld )
	GET_SET_CONST ( ZLAffine2D&, WorldToDrawing, mWorldToDrawing )
	
	GET_SET_CONST ( u32, FillExtraID, mFillExtraID )
	GET_SET_CONST ( u32, StrokeExtraID, mStrokeExtraID )
	
	GET_SET_CONST ( float, MergeNormals, mMergeNormals )
	
	//----------------------------------------------------------------//
	void		Default						();
	float		GetMergeNormalsCosine		() const;
	u32			GetResolutionForWedge		( float radians ) const;
};

#endif
