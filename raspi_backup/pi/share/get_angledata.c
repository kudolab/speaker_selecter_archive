//gcc get_angledata.c -lwiringPi -lm -o get_angledata
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
  double data_x,data_y;
  double data_x_offset,data_y_offset;
  FILE *fp;
  double angle;
  double scale=0.7;
  double frontal_angle;

  ID = 0x1e;//HMC5883L

  /* WHO AM I */
  fd = wiringPiI2CSetup(ID);
  printf("setup return : %d\n",fd);

  /*store the calibration data*/
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

  
  fp = fopen("frontal_angle","r");
  if (fp==NULL) {
    fprintf(stderr,"file open failed:frontal_angle\n");
    exit(-1);
  }
  fscanf(fp,"%lf",&frontal_angle);
  fclose(fp);

  wiringPiI2CWriteReg8(fd,0x01,0x60);

  /* start senser */
  if((wiringPiI2CWriteReg8(fd,0x02,0x00))<0) printf("write error register 0x02");
  fprintf(stderr,"write register:0x02 = 0x00(i.e.continuous measuring mode)\n");

  for (n=0;n<10;n++){
    
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
    
    /*x-y calibration*/
    data_x = (data_x - data_x_offset); //* scale;
    data_y = (data_y - data_y_offset);
    angle = atan2(data_y,data_x);

    angle = angle - frontal_angle / 180.0 * PI;
    angle = - angle;
    if (angle<0)angle += 2.0 * PI;
    angle *= 180.0 / PI;

    angle = 360 - angle;

    //angle = (int)(angle / 5.0);
    //angle = angle * 5.0;
    fp = fopen("angle","w+");
    if (fp==NULL) exit(-1);
    
    
    fprintf(fp,"%3.0lf\n",angle);
    fclose(fp);
    
    fprintf(stdout,"%lf, %lf, %3.0lf\n",data_x,data_y,angle);
    usleep(100000);
  }
  
  return;
}
