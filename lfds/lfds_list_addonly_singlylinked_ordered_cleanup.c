/***** includes *****/
#include "lfds_list_addonly_singlylinked_ordered_internal.h"





/****************************************************************************/
void lfds_list_aso_cleanup( struct lfds_list_aso_state *lasos,
                               void (*element_cleanup_callback)(struct lfds_list_aso_state *lasos, struct lfds_list_aso_element *lasoe) )
{
  struct lfds_list_aso_element
    *lasoe,
    *temp;

  LFDS_PAL_ASSERT( lasos != NULL );
  // TRD : element_cleanup_callback can be NULL

  LFDS_MISC_BARRIER_LOAD;

  if( element_cleanup_callback == NULL )
    return;

  lasoe = LFDS_LIST_ASO_GET_START( *lasos );

  while( lasoe != NULL )
  {
    temp = lasoe;

    lasoe = LFDS_LIST_ASO_GET_NEXT( *lasoe );

    element_cleanup_callback( lasos, temp );
  }

  return;
}

