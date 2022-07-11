#include <FastLED.h>

//7 seg
#define sr_data 4
#define sr_clk  3
#define sr_lat  2

//leds
#define LED_NUM 12
#define LED_PIN 11

CRGB leds[LED_NUM];

//rotary enc
#define PIN_A 7
#define PIN_B 6
#define PIN_S 5

int dial_brightness[6] = {40, 20, 10, 5, 2, 1};
int dial_index;

int a, b, last_a;

long int S_TIMER;

//buttons
#define BUT_L 8
#define BUT_M 9
#define BUT_R 10

long int B_TIMER;

enum color {RED, GREEN, BLUE, NONE};
enum color team;
enum color dont_disp;
enum color dont_disp_last;

//++color
color& operator++(color& orig)
{
  orig = static_cast<color>(orig + 1);
  if(orig == NONE) orig = RED;
  return orig;
}
//color++
color operator++(color& orig, int)
{
  color rVal = orig;
  ++orig;
  return rVal;
}


int prev_score[3];
int team_score[3];

enum dir {CW, CCW};
enum dir cur_dir;
int count;
long int led_timer;
bool led_reset;

byte disp_seg[11] = {
  0b00010000,
  0b01010111,
  0b01100000,
  0b01000001,
  0b00000111,
  0b00001001,
  0b00001000,
  0b01010011,
  0b00000000,
  0b00000011,
  0b01111111
};

byte disp_index[10] = {
  0b00000000, //0
  0b10000000, //1
  0b01000000, //2
  0b00000000, //3
  0b00000100, //4
  0b00000010, //5
  0b00000001, //6
  0b00100000, //7
  0b00010000, //8
  0b00001000, //9
};

int timer;

void setup() {
  Serial.begin(9600);

  //7 seg sr
  pinMode(sr_data, OUTPUT);
  pinMode(sr_clk, OUTPUT);
  pinMode(sr_lat, OUTPUT);

  //leds
  FastLED.addLeds<SK6812, LED_PIN, GRB>(leds, LED_NUM);
  fill_solid(leds, LED_NUM, CRGB(40, 0, 0));
  FastLED.show();
  
  //rotary enc
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_S, INPUT_PULLUP);

  S_TIMER = millis();
  B_TIMER = millis();
  
  last_a = digitalRead(PIN_A);

  //buttons
  pinMode(BUT_L, INPUT);
  pinMode(BUT_M, INPUT);
  pinMode(BUT_R, INPUT);

  timer = millis();
  count = 0;
  led_timer = millis();
  led_reset = false;
  
  team = RED;
  dont_disp = NONE;
  dont_disp_last = NONE;
    
  prev_score[0] = 0;
  prev_score[1] = 0;
  prev_score[2] = 0;
  
  team_score[0] = 0;
  team_score[1] = 0;
  team_score[2] = 0;

  dial_index = 0;

  Serial.println("Setup Complete");
}

void loop() 
{
  //disp scores
  disp_score(GREEN); 
  disp_score(RED); 
  disp_score(GREEN); 
  disp_score(BLUE); 
  disp_score(GREEN);
  
  //check pin_a vs last_a
  if(((a = digitalRead(PIN_A)) != last_a) && (a == 1))
  {
    if((b = digitalRead(PIN_B)) != a)
    {
      //cw
      cur_dir = CW;
      if(team_score[team] < 121)
      {
        team_score[team]++;
        cycle_leds(++count);
        led_timer = millis();
        led_reset = true;
      }
    }
    else
    {
      //ccw
      cur_dir = CCW;
      if(team_score[team] > 0)
      {
        team_score[team]--;
        cycle_leds(--count);
        led_timer = millis();
        led_reset = true;
      }
    }
  }
  last_a = a;

  //check dial timer
  if(led_reset && millis() - led_timer > 1000)
  {
    //Serial.println("Reset Leds!");
    reset_leds();
    led_reset = false;
  }
  //next team
  if(digitalRead(PIN_S) == HIGH && millis()- S_TIMER > 350)
  {
    //change to next team
    next_team();
    S_TIMER = millis();
    prev_score[team] = team_score[team];
  } 
  
  //undo (right button)
  if(digitalRead(BUT_R) == HIGH && (millis() - B_TIMER > 500))
  {
    int temp = team_score[team];
    team_score[team] = prev_score[team];
    prev_score[team] = temp; 

    B_TIMER = millis();
  }
  if(digitalRead(BUT_L) == HIGH && (millis() - B_TIMER > 333))
  {
    //if a team is not being displayed make it be displayed
    if(dont_disp != NONE)
    {
      Serial.println("3 Player Mode Activated");
      dont_disp_last = dont_disp;
      dont_disp = NONE; 
    }
    //if all teams are 0
    else
    {
        Serial.println(dont_disp_last);
        Serial.print("2 Player: No ");
        if((dont_disp_last == BLUE || dont_disp_last == NONE) && team_score[RED] == 0)
        {
          dont_disp = RED;
          Serial.println("RED");
        }
        else if((dont_disp_last == RED || dont_disp_last == NONE) && team_score[GREEN] == 0)
        {
          dont_disp = GREEN;
          dont_disp_last = NONE;
          Serial.println("GREEN");
        }
        else if((dont_disp_last == GREEN || dont_disp_last == NONE) && team_score[BLUE] == 0)
        {
          dont_disp = BLUE;
          Serial.println("BLUE");
        }
        else
        {
          Serial.println("Fell Out");
        }
        dont_disp_last = NONE;
        next_team();
    }
    B_TIMER = millis();
  }
  

  //check victory
}

