
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
#define NAMEI_PARENT	0x100	/* preserve parent */
	struct ucred	*cred;
	struct proc	*proc;

	/* output fields */
	struct vnode	*vp;
	struct vnode	*parentvp;

	/* output fields which will be cleaned up by namei_cleanup() */
	char		*seg;	/* current segment name we're looking */

	/* internal fields */
	char		*pathbuf;	/* internal path buffer */
};

#define NDINIT(nd, ndpath, ndintent, ndflags, ndcred, ndproc) \
	do { \
		(nd)->path = (ndpath); \
		(nd)->intent = (ndintent); \
		(nd)->flags = (ndflags); \
		(nd)->cred = (ndcred); \
		(nd)->proc = (ndproc); \
	} while (0)
/* To provide a default nameidata for vn_open() */
#define NDINIT_EMPTY(nd, ndcred, ndproc) \
	do { \
		(nd)->path = NULL; \
		(nd)->intent = 0; \
		(nd)->flags = 0; \
		(nd)->cred = (ndcred); \
		(nd)->proc = (ndproc); \
	} while (0)

int namei_putparent(struct nameidata *nd);
int namei(struct nameidata *);
void namei_cleanup(struct nameidata *);	/* each namei() should end with this */
bool namei_trim_slash(char *path);

#endif
