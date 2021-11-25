/*
 * Author: Bors David <daviddvd267@gmail.com>
 */

#include <IRremote.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

/* remote mapping */
#define CHm 		0xBA45FF00
#define CH 		0xB946FF00
#define CHp 		0xB847FF00
#define back 		0xBB44FF00
#define forward 	0xBF40FF00
#define playpause 	0xBC43FF00
#define minus 		0xF807FF00
#define plus 		0xEA15FF00
#define eq 		0xF609FF00
#define zero 		0xE916FF00
#define oneh 		0xE619FF00
#define twoh 		0xF20DFF00
#define one 		0xF30CFF00
#define two 		0xE718FF00
#define three 		0xA15EFF00
#define four 		0xF708FF00
#define five 		0xE31CFF00
#define six 		0xA55AFF00
#define seven 		0xBD42FF00
#define eight 		0xAD52FF00
#define nine 		0xB54AFF00

/* led constants */
#define LED_COUNT 144

// pin connected to the data line from the receiver
const int receiver_pin = 11;
/* pin connected to the data line from the led strip */
const int led_pin = 9;

decode_results results;
// the protocol my remote uses
// in ./basics/basic_ir_test you will find a test
// that should also tell you what protocol your remote has
decode_type_t remote_protocol = NEC;

/* default brightness */
char brightness = (int)(.2 * 255);
/* default step through leds */
int _step = 2;
/* strip state */
bool state = 0;

/* strip object */
Adafruit_NeoPixel strip(LED_COUNT, led_pin, NEO_GRB + NEO_KHZ800);
/* default color */
uint32_t curr_color = strip.Color(255, 0, 0);

/* 
 * min brightness allowed: 0%
 * dims the brightness with 5% and keeps it over 0%
 */
void dim_bright ()
{
	if (brightness > (int)(.05 * 255))
		brightness -= (int)(.05 * 255);	
	else if (brightness <= (int)(.05 * 255))
		brightness = 0;
	strip.setBrightness(brightness);
  strip.show();
}

/*
 * max brightness allowed: 25% (with 72 leds)
 * increases the brightness with 5%, and keeps it under 25%
 */
void inc_bright ()
{
	
	if ((brightness <= (int)(.20 * 255)))
		brightness += (int)(.05 * 255);	
	strip.setBrightness(brightness);
  strip.show();
}

/*
 * cycle through lighting mode 1/2 || 1/4 || 1/8
 */
void cycle_step ()
{
	if (_step == 8)
		_step = 2;
	else _step <<= 1;
        strip.clear();
	light_strip(curr_color, 50);
}

/*
 * light the strip in a given color 
 * it will play a color wipe animation
 * with a given delay too
 */
void light_strip (uint32_t color, int __delay)
{
	for (int i = 0; i < strip.numPixels(); i += _step) {
		strip.setPixelColor(i, color);
		strip.show();
		delay(__delay);
	}
}

/* basic arduino functions */
void setup () 
{
	Serial.begin(9600);
	IrReceiver.begin(receiver_pin, ENABLE_LED_FEEDBACK);
	strip.begin();
	strip.setBrightness(brightness);
	Serial.println("started");
}

void loop ()
{
	if (IrReceiver.decode()) {
		/* ignore noise, only account for protocol transmissions */
		if (IrReceiver.decodedIRData.protocol != remote_protocol)
			goto resume;
		/* check data received from the remote */
		switch (IrReceiver.decodedIRData.decodedRawData) {
		case CHm:
			dim_bright();
			break;
		case CH:
      			Serial.println(state);
      			if (state == 0)
				light_strip(curr_color, 50);
      			else strip.clear(), strip.show();
      			state ^= 1;
			break;
		case CHp:
			inc_bright();
			break;
		case back:
			cycle_step();
			break;
		case forward:
			Serial.println("forward");
			break;
		case playpause:
			Serial.println("playpause");
			break;
		case minus:
			Serial.println("minus");
			break;
		case plus:
			Serial.println("plus");
			break;
		case eq:
			Serial.println("eq");
			break;
		case zero:
			Serial.println("zero");
			break;
		case oneh:
			Serial.println("oneh");
			break;
		case twoh:
			Serial.println("twoh");
			break;
		case one:
			Serial.println("one");
			break;
		case two:
			Serial.println("two");
			break;
		case three:
			Serial.println("three");
			break;
		case four:
			Serial.println("four");
			break;
		case five:
			Serial.println("five");
			break;
		case six:
			Serial.println("six");
			break;
		case seven:
			Serial.println("seven");
			break;
		case eight:
			Serial.println("eight");
			break;
		case nine:
			Serial.println("nine");
			break;
		}		
		/* enable receiving of the next value */
		resume:
		IrReceiver.resume();
	}	
	delay(300);
}
