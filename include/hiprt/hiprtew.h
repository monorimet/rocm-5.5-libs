#pragma once

#define HIPRT_MAJOR_VERSION 2
#define HIPRT_MINOR_VERSION 0
#define HIPRT_PATCH_VERSION 0xb68861

#define HIPRT_API_VERSION 2000
#define HIPRT_VERSION_STR "02000"

#include <hiprt/hiprt_types.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define HIPRTAPI __stdcall
#else
#define HIPAPI
#define HIP_CB
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

/* Utility macros. */

typedef HMODULE DynamicLibrary;

#define dynamic_library_open( path ) LoadLibraryA( path )
#define dynamic_library_close( lib ) FreeLibrary( lib )
#define dynamic_library_find( lib, symbol ) GetProcAddress( lib, symbol )
#else
#include <dlfcn.h>

typedef void* DynamicLibrary;

#define dynamic_library_open( path ) dlopen( path, RTLD_NOW )
#define dynamic_library_close( lib ) dlclose( lib )
#define dynamic_library_find( lib, symbol ) dlsym( lib, symbol )
#endif

static DynamicLibrary hiprt_lib;

#define _LIBRARY_FIND_CHECKED( lib, name )               \
	name = (t##name*)dynamic_library_find( lib, #name ); \
	assert( name );

#define _LIBRARY_FIND( lib, name ) name = (t##name*)dynamic_library_find( lib, #name );

#define HIPRT_LIBRARY_FIND_CHECKED( name ) _LIBRARY_FIND_CHECKED( hiprt_lib, name )
#define HIPRT_LIBRARY_FIND( name ) _LIBRARY_FIND( hiprt_lib, name )
#if defined(_WIN32)
#define HIPRT_LIB_NAME             \
	"hiprt" HIPRT_VERSION_STR "64" \
	".dll"
#else
#define HIPRT_LIB_NAME                \
	"libhiprt" HIPRT_VERSION_STR "64" \
	".so"
#endif

enum
{
	HIPRTEW_SUCCESS			  = 0,
	HIPRTEW_ERROR_OPEN_FAILED	  = -1,
	HIPRTEW_ERROR_ATEXIT_FAILED = -2,
	HIPRTEW_ERROR_OLD_DRIVER	  = -3,
	HIPRTEW_NOT_INITIALIZED	  = -4,
};

//function types 
typedef hiprtError HIPRTAPI HIPRTAPI thiprtCreateContext( uint32_t hiprtApiVersion, hiprtContextCreationInput& input, hiprtContext* outContext );
typedef hiprtError HIPRTAPI HIPRTAPI thiprtDestroyContext( hiprtContext context );
typedef hiprtError HIPRTAPI		thiprtCreateGeometry(
		   hiprtContext					  context,
		   const hiprtGeometryBuildInput* buildInput,
		   const hiprtBuildOptions*		  buildOptions,
		   hiprtGeometry*				  outGeometry );
typedef hiprtError HIPRTAPI thiprtDestroyGeometry( hiprtContext context, hiprtGeometry outGeometry );
typedef hiprtError HIPRTAPI thiprtBuildGeometry(
	hiprtContext				   context,
	hiprtBuildOperation			   buildOperation,
	const hiprtGeometryBuildInput* buildInput,
	const hiprtBuildOptions*	   buildOptions,
	hiprtDevicePtr				   temporaryBuffer,
	hiprtApiStream				   stream,
	hiprtGeometry				   outGeometry );
typedef hiprtError HIPRTAPI thiprtGetGeometryBuildTemporaryBufferSize(hiprtContext context, const hiprtGeometryBuildInput* buildInput, const hiprtBuildOptions* buildOptions, size_t* outSize );
typedef hiprtError HIPRTAPI thiprtCreateScene(hiprtContext context, const hiprtSceneBuildInput* buildInput, const hiprtBuildOptions* buildOptions, hiprtScene* outScene );
typedef hiprtError HIPRTAPI thiprtDestroyScene( hiprtContext context, hiprtScene outScene );
typedef hiprtError HIPRTAPI thiprtBuildScene(
	hiprtContext				context,
	hiprtBuildOperation			buildOperation,
	const hiprtSceneBuildInput* buildInput,
	const hiprtBuildOptions*	buildOptions,
	hiprtDevicePtr				temporaryBuffer,
	hiprtApiStream				stream,
	hiprtScene					outScene );
typedef hiprtError HIPRTAPI thiprtGetSceneBuildTemporaryBufferSize(hiprtContext context, const hiprtSceneBuildInput* buildInput, const hiprtBuildOptions* buildOptions, size_t* outSize );
typedef hiprtError HIPRTAPI thiprtCreateFuncTable( hiprtContext context, uint32_t numGeomTypes, uint32_t numRayTypes, hiprtFuncTable* outFuncTable );
typedef hiprtError HIPRTAPI thiprtSetFuncTable( hiprtContext context, hiprtFuncTable funcTable, uint32_t geomType, uint32_t rayType, hiprtFuncDataSet set );
typedef hiprtError HIPRTAPI thiprtDestroyFuncTable( hiprtContext context, hiprtFuncTable funcTable );
typedef hiprtError HIPRTAPI thiprtSaveGeometry( hiprtContext context, hiprtGeometry inGeometry, const char* filename );
typedef hiprtError HIPRTAPI thiprtLoadGeometry( hiprtContext context, hiprtGeometry* outGeometry, const char* filename );
typedef hiprtError HIPRTAPI thiprtSaveScene( hiprtContext context, hiprtScene inScene, const char* filename );
typedef hiprtError HIPRTAPI thiprtLoadScene( hiprtContext context, hiprtScene* outScene, const char* filename );
typedef hiprtError HIPRTAPI thiprtExportGeometryAabb( hiprtContext context, hiprtGeometry inGeometry, hiprtFloat3& outAabbMin, hiprtFloat3& outAabbMax );
typedef hiprtError HIPRTAPI thiprtExportSceneAabb( hiprtContext context, hiprtScene inScene, hiprtFloat3& outAabbMin, hiprtFloat3& outAabbMax );
typedef hiprtError HIPRTAPI thiprtBuildTraceKernels(
	hiprtContext	  context,
	uint32_t		  numFunctions,
	const char**	  funcNames,
	const char*		  src,
	const char*		  moduleName,
	uint32_t		  numHeaders,
	const char**	  headers,
	const char**	  includeNames,
	uint32_t		  numOptions,
	const char**	  options,
	uint32_t		  numGeomTypes,
	uint32_t		  numRayTypes,
	hiprtFuncNameSet* funcNameSets,
	hiprtApiFunction* functionsOut,
	hiprtApiModule*   moduleOut );
typedef hiprtError HIPRTAPI thiprtBuildTraceKernelsFromBitcode(
	hiprtContext	  context,
	uint32_t		  numFunctions,
	const char**	  funcNames,
	const char*		  moduleName,
	const char*		  bitcodeBinary,
	size_t			  bitcodeBinarySize,
	uint32_t		  numGeomTypes,
	uint32_t		  numRayTypes,
	hiprtFuncNameSet* funcNameSets,
	hiprtApiFunction* functionsOut );
typedef void thiprtSetCacheDirPath( hiprtContext context, const char* path );
typedef void thiprtSetLogLevel( hiprtLogLevel level );

//function pointers
extern thiprtCreateContext*						  hiprtCreateContext;
extern thiprtDestroyContext*					  hiprtDestroyContext;
extern thiprtCreateGeometry*					  hiprtCreateGeometry;
extern thiprtDestroyGeometry*					  hiprtDestroyGeometry;
extern thiprtBuildGeometry*						  hiprtBuildGeometry;
extern thiprtGetGeometryBuildTemporaryBufferSize* hiprtGetGeometryBuildTemporaryBufferSize;
extern thiprtCreateScene*						  hiprtCreateScene;
extern thiprtDestroyScene*						  hiprtDestroyScene;
extern thiprtBuildScene*						  hiprtBuildScene;
extern thiprtGetSceneBuildTemporaryBufferSize*	  hiprtGetSceneBuildTemporaryBufferSize;
extern thiprtCreateFuncTable*					  hiprtCreateFuncTable;
extern thiprtSetFuncTable*						  hiprtSetFuncTable;
extern thiprtDestroyFuncTable*					  hiprtDestroyFuncTable;
extern thiprtSaveGeometry*						  hiprtSaveGeometry;
extern thiprtLoadGeometry*						  hiprtLoadGeometry;
extern thiprtSaveScene*							  hiprtSaveScene;
extern thiprtLoadScene*							  hiprtLoadScene;
extern thiprtExportGeometryAabb*				  hiprtExportGeometryAabb;
extern thiprtExportSceneAabb*					  hiprtExportSceneAabb;
extern thiprtBuildTraceKernels*					  hiprtBuildTraceKernels;
extern thiprtBuildTraceKernelsFromBitcode*		  hiprtBuildTraceKernelsFromBitcode;
extern thiprtSetCacheDirPath*					  hiprtSetCacheDirPath;
extern thiprtSetLogLevel*						  hiprtSetLogLevel;

#if defined( _ENABLE_HIPRTEW )
thiprtCreateContext*					      hiprtCreateContext;
thiprtDestroyContext*					      hiprtDestroyContext;
thiprtCreateGeometry*					      hiprtCreateGeometry;
thiprtDestroyGeometry*					      hiprtDestroyGeometry;
thiprtBuildGeometry*						  hiprtBuildGeometry;
thiprtGetGeometryBuildTemporaryBufferSize*	  hiprtGetGeometryBuildTemporaryBufferSize;
thiprtCreateScene*						      hiprtCreateScene;
thiprtDestroyScene*						      hiprtDestroyScene;
thiprtBuildScene*						      hiprtBuildScene;
thiprtGetSceneBuildTemporaryBufferSize*	      hiprtGetSceneBuildTemporaryBufferSize;
thiprtCreateFuncTable*					      hiprtCreateFuncTable;
thiprtSetFuncTable*						      hiprtSetFuncTable;
thiprtDestroyFuncTable*					      hiprtDestroyFuncTable;
thiprtSaveGeometry*						      hiprtSaveGeometry;
thiprtLoadGeometry*						      hiprtLoadGeometry;
thiprtSaveScene*							  hiprtSaveScene;
thiprtLoadScene*							  hiprtLoadScene;
thiprtExportGeometryAabb*				      hiprtExportGeometryAabb;
thiprtExportSceneAabb*					      hiprtExportSceneAabb;
thiprtBuildTraceKernels*					  hiprtBuildTraceKernels;
thiprtBuildTraceKernelsFromBitcode*		      hiprtBuildTraceKernelsFromBitcode;
thiprtSetCacheDirPath*					      hiprtSetCacheDirPath;
thiprtSetLogLevel*						      hiprtSetLogLevel;
#endif

static DynamicLibrary dynamic_library_open_find( const char** paths )
{
	int i = 0;
	while ( paths[i] != NULL )
	{
		DynamicLibrary lib = dynamic_library_open( paths[i] );
		if ( lib != NULL )
		{
			return lib;
		}
		++i;
	}
	return NULL;
}

static void hiprtewHipExit( void )
{
	if ( hiprt_lib != NULL )
	{
		/*  Ignore errors. */
		dynamic_library_close( hiprt_lib );
		hiprt_lib = NULL;
	}
}

static void hiprtewInit( int* resultDriver)
{
#if defined( __APPLE__ )
	const char* hiprt_paths[]	   = { "", NULL };
#else
	const char* hiprt_paths[]	   = { HIPRT_LIB_NAME, NULL };
#endif
	static int initialized	  = 0;
	static int s_resultDriver = 0;

	if ( initialized )
	{
		*resultDriver = s_resultDriver;
		return;
	}

	initialized = 1;

	int error = atexit( hiprtewHipExit );
	if ( error )
	{
		s_resultDriver = HIPRTEW_ERROR_ATEXIT_FAILED;
		*resultDriver  = s_resultDriver;
		return;
	}

	//may be check for old hiprt version

	/* Load library. */
	hiprt_lib	   = dynamic_library_open_find( hiprt_paths );

	if ( hiprt_lib == NULL )
	{
		s_resultDriver = HIPRTEW_ERROR_OPEN_FAILED;
		*resultDriver  = s_resultDriver;
		return;
	}

	/* Fetch all function pointers. */
	HIPRT_LIBRARY_FIND_CHECKED(hiprtCreateContext);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtDestroyContext);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtCreateGeometry);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtDestroyGeometry);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtBuildGeometry);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtGetGeometryBuildTemporaryBufferSize);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtCreateScene);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtDestroyScene);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtBuildScene);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtGetSceneBuildTemporaryBufferSize);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtCreateFuncTable);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtSetFuncTable);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtDestroyFuncTable);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtSaveGeometry);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtLoadGeometry);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtSaveScene);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtLoadScene);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtExportGeometryAabb);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtExportSceneAabb);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtBuildTraceKernels);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtBuildTraceKernelsFromBitcode);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtSetCacheDirPath);
	HIPRT_LIBRARY_FIND_CHECKED(hiprtSetLogLevel);

	s_resultDriver = HIPRTEW_SUCCESS;
	*resultDriver  = s_resultDriver;
}

#ifdef __cplusplus
}
#endif //cpp

