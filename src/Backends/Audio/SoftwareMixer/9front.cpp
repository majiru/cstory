// Released under the MIT licence.
// See LICENCE.txt for details.

#include "Backend.h"

#include <stddef.h>
#include <string.h>

#include "../../Misc.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void (*parent_callback)(long *stream, size_t frames_total);

static void Callback(u8int *stream_uint8, int len)
{
	long mix_buffer[0x800 * 2];
	short *stream = (short*)stream_uint8;
	const size_t frames_total = len / sizeof(short) / 2;

	size_t frames_done = 0;

	while (frames_done != frames_total)
	{

		size_t subframes = MIN(0x800, frames_total - frames_done);

		memset(mix_buffer, 0, subframes * sizeof(long) * 2);

		parent_callback(mix_buffer, subframes);

		for (size_t i = 0; i < subframes * 2; ++i)
		{
			if (mix_buffer[i] > 0x7FFF)
				*stream++ = 0x7FFF;
			else if (mix_buffer[i] < -0x7FFF)
				*stream++ = -0x7FFF;
			else
				*stream++ = mix_buffer[i];
		}

		frames_done += subframes;
	}
}

static uchar audiobuf[8192];
static Lock mixer, audio;

static void SoftwareMixerBackend_Proc(void*)
{
	int fd;

	threadsetname("audioproc");
	fd = open("/dev/audio", OWRITE);
	if(fd < 0)
		return;
	for(;;){
		Callback(audiobuf, sizeof audiobuf);
		write(fd, audiobuf, sizeof audiobuf);
	}
}

unsigned long SoftwareMixerBackend_Init(void (*callback)(long *stream, size_t frames_total))
{
	parent_callback = callback;
	proccreate(SoftwareMixerBackend_Proc, nil, 8192*1024);
	return 44100;
}

void SoftwareMixerBackend_Deinit(void)
{

}

int SoftwareMixerBackend_Start(void)
{
	return 1;
}

void SoftwareMixerBackend_LockMixerMutex(void)
{
	lock(&mixer);
}

void SoftwareMixerBackend_UnlockMixerMutex(void)
{
	unlock(&mixer);
}

void SoftwareMixerBackend_LockOrganyaMutex(void)
{
	lock(&audio);
}

void SoftwareMixerBackend_UnlockOrganyaMutex(void)
{
	unlock(&audio);
}
