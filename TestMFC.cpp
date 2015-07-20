#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <linux/videodev2.h>
#include <poll.h>
#include <unistd.h>

#define MFC_DEVICE "/dev/video6"
#define COMPRESSED_BUFFER_CNT 3
#define MFC_COMPRESSED_PLANES 1
#define MFC_MAX_COMPRESSED_BUF 16
#define MFC_UNCOMPRESSED_PLANES 3
#define MFC_UNCOMPRESSED_MAX_BUF 32
#define RESULT_EXTRA_BUFFER_CNT 2

int m_FileDescriptor;
int m_CompressedBufferSize;
int m_NCompressedBuffers;
size_t m_CompressedBufferMemoryOffset[MFC_MAX_COMPRESSED_BUF];
size_t m_CompressedBufferMemoryLength[MFC_MAX_COMPRESSED_BUF];
unsigned char *m_CompressedBufferMMapAddres[MFC_MAX_COMPRESSED_BUF];
bool m_CompressedBufferDequeued[MFC_MAX_COMPRESSED_BUF];

#define MyAssert(a) if(!(a)) throw #a

void Open()
{
	m_FileDescriptor = open(MFC_DEVICE, O_RDWR | O_NONBLOCK, 0); MyAssert(m_FileDescriptor > 0);

	struct v4l2_format fmt;
	fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_H264;
	fmt.fmt.pix_mp.plane_fmt[0].sizeimage = 1024 * 1024;
	ret = ioctl(m_FileDescriptor, VIDIOC_S_FMT, &fmt); MyAssert(ret == 0);
	m_CompressedBufferSize = fmt.fmt.pix_mp.plane_fmt[0].sizeimage;

	//allocate compressed buffers
	struct v4l2_requestbuffers reqbuf;
	memset(&reqbuf, 0, sizeof(reqbuf));
	reqbuf.count = COMPRESSED_BUFFER_CNT;
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	reqbuf.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(m_FileDescriptor, VIDIOC_REQBUFS, &reqbuf); MyAssert(ret == 0);
	m_NCompressedBuffers = reqbuf.count;
	
	//mmap compressed buffers
	struct v4l2_buffer buf;
	struct v4l2_plane planes[MFC_COMPRESSED_PLANES];
	for (int n = 0; n < m_NCompressedBuffers; n++)
	{
		memset(&buf, 0, sizeof(buf));
		memset(planes, 0, MFC_COMPRESSED_PLANES * sizeof(v4l2_plane));
		buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n;
		buf.m.planes = planes;
		buf.length = MFC_COMPRESSED_PLANES;

		ret = ioctl(m_FileDescriptor, VIDIOC_QUERYBUF, &buf); MyAssert(ret == 0);
		m_CompressedBufferMemoryOffset[n] = buf.m.planes[0].m.mem_offset;
		m_CompressedBufferMemoryLength[n] = buf.m.planes[0].length;
		m_CompressedBufferMMapAddres[n] = (unsigned char*)mmap(NULL, buf.m.planes[0].length, PROT_READ | PROT_WRITE, MAP_SHARED, m_FileDescriptor, buf.m.planes[0].m.mem_offset); MyAssert(m_CompressedBufferMMapAddres[n] != MAP_FAILED);
		m_CompressedBufferDequeued[n] = true;
	}
....
}

void DecodeSomeFrames()
{

}

Close()
{

}

int main(int argc, char **argv)
{
	Open();

	DecodeSomeFrames();

	Close();

	return 0;
}