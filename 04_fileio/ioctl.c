#include <sys/ioctl.h>

// Возвращаемое при успешном завершении значение зависит от request или при ошибке равно –1
int ioctl(int fd, int request, ... /* argp */);









