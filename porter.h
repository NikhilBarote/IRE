
/* This is the Porter stemming algorithm, coded up in ANSI C by the
   author. It may be be regarded as canonical, in that it follows the
   algorithm presented in

   Porter, 1980, An algorithm for suffix stripping, Program, Vol. 14,
   no. 3, pp 130-137,

   only differing from it at the points maked --DEPARTURE-- below.

   See also http://www.tartarus.org/~martin/PorterStemmer

   The algorithm as described in the paper could be exactly replicated
   by adjusting the points of DEPARTURE, but this is barely necessary,
   because (a) the points of DEPARTURE are definitely improvements, and
   (b) no encoding of the Porter stemmer I have seen is anything like
   as exact as this version, even with the points of DEPARTURE!

   You can compile it on Unix with 'gcc -O3 -o stem stem.c' after which
   'stem' takes a list of inputs and sends the stemmed equivalent to
   stdout.

   The algorithm as encoded here is particularly fast.

   Release 1: was many years ago
   Release 2: 11 Apr 2013
       fixes a bug noted by Matt Patenaude <matt@mattpatenaude.com>,

       case 'o': if (ends("\03" "ion") && (b[j] == 's' || b[j] == 't')) break;
           ==>
       case 'o': if (ends("\03" "ion") && j >= 0 && (b[j] == 's' || b[j] == 't')) break;

       to avoid accessing b[-1] when the word in b is "ion".
*/

#include <string.h>  /* for memmove */
#include <string>
using namespace std;
#define TRUE_MAC 1
#define FALSE_MAC 0

/* The main part of the stemming algorithm starts here. b is a buffer
   holding a word to be stemmed. The letters are in b[k0], b[k0+1] ...
   ending at b[k]. In fact k0 = 0 in this demo program. k is readjusted
   downwards as the stemming progresses. Zero termination is not in fact
   used in the algorithm.

   Note that only lower case sequences are stemmed. Forcing to lower case
   should be done before stem(...) is called.
*/

static char * b;       /* buffer for word to be stemmed */
static int k,k0,j;     /* j is a general offset into the string */

/* cons(i) is TRUE_MAC <=> b[i] is a consonant. */

static int cons(int i)
{  switch (b[i])
   {  case 'a': case 'e': case 'i': case 'o': case 'u': return FALSE_MAC;
      case 'y': return (i==k0) ? TRUE_MAC : !cons(i-1);
      default: return TRUE_MAC;
   }
}

/* m() measures the number of consonant sequences between k0 and j. if c is
   a consonant sequence and v a vowel sequence, and <..> indicates arbitrary
   presence,

      <c><v>       gives 0
      <c>vc<v>     gives 1
      <c>vcvc<v>   gives 2
      <c>vcvcvc<v> gives 3
      ....
*/

static int m()
{  int n = 0;
   int i = k0;
   while(TRUE_MAC)
   {  if (i > j) return n;
      if (! cons(i)) break; i++;
   }
   i++;
   while(TRUE_MAC)
   {  while(TRUE_MAC)
      {  if (i > j) return n;
            if (cons(i)) break;
            i++;
      }
      i++;
      n++;
      while(TRUE_MAC)
      {  if (i > j) return n;
         if (! cons(i)) break;
         i++;
      }
      i++;
   }
}

/* vowelinstem() is TRUE_MAC <=> k0,...j contains a vowel */

static int vowelinstem()
{  int i; for (i = k0; i <= j; i++) if (! cons(i)) return TRUE_MAC;
   return FALSE_MAC;
}

/* doublec(j) is TRUE_MAC <=> j,(j-1) contain a double consonant. */

static int doublec(int j)
{  if (j < k0+1) return FALSE_MAC;
   if (b[j] != b[j-1]) return FALSE_MAC;
   return cons(j);
}

/* cvc(i) is TRUE_MAC <=> i-2,i-1,i has the form consonant - vowel - consonant
   and also if the second c is not w,x or y. this is used when trying to
   restore an e at the end of a short word. e.g.

      cav(e), lov(e), hop(e), crim(e), but
      snow, box, tray.

*/

static int cvc(int i)
{  if (i < k0+2 || !cons(i) || cons(i-1) || !cons(i-2)) return FALSE_MAC;
   {  int ch = b[i];
      if (ch == 'w' || ch == 'x' || ch == 'y') return FALSE_MAC;
   }
   return TRUE_MAC;
}

/* ends(s) is TRUE_MAC <=> k0,...k ends with the string s. */

// static int ends(char * s)
// {  int length = s[0];
//    if (s[length] != b[k]) return FALSE_MAC; /* tiny speed-up */
//    if (length > k-k0+1) return FALSE_MAC;
//    if (memcmp(b+k-length+1,s+1,length) != 0) return FALSE_MAC;
//    j = k-length;
//    return TRUE_MAC;
// }

