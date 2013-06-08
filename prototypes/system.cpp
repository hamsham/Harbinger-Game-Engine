
// harbinger system drivers

#if defined(__GNUC__)
	#define LSTR_TYPE char
	#ifdef LSTR
		#undef LSTR
	#endif
	#define LSTR( x ) L##x

#elif defined(_WIN32)
	#define LSTR_TYPE wchar_t
	#ifdef LSTR
		#undef LSTR
	#endif
	#define LSTR( x ) L##x
#endif

typedef unsigned int uint;
typedef unsigned char byte;

///////////////////////////////////////////////////////////////////////////////
//			DISPLAY MODES
///////////////////////////////////////////////////////////////////////////////
typedef struct displayMode{
	uint width;
	uint height;
	uint bpp; // bits per pixel
};

///////////////////////////////////////////////////////////////////////////////
//			DISPLAY OBJECTS (Window objects)
///////////////////////////////////////////////////////////////////////////////
class display {
	public:
		enum displayContext {
			CONTEXT_NATIVE,
			CONTEXT_D3D_9,
			CONTEXT_D3D_11,
			CONTEXT_GL_3,
			CONTEXT_GL_4,
			CONTEXT_GLES_1,
			CONTEXT_GLES_2
		};
		
		enum displayOptions {
			OPTION_VSYNC			= 0x100,
			OPTION_REFRESH_RATE		= 0x200,
			OPTION_ANTIALIASED		= 0x400,
			OPTION_WINDOW			= 0x800,
			OPTION_WINDOW_NO_BORDER	= 0x010,
			OPTION_FULLSCREEN		= 0x020,
			OPTION_FULLSCREEN_WINDOW = 0x040,
			OPTION_RESIZEABLE		= 0x080,
			OPTION_CAPTURE_CURSOR	= 0x001,
			OPTION_HIDE_CURSOR		= 0x002,
			OPTION_MAXIMIZE			= 0x004,
			OPTION_MINIMIZE			= 0x008
		};
	
	private:
		displayMode		mode;
		byte			stencilBits;
		byte			depthBits;
		byte			alphaBits;
		displayHint		newDisplayHint;
		displayContext	newDisplayContext;
		displayOptions	options;
		
	public:
		display					();
		display					( const display& );
		~display				();
		
		display& operator =		( const display& );
		
		uint	getWidth		() const;
		uint	getHeight		() const;
		void	resize			( uint w, uint h );
		
		byte	numPixelBits	() const;
		byte	numStencilBits	() const;
		byte	numDepthBits	() const;
		byte	numAlphaBits	() const;
		
		bool	create			(
									uint width,
									uint height,
									byte pixelBitCount,
									byte stencilBitCount,
									byte depthBitCount,
									byte alphaBitCount,
									displayContext,
									displayOptions
								);
		void	destroy			();
		
		void	setOption		( displayOptions, int value );
		int		getOption		( displayOptions ) const;
		
		void	getCurrentMode	( displayMode& ) const;
		void	getDesktopMode	( displayMode& ) const;
		void	getDisplayModes	( hamLibs::dynArray< displayMode >& ) const;
};

///////////////////////////////////////////////////////////////////////////////
//			Input Objects
///////////////////////////////////////////////////////////////////////////////
namespace input {
	enum inputOption {
		MOUSE_ENABLED,
		MOUSE_CAPTURE,
		MOUSE_VISIBLE,
		KEYS_ENABLED,
		KEYS_REPEAT,
		KEYS_KEEP_STATE
	};
	
	void	attachDisplay		( const display& );
	void	detachDisplay		();
	const display& currentDisplay();
		
	void	setInputFlag		( inputOption, bool );
	int		getInputFlag		( inputOption );
		
	bool	keyIsActive			( uint );
	bool	mouseButtonIsActive	( uint );
	
	void	getMousePos			( uint& x, uint& y );
	void	setMousePos			( uint x, uint y );
	void	setMouseVel			( float );
	float	getMouseVel			();
	
	void	update				();
};

///////////////////////////////////////////////////////////////////////////////
//			Main Function
///////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
	namespace global {
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		LPSTR lpCmdLine;
		int nCmdShow;
	}
	LRESULT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
		global::hInstance = nInstance;
		global::hPrevInstance = hPrevInstance;
		global::lpCmdLine = lpCmdLine;
		global::nCmdShow = nCmdShow;
#else
	namespace global {
		int argc;
		char** argv;
	}
	int main( int argc, char* argv[] ) {
		global::argc = argc;
		global::argv = &argv[0];
#endif