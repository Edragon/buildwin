//#ifdef _BBLM_SUPPORT_2M_3M_EDR_
#if 1
#define SBC_BANK_NUMBER		16
#define SBC_BANK_MASK		0x0F
#else
 #if 1 //def _AP_HEADSET_HANDSFREE_			//For Xdata size issue,reduce SBC Bank buffers if include headset,handsfree
	#define SBC_BANK_NUMBER		32
	#define SBC_BANK_MASK		0x1F
 #else
	#define SBC_BANK_NUMBER		32 //16 //ORG=32, reduced by arthur, 20120426
	#define SBC_BANK_MASK		0x1F //0x0F //ORG=0x1F
 #endif
#endif


#define MAXBITPOOL	53	//53		Should > 53 for PTS high quility test
#define MINBITPOOL	2
//#define SUGGESTMAXBITPOOL	MAXBITPOOL
#define SBCDATABANKSIZE	(MAXBITPOOL*2)//106	//SUGGESTMAXBITPOOL*2


//==========byte0 [1:0] Message type=====================
#define M_TYPE_COMMAND	0
#define M_TYPE_RFD		1
#define M_TYPE_ACCEPT	2
#define M_TYPE_REJECT	3

//==========byte0 [3:2] Packet type=====================
#define P_TYPE_SIGNAL		0
#define P_START_PACKET		1
#define P_CONTINUE_PACKET	2
#define P_END_PACKET		3

//===============byte1 signaling==================
#define S_ID_AVDTP_DISCOVER				0x01
#define S_ID_AVDTP_GET_CAPABILITIES		0x02
#define S_ID_AVDTP_SET_CONFIGURATION	0x03
#define S_ID_AVDTP_GET_CONFIGURATION	0x04
#define S_ID_AVDTP_RECONFIGURE			0x05
#define S_ID_AVDTP_OPEN					0x06
#define S_ID_AVDTP_START				0x07
#define S_ID_AVDTP_CLOSE				0x08
#define S_ID_AVDTP_SUSPEND				0x09
#define S_ID_AVDTP_ABORT				0x0A
#define S_ID_AVDTP_SECURITY_CONTROL		0x0B
//================================================================================

#define AVDTP_SIGNAL_CHANNEL	1
#define AVDTP_MEDIA_CHANNEL		2
#define AVDTP_REPORT_CHANNEL	3
#define AVDTP_RECOVERY_CHANNEL	4

//===========================ERROR Code====================================================
#define BAD_HEADER_FORMAT			0x01
#define BAD_LENGTH					0x11	
#define	BAD_ACP_SEID				0x12
#define	SEP_IN_USE					0x13
#define SEP_NOT_IN_USE				0x14
#define BAD_SERV_CATEGORY			0x17
#define BAD_PAYLOAD_FORMAT			0x18
#define	NOT_SUPPORTED_COMMAND		0x19
#define	INVALID_CAPABILITIES		0x1A
#define	BAD_RECOVERY_TYPE			0x22
#define	BAD_MEDIA_TRANSPORT_FORMAT	0x23
#define	BAD_RECOVERY_FORMAT			0x25
#define	BAD_ROHC_FORMAT				0x26
#define	BAD_CP_FORMAT				0x27
#define	BAD_MULTIPLEXING_FORMAT		0x28
#define	UNSUPPORTED_CONFIGURAION	0x29
#define	BAD_STATE					0X31
//===========================================================================================


#define ST_AVDTP_NEG_IDLE							0x00
#define ST_AVDTP_NEG_TX_DISCOVER_CMD				0x01
#define	ST_AVDTP_NEG_TX_START_CMD					0x02


union avdtp_media_header_byte0_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	CSRC:4,
			HeaderExtension:1,
			Padding:1,	
			RTP:2;
#else
		u8	RTP:2,
			Padding:1,
			HeaderExtension:1,	
			CSRC:4;
#endif
	}f;
} ;
union avdtp_media_header_byte1_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
                u8     	PayloadType:7,
			Mark:1;
#else
		u8	Mark:1,
			PayloadType:7;
#endif
	}f;
} ;

