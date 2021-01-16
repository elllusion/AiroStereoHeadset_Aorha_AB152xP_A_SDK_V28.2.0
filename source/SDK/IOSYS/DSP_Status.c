#include "..\include\DSP_Status.h"

extern DSP_PLAYING_STATUS DSP_Playing_Status(void);

DSP_PLAYING_STATUS SDK_DSP_Playing_Status()
{
	return DSP_Playing_Status();
}
