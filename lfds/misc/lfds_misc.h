/***** defines *****/
#define LFDS_MISC_VERSION_STRING   "7.1.0"
#define LFDS_MISC_VERSION_INTEGER  710

#ifndef NULL
  #define NULL ( (void *) 0 )
#endif

#define POINTER   0
#define COUNTER   1
#define PAC_SIZE  2

#define LFDS_MISC_DELIBERATELY_CRASH  { char *c = 0; *c = 0; }

#if( !defined LFDS_PAL_ATOMIC_ADD )
  #define LFDS_PAL_NO_ATOMIC_ADD
  #define LFDS_MISC_ATOMIC_SUPPORT_ADD 0
  #define LFDS_PAL_ATOMIC_ADD( pointer_to_target, value, result, result_type )        \
  {                                                                                      \
    LFDS_PAL_ASSERT( !"LFDS_PAL_ATOMIC_ADD not implemented for this platform." );  \
    LFDS_MISC_DELIBERATELY_CRASH;                                                     \
  }
#else
  #define LFDS_MISC_ATOMIC_SUPPORT_ADD 1
#endif

#if( !defined LFDS_PAL_ATOMIC_CAS )
  #define LFDS_PAL_NO_ATOMIC_CAS
  #define LFDS_MISC_ATOMIC_SUPPORT_CAS 0
  #define LFDS_PAL_ATOMIC_CAS( pointer_to_destination, pointer_to_compare, new_destination, cas_strength, result )  \
  {                                                                                                                    \
    LFDS_PAL_ASSERT( !"LFDS_PAL_ATOMIC_CAS not implemented for this platform." );                                \
    (result) = 0;                                                                                                      \
    LFDS_MISC_DELIBERATELY_CRASH;                                                                                   \
  }
#else
  #define LFDS_MISC_ATOMIC_SUPPORT_CAS 1
#endif

#if( !defined LFDS_PAL_ATOMIC_DWCAS )
  #define LFDS_PAL_NO_ATOMIC_DWCAS
  #define LFDS_MISC_ATOMIC_SUPPORT_DWCAS 0
  #define LFDS_PAL_ATOMIC_DWCAS( pointer_to_destination, pointer_to_compare, pointer_to_new_destination, cas_strength, result )  \
  {                                                                                                                                 \
    LFDS_PAL_ASSERT( !"LFDS_PAL_ATOMIC_DWCAS not implemented for this platform." );                                           \
    (result) = 0;                                                                                                                   \
    LFDS_MISC_DELIBERATELY_CRASH;                                                                                                \
  }
#else
  #define LFDS_MISC_ATOMIC_SUPPORT_DWCAS 1
#endif

#if( !defined LFDS_PAL_ATOMIC_EXCHANGE )
  #define LFDS_PAL_NO_ATOMIC_EXCHANGE
  #define LFDS_MISC_ATOMIC_SUPPORT_EXCHANGE 0
  #define LFDS_PAL_ATOMIC_EXCHANGE( pointer_to_destination, new_value, original_value, value_type )  \
  {                                                                                                     \
    LFDS_PAL_ASSERT( !"LFDS_PAL_ATOMIC_EXCHANGE not implemented for this platform." );            \
    LFDS_MISC_DELIBERATELY_CRASH;                                                                    \
  }
#else
  #define LFDS_MISC_ATOMIC_SUPPORT_EXCHANGE 1
#endif

#if( !defined LFDS_PAL_ATOMIC_SET )
  #define LFDS_PAL_NO_ATOMIC_SET
  #define LFDS_MISC_ATOMIC_SUPPORT_SET 0
  #define LFDS_PAL_ATOMIC_SET( pointer_to_destination, new_value )                    \
  {                                                                                      \
    LFDS_PAL_ASSERT( !"LFDS_PAL_ATOMIC_SET not implemented for this platform." );  \
    LFDS_MISC_DELIBERATELY_CRASH;                                                     \
  }
#else
  #define LFDS_MISC_ATOMIC_SUPPORT_SET 1
#endif

#if( defined LFDS_PAL_BARRIER_COMPILER_LOAD && defined LFDS_PAL_BARRIER_PROCESSOR_LOAD )
  #define LFDS_MISC_BARRIER_LOAD  ( LFDS_PAL_BARRIER_COMPILER_LOAD, LFDS_PAL_BARRIER_PROCESSOR_LOAD, LFDS_PAL_BARRIER_COMPILER_LOAD )
