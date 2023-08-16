#pragma once

#define HIPRT_MAJOR_VERSION 2
#define HIPRT_MINOR_VERSION 0
#define HIPRT_PATCH_VERSION 0xb68861

#define HIPRT_API_VERSION 2000
#define HIPRT_VERSION_STR "02000"

#include <hiprt/hiprt_types.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined( _MSC_VER )
#ifdef HIPRT_EXPORTS
#define HIPRT_API __declspec( dllexport )
#else
#define HIPRT_API __declspec( dllimport )
#endif
#elif defined( __GNUC__ )
#ifdef HIPRT_EXPORTS
#define HIPRT_API __attribute__( ( visibility( "default" ) ) )
#else
#define HIPRT_API
#endif
#else
#define HIPRT_API
#pragma warning Unknown dynamic link import / export semantics.
#endif

/** \brief Create HIPRT API context.
 *
 * All HIPRT functions expect context as their first argument. Context
 * keeps global data required by HIPRT session. Calls made from different
 * threads with different HIPRT contexts are safe. Calls with the same context
 * should be externally synchronized by the client.
 *
 * \param hiprtApiVersion API version.
 * \param outContext Created context.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtCreateContext( uint32_t hiprtApiVersion, hiprtContextCreationInput& input, hiprtContext* outContext );

/** \brief Destory HIPRT API context.
 *
 * Destroys all the global resources used by HIPRT session. Further calls
 * with this context are prohibited.
 *
 * \param context API context.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtDestroyContext( hiprtContext context );

/** \brief Create a geometry.
 *
 * This function creates
 * hiprtGeometry representing acceleration structure topology.
 *
 * \param context HIPRT API context.
 * \param buildInput Describes input primitive to build geometry from.
 * \param buildOptions Various flags controlling build process.
 * \param outGeometry Resulting geometry.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtCreateGeometry(
	hiprtContext				   context,
	const hiprtGeometryBuildInput* buildInput,
	const hiprtBuildOptions*	   buildOptions,
	hiprtGeometry*				   outGeometry );

/** \brief Destroy a geometry.
 *
 * This function destroys
 * hiprtGeometry representing acceleration structure topology.
 *
 * \param context HIPRT API context.
 * \param outGeometry Resulting geometry.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtDestroyGeometry( hiprtContext context, hiprtGeometry outGeometry );

/** \brief Build or update a geometry.
 *
 * Given geometry description from the client, this function builds
 * hiprtGeometry representing acceleration structure topology (in case of a
 * build) or updates acceleration structure keeping topology intact (update).
 *
 * \param context HIPRT API context.
 * \param buildOperation Type of build operation.
 * \param buildInput Describes input primitive to build geometry from.
 * \param buildOptions Various flags controlling build process.
 * \param attributeOutputs Describes additional values written into vidmem.
 * \param attributeOutputCount Number of additional attributes, can be 0.
 * \param temporaryBuffer Temporary buffer for build operation.
 * \param stream to run acceleration structure build command.
 * \param outGeometry Resulting geometry.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtBuildGeometry(
	hiprtContext				   context,
	hiprtBuildOperation			   buildOperation,
	const hiprtGeometryBuildInput* buildInput,
	const hiprtBuildOptions*	   buildOptions,
	hiprtDevicePtr				   temporaryBuffer,
	hiprtApiStream				   stream,
	hiprtGeometry				   outGeometry );

/** \brief Get temporary storage requirements for geometry build.
 *
 * \param context HIPRT API context.
 * \param buildInput Describes input primitive to build geometry from.
 * \param buildOptions Various flags controlling build process.
 * \param outSize Pointer to write result to.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtGetGeometryBuildTemporaryBufferSize(
	hiprtContext context, const hiprtGeometryBuildInput* buildInput, const hiprtBuildOptions* buildOptions, size_t* outSize );

/** \brief Create a scene.
 *
 * This function creates
 * hiprtScene representing acceleration structure topology.
 *
 * \param context HIPRT API context.
 * \param buildInput Decribes input geometires to build scene for.
 * \param buildOptions Various flags controlling build process.
 * \param outScene Resulting scene.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtCreateScene(
	hiprtContext context, const hiprtSceneBuildInput* buildInput, const hiprtBuildOptions* buildOptions, hiprtScene* outScene );

/** \brief Destroy a scene.
 *
 * This function destroys
 * hiprtScene representing acceleration structure topology.
 *
 * \param context HIPRT API context.
 * \param outScene Resulting scene.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtDestroyScene( hiprtContext context, hiprtScene outScene );

/** \brief Build or update a scene.
 *
 * Given a number of hiprtGeometries from the client, this function builds
 * hiprtScene representing top level acceleration structure topology (in case of
 * a build) or updates acceleration structure keeping topology intact (update).
 *
 * \param context HIPRT API context.
 * \param buildOperation Type of build operation.
 * \param buildInput Decribes input geometires to build scene for.
 * \param buildOptions Various flags controlling build process.
 * \param temporaryBuffer Temporary buffer for build operation.
 * \param stream to run acceleration structure build command.
 * \param outScene Resulting scene.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtBuildScene(
	hiprtContext				context,
	hiprtBuildOperation			buildOperation,
	const hiprtSceneBuildInput* buildInput,
	const hiprtBuildOptions*	buildOptions,
	hiprtDevicePtr				temporaryBuffer,
	hiprtApiStream				stream,
	hiprtScene					outScene );

/** \brief Get temporary storage requirements for scene build.
 *
 * \param context HIPRT API context.
 * \param buildInput Decribes input geometires to build scene for.
 * \param buildOptions Various flags controlling build process.
 * \param outSize Pointer to write result to.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtGetSceneBuildTemporaryBufferSize(
	hiprtContext context, const hiprtSceneBuildInput* buildInput, const hiprtBuildOptions* buildOptions, size_t* outSize );

/** \brief Creates a custom function table (for custom geometry).
 *
 * \param context HIPRT API context.
 * \param numGeomTypes The number of geometry types.
 * \param numRayTypes The number of ray types.
 * \param outFuncTable The resulting table.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtCreateFuncTable( hiprtContext context, uint32_t numGeomTypes, uint32_t numRayTypes, hiprtFuncTable* outFuncTable );

/** \brief Sets a custom function table.
 *
 * \param context HIPRT API context.
 * \param funcTable Function table.
 * \param geomType Geometry type.
 * \param rayType Ray type.
 * \param set Function set to be set.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtSetFuncTable( hiprtContext context, hiprtFuncTable funcTable, uint32_t geomType, uint32_t rayType, hiprtFuncDataSet set ); 

/** \brief Destroys a custom function table.
 *
 * \param context HIPRT API context.
 * \param funcTable Function table.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtDestroyFuncTable( hiprtContext context, hiprtFuncTable funcTable );

/** \brief Saves hiprtGeometry to a binary file.
 *
 * \param context HIPRT API context.
 * \param inGeometry Geometry to be saved.
 * \param filename File name with full path.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtSaveGeometry( hiprtContext context, hiprtGeometry inGeometry, const char* filename );

/** \brief Loads hiprtGeometry to a binary file.
 *
 * \param context HIPRT API context.
 * \param outGeometry Geometry to be loaded.
 * \param filename File name with full path.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtLoadGeometry( hiprtContext context, hiprtGeometry* outGeometry, const char* filename );

/** \brief Saves hiprtScene to a binary file.
 *
 * \param context HIPRT API context.
 * \param inScene Scene to be saved.
 * \param filename File name with full path.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtSaveScene( hiprtContext context, hiprtScene inScene, const char* filename );

/** \brief Loads hiprtScene to a binary file.
 *
 * \param context HIPRT API context.
 * \param outScene Scene to be loaded.
 * \param filename File name with full path.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtLoadScene( hiprtContext context, hiprtScene* outScene, const char* filename );

/** \brief Output scene's AABB.
 *
 * \param context HIPRT API context.
 * \param inGeometry Geometry to be queried.
 * \param outAabbMin The bounding box min. bound.
 * \param outAabbMax The bounding box max. bound.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtExportGeometryAabb( hiprtContext context, hiprtGeometry inGeometry, hiprtFloat3& outAabbMin, hiprtFloat3& outAabbMax );

/** \brief Output scene's AABB.
 *
 * \param context HIPRT API context.
 * \param inScene Scene to be queried.
 * \param outAabbMin The bounding box min. bound.
 * \param outAabbMax The bounding box max. bound.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtExportSceneAabb( hiprtContext context, hiprtScene inScene, hiprtFloat3& outAabbMin, hiprtFloat3& outAabbMax );

/** \brief Get function instance with HIPRT routines. 
 * \param context HIPRT API context.
 * \param numFunctions The number of functions to compile.
 * \param funcNames Functions to to be returned, cannot be NULL.
 * \param src The module source code.
 * \param moduleName The name of the module.
 * \param numHeaders The number of headers.
 * \param headers Sources of the headers, headers can be NULL when numHeaders is 0.
 * \param includeNames The header names to be included to the module, includeNames can be NULL
 * when numHeaders is 0.
 * \param numOptions Number of compiler options, can be 0.
 * \param options The compiler options, can be NULL.
 * \param numGeomTypes The number of geometry types.
 * \param numRayTypes The number of ray types.
 * \param funcNameSets The table of custom function names (numRayTypes x numGeomTypes).
 * \param functionsOut The output function instances.
 * \param moduleOut The output module instance.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtBuildTraceKernels(
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

/** \brief Get function instance with HIPRT routines.
 * \param context HIPRT API context.
 * \param numFunctions The number of functions to compile.
 * \param funcNames Functions to to be returned, cannot be NULL.
 * \param moduleName The name of the bitcode module.
 * \param bitcodeBinary The user compiled bitcode.
 * \param bitcodeBinarySize The size of the compiled bitcode.
 * \param numGeomTypes The number of geometry types.
 * \param numRayTypes The number of ray types.
 * \param funcNameSets The table of custom function names (numRayTypes x numGeomTypes).
 * \param functionsOut The output function instances.
 * \return HIPRT error in case of a failure, hiprtSuccess otherwise.
 */
HIPRT_API hiprtError hiprtBuildTraceKernelsFromBitcode(
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

/** \brief Setting log level.
 * \param context HIPRT API context.
 * \param path user defined path to cache kernels.
 */
HIPRT_API void hiprtSetCacheDirPath( hiprtContext context, const char* path );


/** \brief Setting log level.
 *
 * \param level Desired log level.
 */
HIPRT_API void hiprtSetLogLevel( hiprtLogLevel level );

#ifdef __cplusplus
}
#endif
