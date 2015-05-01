

#include <stdbool.h>
#include "inc/lcd_defines.h"
#include "inc/UART.h"
//#include "inc/channel.h"

extern Channel* ch0;
extern Channel* ch1;
extern Channel* ch2;
extern Channel* ch3;
extern Channel* ch4;
extern Channel* ch5;

extern Effect* noArpeg;
extern Effect* slowArpeg;
extern Effect* fastArpeg;
extern Effect* octLow;
extern Effect* octHigh;

extern Effect* noPitch;
extern Effect* pitchUp;
extern Effect* pitchDown;
extern Effect* vibrato;
extern Effect* delayedVibrato;

extern Effect* noVolume;
extern Effect* ADSR;
extern Effect* shortNote;
extern Effect* longNote;
extern Effect* crescendo;


bool ch0_on, ch1_on, ch2_on, ch3_on, ch4_on, ch5_on;


extern void setCurrentChannel(Channel* newCurrentCh);
//extern void initChannels(void);

void handleLCDinput(char objID, char lsbVal, Effects* myEffectsPtr){
	
	char dataIn[100];
	
	//Effects* myEffectsPtr;
	
	//myEffectsPtr = channel->note->effects;
	sprintf(dataIn, "\n\r objId: %d, lsbVal: %d", objID, lsbVal);
	UART0_TxPoll(dataIn);

	if (objID < 0x05) { // basic mode
		
		enteredBasicMode();
		
		switch(objID){
			case BASIC_NOISE:
				ch3->mute = false;
				setCurrentChannel(ch3);
				break;
			case BASIC_SAW:
				ch2->mute = false;
				setCurrentChannel(ch2);
				break;
			case BASIC_TRIANGLE:
				ch1->mute = false;
				setCurrentChannel(ch1);
				break;
			case BASIC_SQUARE:
				ch0->mute = false;
				setCurrentChannel(ch0);
				break;
				//initChannel(&ch5, &wave5, square12); 
				//setCurrentChannel(ch5);
			case BASIC_SINE:
				ch5->mute = false;
				setCurrentChannel(ch5);
				break;
		} // switch
	} 
	else if (objID < 0x0B) { //advanced waveform selected (green pipes)
		
		enteredAdvancedMode();
		
		switch(objID){
			case ADV_SQUARE2:
				if(lsbVal){
					ch0->mute = false;
				} else {
					ch0->mute = true;
				}
				break;
			case ADV_SAW:
			 if(lsbVal){
					ch2->mute = false;
				} else {
					ch2->mute = true;
				}
				break;
			case ADV_TRIANGLE:
			 if(lsbVal){
					ch1->mute = false;
				} else {
					ch1->mute = true;
				}
				break;
			case ADV_NOISE:
			 if(lsbVal){
					ch3->mute = false;
				} else {
					ch3->mute = true;
				}
				break;
			case ADV_SINE:
			 if(lsbVal){
					ch5->mute = false;
				} else {
					ch5->mute = true;
				}
				break;
			case ADV_SQUARE1:
			 if(lsbVal){
					ch4->mute = false;
				} else {
					ch4->mute = true;
				}
				break;
		} // switch	
	}
	
	else if (objID < 0x2B) { // active channel: 0x11, 0x16, 0x1B, 0x20, 0x25, 0x2A 
		switch(objID){
			case ACTIVE_CHANNEL_SQUARE1:
				setCurrentChannel(ch4);
				break;
			case ACTIVE_CHANNEL_SQUARE2:
				setCurrentChannel(ch0);
				break;
		  case ACTIVE_CHANNEL_SINE:
				setCurrentChannel(ch5);
				break;
		  case ACTIVE_CHANNEL_SAW:
				setCurrentChannel(ch2);
				break;
		  case ACTIVE_CHANNEL_TRI:
				setCurrentChannel(ch1);
				break;
		  case ACTIVE_CHANNEL_NOISE:
				setCurrentChannel(ch3);
				break;
		    //default
		}
	}
	
	else if (objID < 0x36) { // 0x30 - 0x35 = vol
		switch(objID){
			case VOL_NO_EFFECT:
				setEffect(VOLUME, myEffectsPtr , noVolume);
				break;
			case VOL_ADSR:
				setEffect(VOLUME, myEffectsPtr , ADSR);
				break;
			case VOL_SHORT:
				setEffect(VOLUME, myEffectsPtr , shortNote);
				break;
			case VOL_LONG:
				setEffect(VOLUME, myEffectsPtr , longNote);
				break;
			case VOL_CRES:
				setEffect(VOLUME, myEffectsPtr , crescendo);
				break;
			//default
		}

	}
	
	else if (objID < 0x3E) { // 0x39 - 0x3D = arpeggio
		switch(objID){
			case ARP_NO_EFF:
				setEffect(ARPEGGIO, myEffectsPtr , noArpeg);
				break;
			case ARP_SLOW:
				setEffect(ARPEGGIO, myEffectsPtr , slowArpeg);
				break;
			case ARP_FAST:
				setEffect(ARPEGGIO, myEffectsPtr , fastArpeg);
				break;
			case ARP_LOW:
				setEffect(ARPEGGIO, myEffectsPtr , octLow);
				break;
			case ARP_HIGH:
				setEffect(ARPEGGIO, myEffectsPtr , octHigh);
				break;
		}
	}
	
	else if (objID < 0x43) { // 0x3E - 0x42 = pitch
		
		switch(objID){
			case PITCH_NO_EFFECT:
				setEffect(PITCH, myEffectsPtr , noPitch);
				break;       
			case PITCH_UP:   
				setEffect(PITCH, myEffectsPtr , pitchUp);
				break;
			case PITCH_DOWN:  
				setEffect(PITCH, myEffectsPtr , pitchDown);
				break;          
			case PITCH_VIBRATO:
				setEffect(PITCH, myEffectsPtr , vibrato);
				break;  
			case PITCH_VIBRATO_DELAY:
				setEffect(PITCH, myEffectsPtr , delayedVibrato);
				break;
			//default
		}
	}
	
	else{
	 // do nothing
	}

}


