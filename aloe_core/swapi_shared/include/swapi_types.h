
#ifndef SWAPI_TYPES_H_
#define SWAPI_TYPES_H_

typedef enum {
	READ, WRITE
}swapi_itf_mode_t;

struct _s_itf {
	int id;
};
typedef struct _s_itf* itf_t;

struct _s_var {
	int id;
};
typedef struct _s_var* var_t;

struct _s_counter {
	int id;
};
typedef struct _s_counter* counter_t;

struct _s_log {
	int id;
};
typedef struct _s_log* log_t;

#endif /* SWAPI_TYPES_H_ */
