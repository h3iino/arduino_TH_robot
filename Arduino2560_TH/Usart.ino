/*
版权声明：
深圳市小二极客科技（小R科技）旗下：WIFI机器人网·机器人创意工作室版权所有www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
小R科技保留付诸法律起诉侵权的权利！
* 文件名称：Uart
* 文件标识：
* 摘    要：WiFi机器人智能小车控制
* 描    述：串口解析文件
* 当前版本：2560TH v2.5
* 作    者：BY WIFI机器人网·机器人创意工作室
* 完成日期：2017年6月
*/


#define BAUD 9600

/*
*********************************************************************************************************
** 函数名称 ：USART_init()
** 函数功能 ：串口初始化函数
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void USART_init()
{
  SREG = 0x80;                              //开启总中断
  //bitSet(UCSR0A,U2X0);
  bitSet(UCSR0B,RXCIE0);                    //允许接收完成中断// 
  bitSet(UCSR0B,RXEN0);                     //开启接收功能// 
  bitSet(UCSR0B,TXEN0);                     //开启发送功能// 
  bitSet(UCSR0C,UCSZ01);
  bitSet(UCSR0C,UCSZ00);                    //设置异步通信，无奇偶校验，1个终止位，8位数据
  UBRR0=(F_CPU/16/BAUD-1);                  //波特率9600
}
/*
*********************************************************************************************************
** 函数名称 ：put_char()
** 函数功能 ：发送数据帧
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/ 
static void put_char(unsigned int data)  
{  
    if (data == '/r')  
        put_char(0x09);  
    while ( !(UCSR0A & (1<<UDRE0)) )    // 不为空，等之  
        ;  
    UDR0 = data;  
}  

/*
*********************************************************************************************************
** 函数名称 ：myprintf()
** 函数功能 ：自定义串口打印函数
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/ 
static void myprintf(const char* fmt,...)  
{  
    const char* s;  
    int d;  
    char buf[16];  
    va_list ap;  
    va_start(ap,fmt);   // 将ap指向fmt（即可变参数的第一个?下一个？）  
    while (*fmt)  
    {  
        if (*fmt != '%')  
        {  
            put_char(*fmt++);   // 正常发送  
            continue;     
        }  
        switch (*++fmt) // next %  
        {  
        case 's':  
            s = va_arg(ap,const char*); // 将ap指向者转成char*型，并返回之  
            for (; *s; s++)  
                put_char(*s);  
            break;  
        case 'x':  
            d = va_arg(ap,int);     // 将ap指向者转成int型，并返回之  
            itoa(d,buf,16);         // 将整型d以16进制转到buf中  
            for (s = buf; *s; s++)  
                put_char(*s);  
            break;  
        case 'd':  
            d = va_arg(ap,int);  
            itoa(d,buf,10);         // 将整型d以10进制转到buf中  
            for (s = buf; *s; s++)  
                put_char(*s);  
            break;  
        default:  
            put_char(*fmt);  
            break;  
        }  
        fmt++;  
    }  
    va_end(ap);  
}

/*
*********************************************************************************************************
** 函数名称 ：Sendbyte()
** 函数功能 ：串口发送一个字节
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/  
void Sendbyte(char c)
{
  loop_until_bit_is_set(UCSR0A,UDRE0);
  UDR0=c;
}

/*
*********************************************************************************************************
** 函数名称 ：ISR()
** 函数功能 ：串口中断
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
//ISR(USART_RX_vect)                    //适合UNO
ISR(USART0_RX_vect)                     //适合MEGA 2560
{
  UCSR0B &= ~(1 << RXCIE0);         //关闭串口中断
  Get_uartdata(); 
  UCSR0B |=  (1 << RXCIE0);         //打开串口中断
}

/*
*********************************************************************************************************
** 函数名称 ：ISR()
** 函数功能 ：通过uart0发送字符串 ,并在结尾加上回车换行
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void SendString(char *str)
{
  while(*str)
  {
    Sendbyte(*str);
    str++;
  }
  Sendbyte(0x0d);
  Sendbyte(0x0a);  
}
/*
*********************************************************************************************************
** 函数名称 ：Get_uartdata()
** 函数功能 ：读取串口命令
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void Get_uartdata(void)
{
  ServoStatusLED=!ServoStatusLED;
   static int i;
    serial_data = UDR0;//读取串口
    if (rec_flag == 0)
    {
      if (serial_data == 0xff)//第一次获取到0xff(即包头)
      {
        rec_flag = 1;
        i = 0;
        Costtime = 0;
      }
    }
    else
    {
      if (serial_data == 0xff)//第二次获取到0xff(即包尾)
      {
        rec_flag = 0;
        if (i == 3)//获取到中间数据为3个字节，说明此命令格式正确
        {
          Communication_Decode();//执行命令解析函数
        }
        i = 0;
      }
      else
      {
        buffer[i] = serial_data;//暂存数据
        i++;
      }
    }
}
/*
*********************************************************************************************************
** 函数名称 ：UartTimeoutCheck()
** 函数功能 ：串口超时检测
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void UartTimeoutCheck(void)
{
  if (rec_flag == 1)
  {
    Costtime++;
    if (Costtime == 100000)
    {
      rec_flag = 0;
    }
  }
}

/*
*********************************************************************************************************
** 函数名称 ：Communication_Decode()
** 函数功能 ：串口命令解码
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void Communication_Decode()
{
  if (buffer[0] == 0x00)
  {
    MoterStatusLED=!MoterStatusLED;    //电机状态灯翻转
    digitalWrite(ledpin1,MoterStatusLED);
    analogWrite(ENB, Left_Speed_Hold);
    analogWrite(ENA,Right_Speed_Hold);
    switch (buffer[1])  //电机命令
    {
      case 0x01: MOTOR_GO_FORWARD;  return;
      case 0x02: MOTOR_GO_BACK;     return;
      case 0x03: MOTOR_GO_LEFT;     return;
      case 0x04: MOTOR_GO_RIGHT;    return;
      case 0x00: MOTOR_GO_STOP;     return;
    }
  }
  else if (buffer[0] == 0x01) //舵机命令
  {
    ServoStatusLED=!ServoStatusLED;   //舵机状态灯翻转
    digitalWrite(ledpin2,ServoStatusLED);
    if (buffer[2] > 170)return;
    switch (buffer[1])
    {
      case 0x01: angle1 = buffer[2]; servo1.write(angle1);  return;
      case 0x02: angle2 = buffer[2]; servo2.write(angle2);  return;
      case 0x03: angle3 = buffer[2]; servo3.write(angle3);  return;
      case 0x04: angle4 = buffer[2]; servo4.write(angle4);  return;
      case 0x05: angle5 = buffer[2]; servo5.write(angle5);  return;
      case 0x06: angle6 = buffer[2]; servo6.write(angle6);  return;
      case 0x07: angle7 = buffer[2]; servo7.write(angle7);  return;
      case 0x08: angle8 = buffer[2]; servo8.write(angle8);  return;
      default: return;
    }
  }

  else if (buffer[0] == 0x02) //调速
  {
    if (buffer[2] > 100)return;

    if (buffer[1] == 0x01) //左侧调档
    {
      Left_Speed_Hold=buffer[2]*2+55;//速度档位是0~100 换算成pwm 速度pwm低于55电机不转
      analogWrite(ENB, Left_Speed_Hold);
      EEPROM.write(0x09,Left_Speed_Hold);//存储速度
    }
    if (buffer[1] == 0x02) //右侧调档
    {
      Right_Speed_Hold=buffer[2]*2+55;//速度档位是0~100 换算成pwm 速度pwm低于55电机不转
      analogWrite(ENA,Right_Speed_Hold);
      EEPROM.write(0x0A,Right_Speed_Hold);//存储速度
    } else return;
  }
  else if (buffer[0] == 0x33) //读取舵机角度并赋值
  {
    Init_Steer(); return;
  }
  else if (buffer[0] == 0x32) //保存命令
  {
    EEPROM.write(0x01, angle1);
    EEPROM.write(0x02, angle2);
    EEPROM.write(0x03, angle3);
    EEPROM.write(0x04, angle4);
    EEPROM.write(0x05, angle5);
    EEPROM.write(0x06, angle6);
    EEPROM.write(0x07, angle7);
    EEPROM.write(0x08, angle8);
    return;
  }
  else if (buffer[0] == 0x13) //模式切换开关
  {
    switch (buffer[1])
    {
      case 0x01: Cruising_Flag = 1; 
      return;   //跟随模式
      case 0x02: Cruising_Flag = 2;
      LCDA.CLEAR();delay(10);LCDA.DisplayString(1, 2, follow, SIZE(follow));
      return;  //巡线模式
      case 0x03: Cruising_Flag = 3;
      LCDA.CLEAR();delay(10);LCDA.DisplayString(1, 2, Avoid, SIZE(Avoid));
      return;  //避障模式
      case 0x04: Cruising_Flag = 4;
      LCDA.CLEAR();delay(10);LCDA.DisplayString(1, 1, WaveAvoid, SIZE(WaveAvoid));
      return;  //雷达避障模式
      case 0x05: Cruising_Flag = 5;
      return;  //超声波距离PC端显示
      case 0x07: Cruising_Flag = 7;               //路径规划 
          analogWrite(ENA,140);
          analogWrite(ENB,140);
          return;
      case 0x00: Cruising_Flag = 0;
          LCDA.CLEAR();delay(10);LCDA.DisplayString(1, 2, Normal, SIZE(Normal));
          analogWrite(ENA,Left_Speed_Hold);
          analogWrite(ENB,Right_Speed_Hold);
      return;  //正常模式
      default: Cruising_Flag = 0;
      LCDA.CLEAR();delay(10);LCDA.DisplayString(1, 2, Normal, SIZE(Normal));
      return;  //正常模式
    }
  }
  else if (buffer[0] == 0x04)//开车灯指令为FF040000FF,关车灯指令为FF040100FF
  {
    switch (buffer[1])  
    {
      case 0x00: Close_Light(); return;  //关车灯 
      case 0x01: Open_Light(); return;   //开车灯
      default: return;
    }
  }
  else if (buffer[0] == 0x40) //存储电机标志
  {
    adjust = buffer[1];
    EEPROM.write(0x10, adjust);
  }
        else if(buffer[0]==0xA0)//收到右转
      {
         RevStatus = 2;
         TurnAngle=buffer[1];
         Golength=buffer[2];
      }
      else if(buffer[0]==0xA1)//收到左转
      {
         RevStatus = 1;
         TurnAngle=buffer[1];
         Golength=buffer[2];
      }
}
