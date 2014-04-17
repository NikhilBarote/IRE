import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.TreeMap;
import java.util.TreeSet;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;


public class SaxParser extends DefaultHandler  {
	boolean pageTag=false, titleTag=false, textTag=false, categoryFlag=false, idFlag=false;
	String titleString;
	TreeMap<String,String> tm = new TreeMap<String, String>();
	TreeMap<String,String> entityOrNot = new TreeMap<String, String>();
	TreeSet<String> stopWords = new TreeSet<String>();
	TreeSet<Character> capAlpha = new TreeSet<Character>();
	TreeSet<String> organizationWildCards = new TreeSet<String>();
	TreeSet<String> locationWildCards = new TreeSet<String>();
	TreeMap<String, Integer> wordCount = new TreeMap<String, Integer>();
	TreeMap<String, TreeMap<String,Integer>> idWordCountIndex = new TreeMap<String,TreeMap<String,Integer>>();
	int pageCount=0;
	String pageId;
	int count=0;
	int count1=0, count2=0;
	int flag=1;
	float result = 0;
	static Stemmer stemObj = new Stemmer();
	
	
	@Override
	public void startDocument() throws SAXException {
		createHashSet();
	}
	
	@Override
	public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
		if (qName.equalsIgnoreCase("page")){
			pageTag=true;
		}
		if (qName.equalsIgnoreCase("title"))
			titleTag=true;
		if(qName.equalsIgnoreCase("id")){
			if(flag==1){
				idFlag=true;
				flag=0;
			}
		}
		if (qName.equalsIgnoreCase("text"))
			textTag=true;
	}
	
	@Override
	public void characters(char[] ch, int start, int length)throws SAXException {
		if(titleTag==true){
			titleString = new String(ch, start, length);
			isEntity(titleString);
		}
		if(idFlag==true){
			pageId = new String(ch, start, length);
			
		}
		if(textTag==true){
			String textString = new String(ch, start, length);
			String check = entityOrNot.get(titleString);
			String[] split = titleString.split(" ");
			if(check!=null){
				if (check.trim().equals("Non Entity")&&split.length==1){
					checkTheAplpha(textString, titleString);
				}
			}
		parseCategory(textString,titleString.toLowerCase());
		}
	}
	
	public void isEntity(String titleString){
		int c=0 , cCount=0, dCount=0, q=0;
		if(titleString.contains("(")){
			int index=titleString.indexOf("(");
			titleString = titleString.substring(0,index);
		}
		titleString = titleString.trim();
		String[] splitTitle = titleString.split(" ");
		String[] titleArray = new String[splitTitle.length];
		int i=0;
		for(String key : splitTitle){
			if(!stopWords.contains(key)){
				titleArray[i]=key;
				i++;
			}
		}
		for(String key : titleArray){
			if(key!=null){
				char[] tokens = key.toCharArray();
				for (char j : tokens){
					if(capAlpha.contains(tokens[0])) c=1;
					if(capAlpha.contains(j)) {q++;}
				}
				if (q>=2) dCount = dCount + 1;
				cCount = cCount + c;
				c=0;
				q=0;
			}
		}
		if (cCount>=2 || dCount >=2 || (dCount==1 && titleArray.length==1)){
			entityOrNot.put(titleString, "Entity");
			
		}
		else{
			entityOrNot.put(titleString, "Non Entity");
			
		}
	}
	
	public void checkTheAplpha(String textString, String titleString){
		char[] textArray = textString.toCharArray();
		int len = textString.length();
		StringBuffer str = new StringBuffer();
		for (int i=0 ; i< len; i++){
			if ((textArray[i]>='a'&&textArray[i]<='z')||(textArray[i]>='A'&&textArray[i]<='Z')){
				str.append(textArray[i]);
			}
			else if(str.toString().equals(titleString.trim())) { count1++; str.setLength(0);}
 			else if(str.toString().equalsIgnoreCase(titleString.trim())) { count2++; str.setLength(0);}
 		}
	}
	
	public void parseCategory(String textString, String titleString){
		char[] textArray = textString.toCharArray();
		int len = textString.length();
		StringBuffer str = new StringBuffer();
		StringBuffer wildCard = new StringBuffer();
		for(int i=0; i<len; i++){
			if (((textArray[i]>='a'&&textArray[i]<='z')||(textArray[i]>='A'&&textArray[i]<='Z'))&&(categoryFlag==false)){
					str.append(textArray[i]);
			}
			else if(str.toString().trim().equals("Category")||categoryFlag==true){
				categoryFlag=true;
				if ((textArray[i]>='a'&&textArray[i]<='z')||(textArray[i]>='A'&&textArray[i]<='Z')){
					wildCard.append(textArray[i]);
				}
				else if(wildCard.length()!=0){
					String wild = wildCard.toString().toLowerCase();
					if(wild.contains("births")||wild.contains("livingpeople")){
						tm.put(titleString, "Person");
					}
					for(String key : locationWildCards){
						if(wild.trim().contains(key.trim()))
							if(!tm.containsKey(titleString))	
								tm.put(titleString, "Location");
					}
					for(String key : organizationWildCards){
						if(wild.trim().contains(key.trim()))
							tm.put(titleString, "Organization");
					}
				}
				else if(textArray[i]==']'){ 
					str.setLength(0);
					wildCard.setLength(0);
					categoryFlag=false;
				}
			}
			else{
				str.setLength(0);
			}
		}
	}
	
	@Override
	public void endElement(String uri, String localName, String qName)throws SAXException {
		if (qName.equalsIgnoreCase("page")){
			pageCount++;
			pageTag=false;
			flag=1;
			TreeMap<String , Integer> tm = new TreeMap<String , Integer>();
			tm.putAll(wordCount);
			idWordCountIndex.put(pageId,tm);
			wordCount.clear();
			categoryFlag=false;
			
		}
		if (qName.equalsIgnoreCase("title"))
			titleTag=false;
		if(qName.equalsIgnoreCase("id"))
			idFlag=false;
		if (qName.equalsIgnoreCase("text")){
			textTag=false;
			if (count2>0){
				result = count1/(count2+count1);
				if (result>0.75) {  
					entityOrNot.put(titleString, "Entity");
					
				}
			}
			else{
				if (count1>0){
					entityOrNot.put(titleString, "Entity");
					
				}
			}
			count1=0; count2=0; result=0;
		}
		if(pageCount==10000){
			
			try {
				writeToCategoryFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
			try {
				writeToGenericFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
			pageCount=0;
		}
	}
	
	public void createHashSet(){
		String[] stopW = {"a", "able", "about", "across","after","all","almost","also","am","among","an","and",
				"any","are","as","at","be","because","been","but","by","can","cannot","could","dear"
				,"did","do","does","either","else","ever","every","for","from","get","got","had","has"
				,"have","he","her","hers","him","his","how","however","i","if","in","into","is","it","its"
				,"just","least","let","like","likely","may","me","might","most","must","my","neither",
				"no","nor","not","of","off","often","on","only","or","other","our","own","rather","said",
				"say","says","she","should","since","so","some","than","that","the","their","them","then",
				"there","these","they","this","tis","to","too","twas","us","wants","was","we","were","what",
				"when","where","which","while","who","whom","why","will","with","would","yet","you","your"};
		
		int len=stopW.length;
		for (int i=0; i<len; i++){
			stopWords.add(stopW[i]);
		}
		capAlpha.add('A');
		capAlpha.add('B');
		capAlpha.add('C');
		capAlpha.add('D');
		capAlpha.add('E');
		capAlpha.add('F');
		capAlpha.add('G');
		capAlpha.add('H');
		capAlpha.add('I');
		capAlpha.add('J');
		capAlpha.add('K');
		capAlpha.add('L');
		capAlpha.add('M');
		capAlpha.add('N');
		capAlpha.add('O');
		capAlpha.add('P');
		capAlpha.add('Q');
		capAlpha.add('R');
		capAlpha.add('S');
		capAlpha.add('T');
		capAlpha.add('U');
		capAlpha.add('V');
		capAlpha.add('W');
		capAlpha.add('X');
		capAlpha.add('Y');
		capAlpha.add('Z');
		
		organizationWildCards.add("organizations");
		organizationWildCards.add("organisationsbasedin");
		organizationWildCards.add("organisationsestablishedin");
		
		locationWildCards.add("citiesandtownsin");
		locationWildCards.add("statesandterritories");
		locationWildCards.add("townsin");
		locationWildCards.add("citiesin");
		locationWildCards.add("lakesof");
		locationWildCards.add("visitorplaces");
		locationWildCards.add("geographyof");
		locationWildCards.add("visitorattractions");
		locationWildCards.add("statesandterritoriesof");
		locationWildCards.add("Citiesof");
		locationWildCards.add("countriesandterritories");
	}
	
	@Override
	public void endDocument() throws SAXException {
		
		if(pageCount<10000&&pageCount!=0){
			try {
				writeToCategoryFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
			try {
				writeToGenericFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
			pageCount=0;
		}
	
	}
	
	public void writeToCategoryFile() throws IOException{
		File fp = new File("category.txt");
		if(!fp.exists()) fp.createNewFile();
		BufferedWriter bw = new BufferedWriter(new FileWriter(fp,true));
		for (String key : tm.keySet()){
			bw.write(key);
			bw.write(" : ");
			bw.write(tm.get(key));
			bw.write("\n");
		}
		bw.close();
		tm.clear();
	}
	
	public void writeToGenericFile() throws IOException{
		File fp = new File("generic.txt");
		if(!fp.exists()) fp.createNewFile();
		BufferedWriter bw = new BufferedWriter(new FileWriter(fp,true));
		for (String key : entityOrNot.keySet()){
			String test = entityOrNot.get(key);
			if(test.trim().equalsIgnoreCase("entity")){
				bw.write(key);
				bw.write("\n");
			}
		}
		bw.close();
		entityOrNot.clear();
	}
}