static int ends(string ns)
{
  char s[1005];
  int length = (int)ns.size();
  for(int i=0;i<length;i++) s[i]=ns[i];
   if (s[length-1] != b[k]) return FALSE_MAC; /* tiny speed-up */
   if (length > k-k0+1) return FALSE_MAC;
   if (memcmp(b+k-length+1,s,length) != 0) return FALSE_MAC;
   j = k-length;
   return TRUE_MAC;
}

// int ends1(string s)
// {
  
//    if (s[length-1] != b[k]) return FALSE_MAC; /* tiny speed-up */
//    if (length > k-k0+1) return FALSE_MAC;
//    if (memcmp(b+k-length+1,s,length) != 0) return FALSE_MAC;
//    j = k-length;
//    return TRUE_MAC;
// }

/* setto(s) sets (j+1),...k to the characters in the string s, readjusting
   k. */

static void setto(string ns)
{
  char s[1005];
  int length = (int)ns.size();
  for(int i=0;i<length;i++) s[i]=ns[i];

   memmove(b+j+1,s,length);
   k = j+length;
}

/* r(s) is used further down. */

static void r(string s) { if (m() > 0) setto(s); }

/* step1ab() gets rid of plurals and -ed or -ing. e.g.

       caresses  ->  caress
       ponies    ->  poni
       ties      ->  ti
       caress    ->  caress
       cats      ->  cat

       feed      ->  feed
       agreed    ->  agree
       disabled  ->  disable

       matting   ->  mat
       mating    ->  mate
       meeting   ->  meet
       milling   ->  mill
       messing   ->  mess

       meetings  ->  meet

*/

static void step1ab()
{  if (b[k] == 's')
   {  if (ends("sses")) k -= 2; else
      if (ends("ies")) setto("i"); else
      if (b[k-1] != 's') k--;
   }
   if (ends("eed")) { if (m() > 0) k--; } else
   if ((ends("ed") || ends("ing")) && vowelinstem())
   {  k = j;
      if (ends("at")) setto("ate"); else
      if (ends("bl")) setto("ble"); else
      if (ends("iz")) setto("ize"); else
      if (doublec(k))
      {  k--;
         {  int ch = b[k];
            if (ch == 'l' || ch == 's' || ch == 'z') k++;
         }
      }
      else if (m() == 1 && cvc(k)) setto("e");
   }
}

/* step1c() turns terminal y to i when there is another vowel in the stem. */

static void step1c() { if (ends("y") && vowelinstem()) b[k] = 'i'; }


/* step2() maps double suffices to single ones. so -ization ( = -ize plus
   -ation) maps to -ize etc. note that the string before the suffix must give
   m() > 0. */

static void step2() { switch (b[k-1])
{
    case 'a': if (ends("ational")) { r("ate"); break; }
              if (ends("tional")) { r("tion"); break; }
              break;
    case 'c': if (ends("enci")) { r("ence"); break; }
              if (ends("anci")) { r("ance"); break; }
              break;
    case 'e': if (ends("izer")) { r("ize"); break; }
              break;
    case 'l': if (ends("bli")) { r("ble"); break; } /*-DEPARTURE-*/

 /* To match the published algorithm, replace this line with
    case 'l': if (ends("abli")) { r("able"); break; } */

              if (ends("alli")) { r("al"); break; }
              if (ends("entli")) { r("ent"); break; }
              if (ends("eli")) { r("e"); break; }
              if (ends("ousli")) { r("ous"); break; }
              break;
    case 'o': if (ends("ization")) { r("ize"); break; }
              if (ends("ation")) { r("ate"); break; }
              if (ends("ator")) { r("ate"); break; }
              break;
    case 's': if (ends("alism")) { r("al"); break; }
              if (ends("iveness")) { r("ive"); break; }
              if (ends("fulness")) { r("ful"); break; }
              if (ends("ousness")) { r("ous"); break; }
              break;
    case 't': if (ends("aliti")) { r("al"); break; }
              if (ends("iviti")) { r("ive"); break; }
              if (ends("biliti")) { r("ble"); break; }
              break;
    case 'g': if (ends("logi")) { r("log"); break; } /*-DEPARTURE-*/

 /* To match the published algorithm, delete this line */

} }

/* step3() deals with -ic-, -full, -ness etc. similar strategy to step2. */

static void step3() { switch (b[k])
{
    case 'e': if (ends("icate")) { r("ic"); break; }
              if (ends("ative")) { r(""); break; }
              if (ends("alize")) { r("al"); break; }
              break;
    case 'i': if (ends("iciti")) { r("ic"); break; }
              break;
    case 'l': if (ends("ical")) { r("ic"); break; }
              if (ends("ful")) { r(""); break; }
              break;
    case 's': if (ends("ness")) { r(""); break; }
              break;
} }

