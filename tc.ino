#define clk 0
#define rclk 1
#define data 2
#define push 3
#define sw1 4
#define sw2 5

//player info
int p_score[3];
int p_turn;

//time info
int time;
int time_inc = 5;

//disp info

//debounce info
bool db;
int db_time;
int db_inc = 750;

//seven seg {a, b, c, d, e, f, g} 0-9, null, -
int sev_seg[12][7] = 
{   {1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0},
    {1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 1},
    {1, 0, 1, 1, 0, 1, 1},
    {0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1}};

void setup() 
{
    //set output pins
    pinMode(clk, OUTPUT);
    pinMode(rclk, OUTPUT);
    pinMode(data, OUTPUT);
    //set input pins
    pinMode(push, INPUT);
    pinMode(sw1, INPUT);
    pinMode(sw2, INPUT);
    
    //set scores to 0
    p_score[0] = 0;
    p_score[1] = 0;
    p_score[2] = 0;
    //set turns to 0 -> settings
    p_turn = 0;

    //set debounce to false
    db = false;
    db_time = millis();

    //set time
    time = millis();
}

void loop() 
{
    //update display every time_inc
    if(time_inc > millis() - time)
    {
        disp();
        time = millis();
    }
    //check push button, confirm db
    if(digitalRead(push) == HIGH && !db)
    {
        //switch turn of players
        db = true;
        db_time = millis();
    }
    //check rotary encoder

    //check win condition

    //check debounce
    if(db && db_inc < millis() - db_time)
    {
        db = false;
    }
}

void clock()
{
    digitalWrite(clk, LOW);
    digitalWrite(clk, HIGH);
}

void latch()
{
    digitalWrite(rlck, LOW);
    digitalWrite(rlck, HIGH);
}

void disp()
{
    //first segment select (6 bits)
    for(int i = 0; i < 6; i++)
    {
        //write relevant seg to display from
        clock();
    }
    //second, rgb led color (3 bits)
    for(int i = 0; i < 3; i++)
    {
        //write color
        clock();
    }
    //third, 7 seg (7 bits)
    for(int i = 6; i >= 0; i--)
    {
        //write relevant seven seg disp
        //digitalWrite(data, sev_seg[]);
        clock();
    }
    //flip rlck
    latch();
}