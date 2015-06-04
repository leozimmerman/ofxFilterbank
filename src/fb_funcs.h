
#pragma once



#include <math.h>

#define myPI M_PI ///*
#define PI2 M_PI*2.


#define ONEZERO_ARGS  2 /*b0 b1*/
#define ONEPOLE_ARGS  2 /*b0 a1*/
#define TWOZERO_ARGS  3 /*b0 b1 b2*/
#define TWOPOLE_ARGS  3 /*b0 a1 a2*/
#define COMB_ARGS     3 /*delay feedback gain*/
#define RESON_ARGS    3 /*FC BW gain*/

#define FIR       0
#define IIR       1

#define MAX_TAPS  32

#define ONEZERO   0
#define ONEPOLE   1
#define TWOZERO   2
#define TWOPOLE   3
#define RESON     4
#define COMB      5
#define ALPASS    6
#define DELAY     7

#define LP       0
#define HP       1

#define ONEZERO_SIZE   2
#define ONEPOLE_SIZE   2
#define TWOZERO_SIZE   3
#define TWOPOLE_SIZE   3
#define BIQUAD_SIZE    3
#define RESON_SIZE     4

//#endif

typedef struct {

  double R;
  double G;
  double a1;
  double a2;
  double ih1;
  double ih2;
  double oh1;
  double oh2;
  float gain;

}RESONDATA;

/******************************************************************/
/******************************************************************/


void   set_reson(double frec, float bw, RESONDATA *fdata, float srate, float gain);
float  reson(float input, RESONDATA *fdata);
void set_reson_freq(float frec, RESONDATA *fdata, float sr);

/******************************************************************/
/******************************************************************/
