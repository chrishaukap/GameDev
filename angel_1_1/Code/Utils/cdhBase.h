#ifndef CDHBASE_H
#define CDHBASE_H

namespace CDH
{

typedef int CHint;
typedef unsigned int CHUint;
typedef unsigned long CHUlong;
typedef CHUlong RES_HANDLE;
const CHUlong InvalidHandle = 0;
enum Status
{
   StatusSuccess = 0,
   StatusFailure ,
   StatusFileNotFound,
   StatusInvalidArgs,
   StatusOutOfMemory,
   StatusBadContext
};

#define StatusFailed(stat) ((stat) == StatusSuccess ? false : true)
#define StatusSucceeded(stat) ((stat) == StatusSuccess ? true : false)


#define PI 3.141592f
#define PIOver2 1.570796f
#define PIOver3 1.047197f
#define PIOver4 0.785398f
#define PIOver5 0.628318f
#define PIOver6 0.523598f
#define PIOver7 0.448798f
#define PIOver8 0.392699f
#define PIOver9 0.349065f
#define PIOver16 0.1963495
#define PIOver32 0.09817475
#define PIOver64 0.049087375
#define PIOver128 0.0245436875
#define PIOver256 0.01227184375
#define TWOPI 6.283184

}

#endif //CDHBASE_H
