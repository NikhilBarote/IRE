import java.io.IOException;

import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;

public class Driver {
	public static void main(String[] args) throws SAXException, IOException {
		long s=System.currentTimeMillis();
		XMLReader p = XMLReaderFactory.createXMLReader();
		String file_Path = args[0];
		p.setContentHandler(new SaxParser());
		p.parse(file_Path);
		p.setContentHandler(new synonymExtraction());
		p.parse(file_Path);
		long e=System.currentTimeMillis();
		System.out.println("Total time:"+(e-s)/1000.0);
	}
}
