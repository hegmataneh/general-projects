// ChatGPT generated

#if defined Uses_packet_queue || !defined __COMPILING

#define QUEUE_CAPACITY 10240
#define PACKET_SIZE 4096

typedef struct
{
	char data[ QUEUE_CAPACITY ][ PACKET_SIZE ];
	size_t lengths[ QUEUE_CAPACITY ];
	int head;
	int tail;
	int count;

	pthread_mutex_t lock;
	pthread_cond_t not_empty;
	pthread_cond_t not_full;
} PacketQueue;

// Initialize the queue
void queue_init( PacketQueue * q );

// Destroy the queue
void queue_destroy( PacketQueue * q );

// Blocking push: waits if full
void queue_push( PacketQueue * q , const char * buf , size_t len );

// Blocking pop: waits if empty
int queue_pop( PacketQueue * q , char * out_buf , size_t * out_len );

// Non-blocking: returns 1 if data exists, 0 otherwise
int queue_peek_available( PacketQueue * q );

// Non-blocking pop: returns 0 if successful, -1 if empty
int queue_try_pop( PacketQueue * q , char * out_buf , size_t * out_len );

#endif