union avdtp_media_header_byte23_tag {
	u16 SequenceNo ;
	struct  {
#ifdef __IAR_SYSTEMS_ICC__
	u8  lo ;	// [ 7~0]
        u8  hi ;	// [15~8]
#else
	u8  hi ;	// [15~8]
	u8  lo ;	// [ 7~0]
#endif
	} byte ;
} ;

union avdtp_media_header_byte47_tag {
	u32 TimeStamp ;
	struct {
#ifdef __IAR_SYSTEMS_ICC__
	u8  lo ;	// [ 7~ 0]
	u8  f8 ;	// [15~ 8]
	u8  mm ;	// [23~16]
	u8  hi ;	// [31~24]
#else
	u8  hi ;	// [31~24]
	u8  mm ;	// [23~16]
	u8  f8 ;	// [15~ 8]
	u8  lo ;	// [ 7~ 0]
#endif
	} byte ;
};

union avdtp_media_header_byte811_tag {
	u32 SyncSource ;
	struct  {
#ifdef __IAR_SYSTEMS_ICC__
	u8  lo ;	// [ 7~ 0]
	u8  f8 ;	// [15~ 8]
	u8  mm ;	// [23~16]
	u8  hi ;	// [31~24]
#else
	u8  hi ;	// [31~24]
	u8  mm ;	// [23~16]
	u8  f8 ;	// [15~ 8]
	u8  lo ;	// [ 7~ 0]
#endif
	} byte ;
};

struct AVDTP_Media_Header_tag{
	union avdtp_media_header_byte0_tag 		byte0;
	union avdtp_media_header_byte0_tag 		byte1;
	union avdtp_media_header_byte23_tag 	byte23;
	union avdtp_media_header_byte47_tag 	byte47;
	union avdtp_media_header_byte811_tag 	byte811;
} ;


union A2DP_Media_Header_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	SBCNumber:4,
			z:1,
			Lastpacket:1,
			Startpacket:1,
			Fragmented:1;
#else
		u8	Fragmented:1,
			Startpacket:1,
			Lastpacket:1,
			z:1,
			SBCNumber:4;
#endif
	}f;
};


struct SBC_Frame_tag{
	u8 Length;
//	u8 FrameHeader[5];
//	u8 ScaleFactor[8];
	u8 SBCFrame[SBCDATABANKSIZE+13];
} ;

union avdtp_byte0_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	message01:2,
			packet23:2,
			label47:4;		//0:master, 1:slave  means the role of BT8101 in this connection
#else
		u8	label47:4,
			packet23:2,
			message01:2;		//0:master, 1:slave  means the role of BT8101 in this connection
#endif
	}f;
};

union avdtp_byte1_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	signaling05:6,
			zz:2;
#else
		u8	zz:2,
			signaling05:6;
#endif
	}f;
};

typedef struct  _AVDTP_Signal_tag{
	union avdtp_byte0_tag byte0;
	union avdtp_byte1_tag byte1;
}AVDTP_Signal_tag;

union capabilities_byte6_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	zzzz:4,
			MediaType:4;
#else
		u8	MediaType:4,
			zzzz:4;
#endif
	}f;
} ;

union capabilities_byte8_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	ChannelSupport:4,
			SamplingFreq:4;	
#else
		u8	SamplingFreq:4,
			ChannelSupport:4;	
#endif
	}f;
} ;
union capabilities_byte9_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	AllocationMethod:2,
			SubbandSupport:2,
			BlockLength:4;
#else
		u8	BlockLength:4,
			SubbandSupport:2,
			AllocationMethod:2;	
#endif
	}f;
} ;
typedef struct _AVDTP_Capabilities_RSP_tag{
	union avdtp_byte0_tag byte0;
	union avdtp_byte1_tag byte1;
	u8 byte2;	//Service Category:				0x01 Media Transport
	u8 byte3;	//Length of Service Capability:	0x00
	u8 byte4;	//Service Category:				0x07 Media Codec
	u8 byte5;	//Length of Service Capability:	0x06
	union capabilities_byte6_tag byte6;	//Media type: 0x00 Audio
	u8 byte7;	//Media codec type:				0x00 SBC
	union capabilities_byte8_tag byte8;
	union capabilities_byte9_tag byte9;
	u8 byte10;	//Minimum bitpool:				0x02
	u8 byte11;	//Maxmum bitpool:				0x20
}  AVDTP_Capabilities_RSP_tag;

