#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define processnumber 20
#define processnumberupper 30
#define times 10
#define range 200

#define square 1000

FILE *fp1;
FILE *fp2;

int pd(int lamda);

int main(int argc, char *argv[])
{
  //srand(time(NULL));
  srand(atoi(argv[1]));

  int processx[processnumberupper] = {0};
  int processy[processnumberupper] = {0};
  int topology[processnumberupper][processnumberupper] = {0};
  int totaledge = 0;
  int trigger = 10000;
  int mytrigger[processnumberupper] = {0};
  int messagesize = 0;
  int join = 0, leave = 0;
  int jointag = processnumber, leavetag = 0;
  int live[processnumberupper] = {0};
  int lived[processnumberupper] = {0};
  int totallived = 0;
  int current = 0;
  int thr = 0;
  int receivedtrigger = 0;
  int mythr[processnumberupper] = {0};
  int messagenumber = 0;
  
  int i = 0, j = 0, k = 0, l = 0, temp = 0, z = 0;
  
  thr = floor(trigger/processnumberupper);
  for(i=0;i<processnumberupper;i++)
    mythr[i] = thr;
  
  for(i=0;i<processnumberupper;i++)
  {
    processx[i] = rand() % square;
    processy[i] = rand() % square;
    if(i<processnumber)
    {
      live[i] = 1;
      lived[i] = 1;
    }
  }
  
  for(i=0;i<trigger;i++)
  {
    totaledge = 0;
    for(k=0;k<processnumberupper;k++)
    {
      for(l=0;l<processnumberupper;l++)
      {
        if(((processx[k]-processx[l])*(processx[k]-processx[l])+(processy[k]-processy[l])*(processy[k]-processy[l])<range*range) && k!=l && live[k]==1 && live[l]==1)
        {
          topology[k][l] = 1;
          totaledge++;
        }
        else
        {
          topology[k][l] = 0;
        }
      }
    }

    temp = 0;
    totallived = 0;
    for(j=0;j<processnumberupper;j++)
    {
      //printf("%2d",live[j]);
      if(live[j]==1)
        temp++;
    }
    for(j=0;j<processnumberupper;j++)
    {
      if(lived[j]==1)
        totallived++;
    }    
    //printf("%3d\n",temp);
    //printf("totallived = %2d\n",totallived);
                        
    if(join==0)
    {
      live[jointag] = 1;
      lived[jointag] = 1;
      jointag++;
      if(jointag>=processnumberupper)
        jointag = 0;
      join = pd(100) + 1;
      join = join * times;
      messagesize = messagesize + 2*totaledge;
      messagesize = messagesize + 2*totaledge*5*(totallived-temp+1)*temp;
      messagenumber = messagenumber + 2*totaledge;
      messagenumber = messagenumber + 2*totaledge*(totallived-temp+1)*temp;
    }
    if(leave==0)
    {
      live[leavetag] = 0;
      if(jointag-leavetag>1)
        leavetag++;
      if(leavetag>=processnumberupper)
        leavetag = 0;
      leave = pd(100) + 1;
      leave = leave * times;
      messagesize = messagesize + 2*totaledge*2;
      messagenumber = messagenumber + 2*totaledge;
    }
    //printf("%2d%2d\n",join,leave);

    while(1)
    {
      current = rand()%processnumberupper;
      if(live[current]==1)
      {
        mytrigger[current]++;
        mythr[current]--;
        break;
      }
    }
    
    if(mythr[current]==0)
    {
      receivedtrigger = 0;
      for(z=0;z<processnumberupper;z++)
        receivedtrigger = receivedtrigger + mytrigger[z];
      thr = floor((trigger - receivedtrigger)/processnumberupper);
      if(thr>=processnumberupper)
      {
        for(z=0;z<processnumberupper;z++)
          mythr[z] = thr;
        messagesize = messagesize + 2*totaledge*3*temp;
        messagenumber = messagenumber + 2*totaledge*temp;
      }
      else
        for(z=0;z<processnumberupper;z++)
          mythr[z] = -1;
    }
    
    if(mythr[current]==-1)
    {
      messagesize = messagesize + 2*totaledge*2*(trigger-receivedtrigger);
      messagenumber = messagenumber + 2*totaledge*(trigger-receivedtrigger);
    }
    
    join--;
    leave--;
  }  
  
  printf("messagesize %d\n",messagesize);
  printf("messagenumber %d\n",messagenumber);
  for(i=0;i<processnumberupper;i++)
  {
    //printf("mytrigger[%2d]%3d\n",i,mytrigger[i]);
  }
  for(i=0;i<processnumberupper;i++)
  {
    for(j=0;j<processnumberupper;j++)
    {
      //printf("%2d",topology[i][j]);
    }
    //printf("\n");
  }
  
  fp1 = fopen("messagesize.txt", "a");
  fp2 = fopen("messagenumber.txt", "a");
    
  fprintf(fp1,"%d\n",messagesize);
  fprintf(fp2,"%d\n",messagenumber);
        
  fclose(fp1);
  fclose(fp2);

  //system("PAUSE");	
  return 0;
}

int pd(int lamda)
{  
  int k = 0;
  float p = 1, L, u;
  L= exp(-lamda);
  while(1)
  {
    k++;
    u = (float)(rand()%100)/100;
    p = p * u;
    if (p < L)
      break;
  }
  return(k-1);
}
