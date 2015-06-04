

#include "fb_funcs.h"

/******************************************************************/
void set_reson(double frec, float bw, RESONDATA *fdata, float srate, float gain)
{
  /*
    Tomado de "Elements of Computer Music, F. R. Moore, 1990", pp.268-69
    R = exp(-PI*bw/SR)
    G = 1-R
    a1= 2R * cos(2*PI*f/SR)
    a2= -R*R
    (aqui usamos G para guardar G) y gain para guardar el ajuste general
    de ganancia
  */


  fdata->R = exp(-myPI*bw/(double)srate);
  fdata->G = 1. - fdata->R;
  fdata->a1= 2.* fdata->R * cos(2.*myPI*frec/(double)srate);
  fdata->a2= - (fdata->R*fdata->R);
  fdata->gain = gain;

  fdata->ih1= fdata->ih2 = fdata->oh1 = fdata->oh2 = 0.;

  return;
}
/******************************************************************/
void set_reson_freq(float frec, RESONDATA *fdata, float sr)
{
 fdata->a1= 2. * fdata->R * cos(2.*myPI*frec/(double)sr);
return;
}
/******************************************************************/
float  reson(float input, RESONDATA *fdata)
{
  double output;


  output= fdata->G*(input - fdata->R*fdata->ih2) +
          fdata->oh1*fdata->a1 + fdata->oh2*fdata->a2;

  fdata->oh2=fdata->oh1;
  fdata->oh1=output;
  fdata->ih2=fdata->ih1;
  fdata->ih1=input;

  float out = (float) output;

//  return(output*fdata->gain);
    return(out*fdata->gain);
}


