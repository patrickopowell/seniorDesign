

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

void init_mem()
{
	// initialize stat memory
	
	key_t stat_key = ftok( MEMORY_ID_STAT, 1 );

	int shmid = shmget( stat_key, sizeof( stat_info_memory ), 0600 | IPC_CREAT );
	if ( shmid == -1 )
		fail( "Can't create shared memory" );

	stat_info = (stat_info_memory *)shmat( shmid, 0, 0 );
	if ( stat_info == (void *)-1 )
		fail( "Can't map shared memory segment into address space" );

	stat_lock = sem_open( STAT_LOCK, O_CREAT, 0600, 1 );
	if ( stat_lock == SEM_FAILED )
		fail( "Can't make semaphore" );
	
	// initialize sla shared memory
	
	key_t sla_key = ftok( MEMORY_ID_SLA, 1 );

	int shmid = shmget( sla_key, sizeof( sla_info_memory ), 0600 | IPC_CREAT );
	if ( shmid == -1 )
		fail( "Can't create shared memory" );

	sla_info = (sla_info_memory *)shmat( shmid, 0, 0 );
	if ( sla_info == (void *)-1 )
		fail( "Can't map shared memory segment into address space" );

	sla_lock = sem_open( SLA_LOCK, O_CREAT, 0600, 1 );
	if ( sla_lock == SEM_FAILED )
		fail( "Can't make semaphore" );
}

void close_mem()
{
	sem_close( stat_lock );
	shmdt( stat_info );
	
	sem_close( sla_lock );
	shmdt( sla_info );
}