#include <sys/ioctl.h>                              /* ioctl */
#include <linux/fs.h>                               /* FS_IOC_GETFLAGS, FS_NOATIME_FL, FS_IOC_SETFLAGS */
#include <fcntl.h>                                  /* open */
#include <sys/stat.h>                               /* S_IRWXU, S_IRWXG, S_IRWXO */

#include "../lib/error_functions.h"
/*
    Флаги индексного дескриптора
    Константа           Параметр команды chattr     Назначение
    FS_APPEND_FL        a                           Только добавление (необходима привилегия)
    FS_COMPR_FL         c                           Задействовать сжатие файла (не реализовано)
    FS_DIRSYNC_FL D     D                           Синхронное обновление каталогов
    FS_IMMUTABLE_FL     i                           Неизменяемый (необходима привилегия)
    FS_JOURNAL_DATA_FL  j                           Задействовать журналирование данных (необходима привилегия)
    FS_NOATIME_FL       A                           Не обновлять время последнего доступа к файлу
    FS_NODUMP_FL        d                           Без дампа
    FS_NOTAIL_FL        t                           Без упаковки хвостов
    FS_SECRM_FL         s                           Защищенное удаление (не реализовано)
    FS_SYNC_FL          S                           Синхронное обновление файла (и каталога)
    FS_TOPDIR_FL        T                           Считать каталогом верхнего уровня для стратегии Орлова
    FS_UNRM_FL          u                           Можно восстановить удаленный файл (не реализовано)

    современные флаги
	FS_EXTENT_FL        e                       	Использование extents
	FS_CASEFOLD_FL      E                       	Каталог с нечувствительностью к регистру
	FS_NOCOW_FL         F                       	Отключить Copy-on-Write (например, btrfs)
	FS_NOCOW_FL         C                       	Для некоторых ФС отображается как C
	FS_PROJINHERIT_FL   P                       	Наследование Project ID
	FS_VERITY_FL        V                       	fs-verity
	FS_DAX_FL           x                       	Direct Access (DAX)
*/


int main()
{
    int fd, attr;

    fd = open("startup", O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

    if (ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1)    /* Извлечь текущие флаги */
        errExit("ioctl");

    attr |= FS_NOATIME_FL;
    if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1)    /* Обновить флаги */
        errExit("ioctl");


    return 0;
}