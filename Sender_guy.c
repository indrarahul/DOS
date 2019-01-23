#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

//FUNCTIONS
unsigned long long int factorial(unsigned int n);
void getMacAdd();
void prepare();
void snd_pckt();
int access_type();
void cook_well();
int release_type();
//FUNCTIONS


//VARIABLES
unsigned char src[6];
//unsigned char dst[6] = "\x08\x00\x27\x52\xbf\x0f";
unsigned char dst[6] = "\x08\x00\x27\x19\x44\x41";

unsigned char data[64];
unsigned char type[] = "\xff\xff";
unsigned char num[2];
unsigned char rcv[200];
unsigned char brd[] = "\xff\xff\xff\xff\xff\xff";
unsigned char bufff[50];
unsigned char c[2];
int j=0,cc;
int len_buf,lim=0;
unsigned char choice;
unsigned char new_buf[20];
unsigned char banana[10];
int i,handle,fact,rcv_len,y=0;
unsigned long long int n,k,x;
//VARIABLES


//RECIEVER
void interrupt receiver(bp, di, si, ds, es, dx, cx, bx, ax, ip, cs, flags)
unsigned short bp, di, si, ds, es, dx, cx, bx, ax, ip, cs, flags;
{

        if (ax == 0) {

                if(cx > 200)
                {
                        es=0;
                        di=0;
                        return;
                }

                es=FP_SEG(rcv);
                di=FP_OFF(rcv);
                rcv_len=cx;
        
        }

        else 
        {
                j=0;     
                if(memcmp(rcv,brd,6)==0)  // IF BROADCASTING THEN IGNORE
                        return;

                for(i=6;i<12;i++)
                        cprintf("%02x:",rcv[i]);
                cprintf("$ RECEIVED!! CALCULATING AND SENDING THE FACTORIAL OF :");

                for(i=14;i<rcv_len;i++)
                {
                        if(rcv[i]==0x00)
                                break;
                        putch(rcv[i]);
                        num[j] = rcv[i];
                        j++;
                }
                
                n = factorial(atoi(num));
                //printf("%d", n);
                sprintf(new_buf,"%lld",n);
                cook_well(new_buf, strlen(new_buf));
                snd_pckt();                
                //k=0;
                putch('\r');
                putch('\n');
                
                return;
        
        }

}
// RECIEVER


void main()
{
        printf("  <><><>  Game of Factorial  <><><>\n");
        getMacAdd();
        prepare();
        access_type();
        len_buf=0;        
        while(1){
                choice = getchar();
                if(choice==0x0A || choice==0x0D)
                {
                        if(lim>=1)
                        break;
                        if(len_buf==0)
                        lim++;
                        cook_well(bufff,len_buf,0);
                        len_buf=0;
                        snd_pckt();
                }
                else
                {
                        lim=0;
                        bufff[len_buf]=choice;
                        len_buf++;
                }       
        }
        release_type();
        printf("TATA !! See You Soon\n");     
        return;
}



unsigned long long int factorial(unsigned int n)
{
        if(n==0 || n==1)
        return 1;
        return n*factorial(n-1);
}

void cook_well(unsigned char *m, int leng)
{
        memcpy(data+14,m,leng);
        for(i=leng+14;i<64;i++)
                data[i] = 0;
}


void snd_pckt()
{
        union REGS in,out;
        struct SREGS s;
        segread(&s);
        in.h.ah = 4;
        in.x.cx = 64;   
        in.x.si = FP_OFF(data);
        s.ds = FP_SEG(data);
        int86x(0x60,&in,&out,&s);
}

void getMacAdd()
{
        union REGS in,out;
        struct SREGS s;
        char far *buf;
        in.h.ah=6;
        in.x.cx=6;
        buf = (char far *)src;
        in.x.di=FP_OFF(src);
        s.es=FP_SEG(buf);
        int86x(0x60,&in,&out,&s);    
}

void prepare()
{
        memcpy(data,dst,6);
        memcpy(data+6,src,6);
        memcpy(data+12,type,2);
}

int access_type()
{
        union REGS in,out;
        struct SREGS s;
        in.h.ah = 2;
        in.h.al = 1;
        in.x.bx =-1;
        in.h.dl = 0;
        in.x.cx = 0;
        s.es = FP_SEG(receiver);
        in.x.di = FP_OFF(receiver);
        in.x.si = FP_OFF(type);
        s.ds = FP_SEG(type);
        int86x(0x60,&in,&out,&s);
        handle = out.x.ax;
        return out.x.cflag;
}

int release_type()
{
        union REGS in,out;
        struct SREGS s;
        in.h.ah=3;
        in.x.bx=handle;
        int86x(0x60,&in,&out,&s);
        return 0; 
}
                