void enteredBasicMode(){
	
	//save off state
	ch0_on = ch0->mute;
	ch1_on = ch1->mute;
	ch2_on = ch2->mute;
	ch3_on = ch3->mute;
	ch4_on = ch4->mute;
	ch5_on = ch5->mute;
	
	// mute all channels
	ch0->mute = false;
	ch1->mute = true;
	ch2->mute = true;
	ch3->mute = true;
	ch4->mute = true;
	ch5->mute = true;

	ch0->note->effects->arpeggio = noArpeg;
	ch1->note->effects->arpeggio = noArpeg;
	ch2->note->effects->arpeggio = noArpeg;
	ch3->note->effects->arpeggio = noArpeg;
	ch4->note->effects->arpeggio = noArpeg;
	ch5->note->effects->arpeggio = noArpeg;
	
	ch0->note->effects->pitch = noPitch;
	ch1->note->effects->pitch = noPitch;
	ch2->note->effects->pitch = noPitch;
	ch3->note->effects->pitch = noPitch;
	ch4->note->effects->pitch = noPitch;
	ch5->note->effects->pitch = noPitch;
	
	ch0->note->effects->volume = noVolume;
	ch1->note->effects->volume = noVolume;
	ch2->note->effects->volume = noVolume;
	ch3->note->effects->volume = noVolume;
	ch4->note->effects->volume = noVolume;
	ch5->note->effects->volume = noVolume;
	
	setCurrentChannel(ch0);
	
}


void enteredAdvancedMode(){
	
	// turn the channels that are currently on back on
	ch0->mute = ch0_on;
	ch1->mute = ch1_on;
	ch2->mute = ch2_on;
	ch3->mute = ch3_on;
	ch4->mute = ch4_on;
	ch5->mute = ch5_on;
	
	ch0->note->effects->enabled = true;
	ch1->note->effects->enabled = true;
	ch2->note->effects->enabled = true;
	ch3->note->effects->enabled = true;
	ch4->note->effects->enabled = true;
	ch5->note->effects->enabled = true;
	
	
}