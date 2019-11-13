
void free(void* n){
return;
}

/*
� 2000-2005 Klocwork Inc.  All rights reserved.
Permission is hereby granted, to use, copy, modify, and distribute this software
and its documentation for any purpose, provided that the above copyright notice
and the following three  paragraphs appear in all copies of this software:
1.	IN NO EVENT SHALL KLOCWORK INC. BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
        SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS
        SOFTWARE  AND ITS DOCUMENTATION, EVEN IF KLOCWORK INC. HAS BEEN ADVISED OF THE
        POSSIBILITY OF SUCH DAMAGE.
2.	KLOCWORK INC. HEREBY EXPRESSLY DISCLAIMS ANY IMPLIED WARRANTIES OR CONDITIONS,
        INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES AND CONDITIONS OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
3.	THE SOFTWARE IS PROVIDED ON AN "AS-IS" BASIS AND KLOCWORK INC. HAS NO OBLIGATION
        TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Restricted Rights. The Software (a) was developed at private expense, is existing
computer software and no part of it was developed with government funds,
(b) is �restricted computer software� licensed in accordance with restricted rights
provisions of subparagraphs (a) through (d) of the Commercial Computer Software-Restricted
Rights clause FAR 52.227-19 and its successors, and
(c) is unpublished and all rights are reserved under the copyrights laws of the
United States. Use, duplication, or disclosure by the U.S. Government is also subject to
restrictions as set forth herein and in DFARS 227.7202-1(a) and 227.7202-3(a) (1995),
DFARS 252.227-7013(c)(1)(ii) (Oct 1988), FAR 12.212(a) 1995, or FAR 52.227-14,
as applicable.  Only the rights to the Software set forth above are provided.
*/
#include <stdlib.h>
#include <stdio.h>

int main(){
      char * x = (char *) malloc(4);
      if (x == NULL)
        {printf("Memory allocation problem"); return 1;}
      free(x);
      if(x) *x = 'a';									/* FLAW */
      return 0;
}
