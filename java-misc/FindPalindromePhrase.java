import java.util.Arrays;
import java.util.Scanner;
import java.io.File;
import java.util.ArrayList;
import java.io.FileNotFoundException;

//wrapping the example from wikipedia, thank god it works.
//http://en.wikipedia.org/wiki/Longest_palindromic_substring

public class FindPalindromePhrase {
 
  public static void main(String[] arvs) throws FileNotFoundException
  {
    String foo = "";
    String temp_string = "";
    String[] quatz;
    Scanner zap;
    int count;
    ArrayList<Integer> bar_index = new ArrayList<Integer>();
    ArrayList<String> bar = new ArrayList<String>();

      zap = new Scanner(new File(arvs[0]));

    count = 0;
    while(zap.hasNextLine())
    {
      String line = zap.nextLine();
      temp_string = line.toLowerCase();
      quatz = temp_string.split("[,\\s\\-:\\?\\b]");
      //assemble the candiate strings
      for(int fu = 0; fu < quatz.length; fu++){
        String candidate = quatz[fu];
        for(int pf = fu+1; pf < quatz.length; pf++)
        {
          if(candidate == FindPalindromePhrase.findLongestPalindrome(candidate))
          {
            bar.add(candidate);
          }
          candidate = candidate + quatz; //grow the string
        }
      }
      /*
      quatz = line.replace(",","");//this is the stripped output to 
      quatz = quatz.replace(".","");
      quatz = quatz.replace(" ","");
      quatz = quatz.replace("!","");
      quatz = quatz.replace("?","");
      quatz = quatz.replace(":","");
      
      quatz = quatz.toLowerCase(); // convert all the lower case characters
      //for 
      */
      //foo = FindPalindromePhrase.findLongestPalindrome(quatz); //find the longest string
      //String spaceer=" ";
      /*
      if(quatz.charAt(quatz.indexOf(foo)+foo.length())!=spaceer.charAt(0)) {
        String foo123 = test.findLongestPalindrome(quatz.substring(0,quatz.indexOf(foo))+quatz.substring(quatz.indexOf(foo)+1)); //find the longest string
        String foo234 = test.findLongestPalindrome(quatz.substring(0,quatz.indexOf(foo)+foo.length()-1)+quatz.substring(quatz.indexOf(foo)+foo.length())); //find the longest string
        if(foo123.length()>foo234.length()) {
          foo=foo123;
        } else {
          foo=foo234;
        }
      }
      */
      count++; //increment count
    }
    for (int i=0;i < bar.size(); i++)
    {
      System.out.println(bar.get(i)+ "   " + bar_index.get(i));
    }
  
  }
  
  public static String findLongestPalindrome(String s) {
    if (s==null || s.length()==0)
      return "";
 
    char[] s2 = addBoundaries(s.toCharArray());
    int[] p = new int[s2.length]; 
    int c = 0, r = 0; // Here the first element in s2 has been processed.
    int m = 0, n = 0; // The walking indices to compare if two elements are the same
    for (int i = 1; i<s2.length; i++) {
      if (i>r) {
        p[i] = 0; m = i-1; n = i+1;
      } else {
        int i2 = c*2-i;
        if (p[i2]<(r-i)) {
          p[i] = p[i2];
          m = -1; // This signals bypassing the while loop below. 
        } else {
          p[i] = r-i;
          n = r+1; m = i*2-n;
        }
      }
      while (m>=0 && n<s2.length && s2[m]==s2[n]) {
        p[i]++; m--; n++;
      }
      if ((i+p[i])>r) {
        c = i; r = i+p[i];
      }
    }
    int len = 0; c = 0;
    for (int i = 1; i<s2.length; i++) {
      if (len<p[i]) {
        len = p[i]; c = i;
      }
    }
    char[] ss = Arrays.copyOfRange(s2, c-len, c+len+1);
    return String.valueOf(removeBoundaries(ss));
  }
 
  private static char[] addBoundaries(char[] cs) {
    if (cs==null || cs.length==0)
      return "||".toCharArray();
 
    char[] cs2 = new char[cs.length*2+1];
    for (int i = 0; i<(cs2.length-1); i = i+2) {
      cs2[i] = '|';
      cs2[i+1] = cs[i/2];
    }
    cs2[cs2.length-1] = '|';
    return cs2;
  }
 
  private static char[] removeBoundaries(char[] cs) {
    if (cs==null || cs.length<3)
      return "".toCharArray();
 
    char[] cs2 = new char[(cs.length-1)/2];
    for (int i = 0; i<cs2.length; i++) {
      cs2[i] = cs[i*2+1];
    }
    return cs2;
  }    
}