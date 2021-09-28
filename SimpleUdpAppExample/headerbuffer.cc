#include "headerbuffer.h"
#include <stdlib.h>
#include "ns3/log.h"

namespace ns3 {

  NS_LOG_COMPONENT_DEFINE("HeaderBuffer");
 // NS_OBJECT_ENSURE_REGISTERED(HeaderBuffer);
  HeaderBuffer::HeaderBuffer()
  {

  }

  int HeaderBuffer::put_uc (char *bfr, int n, unsigned char x)
  {
    bfr[n] = (unsigned char) x;
    return EXIT_SUCCESS;
  }

  int HeaderBuffer::put_us (char *bfr, int n, unsigned short x)
  {
    bfr[n] = (unsigned char) (x >> 8) & 0xFF;
    bfr[n + 1] = (unsigned char) x & 0xFF;
    return EXIT_SUCCESS;
  }

  int HeaderBuffer::put_ul (char *bfr, int n, unsigned long x)
  {
    bfr[n] = (unsigned char) (x >> 24) & 0xFF;
    bfr[n + 1] = (unsigned char) (x >> 16) & 0xFF;
    bfr[n + 2] = (unsigned char) (x >> 8) & 0xFF;
    bfr[n + 3] = (unsigned char) x & 0xFF;
    return EXIT_SUCCESS;
  }

/*
  int HeaderBuffer::put_f (char *bfr, int n, float x)
  {
    unsigned char *bytes = (unsigned char *) &x;
    unsigned int i;
    for(i = 0; i < sizeof(x); i++){
        bfr[n + i] = bytes[i];
        }
    return EXIT_SUCCESS;
  }
*/

  unsigned char HeaderBuffer::get_uc (char *bfr, int n)
  {
    return (unsigned char)bfr[n];
  }

  unsigned short HeaderBuffer::get_us (char *bfr, int n)
  {
    return ((unsigned char)bfr[n] << 8) + (unsigned char) bfr[n + 1];
  }

  unsigned long HeaderBuffer::get_ul (char *bfr, int n)
  {
    return ((unsigned char)bfr[n] << 24) + ((unsigned char)bfr[n + 1] << 16) + ((unsigned char)bfr[n + 2] << 8) + (unsigned char)bfr[n + 3];
  }


}


