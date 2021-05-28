#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/time.h>

int main()
{
  int fd,ret,ID;
  int n;
  int data_x,data_y;
  int data_x_min=0;
  int data_x_max=0;
  int data_y_min=0;
  int data_y_max=0;
  double data_x_offset;
  double data_y_offset;
  double data_x_radius;
  double data_y_radius;
  FILE *fp;
  double scale=0.92;
  
  ID = 0x1e;//HMC5883L

  /* WHO AM I */
  fd = wiringPiI2CSetup(ID);
  fprintf(stderr,"setup return : %d\n",fd);

  wiringPiI2CWriteReg8(fd,0x01,0x60);

  /* start senser */
  if((wiringPiI2CWriteReg8(fd,0x02,0x00))<0){
    fprintf(stderr,"write error register 0x02");
  }
  fprintf(stderr,"write register:0x02 = 0x00(i.e.continuous measuring mode)\n");

  fprintf(stderr,"During the calibration....\n");
  for (n=0;n<150;n++) {

    ret = wiringPiI2CReadReg8(fd,0x03);
    data_x = ret*256;
    ret = wiringPiI2CReadReg8(fd,0x04);
    data_x = data_x + ret;
    if ( data_x >= 0x8000 ) data_x = -((65535 - data_x) + 1);

    if ( data_x > data_x_max && data_x < 255) data_x_max = data_x;
    if ( data_x < data_x_min && data_x > -257) data_x_min = data_x;
    
    /*dummy reading for z-axis data*/
    ret = wiringPiI2CReadReg8(fd,0x05);
    ret = wiringPiI2CReadReg8(fd,0x06);

    ret = wiringPiI2CReadReg8(fd,0x07);
    data_y = ret * 256;
    ret = wiringPiI2CReadReg8(fd,0x08);
    data_y = data_y + ret;
    if ( data_y >= 0x8000 ) data_y = -((65535 - data_y) + 1);

    if ( data_y > data_y_max && data_y < 255) data_y_max = data_y;
    if ( data_y < data_y_min && data_y > -257) data_y_min = data_y;
    
    fprintf(stdout,"%d, %d\n",data_x,data_y);

    usleep(100000);
  }

  data_x_offset = (data_x_max + data_x_min)/2.0;
  data_y_offset = (data_y_max + data_y_min)/2.0;

  data_x_radius = (data_x_max - data_x_min);
  data_y_radius = (data_y_max - data_y_min);
  scale       = data_y_radius / data_x_radius;

  fp = fopen("x_offset","w");
  if (fp==NULL) {
    fprintf(stderr,"file open failed.\n");
    exit(-1);
  }
  fprintf(fp,"%lf\n",data_x_offset);
  fclose(fp);

  fp = fopen("y_offset","w");
  if (fp==NULL) {
    fprintf(stderr,"file open failed.\n");
    exit(-1);
  }
  fprintf(fp,"%lf\n",data_y_offset);
  fclose(fp);

  fp = fopen("scale","w");
  if (fp==NULL) {
    fprintf(stderr,"file open failed.\n");
    exit(-1);
  }
  fprintf(fp,"%lf\n",scale);
  fclose(fp);

  fprintf(stderr,"x_max = %d\n",data_x_max);
  fprintf(stderr,"x_min = %d\n",data_x_min);
  fprintf(stderr,"y_max = %d\n",data_y_max);
  fprintf(stderr,"y_min = %d\n",data_y_min);
  fprintf(stderr,"x_offset = %lf\n",data_x_offset);
  fprintf(stderr,"y_offset = %lf\n",data_y_offset);
  fprintf(stderr,"x_radius = %lf\n",data_x_radius);
  fprintf(stderr,"y_radius = %lf\n",data_y_radius);
  fprintf(stderr,"scale(y/x) = %lf\n",scale);

  return;
}
