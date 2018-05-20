void ExecuteDSP(void);
void ForceStopDSP(void);
void StopDSP(void);
void SSP_SFR_Write(u16 Addr,u16 Data);
u16	 SSP_SFR_Read(u16 Addr);
void initialDSPMidi(void);
void SSP_Midi_Load_To_DSP(void);
void SSP_Midi_Play(u8 sel);
bool SSP_Midi_Check_Stop(void);
bool Check_DSP_Exist(void);
//void PlayTone_PowerOn(void){};
//void PlayTone_PowerDown(void){};
//void PlayTone_VolumeUp(void){};
//void PlayTone_VolumeDown(void){};
//void PlayTone_Backward(void){};
//void PlayTone_Forward(void){};	
//void PlayTone_PowerStop(void){};
//void PlayTone_PauseStart(void){};
//void PlayTone_MaxVolume(void){};
//void UpSampling(void){};
//void DownSampling(void){};


//================DSP Control Register SFR==================
#define DSP_ADDRESS_VOL			0x09A5
#define DSP_ADDRESS_MIDI_SEL	0x08F7
#define DSP_ADDRESS_MIDI_PLAY	0x09A7
#define DSP_ADDRESS_1K_SIN		0x09A7
#define DSP_ADDRESS_MIDI_MEM	0x0D00
//==========================================================
#define MIN_VOLUME			0x3F
#define VOLUME_LV			0x04
