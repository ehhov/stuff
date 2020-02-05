/*

print_essid(int			skfd,
	    const char *	ifname,
	    int			format)
 {
  struct iwreq		wrq;
  char			essid[IW_ESSID_MAX_SIZE + 1];	//ESSID 
  
  // Make sure ESSID is always NULL terminated 
  memset(essid, 0, sizeof(essid));

  // Get ESSID 
  wrq.u.essid.pointer = (caddr_t) essid;
  wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
  wrq.u.essid.flags = 0;
  iw_get_ext(skfd, ifname, SIOCGIWESSID, &wrq)

	printf("%s\n", essid);

  return(0);
}


ret = print_essid(skfd, ifname, format);
      if(ret < 0)
	{
	  // Try to print a nwid 
	  ret = print_nwid(skfd, ifname, format);
}

static int
print_nwid(int		skfd,
	   const char *	ifname,
	   int		format)
{
  struct iwreq		wrq;

  // Get network ID 
  if(iw_get_ext(skfd, ifname, SIOCGIWNWID, &wrq) < 0)
    return(-1);

  switch(format)
    {
    case FORMAT_SCHEME:
      // Prefix with nwid to avoid name space collisions 
      printf("nwid%X\n", wrq.u.nwid.value);
      break;
    case FORMAT_RAW:
      printf("%X\n", wrq.u.nwid.value);
      break;
    default:
      printf("%-8.16s  NWID:%X\n", ifname, wrq.u.nwid.value);
      break;
    }

  return(0);
}


int
main(int	argc,
     char **	argv)
{
  int	skfd;			// generic raw socket desc.
  int	format = FORMAT_DEFAULT;
  int	wtype = WTYPE_ESSID;
  int	opt;
  int	ret = -1;

	  format = FORMAT_RAW;

  // Create a channel to the NET kernel. 
  skfd = iw_sockets_open();

	ret = print_one_device(skfd, format, wtype, ifname);
  fflush(stdout);
  iw_sockets_close(skfd);
  return(ret);
}


*/

#include <iwlib.h>

int main () {
	
	int skfd;

	skfd = iw_sockets_open();

	struct iwreq		wrq;
  char			essid[IW_ESSID_MAX_SIZE + 1];	//ESSID 
  
  // Make sure ESSID is always NULL terminated 
  memset(essid, 0, sizeof(essid));

  // Get ESSID 
  wrq.u.essid.pointer = (caddr_t) essid;
  wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
  wrq.u.essid.flags = 0;
  iw_get_ext(skfd, "wlp58s0", SIOCGIWESSID, &wrq);

	printf("%s\n", essid);

	iw_sockets_close(skfd);
	
	return 0;
} 