#endif

#if( (!defined LFDS_PAL_BARRIER_COMPILER_LOAD || defined LFDS_PAL_COMPILER_BARRIERS_MISSING_PRESUMED_HAVING_A_GOOD_TIME) && defined LFDS_PAL_BARRIER_PROCESSOR_LOAD )
  #define LFDS_MISC_BARRIER_LOAD  LFDS_PAL_BARRIER_PROCESSOR_LOAD
#endif

#if( defined LFDS_PAL_BARRIER_COMPILER_LOAD && !defined LFDS_PAL_BARRIER_PROCESSOR_LOAD )
  #define LFDS_MISC_BARRIER_LOAD  LFDS_PAL_BARRIER_COMPILER_LOAD
#endif

#if( !defined LFDS_PAL_BARRIER_COMPILER_LOAD && !defined LFDS_PAL_BARRIER_PROCESSOR_LOAD )
  #define LFDS_MISC_BARRIER_LOAD
#endif

#if( defined LFDS_PAL_BARRIER_COMPILER_STORE && defined LFDS_PAL_BARRIER_PROCESSOR_STORE )
  #define LFDS_MISC_BARRIER_STORE  ( LFDS_PAL_BARRIER_COMPILER_STORE, LFDS_PAL_BARRIER_PROCESSOR_STORE, LFDS_PAL_BARRIER_COMPILER_STORE )
#endif

#if( (!defined LFDS_PAL_BARRIER_COMPILER_STORE || defined LFDS_PAL_COMPILER_BARRIERS_MISSING_PRESUMED_HAVING_A_GOOD_TIME) && defined LFDS_PAL_BARRIER_PROCESSOR_STORE )
  #define LFDS_MISC_BARRIER_STORE  LFDS_PAL_BARRIER_PROCESSOR_STORE
#endif

#if( defined LFDS_PAL_BARRIER_COMPILER_STORE && !defined LFDS_PAL_BARRIER_PROCESSOR_STORE )
  #define LFDS_MISC_BARRIER_STORE  LFDS_PAL_BARRIER_COMPILER_STORE
#endif

#if( !defined LFDS_PAL_BARRIER_COMPILER_STORE && !defined LFDS_PAL_BARRIER_PROCESSOR_STORE )
  #define LFDS_MISC_BARRIER_STORE
#endif

#if( defined LFDS_PAL_BARRIER_COMPILER_FULL && defined LFDS_PAL_BARRIER_PROCESSOR_FULL )
  #define LFDS_MISC_BARRIER_FULL  ( LFDS_PAL_BARRIER_COMPILER_FULL, LFDS_PAL_BARRIER_PROCESSOR_FULL, LFDS_PAL_BARRIER_COMPILER_FULL )
#endif

#if( (!defined LFDS_PAL_BARRIER_COMPILER_FULL || defined LFDS_PAL_COMPILER_BARRIERS_MISSING_PRESUMED_HAVING_A_GOOD_TIME) && defined LFDS_PAL_BARRIER_PROCESSOR_FULL )
  #define LFDS_MISC_BARRIER_FULL  LFDS_PAL_BARRIER_PROCESSOR_FULL
#endif

#if( defined LFDS_PAL_BARRIER_COMPILER_FULL && !defined LFDS_PAL_BARRIER_PROCESSOR_FULL )
  #define LFDS_MISC_BARRIER_FULL  LFDS_PAL_BARRIER_COMPILER_FULL
#endif

#if( !defined LFDS_PAL_BARRIER_COMPILER_FULL && !defined LFDS_PAL_BARRIER_PROCESSOR_FULL )
  #define LFDS_MISC_BARRIER_FULL
#endif

#if( (defined LFDS_PAL_BARRIER_COMPILER_LOAD && defined LFDS_PAL_BARRIER_COMPILER_STORE && defined LFDS_PAL_BARRIER_COMPILER_FULL) || (defined LFDS_PAL_COMPILER_BARRIERS_MISSING_PRESUMED_HAVING_A_GOOD_TIME) )
  #define LFDS_MISC_ATOMIC_SUPPORT_COMPILER_BARRIERS  1
