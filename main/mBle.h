#ifdef __cplusplus
extern "C"
{
#endif

	// all of your legacy C code here

	void mBle_init(void);
	void mBle_host_task(void *param);
	void mBle_hs_sync(void);
	void mBle_hs_reset(int reason);

#ifdef __cplusplus
}
#endif