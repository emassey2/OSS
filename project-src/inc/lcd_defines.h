
#include "effects.h"
#include "channel.h"

#define EVENT_OCCURED            0x07
#define BUTTON_ID                0x1F
#define OBJECT_ID                0x21  // pitches, vol, arpeggio, act_channel 

#define BASIC_SAW                0x00          // note isNoise = true
#define BASIC_TRIANGLE           0x01         // setcurrentchannel
#define BASIC_SQUARE             0x02
#define BASIC_NOISE              0x03
#define BASIC_SINE               0x04

#define ADV_ON_LSB               0x01
#define ADV_OFF_LSB              0x00

#define ADV_SQUARE2              0x05
#define ADV_SAW                  0x06
#define ADV_TRIANGLE             0x07
#define ADV_NOISE                0x08          // note isNoise = true
#define ADV_SINE                 0x09 
#define ADV_SQUARE1              0x0A

// ACT CHANNEL --------------------------------
// setcurrentchannel
#define ACTIVE_CHANNEL_SQUARE1   0x11 
#define ACTIVE_CHANNEL_SQUARE2   0x16   
#define ACTIVE_CHANNEL_SINE      0x1B
#define ACTIVE_CHANNEL_SAW       0x20        
#define ACTIVE_CHANNEL_TRI       0x25
#define ACTIVE_CHANNEL_NOISE     0x2A  


// PITCH --------------------------------------
#define PITCH_NO_EFFECT          0x3E             // pitch_enabled = false
#define PITCH_UP                 0x3F             // pitch_enabled = true
#define PITCH_DOWN               0x40             // pitch_enabled = true
#define PITCH_VIBRATO            0x41             // pitch_enabled = true
#define PITCH_VIBRATO_DELAY      0x42             // pitch_enabled = true

#define PITCH_NO_EFFECT_LSB      0x18
#define PITCH_UP_LSB             0x19
#define PITCH_DOWN_LSB           0x66
#define PITCH_VIBRATO_LSB        0x67
#define PITCH_VIBRATO_DELAY_LSB  0x64
    
// VOL -------------------------------------- 
#define VOL_NO_EFFECT            0x30             // vol_enabled = false
#define VOL_ADSR                 0x31             // vol_enabled = true
#define VOL_SHORT                0x32             // vol_enabled = true
#define VOL_LONG                 0x34             // vol_enabled = true
#define VOL_CRES                 0x35             // vol_enabled = true

#define VOL_NO_EFFECT_LSB        0x16
#define VOL_ADSR_LSB             0x17
#define VOL_SHORT_LSB            0x14
#define VOL_LONG_LSB             0x12
#define VOL_CRES_LSB             0x13

// ARP --------------------------------------
#define ARP_NO_EFF               0x39            // arp_enabled = false
#define ARP_SLOW                 0x3A            // arp_enabled = true
#define ARP_FAST                 0x3B            // arp_enabled = true
#define ARP_LOW                  0x3C            // arp_enabled = true
#define ARP_HIGH                 0x3D            // arp_enabled = true

#define ARP_NO_EFF_LSB           0x1F
#define ARP_SLOW_LSB             0x1C
#define ARP_FAST_LSB             0x1D
#define ARP_LOW_LSB              0x1A
#define ARP_HIGH_LSB             0x1B

void handleLCDinput(char objID, char lsbVal, Effects* myEffectsPtr);
void enteredBasicMode();
void enteredAdvancedMode();