#else
  #define LFDS_MISC_ATOMIC_SUPPORT_COMPILER_BARRIERS  0
#endif

#if( defined LFDS_PAL_BARRIER_PROCESSOR_LOAD && defined LFDS_PAL_BARRIER_PROCESSOR_STORE && defined LFDS_PAL_BARRIER_PROCESSOR_FULL )
  #define LFDS_MISC_ATOMIC_SUPPORT_PROCESSOR_BARRIERS  1
#else
  #define LFDS_MISC_ATOMIC_SUPPORT_PROCESSOR_BARRIERS  0
#endif

#define LFDS_MISC_MAKE_VALID_ON_CURRENT_LOGICAL_CORE_INITS_COMPLETED_BEFORE_NOW_ON_ANY_OTHER_LOGICAL_CORE  LFDS_MISC_BARRIER_LOAD
#define LFDS_MISC_FLUSH                                                                                    { LFDS_MISC_BARRIER_STORE; lfds_misc_force_store(); }

/***** enums *****/
enum lfds_misc_cas_strength
{
  // TRD : GCC defined values
  LFDS_MISC_CAS_STRENGTH_STRONG = 0,
  LFDS_MISC_CAS_STRENGTH_WEAK   = 1,
};

enum lfds_misc_validity
{
  LFDS_MISC_VALIDITY_UNKNOWN,
  LFDS_MISC_VALIDITY_VALID,
  LFDS_MISC_VALIDITY_INVALID_LOOP,
  LFDS_MISC_VALIDITY_INVALID_MISSING_ELEMENTS,
  LFDS_MISC_VALIDITY_INVALID_ADDITIONAL_ELEMENTS,
  LFDS_MISC_VALIDITY_INVALID_TEST_DATA,
  LFDS_MISC_VALIDITY_INVALID_ORDER,
  LFDS_MISC_VALIDITY_INVALID_ATOMIC_FAILED,
  LFDS_MISC_VALIDITY_INDETERMINATE_NONATOMIC_PASSED,
};

enum lfds_misc_flag
{
  LFDS_MISC_FLAG_LOWERED,
  LFDS_MISC_FLAG_RAISED
};

enum lfds_misc_query
{
  LFDS_MISC_QUERY_GET_BUILD_AND_VERSION_STRING
};

enum lfds_misc_data_structure
{
  LFDS_MISC_DATA_STRUCTURE_BTREE_AU,
  LFDS_MISC_DATA_STRUCTURE_FREELIST,
  LFDS_MISC_DATA_STRUCTURE_HASH_A,
  LFDS_MISC_DATA_STRUCTURE_LIST_AOS,
  LFDS_MISC_DATA_STRUCTURE_LIST_ASU,
  LFDS_MISC_DATA_STRUCTURE_QUEUE_BMM,
  LFDS_MISC_DATA_STRUCTURE_QUEUE_BSS,
  LFDS_MISC_DATA_STRUCTURE_QUEUE_UMM,
  LFDS_MISC_DATA_STRUCTURE_RINGBUFFER,
  LFDS_MISC_DATA_STRUCTURE_STACK,
  LFDS_MISC_DATA_STRUCTURE_COUNT
};

/***** struct *****/
struct lfds_misc_backoff_state
{
  lfds_pal_uint_t volatile LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    lock;

  lfds_pal_uint_t
    backoff_iteration_frequency_counters[2],
    metric,
    total_operations;
};

struct lfds_misc_globals
{
  struct lfds_prng_state
    ps;
};

struct lfds_misc_validation_info
{
  lfds_pal_uint_t
    min_elements,
    max_elements;
};

/***** externs *****/
extern struct lfds_misc_globals
  lfds_misc_globals;

/***** public prototypes *****/
static LFDS_PAL_INLINE void lfds_misc_force_store( void );

void lfds_misc_query( enum lfds_misc_query query_type, void *query_input, void *query_output );

/***** public in-line functions *****/
#pragma prefast( disable : 28112, "blah" )

static LFDS_PAL_INLINE void lfds_misc_force_store()
{
  lfds_pal_uint_t volatile LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    destination;

  LFDS_PAL_ATOMIC_SET( &destination, 0 );

  return;
}

