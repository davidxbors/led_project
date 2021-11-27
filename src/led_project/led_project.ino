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


/* rainbow cycle along the strip */
/* _delay -> delay time between frames */
void rainbow (int _delay) 
{
  /* Hue of first pixel runs 5 complete loops through the color wheel. */
  /* Color wheel has a range of 65536 but it's OK if we roll over, so */
  /* just count from 0 to 5*65536. Adding 256 to firstPixelHue each time */
  /* means we'll make 5*65536/256 = 1280 passes through this outer loop: */
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      /* Offset pixel hue by an amount to make one full revolution of the */
      /* color wheel (range of 65536) along the length of the strip */
      /* (strip.numPixels() steps): */
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      /* strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or */
      /* optionally add saturation and value (brightness) (each 0 to 255). */
      /* Here we're using just the single-argument hue variant. The result */
      /* is passed through strip.gamma32() to provide 'truer' colors */
      /* before assigning to each pixel: */
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); 
    delay(_delay); 
  }
}

void _christmas_1 (void)
{
	strip.clear();
	/* initial wiping of the strip */
	uint32_t colors[3] = {strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(255, 255, 255)};
	for (int i = 0; i < strip.numPixels(); i += _step) {
		strip.setPixelColor(i, colors[i % 3]);
		strip.show();
		delay(50);				
	}
	/* pulsing the lights */
	for (int i = .05; i < .25; i += .05) {
		strip.setBrightness(i * 255);
		strip.show();
		delay(25);	
	}
	for (int i = .25; i >= .05; i -= .05) {
		strip.setBrightness(i * 255);
		strip.show();
		delay(25);
	}
	for (int i = .05; i < (brightness / 255); i += .05) {
		strip.setBrightness(i * 255);
		strip.show();
		delay(25);
	}
	/* theather chase effect */
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 3; j++) {
			strip.clear();
			for (int k = j; k < strip.numPixels(); k += 3) {
				strip.setPixelColor(k, colors[k % 3]);
			}
			strip.show();
			delay(50);
		}
	}
	/* end with another color wipe */
	strip.clear();
	for (int i = 0; i < strip.numPixels(); i += _step) {
		strip.setPixelColor(i, colors[i % 3]);
		strip.show();
		delay(50);				
  	}
}

/* second christmas effect; basically a slow theather chase */
void _christmas_2 (void)
{
	strip.clear();
	uint32_t colors[3] = {strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(255, 255, 255)};
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 3; j++) {
			strip.clear();
			for (int k = j; k < strip.numPixels(); k += 3) {
				strip.setPixelColor(k, colors[k % 3]);
			}
			strip.show();
			/* TODO check if wait time is enough */
			delay(500);
		}
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
			/* red */
			curr_color = strip.Color(255, 0, 0);
			light_strip(curr_color, 50);
			break;
		case oneh:
			/* green */
			curr_color = strip.Color(0, 255, 0);
			light_strip(curr_color, 50);
			break;
		case twoh:
			/* blue */
			curr_color = strip.Color(0, 0, 255);
			light_strip(curr_color, 50);
			break;
		case one:
			/* effect 1 -> rainbow */
			rainbow(10);
			break;
		case two:
			/* effect 2 -> christmas 1 */
			_christmas_1();
			break;
		case three:
			/* effect 3 -> christmas 2 */
			_christmas_2();
			break;
		/* TODO find usage for these buttons too */
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
