#ifndef LIBLFDS_H

  /***** defines *****/
  #define LIBLFDS_H

  /***** pragmas on *****/
  #pragma warning( push )
  #pragma warning( disable : 4324 )                                          // TRD : 4324 disables MSVC warnings for structure alignment padding due to alignment specifiers
  #pragma prefast( disable : 28113 28182 28183, "blah" )

#ifdef __cplusplus
extern "C" {
#endif
/***** includes *****/
#include "lfds_porting_abstraction_layer_compiler.h"
#include "lfds_porting_abstraction_layer_operating_system.h"
#include "lfds_porting_abstraction_layer_processor.h"

#include "misc/lfds_prng.h"                                       // TRD : misc requires prng
#include "misc/lfds_misc.h"                                       // TRD : everything after depends on misc

#include "lfds_list_addonly_singlylinked_ordered.h"

#ifdef __cplusplus
}
#endif

  /***** pragmas off *****/
  #pragma warning( pop )

#endif

