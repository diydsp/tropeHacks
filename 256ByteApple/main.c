#include <stdio.h>
#include <stdlib.h>
 

// R1 Note Select
int8_t note = 0;
uint8_t note_count = 24;
uint8_t rot_count_max = 2;
uint8_t fund = 0;  // fundamental
uint8_t seq_pos = 0;

// R2 Envelope
int8_t env = 0; // current level
int8_t env_state = 0;   // attack, decay, paused
int8_t env_pd[ 3 ] = { 4, -1, 0 };    // phase delta for env level
int8_t env_peak = 100;

int8_t grain_count_max = 127;
int8_t grain_count=0;

// R4 rotate
int8_t phase       [ 2 ] = {  0,  0 };
uint8_t phase_delta[ 4 ] = {  10,  10, 10, 20 };
int8_t rot_count;


// R7 Noise Embedder

// output

FILE *fi_out;
void txtout( float val )
{
  fprintf( fi_out, "%f\n", val);
}

void init( void )
{
  fi_out =fopen("out.csv","w");
  if( fi_out== NULL )
  {
    printf("fopen failed.\n"); exit(-1);
  }

}

void R0_init( void )
{
  
}

void R1_Note_Select( void )
{
  uint8_t seq[]   = { 7,3,0,  5,2,0, 3,0,   12,15,12,9,  10,14,10,18 };
    
  uint8_t scale[] = { 16*4, 17*4, 18*4, 19*4, 20*4, 22*4, 23*4,   // C - F#
                      24*4, 25*4, 27*4, 28*4, 30*4 };         // G - B

  seq_pos++;if( seq_pos >= 16 ){ seq_pos = 0; }
  static int8_t scale_idx = -1;
  scale_idx++;
  if(scale_idx>=24){ scale_idx=0; }
  
  //uint8_t val = scale_idx;
  uint8_t val = seq[ seq_pos ];
  
  uint8_t count = 0;
  uint8_t acc = 0;
  while( val > 11 )
  {
    val -= 12;
    count++;
  }  
  fund = scale[ val ] >> 1;

  while( count > 0 )
  {
    fund >>= 1;
    count--;
  }

  phase_delta[ 0 ] = fund;
  phase_delta[ 1 ] = fund;
  phase_delta[ 2 ] = fund;
  phase_delta[ 3 ] = fund;
  
  printf("R1,  %d, %d, %d, %d", phase_delta[ 0 ], phase_delta[ 1 ],
                                phase_delta[ 2 ], phase_delta[ 3 ] );
  printf("\n");                              

}


void R2_Envelope( void )
{
  env += env_pd[ env_state ];

  switch( env_state )
  {
    case 0:
    if( env >= env_peak )        // increasing
    {
        env_state = 1;
        break;
    }
    case 1:  // decreasing
    if( env < 0   )   
    {
      env = 0;
      env_state = 2;
      break;
    }
  }


  int8_t harm = 1 + ( env >> 4 );
  uint16_t val = 0;  
  for( ; harm >0; harm-- )
  {
    val += fund;  
  }
  val >>= 1;
  phase_delta[ 0 ] = val;
  phase_delta[ 1 ] = val;
  phase_delta[ 2 ] = val;
  phase_delta[ 3 ] = val;

  printf("R2, %d, %d, %d, %d, %d, %d", env_state, env, env_pd[ env_state ], env_peak, val, fund);
  printf("\n");
      
}

void R3_Trigger( void )
{
    env_state = 0;
    env = 0;
}

void vca( int8_t osc_in )
{
  switch( env_state )
  {
    case 0: txtout( osc_in ); break;
    case 1: txtout( osc_in ); break;
    case 2: txtout(      0 ); break;
  }  
}

void R4_Rotate( void )
{        
        // R4 = update
        while( phase[ 0 ] >= 0 )     // phase wrap will muck it up!
        {
          //printf("0, ");
          phase[ 0 ] += phase_delta[ 0 ];	
          //printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          //printf("\n");
          vca( 1 );
        }
            
        while( phase[ 1 ] >= 0 )    
        {
          //printf("1, ");
          //printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          //printf("\n");
          phase[ 1 ] += phase_delta[ 1 ];
          vca( 0 );
          
        }  
        while( phase[ 0 ] <= 0 )    
        {
          //printf("2, ");
          phase[ 0 ] += phase_delta[ 2 ];
          //printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          //printf("\n");
          vca( 1 );
        }  
        
        while( phase[ 1 ] < 0 )    {
          //printf("3, ");
          //printf(" ph0,1=( %d, %d ) ",phase[ 0] , phase[ 1] );
          phase[ 1 ] += phase_delta[ 3 ];
          //printf("\n");
          vca( 0 );
        }  
        //printf("ph0,1=( %d, %d )",phase[ 0] , phase[ 1] );
        //printf("\n");

}


int main( int argc, char *argv [] )
{
  uint8_t note_count;
  
  init();
  printf("\n");
  
    // R0 = init

//#define DO_X_N_TIMES(variable,numTimes) variable = numTimes; while( variable-- > 0 ){

  note_count = 24;while( note_count-- > 0 ){ 
               R1_Note_Select();
               R3_Trigger();  // set env_state
               grain_count = grain_count_max; while( grain_count-- > 0 ){ 
                             R2_Envelope();  // env += env_pd
                             rot_count = rot_count_max; while( rot_count-- > 0 ){ 
                                         R4_Rotate();
        }  
      }
    }


    return 0;
}