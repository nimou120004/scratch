/* *** THIS CLASS IS DESIGNED TO FETCH ELEMENTS OF ARQ HEADER INTO A BUFFER. ELEMENTS LIKE
 * SEQUENCE NUMBER, TIMESTAMP ... ALL THESE ELEMENTS ARE RELATED TO THE APPLICATION LAYER
*/

#ifndef HEADERBUFFER_H
#define HEADERBUFFER_H


//using namespace ns3;
namespace ns3 {

  class HeaderBuffer
  {
  public:
    HeaderBuffer();
    int put_uc (char *bfr, int n, unsigned char x);
    int put_us (char *bfr, int n, unsigned short x);
    int put_ul (char *bfr, int n, unsigned long x);
    //int put_f (char *bfr, int n, float x);

    unsigned char get_uc (char *bfr, int n);
    unsigned short get_us (char *bfr, int n);
    unsigned long get_ul (char *bfr, int n);

  };
}


#endif // HEADERBUFFER_H
