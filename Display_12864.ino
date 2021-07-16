/*
版权声明：
深圳市小二极客科技（小R科技）旗下：WIFI机器人网·机器人创意工作室版权所有www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
小R科技保留付诸法律起诉侵权的权利！
* 文件名称：Display_12864
* 文件标识：
* 摘    要：WiFi机器人智能小车控制
* 描    述：12864显示驱动文件
* 当前版本：2560TH v2.5
* 作    者：BY WIFI机器人网·机器人创意工作室
* 完成日期：2017年6月
*/

/***********************************************************************************************************************************************************************************************/
                                                                /*12864液晶显示相关函数*/                                              



unsigned char show0[] = { 0xD0, 0xA1, 0xB6, 0xFE, 0xBF, 0xC6, 0xBC, 0xBC}; //小二科技
unsigned char show1[] = "wifi-robots";
unsigned char Normal_S[] = {
  0xA1, 0xF1,
  0xD5, 0xFD,
  0xB3, 0xA3,
  0xC4, 0xA3,
  0xCA, 0xBD
};                    //●正常模式
unsigned char Normal[]={
  0xD5, 0xFD,
  0xB3, 0xA3,
  0xC4, 0xA3,
  0xCA, 0xBD
     };                    //正常模式
unsigned char Follow_S[] = {
  0xA1, 0xF1,
  0xBA, 0xEC,
  0xCD, 0xE2,
  0xD1, 0xAD,
  0xBC, 0xA3
};                    //●红外循迹
unsigned char follow[] = {
  0xBA, 0xEC,
  0xCD, 0xE2,
  0xD1, 0xAD,
  0xBC, 0xA3
};                    //红外循迹
unsigned char Avoid_S[]={
  0xA1, 0xF1,
  0xBA, 0xEC,
  0xCD, 0xE2,
  0xB1, 0xDC,
  0xD5, 0xCF
};                    //●红外避障
unsigned char Avoid[]={
  0xBA, 0xEC,
  0xCD, 0xE2,
  0xB1, 0xDC,
  0xD5, 0xCF
};                    //红外避障
unsigned char WaveAvoid_S[] = {
  0xA1, 0xF1,
  0xB3, 0xAC,
  0xC9, 0xF9,
  0xB2, 0xA8,
  0xB1, 0xDA,
  0xD5, 0xCF
};                    //●超声波壁障
unsigned char WaveAvoid[] = {
  0xB3, 0xAC,
  0xC9, 0xF9,
  0xB2, 0xA8,
  0xB1, 0xDA,
  0xD5, 0xCF
};                    //超声波壁障

/*

/*************************************************************
                        外部中断2函数 Key1()
*************************************************************/
void Key1()
{
 if(Key1_times==0)
{
  Key1_times=7000;
  MENU(Level,Mode);MENU(Level,Mode);
  Refresh = 1;
  switch(Level)
  {
    case 0:Level = 1; Mode = 0;  MENU(Level,Mode);MENU(Level,Mode);return;
    case 1:Level = 2;
           switch(Mode)
           {
             case NORMAL:Cruising_Flag = 0; MENU(Level,Mode);MENU(Level,Mode);return;
             case FOLLOW:Cruising_Flag = 2; MENU(Level,Mode);MENU(Level,Mode);return;
             case AVOID: Cruising_Flag = 3; MENU(Level,Mode);MENU(Level,Mode);return;
             case WAVEAVOID:Cruising_Flag =4; MENU(Level,Mode);MENU(Level,Mode);return;
             default:Cruising_Flag = 0; MENU(Level,Mode);MENU(Level,Mode);return;
           }
             MENU(Level,Mode);MENU(Level,Mode);return;
    default:Level = 2;  MENU(Level,Mode);MENU(Level,Mode);return;
  }
 MENU(Level,Mode);MENU(Level,Mode);
 }
}
/*************************************************************
                        外部中断3函数 Key2()
*************************************************************/
void Key2()
{ 
if(Key2_times==0)
{
  Key2_times=7000;
  MENU(Level,Mode);MENU(Level,Mode);
  Refresh = 1;
  switch(Level)
  {
    case 0:  MENU(Level,Mode);MENU(Level,Mode);return;
    case 1:Mode++;if (Mode > 3)Mode = 0;  MENU(Level,Mode);MENU(Level,Mode);return;
    default:Level = 1;Cruising_Flag = 0;  MENU(Level,Mode);MENU(Level,Mode);return;
  }  
 MENU(Level,Mode);MENU(Level,Mode);
}
}
/*************************************************************
                       菜单选项显示
*************************************************************/

void MENU(int Level,int Mode)
{
  if(Refresh)    //刷新屏幕
  {
    Refresh = 0;
    LCDA.CLEAR();   
  }
  delay(10);//这个延时必须要
  switch(Level)
  {
    case 0:
         if(!Mode)
         {
           LCDA.DisplayString(0, 2, show0, SIZE(show0)); //第一行第三格开始，显示文字小二科技
           LCDA.DisplayString(2, 1, show1, SIZE(show1)); //第三行第二格开始，显示文字wifi-robots
         }
         return;
    case 1:
          switch(Mode)
          {
            case NORMAL:   LCDA.DisplayString(0, 0, Normal_S, SIZE(Normal_S));
                           LCDA.DisplayString(1, 1, follow, SIZE(follow));
                           LCDA.DisplayString(2, 1, Avoid, SIZE(Avoid));
                           LCDA.DisplayString(3, 1, WaveAvoid, SIZE(WaveAvoid));
                           return;                                                   //选择项为正常模式
            case FOLLOW:   LCDA.DisplayString(0, 1, Normal, SIZE(Normal));
                           LCDA.DisplayString(1, 0, Follow_S, SIZE(Follow_S));
                           LCDA.DisplayString(2, 1, Avoid, SIZE(Avoid));
                           LCDA.DisplayString(3, 1, WaveAvoid, SIZE(WaveAvoid)); 
                           return;                                                   //选择项为红外循迹模式
            case AVOID:    LCDA.DisplayString(0, 1, Normal, SIZE(Normal));
                           LCDA.DisplayString(1, 1, follow, SIZE(follow));
                           LCDA.DisplayString(2, 0, Avoid_S, SIZE(Avoid_S));
                           LCDA.DisplayString(3, 1, WaveAvoid, SIZE(WaveAvoid));
                           return;                                                   //选择项为红外壁障模式
            case WAVEAVOID:LCDA.DisplayString(0, 1, Normal, SIZE(Normal));
                           LCDA.DisplayString(1, 1, follow, SIZE(follow));
                           LCDA.DisplayString(2, 1, Avoid, SIZE(Avoid));
                           LCDA.DisplayString(3, 0, WaveAvoid_S, SIZE(WaveAvoid_S));
                           return;                                                   //选择项为超声波壁障模式
          }
          return;
    case 2:
          switch(Mode)
          {
            case NORMAL:LCDA.DisplayString(1, 2, Normal, SIZE(Normal));return;               //正常模式
            case FOLLOW:LCDA.DisplayString(1, 2, follow, SIZE(follow));return;               //红外循迹模式
            case AVOID: LCDA.DisplayString(1, 2, Avoid, SIZE(Avoid));return;                 //红外壁障模式
            case WAVEAVOID:LCDA.DisplayString(1, 1, WaveAvoid, SIZE(WaveAvoid));return;      //超声波壁障模式
          }
          return;
    default:return;
  }
}
                                                                    /*12864液晶显示相关函数 */                                           
/***********************************************************************************************************************************************************************************************/

