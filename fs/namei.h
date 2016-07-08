
#ifndef _FS_NAMEI_H
#define _FS_NAMEI_H

#include <sys/types.h>

struct vnode;	/* fs/vnode.h */
struct proc;	/* include/proc.h */

struct nameidata {
	/* input fields */
	char		*path;
	int		intent;
#define NAMEI_LOOKUP	0
#define NAMEI_CREATE	1
#define NAMEI_DELETE	2
#define NAMEI_RENAME	3
	uint32_t	flags;
#define NAMEI_FOLLOW	0x1	/* follow symlinks */

	/* output fields */
	struct vnode	*vp;
	struct vnode	*parentvp;
};

int namei(struct nameidata *, struct proc *);

#endif
