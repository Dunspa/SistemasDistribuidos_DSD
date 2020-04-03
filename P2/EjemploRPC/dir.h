/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _DIR_H_RPCGEN
#define _DIR_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAX 255

typedef char *nametype;

typedef struct namenode *namelist;

struct namenode {
	nametype name;
	namelist next;
};
typedef struct namenode namenode;

struct readdir_res {
	int err;
	union {
		namelist list;
	} readdir_res_u;
};
typedef struct readdir_res readdir_res;

#define DIRPROG 0x20000155
#define DIRVER 1

#if defined(__STDC__) || defined(__cplusplus)
#define READDIR 1
extern  readdir_res * readdir_1(nametype , CLIENT *);
extern  readdir_res * readdir_1_svc(nametype , struct svc_req *);
extern int dirprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define READDIR 1
extern  readdir_res * readdir_1();
extern  readdir_res * readdir_1_svc();
extern int dirprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_nametype (XDR *, nametype*);
extern  bool_t xdr_namelist (XDR *, namelist*);
extern  bool_t xdr_namenode (XDR *, namenode*);
extern  bool_t xdr_readdir_res (XDR *, readdir_res*);

#else /* K&R C */
extern bool_t xdr_nametype ();
extern bool_t xdr_namelist ();
extern bool_t xdr_namenode ();
extern bool_t xdr_readdir_res ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_DIR_H_RPCGEN */
