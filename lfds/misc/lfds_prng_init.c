/***** includes *****/
#include "lfds_prng_internal.h"





/****************************************************************************/
void lfds_prng_init_valid_on_current_logical_core( struct lfds_prng_state *ps, lfds_pal_uint_t seed )
{
  LFDS_PAL_ASSERT( ps != NULL );
  LFDS_PAL_ASSERT( (lfds_pal_uint_t) &ps->entropy % LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES == 0 );
  // TRD : seed can be any value in its range (unlike for the mixing function)

  LFDS_PRNG_ST_MIXING_FUNCTION( seed );

  ps->entropy = seed;

  LFDS_MISC_BARRIER_STORE;

  lfds_misc_force_store();

  return;
}





/****************************************************************************/
void lfds_prng_st_init( struct lfds_prng_st_state *psts, lfds_pal_uint_t seed )
{
  LFDS_PAL_ASSERT( psts != NULL );
  LFDS_PAL_ASSERT( seed != 0 );

  LFDS_PRNG_ST_MIXING_FUNCTION( seed );

  psts->entropy = seed;

  return;
}