union acpseid_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	zz:2,
			ACPSEID:6;
#else
		u8	ACPSEID:6,
			zz:2;
#endif
	}f;
} ;
#if 1

typedef struct _AVDTP_Header_tag{
	union avdtp_byte0_tag byte0;
	union avdtp_byte1_tag byte1;
	union acpseid_tag byte2;									//ACPSEID[7:2]
}  AVDTP_Header_tag;
#else
struct AVDTP_Header_tag{
	union avdtp_byte0_tag byte0;
	union avdtp_byte1_tag byte1;
	union acpseid_tag byte2;									//ACPSEID[7:2]
};
#endif


union intseid_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	zz:2,
			INTSEID:6;
#else
		u8	INTSEID:6,
			zz:2;
#endif
	}f;
} ;
typedef struct _AVDTP_SetConfig_REQ_tag{
	union avdtp_byte0_tag byte0;
	union avdtp_byte1_tag byte1;
	union acpseid_tag byte2;
	union intseid_tag byte3;
	u8 byte4;	//Service Category:				0x01 Media Transport
	u8 byte5;	//Length of Service Capability:	0x00
	u8 byte6;	//Service Category:				0x07 Media Codec
	u8 byte7;	//Length of Service Capability:	0x06
	union capabilities_byte6_tag byte8;	//Media type: 0x00 Audio
	u8 byte9;	//Media codec type:				0x00 SBC
	union capabilities_byte8_tag byte10;
	union capabilities_byte9_tag byte11;
	u8 byte12;	//Minimum bitpool:				0x02
	u8 byte13;	//Maxmum bitpool:				0x20
} AVDTP_SetConfig_REQ_tag;




union discover_byte2_tag{
	u8 param;
	struct {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	z:1,
			InUse:1,
			ACPSEID:6;
#else
		u8	ACPSEID:6,
			InUse:1,
			z:1;
#endif
	}f;
};
union discover_byte3_tag{
	u8 param;
	struct  {
		//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
		u8	zzz:3,
			TSEP:1,
			MediaType:4;		//0:master, 1:slave  means the role of BT8101 in this connection
#else
		u8	MediaType:4,
			TSEP:1,
			zzz:3;		//0:master, 1:slave  means the role of BT8101 in this connection
#endif
	}f;
} ;

typedef struct _AVDTP_Discover_RSP_tag{
	union avdtp_byte0_tag byte0;
	union avdtp_byte1_tag byte1;
	union discover_byte2_tag byte2;
	union discover_byte3_tag byte3;
} AVDTP_Discover_RSP_tag;

struct  AVDTP_Discover_REQ_tag{
	union avdtp_byte0_tag byte0;
	union avdtp_byte1_tag byte1;
};

struct avdtp_media_codec_config_tag{
	union {
		u8 param;
		struct {
			//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
			u8	zzzz:4,
				type:4;
#else
			u8	type:4,
				zzzz:4;
#endif
		}f;
	}  Byte0_Media_Type;

	u8 Byte1_Media_Codec_Type;

	union {
		u8 param;
		struct {
			//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
			u8	ChannelModeSupported:4,
				SampleFeqSupport:4;
#else
			u8	SampleFeqSupport:4,
				ChannelModeSupported:4;
#endif
		}f;

	}  Byte2_CodecInfo;

	union {
		u8 param;
		struct {
			//change bit order, arthur
#ifdef __IAR_SYSTEMS_ICC__
			u8	MethodSupported:2,
				SubbandSupported:2,
				BlockLengthSupport:4;
#else
			u8	BlockLengthSupport:4,
				SubbandSupported:2,
				MethodSupported:2;
#endif
		}f;
	}  Byte3_CodecInfo;

	u8 Byte4_CodecInfo_MinBitpool;
	u8 Byte5_CodecInfo_MaxBitpool;
} ;


__near_func u8 AVDTPReceiveData();
void NegotiationAVDTPSignalStart(void);
void NegotiationAVDTPSignalFinish(void);
