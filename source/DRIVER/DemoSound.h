
PUBLIC void DemoSound_Init(void);
PUBLIC BOOL DemoSound_IsAvailable(void);
PUBLIC void DemoSound_Polling(void);

PUBLIC U8 DemoSound_Play(DriverMsg XDATA_PTR msgPtr);
PUBLIC U8 DemoSound_Pause(DriverMsg XDATA_PTR msgPtr);
PUBLIC U8 DemoSound_Stop(DriverMsg XDATA_PTR msgPtr);
PUBLIC U8 DemoSound_ChangeSongBackward(DriverMsg XDATA_PTR msgPtr);
PUBLIC U8 DemoSound_ChangeSongForward(DriverMsg XDATA_PTR msgPtr);