import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;
import java.util.TreeSet;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class synonymExtraction extends DefaultHandler {
	searchQuery obj1 = new searchQuery();
	boolean pageTag = false, titleTag = false, textTag = false;
	String titleString, test;
	int count = 0;

	TreeSet<String> checkEntity = new TreeSet<String>();
	TreeMap<String, List<String>> synonyms = new TreeMap<String, List<String>>();

	@Override
	public void startDocument() throws SAXException {
		System.out.println("Started Merging2");
		try {
			bringToMemory();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Bought to memory!");
	}

	@Override
	public void startElement(String uri, String localName, String qName,
			Attributes attributes) throws SAXException {
		if (qName.equalsIgnoreCase("page"))
			pageTag = true;
		if (qName.equalsIgnoreCase("title"))
			titleTag = true;
		if (qName.equalsIgnoreCase("text"))
			textTag = true;
	}

	@Override
	public void characters(char[] ch, int start, int length)
			throws SAXException {
		if (titleTag == true) {
			titleString = new String(ch, start, length);
		}
		if (textTag == true) {
			String textString = new String(ch, start, length);
			try {
				parseSynonyms(textString);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void parseSynonyms(String textString) throws IOException {
		char[] textArray = textString.toCharArray();
		int len = textString.length();
		StringBuffer str = new StringBuffer();
		for (int i = 0; i < len; i++) {
			if (textArray[i] == '[')
				count++;
			else if (textArray[i] == ']') {
				if (count != 0)
					count--;
			} else if (count == 2) {
				while (textArray[i] != ']') {
					str.append(textArray[i]);
					i++;
					if (i >= len)
						break;
				}
				extractSynonyms(str.toString());
				i--;
			} else
				str.setLength(0);
		}
	}

	public void extractSynonyms(String link) throws IOException {
		
		if(link.trim().equals("|")||link.trim().length()<=1) return;
		if(link.contains(":")||link.contains("||")) return;
		if(link.contains("(")){
			int index=link.indexOf("(");
			link = link.substring(0,index);
		}
		link = link.trim();
		try {
			if(link.contains("|")){
				String[] split = link.split("\\|");
				if (checkEntity.contains(split[0])) {
					if (synonyms.containsKey(split[0])) {
						List<String> temp = synonyms.get(split[0]);
						if (split.length > 1) {
							if (!temp.contains(split[1]))
							temp.add(split[1]);
						}
					} else {
						List<String> temp = new ArrayList<String>();
						if (split.length > 1)
							temp.add(split[1]);
						synonyms.put(split[0], temp);
					}
				}
			}
		} catch (Exception e) {

			System.out.println(link);
			System.out.println(link.length());
		}
	}

	public boolean checkSyn(String word, BufferedWriter bw) throws IOException {
		RandomAccessFile fp = new RandomAccessFile(
				"/home/aragon/Documents/workspace/IRE.MAJOR/synonym.txt", "r");
		long start = 0;
		long end = fp.length();
		long mid;
		String res;
		try {
			while (start <= end) {
				mid = (start + end) / 2;
				fp.seek(mid);
				fp.readLine();
				if ((res = fp.readLine()) != null) {
					String[] arr = res.split(":");
					int compare = word.trim()
							.compareToIgnoreCase(arr[0].trim());
					if (compare == 0) {
						bw.write(res + "\n");
						fp.close();
						return true;
					} else if (compare > 0) {
						start = mid + 1;
					} else {
						end = mid - 1;
					}
				} else {
					end = mid - 1;
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		fp.close();
		return false;
	}

	@Override
	public void endElement(String uri, String localName, String qName)
			throws SAXException {
		if (qName.equalsIgnoreCase("page"))
			pageTag = false;
		if (qName.equalsIgnoreCase("title"))
			titleTag = false;
		if (qName.equalsIgnoreCase("text"))
			textTag = false;

	}

	@Override
	public void endDocument() throws SAXException {
		try {
			mergeMaps();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	public void mergeMaps() throws IOException {
		System.out.println("Writing to file!");
		File fp = new File("knowledgeBase.txt");
		if (!fp.exists())
			fp.createNewFile();
		BufferedWriter bw = new BufferedWriter(new FileWriter(fp));
		for (String entity : checkEntity) {
			int count = 0;
			if (synonyms.containsKey(entity)) {
				bw.write(entity + "#");
				List<String> temp = synonyms.get(entity);
				for (String i : temp) {
					bw.write(i);
					if (count != temp.size() - 1) {
						bw.write(",");
						count++;
					}
				}
				bw.write("\n");
			} else {
				bw.write(entity + "#");
				bw.write("\n");
			}
		}
		bw.close();
		checkEntity.clear();
		synonyms.clear();
	}

	public void bringToMemory() throws IOException {
		File fileGen = new File("generic.txt");
		BufferedReader brGen = new BufferedReader(new FileReader(fileGen));
		String lineGen = brGen.readLine();
		while (lineGen!=null){
			if(lineGen.length()<20){
				checkEntity.add(lineGen.toString());
			}
			lineGen=brGen.readLine();
		}
	
		brGen.close();
	}
}
