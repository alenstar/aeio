/***** includes *****/
#include "lfds_misc_internal.h"





/****************************************************************************/
void lfds_misc_internal_backoff_init( struct lfds_misc_backoff_state *bs )
{
  LFDS_PAL_ASSERT( bs != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &bs->lock % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );

  bs->lock = LFDS_MISC_FLAG_LOWERED;
  bs->backoff_iteration_frequency_counters[0] = 0;
  bs->backoff_iteration_frequency_counters[1] = 0;
  bs->metric = 1;
  bs->total_operations = 0;

  return;
}