void next_team()
{
  team++;
  if(team == NONE)
  {
    team = RED;
  }
  if(team == dont_disp)
  {
    next_team();
  }
  reset_leds();
}
void cycle_leds(int index)
{
  int zero = 0;
  int value = 60;
  int* r_value = (team == RED ? &value : &zero);
  int* g_value = (team == GREEN ? &value : &zero);
  int* b_value = (team == BLUE ? &value : &zero);
  if(cur_dir == CW)
  {
    for(int i = 0; i < LED_NUM; i++)
    {
        leds[((i+index)%LED_NUM)] = CRGB(*r_value, *g_value, *b_value);
        value /= 1.3; 
    }
  }
  else //if CCW
  {
    for(int i = LED_NUM; i > 0; i--)
    {
        leds[((i+index)%LED_NUM)] = CRGB(*r_value, *g_value, *b_value);
        value /= 1.3;  
    }
  }
  FastLED.show();
}
void reset_leds()
{
  int zero = 0;
  int value = 30;
  int* r_value = (team == RED ? &value : &zero);
  int* g_value = (team == GREEN ? &value : &zero);
  int* b_value = (team == BLUE ? &value : &zero);
  
  for(int i = 0; i < LED_NUM; i++)
  {
    leds[i] = CRGB(*r_value, *g_value, *b_value);
  }
  FastLED.show();
}
void disp_score(color c_team)
{
  int score = team_score[c_team];
  if(c_team == RED && dont_disp != RED)
  {
    //disp 1's place
    disp_num(score%10, 3);
    score -= score%10;
    if(score / 10 >= 1)
    {
      //disp 10's place
      disp_num((score%100)/10, 2);
      score -= score%100;
    }
    if(score / 100 >= 1)
    {
      //disp 100's place
      disp_num(1, 1);
    }
  }
  if(c_team == GREEN && dont_disp != GREEN)
  {
    //disp 1's place
    disp_num(score%10, 6);
    score -= score%10;
    if(score / 10 >= 1)
    {
      //disp 10's place
      disp_num((score%100)/10, 5);
      score -= score%100;
    }
    if(score / 100 >= 1)
    {
      //disp 100's place
      disp_num(1, 4);
    }
  }
  if(c_team == BLUE && dont_disp != BLUE)
  {
    //disp 1's place
    disp_num(score%10, 9);
    score -= score%10;
    if(score / 10 >= 1)
    {
      //disp 10's place
      disp_num((score%100)/10, 8);
      score -= score%100;
    }
    if(score / 100 >= 1)
    {
      //disp 100's place
      disp_num(1, 7);
    }
  }
}
void disp_num(int num, int index)
{
  byte seg = disp_seg[num];
  if(index == 3)
  {
    seg |= 0b10000000;
  }
  digitalWrite(sr_lat, LOW);
  shiftOut(sr_data, sr_clk, MSBFIRST, disp_index[index]);
  shiftOut(sr_data, sr_clk, LSBFIRST, seg);
  digitalWrite(sr_lat, HIGH);
}