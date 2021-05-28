#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/time.h>
#include <math.h>
#define PI 3.141592654

int main()
{
  int fd,ret,ID;
  int n;
  int N=1;//Numbers of the continuous measurement
  int data_x,data_y;
  double data_x_ave=0;
  double data_y_ave=0;
  double data_x_offset;
  double data_y_offset;
  FILE *fp;
  double scale=0.92;
  double frontal_angle;
  
  ID = 0x1e;//HMC5883L

  /* WHO AM I */
  fd = wiringPiI2CSetup(ID);
  fprintf(stderr,"setup return : %d\n",fd);

  /*read offset and scale data*/
  fp = fopen("x_offset","r");
  if (fp==NULL) {
    fprintf(stderr,"file open failed:x_offset\n");
    exit(-1);
  }
  fscanf(fp,"%lf",&data_x_offset);
  fclose(fp);

  fp = fopen("y_offset","r");
  if (fp==NULL) {
    fprintf(stderr,"file open failed:y_offset\n");
    exit(-1);
  }
  fscanf(fp,"%lf",&data_y_offset);
  fclose(fp);

  fp = fopen("scale","r");
  if (fp==NULL) {
    fprintf(stderr,"file open failed:scale\n");
    exit(-1);
  }
  fscanf(fp,"%lf",&scale);
  fclose(fp);

  wiringPiI2CWriteReg8(fd,0x01,0x60);

  /* start senser */
  if((wiringPiI2CWriteReg8(fd,0x02,0x00))<0){
    fprintf(stderr,"write error register 0x02");
  }
  fprintf(stderr,"write register:0x02 = 0x00(i.e.continuous measuring mode)\n");

  fprintf(stderr,"During the calibration....\n");
  for (n=0;n<N;n++) {

    ret = wiringPiI2CReadReg8(fd,0x03);
    data_x = ret*256;
    ret = wiringPiI2CReadReg8(fd,0x04);
    data_x = data_x + ret;
    if ( data_x >= 0x8000 ) data_x = -((65535 - data_x) + 1);
    
    /*dummy reading for z-axis data*/
    ret = wiringPiI2CReadReg8(fd,0x05);
    ret = wiringPiI2CReadReg8(fd,0x06);

    ret = wiringPiI2CReadReg8(fd,0x07);
    data_y = ret * 256;
    ret = wiringPiI2CReadReg8(fd,0x08);
    data_y = data_y + ret;
    if ( data_y >= 0x8000 ) data_y = -((65535 - data_y) + 1);

    data_x = (data_x - data_x_offset); //* scale;
    data_y = (data_y - data_y_offset);

    data_x_ave = data_x_ave + data_x;
    data_y_ave = data_y_ave + data_y;

    usleep(100000);
  }

  data_x_ave = data_x_ave / (double)N;
  data_y_ave = data_y_ave / (double)N;

  frontal_angle = atan2(data_y_ave,data_x_ave);
  frontal_angle = frontal_angle * 180 / PI;

  fp = fopen("frontal_angle","w");
  if (fp==NULL) {
    exit(-1);
  }
  fprintf(fp,"%d\n",(int)frontal_angle);
  fclose(fp);

  fprintf(stderr,"frontal_angle = %3.0lf[deg]\n",frontal_angle);

  return;
}