/* step4() takes off -ant, -ence etc., in context <c>vcvc<v>. */

static void step4()
{  switch (b[k-1])
    {  case 'a': if (ends("al")) break; return;
       case 'c': if (ends("ance")) break;
                 if (ends("ence")) break; return;
       case 'e': if (ends("er")) break; return;
       case 'i': if (ends("ic")) break; return;
       case 'l': if (ends("able")) break;
                 if (ends("ible")) break; return;
       case 'n': if (ends("ant")) break;
                 if (ends("ement")) break;
                 if (ends("ment")) break;
                 if (ends("ent")) break; return;
       case 'o': if (ends("ion") && j >= 0 && (b[j] == 's' || b[j] == 't')) break;
                 if (ends("ou")) break; return;
                 /* takes care of -ous */
       case 's': if (ends("ism")) break; return;
       case 't': if (ends("ate")) break;
                 if (ends("iti")) break; return;
       case 'u': if (ends("ous")) break; return;
       case 'v': if (ends("ive")) break; return;
       case 'z': if (ends("ize")) break; return;
       default: return;
    }
    if (m() > 1) k = j;
}

/* step5() removes a final -e if m() > 1, and changes -ll to -l if
   m() > 1. */

static void step5()
{  j = k;
   if (b[k] == 'e')
   {  int a = m();
      if (a > 1 || (a == 1 && !cvc(k-1))) k--;
   }
   if (b[k] == 'l' && doublec(k) && m() > 1) k--;
}

/* In stem(p,i,j), p is a char pointer, and the string to be stemmed is from
   p[i] to p[j] inclusive. Typically i is zero and j is the offset to the last
   character of a string, (p[j+1] == '\0'). The stemmer adjusts the
   characters p[i] ... p[j] and returns the new end-point of the string, k.
   Stemming never increases word length, so i <= k <= j. To turn the stemmer
   into a module, declare 'stem' as extern, and delete the remainder of this
   file.
*/

int stem(char * p, int i, int j)
{  b = p; k = j; k0 = i; /* copy the parameters into statics */
   if (k <= k0+1) return k; /*-DEPARTURE-*/

   /* With this line, strings of length 1 or 2 don't go through the
      stemming process, although no mention is made of this in the
      published algorithm. Remove the line to match the published
      algorithm. */

   step1ab(); step1c(); step2(); step3(); step4(); step5();
   return k;
}

/*--------------------stemmer definition ends here------------------------*/

#include <stdio.h>
#include <stdlib.h>      /* for malloc, free */
#include <ctype.h>       /* for isupper, islower, tolower */

static char * s;         /* a char * (=string) pointer; passed into b above */

#define INC 50           /* size units in which s is increased */
static int i_max = INC;  /* maximum offset in s */

void increase_s()
{  i_max += INC;
   {  char * new_s = (char *) malloc(i_max+1);
      { int i; for (i = 0; i < i_max; i++) new_s[i] = s[i]; } /* copy across */
      free(s); s = new_s;
   }
}

#define LETTER(ch) (isupper(ch) || islower(ch))


static string stemword(string nword)
{
      s = (char *) malloc(1000);
      int size=(int)nword.size();
      char ch;
      int i;
      for(i=0;i<size;i++)
      {
          ch=nword[i];
        if (ch>='a' && ch<='z')
        {
          // cerr << ch ;
         // return nword;
          if (i == i_max) increase_s();
            //ch = tolower(ch); /* forces lower case */
            s[i] = ch;
         }
      }
      nword=nword.substr(0,stem(s,0,i-1)+1);
      free(s);
   return nword;
}

// static void stemfile(FILE * f)
// {  while(TRUE_MAC)
//    {  int ch = getc(f);
//       if (ch == EOF) return;
//       if (LETTER(ch))
//       {  int i = 0;
//          while(TRUE_MAC)
//          {  if (i == i_max) increase_s();

//             ch = tolower(ch); /* forces lower case */

//             s[i] = ch; i++;
//             ch = getc(f);
//             if (!LETTER(ch)) { ungetc(ch,f); break; }
//          }
//          s[stem(s,0,i-1)+1] = 0;
//      /*     the previous line calls the stemmer and uses its result to
//             zero-terminate the string in s */
//          printf("%s",s);
//       }
//       else putchar(ch);
//    }
// }

// int main(int argc, char * argv[])
// {  int i;
   // s = (char *) malloc(i_max+1);
//    for (i = 1; i < argc; i++)
//    {  FILE * f = fopen(argv[i],"r");
//       if (f == 0) { fprintf(stderr,"File %s not found\n",argv[i]); exit(1); }
//       stemfile(f);
//    }
//    free(s);
//    return 0;
// }