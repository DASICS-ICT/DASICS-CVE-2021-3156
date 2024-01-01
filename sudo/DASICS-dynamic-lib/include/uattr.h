#ifndef _UATTR_H_
#define _UATTR_H_

#define ATTR_UMAIN_TEXT __attribute__((section(".umaintext")))
#define ATTR_UMAIN_DATA __attribute__((section(".umaindata")))

#define ATTR_ULIB_TEXT __attribute__((section(".ulibtext")))
#define ATTR_ULIB_DATA __attribute__((section(".ulibdata")))

#define ATTR_ULIB_TEXT1 __attribute__((section(".ulibtext1")))
#define ATTR_ULIB_TEXT2 __attribute__((section(".ulibtext2")))

#define ATTR_UFREEZONE_TEXT __attribute__((section(".ufreezonetext")))
#define ATTR_UFREEZONE_DATA __attribute__((section(".ufreezonedata")))

#endif