// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIFourier.h>
#include <moai-util/MOAIStream.h>

// http://madebyevan.com/dft/
// http://betterexplained.com/articles/an-interactive-guide-to-the-fourier-transform/
// http://toxicdump.org/stuff/FourierToy.swf
// http://stackoverflow.com/questions/4364823/how-do-i-obtain-the-frequencies-of-each-value-in-a-fft
// http://code.compartmental.net/2007/03/21/fft-averages/
// http://stackoverflow.com/questions/20408388/how-to-filter-fft-data-for-audio-visualisation
// http://www.ni.com/white-paper/4844/en/

enum {
	KISS_FFT,
	KISS_FFTR,
	KISS_FFTRI,
};

enum {
	STREAM_CCI,		// 000
	STREAM_CCF,		// 001
	STREAM_CRI,		// 010
	STREAM_CRF,		// 011
	STREAM_RCI,		// 100
	STREAM_RCF,		// 101
	STREAM_RRI,		// 110
	STREAM_RRF,		// 111
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_countBands ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	state.Push (( u32 )self->CountBands ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_countOctaves ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	state.Push (( u32 )self->CountOctaves ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getCenterFrequencyForBand ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )

	state.Push ( self->GetCenterFrequencyForBand ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getCenterFrequencyForOctave ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )

	state.Push ( self->GetCenterFrequencyForOctave ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getFastSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	int size = state.GetValue < int >( 2, 0 );
	
	state.Push ( kiss_fft_next_fast_size ( size ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getFrequencyForIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	state.Push (( float )self->GetFrequencyForIndex ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getIndexForFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	state.Push (( u32 )self->GetIndexForFrequency ( state.GetValue < float >( 2, 0.0f )));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )

	state.Push ( self->GetWidth ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getWidthOfBand ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )

	state.Push ( self->GetWidthOfBand ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_getWidthOfOctave ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )

	state.Push ( self->GetWidthOfOctave ( state.GetValue < u32 >( 2, 1 ) - 1 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	u32 size		= state.GetValue < u32 >( 2, 0 );
	bool inverse	= state.GetValue < bool >( 3, false );
	
	self->Init ( size, inverse );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_setOutputType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	u32 outputType					= state.GetValue < u32 >( 2, OUTPUT_COMPLEX );
	u32 sampleRate					= state.GetValue < u32 >( 3, SAMPLE_RATE );
	u32 bands						= state.GetValue < u32 >( 4, 1 ); // total bands for linear average OR bands per octave
	float minOctaveBandWidth		= state.GetValue < float >( 5, 0.0f );
	
	self->SetOutputType ( outputType, sampleRate, bands, minOctaveBandWidth );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_setWindowFunction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "U" )
	
	u32 func	= state.GetValue < u32 >( 1, RECTANGULAR );
	float a		= state.GetValue < float >( 4, MOAIFourier::GetDefaultWindowAlpha ( func ));
	
	self->SetWindowFunction ( func, a );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_transform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFourier, "UUNBUN" )
	
	MOAIStream* inStream		= state.GetLuaObject < MOAIStream >( 2, true );
	u32 inStreamType			= state.GetValue < u32 >( 3, ZLSample::SAMPLE_FLOAT );
	bool complexIn				= state.GetValue < bool >( 4, false );
	MOAIStream* outStream		= state.GetLuaObject < MOAIStream >( 5, true );
	u32 outStreamType			= state.GetValue < u32 >( 6, ZLSample::SAMPLE_FLOAT );
    u32 stride                  = state.GetValue < u32 >( 7, 1 );
    u32 average                 = state.GetValue < u32 >( 8, 1 );
	
	if ( inStream && outStream ) {
		self->Transform ( *inStream, inStreamType, complexIn, *outStream, outStreamType, stride, average );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFourier::_window ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "NNN" )
	
	u32 func	= state.GetValue < u32 >( 1, RECTANGULAR );
	u32 index	= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 length	= state.GetValue < u32 >( 3, 0 );
	float a		= state.GetValue < float >( 4, MOAIFourier::GetDefaultWindowAlpha ( func ));
	
	state.Push ( MOAIFourier::Window ( func, index, length, a ));
	return 1;
}

//================================================================//
// MOAIFourier
//================================================================//

//----------------------------------------------------------------//
void MOAIFourier::Affirm ( u32 fft ) {

	if ( fft == KISS_FFT ) {
		if ( !this->mKissFFT ) {
			this->Clear ();
			this->mKissFFT = kiss_fft_alloc (( int )this->mSize, this->mInverse ? 1 : 0, 0, 0 );
		}
	}
	else {
		if ( !this->mKissFFTR ) {
			this->Clear ();
			this->mKissFFTR = kiss_fftr_alloc (( int )this->mSize, this->mInverse ? 1 : 0, 0, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFourier::Clear () {

	if ( this->mKissFFT ) {
		free ( this->mKissFFT );
		this->mKissFFT = 0;
	}
	
	if ( this->mKissFFTR ) {
		free ( this->mKissFFTR );
		this->mKissFFTR = 0;
	}
}

//----------------------------------------------------------------//
size_t MOAIFourier::CountBands () {

	return this->mOutputBands;
}

//----------------------------------------------------------------//
size_t MOAIFourier::CountOctaves () {

	return this->mOutputOctaves;
}

//----------------------------------------------------------------//
float MOAIFourier::GetCenterFrequencyForBand ( size_t band ) {

	if ( this->mOutputType == OUTPUT_OCTAVES ) {

		float lowerBound;
		float upperBound;
		
		this->GetOctaveFrequencyBounds (( size_t )( band / this->mBandsPerOctave ), lowerBound, upperBound );
		
		float step = ( upperBound - lowerBound ) / ( float )this->mBandsPerOctave;
		
		return lowerBound + (( float )( band % this->mBandsPerOctave ) * step ) + ( step * 0.5f );
	}
	
	float step = this->GetWidthOfBand ( band );
	
	return (( float )band * step ) + ( step * 0.5 );
}

//----------------------------------------------------------------//
float MOAIFourier::GetCenterFrequencyForOctave ( size_t octave ) {

	float lowerBound;
	float upperBound;

	this->GetOctaveFrequencyBounds ( octave, lowerBound, upperBound );
	
	return ( upperBound - lowerBound ) * 0.5f;
}

//----------------------------------------------------------------//
float MOAIFourier::GetDefaultWindowAlpha ( u32 func ) {

	if ( func == BLACKMAN ) return 0.16;
	if ( func == GAUSS ) return 0.25;
	
	return 0.0f;
}

//----------------------------------------------------------------//
size_t MOAIFourier::GetFastSize ( size_t size ) {

	return ( size_t )kiss_fft_next_fast_size (( int )size );
}

//----------------------------------------------------------------//
float MOAIFourier::GetFrequencyForIndex ( size_t index ) {

	return (( float )index * ( float )this->mSampleRate ) / ( float )this->mSize;
}

//----------------------------------------------------------------//
size_t MOAIFourier::GetIndexForFrequency ( float frequency ) {

	float hBandWidth = this->GetWidth () / 2.0f;

	if ( frequency < hBandWidth ) return 0;
	if ( frequency > (( float )( this->mSampleRate >> 1 ) - hBandWidth )) return this->mSize >> 1;
	
	return ( u32 )ZLFloat::Round (( float )this->mSize * ( frequency / ( float )this->mSampleRate ));
}

//----------------------------------------------------------------//
void MOAIFourier::GetOctaveFrequencyBounds ( size_t octave, float& lowerBound, float& upperBound ) {

	size_t hSampleRate = this->mSampleRate >> 1;

	lowerBound = octave == 0 ? 0.0f : (( float )hSampleRate / powf ( 2.0f, ( float )( this->mOutputOctaves - octave )));
	upperBound = (( float )hSampleRate / powf ( 2.0f, ( float )( this->mOutputOctaves - octave - 1 )));
}

//----------------------------------------------------------------//
float MOAIFourier::GetWidth () {

	return ( 2.0f / ( float )this->mSize ) * ( this->mSampleRate / 2.0f );
}

//----------------------------------------------------------------//
float MOAIFourier::GetWidthOfBand ( size_t band ) {

	if ( this->mOutputType == OUTPUT_OCTAVES ) {
		return this->GetWidthOfOctave (( size_t )( band / this->mBandsPerOctave )) / ( float )this->mBandsPerOctave;
	}
	return this->GetWidth () / ( float )this->mOutputBands;
}

//----------------------------------------------------------------//
float MOAIFourier::GetWidthOfOctave ( size_t octave ) {

	float lowerBound;
	float upperBound;
	
	this->GetOctaveFrequencyBounds ( octave, lowerBound, upperBound );

	return upperBound - lowerBound;
}

//----------------------------------------------------------------//
void MOAIFourier::Init ( size_t size, bool inverse ) {

	this->Clear ();
	this->mSize		= size;
	this->mInverse	= inverse;
}

//----------------------------------------------------------------//
MOAIFourier::MOAIFourier () :
	mSize ( 0 ),
	mInverse ( false ),
	mKissFFT ( 0 ),
	mKissFFTR ( 0 ),
	mSampleRate ( SAMPLE_RATE ),
	mOutputType ( OUTPUT_COMPLEX ),
	mOutputBands ( 0 ),
	mOutputOctaves ( 0 ),
	mBandsPerOctave ( 0 ),
	mWindowFunction ( RECTANGULAR ),
	mWindowAlpha ( 0.0f ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIFourier::~MOAIFourier () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIFourier::ReadSample ( ZLStream& inStream, u32 inStreamType, bool complexIn, float& real, float& imag ) {

	real = 0.0f;
	imag = 0.0f;

	ZLSample::ReadSample ( inStream, inStreamType, &real, ZLSample::SAMPLE_FLOAT );
	if ( complexIn ) {
		ZLSample::ReadSample ( inStream, inStreamType, &imag, ZLSample::SAMPLE_FLOAT );
	}
}

//----------------------------------------------------------------//
void MOAIFourier::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "SAMPLE_S8",			( u32 )ZLSample::SAMPLE_S8 );
	state.SetField ( -1, "SAMPLE_U8",			( u32 )ZLSample::SAMPLE_U8 );
	state.SetField ( -1, "SAMPLE_S16",			( u32 )ZLSample::SAMPLE_S16 );
	state.SetField ( -1, "SAMPLE_U16",			( u32 )ZLSample::SAMPLE_U16 );
	state.SetField ( -1, "SAMPLE_S32",			( u32 )ZLSample::SAMPLE_S32 );
	state.SetField ( -1, "SAMPLE_U32",			( u32 )ZLSample::SAMPLE_U32 );
	state.SetField ( -1, "SAMPLE_FLOAT",		( u32 )ZLSample::SAMPLE_FLOAT );
	
	state.SetField ( -1, "OUTPUT_COMPLEX",		( u32 )OUTPUT_COMPLEX );
	state.SetField ( -1, "OUTPUT_REAL",			( u32 )OUTPUT_REAL );
	state.SetField ( -1, "OUTPUT_IMAGINARY",	( u32 )OUTPUT_IMAGINARY );
	state.SetField ( -1, "OUTPUT_AMPLITUDE",	( u32 )OUTPUT_AMPLITUDE );
	state.SetField ( -1, "OUTPUT_AVERAGE",		( u32 )OUTPUT_AVERAGE );
	state.SetField ( -1, "OUTPUT_OCTAVES",		( u32 )OUTPUT_OCTAVES );
	
	state.SetField ( -1, "BARTLETT",			( u32 )BARTLETT );
	state.SetField ( -1, "BARTLETT_HANN",		( u32 )BARTLETT_HANN );
	state.SetField ( -1, "BLACKMAN",			( u32 )BLACKMAN );
	state.SetField ( -1, "COSINE",				( u32 )COSINE );
	state.SetField ( -1, "GAUSS",				( u32 )GAUSS );
	state.SetField ( -1, "HAMMING",				( u32 )HAMMING );
	state.SetField ( -1, "HANN",				( u32 )HANN );
	state.SetField ( -1, "LANCZOS",				( u32 )LANCZOS );
	state.SetField ( -1, "RECTANGULAR",			( u32 )RECTANGULAR );
	state.SetField ( -1, "WELCH",				( u32 )WELCH );
	
	luaL_Reg regTable [] = {
		{ "window",						_window },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFourier::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "countBands",					_countBands },
		{ "countOctaves",				_countOctaves },
		{ "getFastSize",				_getFastSize },
		{ "getFrequencyForIndex",		_getFrequencyForIndex },
		{ "getIndexForFrequency",		_getIndexForFrequency },
		{ "getWidth",					_getWidth },
		{ "getWidthOfBand",				_getWidthOfBand },
		{ "getWidthOfOctave",			_getWidthOfOctave },
		{ "init",						_init },
		{ "setOutputType",				_setOutputType },
		{ "setWindowFunction",			_setWindowFunction },
		{ "transform",					_transform },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFourier::SetOutputType ( u32 outputType, u32 sampleRate, size_t bands, float minOctaveBandWidth ) {

	this->mOutputType = outputType;
	this->mSampleRate = sampleRate;
	
	if ( outputType == OUTPUT_OCTAVES ) {
		
		float firstFreq = this->GetFrequencyForIndex ( 1 );
		minOctaveBandWidth = minOctaveBandWidth < firstFreq ? firstFreq : minOctaveBandWidth;
		
		float nyquist = ( float )this->mSampleRate / 2.0f;
	
		this->mOutputOctaves = 1;
		while (( nyquist /= 2.0f ) >= minOctaveBandWidth ) {
		  this->mOutputOctaves++;
		}
		this->mBandsPerOctave = bands > 0 ? bands : 1;
		this->mOutputBands = this->mOutputOctaves * this->mBandsPerOctave;
	}
	else {
		
		this->mOutputBands = bands;
		this->mOutputOctaves = 0;
		this->mBandsPerOctave = 0;
	}
}

//----------------------------------------------------------------//
void MOAIFourier::SetWindowFunction ( u32 func, float a ) {

	this->mWindowFunction = func;
	this->mWindowAlpha = a;
}

//----------------------------------------------------------------//
void MOAIFourier::Transform ( ZLStream& inStream, u32 inStreamType, bool complexIn, ZLStream& outStream, u32 outStreamType, u32 stride, u32 average ) {

	assert ( sizeof ( kiss_fft_scalar ) == sizeof ( float ));

	bool complexOut = (( this->mOutputType == OUTPUT_COMPLEX ) || ( this->mOutputType == OUTPUT_IMAGINARY ));

	size_t halfSize = this->mSize >> 1;

	// the way kissft is written the cases are either C to C, R to C or C to R. There appears to be no R to R.

	u32 code = (( this->mInverse ? 0 : 1 ) + ( complexOut ? 0 : 2 ) + ( complexIn ? 0 : 4 ));

	u32 fft;
	bool fftComplexIn;
	bool fftComplexOut;
	
	bool halfInput = false;
	bool halfOutput = false;
	
	switch ( code ) {
	
		case STREAM_CCI:
		case STREAM_CCF:
		case STREAM_RCI:
		case STREAM_CRF:
			fft				= KISS_FFT;
			fftComplexIn	= true;
			fftComplexOut	= true;
			break;
			
		case STREAM_CRI:
		case STREAM_RRI:
			fft				= KISS_FFTRI;
			fftComplexIn	= true;
			fftComplexOut	= false;
			halfInput		= true;
			break;
			
		case STREAM_RCF:
		case STREAM_RRF:
			fft				= KISS_FFTR;
			fftComplexIn	= false;
			fftComplexOut	= true;
			halfOutput		= true;
			break;
	}
	
	this->Affirm ( fft );

	void* in	= alloca ( this->mSize * ( fftComplexIn ? sizeof ( kiss_fft_cpx ) : sizeof ( kiss_fft_scalar )));
	void* out	= alloca ( this->mSize * ( fftComplexOut ? sizeof ( kiss_fft_cpx ) : sizeof ( kiss_fft_scalar ))); // TODO: get smarter about this size

	size_t sampleSize = ZLSample::GetSize ( inStreamType );
	
	stride *= complexIn ? sampleSize * 2 : sampleSize;

	for ( size_t i = 0; i < this->mSize; ++i ) {
		
		float realResult = 0.0f;
		float imagResult = 0.0f;
		
		size_t next = inStream.GetCursor () + stride;
		
		if ( average > 1 ) {
		
			float realAvg = 0.0f;
			float imagAvg = 0.0f;
			
			float scale = 1 / ( float )average;
			
			for ( u32 j = 0; j < average; ++j ) {
			
				float real;
				float imag;
				
				this->ReadSample ( inStream, inStreamType, complexIn, real, imag );
				
				real = 0;
				imag = 0;
				
				realAvg += real * scale;
				imagAvg += imag * scale;
			}
		
			realResult = realAvg;
			imagResult = imagAvg;
		}
		else {
		
			this->ReadSample ( inStream, inStreamType, complexIn, realResult, imagResult );
		}
		
		float window = MOAIFourier::Window ( this->mWindowFunction, i, this->mSize, this->mWindowAlpha );
		
		if ( fftComplexIn ) {
		
			kiss_fft_cpx* complexBuffer = ( kiss_fft_cpx* )in;
		
			complexBuffer [ i ].r = realResult * window;
			complexBuffer [ i ].i = imagResult * window;
		}
		else {
		
			(( kiss_fft_scalar* )in )[ i ] = realResult * window;
		}
		
		if ( inStream.GetCursor () != next ) {
			inStream.Seek ( next, SEEK_SET );
		}
		
		if ( halfInput && ( i > halfSize )) break;
	}
	
	switch ( fft ) {
	
		case KISS_FFT:
			kiss_fft ( this->mKissFFT, ( kiss_fft_cpx* )in, ( kiss_fft_cpx* )out );
			break;
			
		case KISS_FFTR:
			kiss_fftr ( this->mKissFFTR, ( kiss_fft_scalar* )in, ( kiss_fft_cpx* )out );
			break;
		
		case KISS_FFTRI:
			kiss_fftri ( this->mKissFFTR, ( kiss_fft_cpx* )in, ( kiss_fft_scalar* )out );
			break;
	}
	
	float* amplitudes = 0;
	
	if (( this->mOutputType == OUTPUT_AMPLITUDE ) || ( this->mOutputType == OUTPUT_AMPLITUDE ) || ( this->mOutputType == OUTPUT_AMPLITUDE )) {
		amplitudes = ( float* )in; // let's re-use this buffer
	}
	
	for ( size_t i = 0; i < this->mSize; ++i ) {
		
		float realResult = 0.0f;
		float imagResult = 0.0f;
		
		if ( fftComplexOut ) {
		
			size_t j = halfOutput ? (( i < halfSize ) ? i : halfSize - ( i % halfSize )) : i;
		
			kiss_fft_cpx* complexBuffer = ( kiss_fft_cpx* )out;
		
			realResult = complexBuffer [ j ].r;
			imagResult = complexBuffer [ j ].i;
		}
		else {
		
			realResult = (( kiss_fft_scalar* )out )[ i ];
		}
		
		switch ( this->mOutputType ) {
			
			case OUTPUT_COMPLEX:
				ZLSample::WriteSample ( outStream, outStreamType, &realResult, ZLSample::SAMPLE_FLOAT );
				ZLSample::WriteSample ( outStream, outStreamType, &imagResult, ZLSample::SAMPLE_FLOAT );
				break;
			
			case OUTPUT_REAL:
				ZLSample::WriteSample ( outStream, outStreamType, &realResult, ZLSample::SAMPLE_FLOAT );
				break;
			
			case OUTPUT_IMAGINARY:
				ZLSample::WriteSample ( outStream, outStreamType, &imagResult, ZLSample::SAMPLE_FLOAT );
				break;
			
			case OUTPUT_AMPLITUDE:
			case OUTPUT_AVERAGE:
			case OUTPUT_OCTAVES: {
			
				float amplitude = complexOut ? sqrtf (( realResult * realResult ) + ( imagResult * imagResult )) : ABS ( realResult );
			
				if ( this->mOutputType == OUTPUT_AMPLITUDE ) {
					ZLSample::WriteSample ( outStream, outStreamType, &amplitude, ZLSample::SAMPLE_FLOAT );
				}
				else {
					amplitudes = ( float* )in;
					amplitudes [ i ] = amplitude;
				}
				break;
			}
		}
	}
	
	switch ( this->mOutputType ) {
		
		case OUTPUT_COMPLEX:
		case OUTPUT_REAL:
		case OUTPUT_IMAGINARY:
		case OUTPUT_AMPLITUDE:
			return;
		
		case OUTPUT_AVERAGE:
			this->WriteAverage ( amplitudes, outStream, outStreamType );
			return;
		
		case OUTPUT_OCTAVES:
			this->WriteOctaves ( amplitudes, outStream, outStreamType );
			return;
	}
}

//----------------------------------------------------------------//
float MOAIFourier::Window ( u32 func, size_t index, size_t length, float a ) {

	double n = ( double )index;
	double N = ( double )length;

	switch ( func ) {
	
		case BARTLETT:
		
			return ( float )( 1.0 - ABS (( n - (( N - 2 ) / 2.0 )) / (( N + a ) / 2.0 )));

		case BARTLETT_HANN: {

			return ( float )( 0.62 - ( 0.48 * ABS (( n / ( N - 1 )) - 0.5 )) - ( 0.38 * cos (( TWOPI * n ) / ( N - 1 ))));
		}

		case BLACKMAN: {

			double a0 = ( 1.0 - a ) / 2.0;
			double a1 = 0.5;
			double a2 = a / 2.0;

			return ( float )( a0 - ( a1 * cos (( TWOPI * n ) / ( N - 1 ))) + ( a2 * cos (( 4.0 * PI * n ) / ( N - 1 ))));
		}

		case COSINE:

			return ( float )( cos (( PI * n ) / ( N - 1 ) - ( PI / 2.0 )));

		case GAUSS:

			return ( float )pow ( M_E, -0.5 * pow (( n - ( N - 1 ) / 2.0 ) / ( a * ( N - 1 ) / 2.0 ), 2.0 ));

		case HAMMING:
	
			return ( float )( 0.54 - ( 0.46 * cos (( TWOPI * n ) / ( N - 1 ))));

		case HANN:

			return 0.5 * ( 1.0 - cos (( TWOPI * n ) / ( N - 1 )));

		case LANCZOS: {

			double x = (( 2 * n ) / ( N - 1 )) - 1.0;
			return ( float )( sin( PI * x ) / ( PI * x ));
		}

		case RECTANGULAR:
		
			return 1.0f;

		case WELCH: {
			double x = ( N - 1 ) / 2.0;
			return ( float )( 1.0 - pow ((( n  - x ) / x ), 2.0 ));
		}
	}
	
	return 1.0f;
}

//----------------------------------------------------------------//
void MOAIFourier::WriteAverage ( float* amplitudes, ZLStream& outStream, u32 outStreamType ) {

	size_t chunkSize = this->mSize / this->mOutputBands;
	
	for ( size_t i = 0; i < this->mOutputBands; ++i ) {
	
		size_t chunk = i * chunkSize;
		
		float avg = 0.0f;
		float div = 1.0f / ( float )chunkSize;
		
		for ( size_t j = 0; j < chunkSize; ++j ) {
			avg += amplitudes [ chunk + j ] * div;
		}
		
		ZLSample::WriteSample ( outStream, outStreamType, &avg, ZLSample::SAMPLE_FLOAT );
	}
}

//----------------------------------------------------------------//
void MOAIFourier::WriteOctaves ( float* amplitudes, ZLStream& outStream, u32 outStreamType ) {

	for ( size_t i = 0; i < this->mOutputOctaves; i++ ) {
		
		float lowerBound;
		float upperBound;
		
		this->GetOctaveFrequencyBounds ( i, lowerBound, upperBound );
		
		float step = ( upperBound - lowerBound ) / ( float )this->mBandsPerOctave;
		
		float freq = lowerBound;
		for ( size_t j = 0; j < this->mBandsPerOctave; ++j ) {
		
			size_t lowerBand = this->GetIndexForFrequency ( freq );
			size_t upperBand = this->GetIndexForFrequency ( freq + step );
			
			float avg = 0.0f;
			float div = 1.0f / ( float )( upperBand - lowerBand + 1 );
			
			for ( size_t k = lowerBand; k <= upperBand; k++ ) {
				avg += amplitudes [ k ] * div;
			}
			
			ZLSample::WriteSample ( outStream, outStreamType, &avg, ZLSample::SAMPLE_FLOAT );
			
			freq += step;
		}
	}
}
