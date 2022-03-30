
#ifndef _DDS_H_
#define _DDS_H_

struct DDSPixel
{
  unsigned int    size;
  unsigned int    flags;
  unsigned int    fourCC;
  unsigned int    RGBBitCount;
  unsigned int    RBitMask;
  unsigned int    GBitMask;
  unsigned int    BBitMask;
  unsigned int    ABitMask;
};

struct DDSHeader
{
  unsigned int size;
  unsigned int flags;
  unsigned int height;
  unsigned int width;
  unsigned int pitchOrLinearSize;
  unsigned int depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
  unsigned int mipMapCount;
  unsigned int reserved1[11];
  DDSPixel ddspf;
  unsigned int caps;
  unsigned int caps2;
  unsigned int caps3;
  unsigned int caps4;
  unsigned int reserved2;
};

#endif
