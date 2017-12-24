/***** defines *****/
#define LFDS_LIST_ASO_GET_START( list_aso_state )                                             ( LFDS_MISC_BARRIER_LOAD, (list_aso_state).start->next )
#define LFDS_LIST_ASO_GET_NEXT( list_aso_element )                                            ( LFDS_MISC_BARRIER_LOAD, (list_aso_element).next )
#define LFDS_LIST_ASO_GET_START_AND_THEN_NEXT( list_aso_state, pointer_to_list_aso_element )  ( (pointer_to_list_aso_element) == NULL ? ( (pointer_to_list_aso_element) = LFDS_LIST_ASO_GET_START(list_aso_state) ) : ( (pointer_to_list_aso_element) = LFDS_LIST_ASO_GET_NEXT(*(pointer_to_list_aso_element)) ) )
#define LFDS_LIST_ASO_GET_KEY_FROM_ELEMENT( list_aso_element )                                ( (list_aso_element).key )
#define LFDS_LIST_ASO_SET_KEY_IN_ELEMENT( list_aso_element, new_key )                         ( (list_aso_element).key = (void *) (lfds_pal_uint_t) (new_key) )
#define LFDS_LIST_ASO_GET_VALUE_FROM_ELEMENT( list_aso_element )                              ( LFDS_MISC_BARRIER_LOAD, (list_aso_element).value )
#define LFDS_LIST_ASO_SET_VALUE_IN_ELEMENT( list_aso_element, new_value )                     { LFDS_PAL_ATOMIC_SET( &(list_aso_element).value, new_value ); }
#define LFDS_LIST_ASO_GET_USER_STATE_FROM_STATE( list_aso_state )                             ( (list_aso_state).user_state )

/***** enums *****/
enum lfds_list_aso_existing_key
{
  LFDS_LIST_ASO_EXISTING_KEY_OVERWRITE,
  LFDS_LIST_ASO_EXISTING_KEY_FAIL
};

enum lfds_list_aso_insert_result
{
  LFDS_LIST_ASO_INSERT_RESULT_FAILURE_EXISTING_KEY,
  LFDS_LIST_ASO_INSERT_RESULT_SUCCESS_OVERWRITE,
  LFDS_LIST_ASO_INSERT_RESULT_SUCCESS
};

enum lfds_list_aso_query
{
  LFDS_LIST_ASO_QUERY_GET_POTENTIALLY_INACCURATE_COUNT,
  LFDS_LIST_ASO_QUERY_SINGLETHREADED_VALIDATE
};

/***** structures *****/
struct lfds_list_aso_element
{
  struct lfds_list_aso_element LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_SINGLE_POINTER)
    *volatile next;

  void LFDS_PAL_ALIGN(LFDS_PAL_ALIGN_SINGLE_POINTER)
    *volatile value;

  void
    *key;
};

struct lfds_list_aso_state
{
  struct lfds_list_aso_element LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    dummy_element;

  struct lfds_list_aso_element LFDS_PAL_ALIGN(LFDS_PAL_ATOMIC_ISOLATION_IN_BYTES)
    *start;

  int
    (*key_compare_function)( void const *new_key, void const *existing_key );

  enum lfds_list_aso_existing_key
    existing_key;

  void
    *user_state;

  struct lfds_misc_backoff_state
    insert_backoff;
};

/***** public prototypes *****/
void lfds_list_aso_init_valid_on_current_logical_core( struct lfds_list_aso_state *lasos,
                                                          int (*key_compare_function)(void const *new_key, void const *existing_key),
                                                          enum lfds_list_aso_existing_key existing_key,
                                                          void *user_state );
  // TRD : used in conjunction with the #define LFDS_MISC_MAKE_VALID_ON_CURRENT_LOGICAL_CORE_INITS_COMPLETED_BEFORE_NOW_ON_ANY_OTHER_LOGICAL_CORE

void lfds_list_aso_cleanup( struct lfds_list_aso_state *lasos,
                               void (*element_cleanup_callback)(struct lfds_list_aso_state *lasos, struct lfds_list_aso_element *lasoe) );

enum lfds_list_aso_insert_result lfds_list_aso_insert( struct lfds_list_aso_state *lasos,
                                                             struct lfds_list_aso_element *lasoe,
                                                             struct lfds_list_aso_element **existing_lasoe );

int lfds_list_aso_get_by_key( struct lfds_list_aso_state *lasos,
                                 void *key,
                                 struct lfds_list_aso_element **lasoe );

void lfds_list_aso_query( struct lfds_list_aso_state *lasos,
                             enum lfds_list_aso_query query_type,
                             void *query_input,
                             void *query_output );

