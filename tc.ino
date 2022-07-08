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

int a, b, last_a;

long int S_TIMER;

//buttons
#define BUT_L 8
#define BUT_M 9
#define BUT_R 10

enum color {RED, GREEN, BLUE};
enum color team;
int team_score[3];

//state machine
enum SM {WAIT, SCORE, NEXT};
enum SM state;

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
int count;

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

  last_a = digitalRead(PIN_A);

  //buttons
  pinMode(BUT_L, INPUT);
  pinMode(BUT_M, INPUT);
  pinMode(BUT_R, INPUT);

  timer = millis();
  count = 0;
  team = RED;
  team_score[0] = 0;
  team_score[1] = 0;
  team_score[2] = 0;

  state = WAIT;
}

void loop() 
{
  
  //wait
  while(state == WAIT)
  {
    //display all scores
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
        Serial.println("->");
        team_score[team]++;
      }
      else
      {
        //ccw
        Serial.println("<-");
        if(team_score[team] > 0)
        {
          team_score[team]--;
        }
      }
    }
    last_a = a;
    //check pin_s
    if(digitalRead(PIN_S) == HIGH && millis()- S_TIMER > 350)
    {
      Serial.println(team);
      //change to next team
      if(team == BLUE)
      {
        team = RED;
        fill_solid(leds, LED_NUM, CRGB(40, 0, 0));
        FastLED.show();
      }
      else if(team == RED)
      {
        team = GREEN;
        fill_solid(leds, LED_NUM, CRGB(0, 40, 0));
        FastLED.show();
      }
      else if(team == GREEN)
      {
        team = BLUE;
        fill_solid(leds, LED_NUM, CRGB(0, 0, 40));
        FastLED.show();
      }
      S_TIMER = millis();
    }
    //check but_l, but_m, but_r
    //check victory
  }
    

  //scoring
    //change relevant score
    //turn dial leds in relevant direction

  //change team
    //change to next team
    //change dial color to team

  //change players
    //change between 2 and 3 teams

  //undo
    //undo last action
}

void disp_score(color c_team)
{
  int score = team_score[c_team];
  if(c_team == RED)
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
  if(c_team == GREEN)
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
  if(c_team == BLUE)
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