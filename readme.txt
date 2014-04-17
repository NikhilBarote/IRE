Mining The Named Entites from Wikipedia

Functions And Their Explanations:

Driver Class:
	1. Main Function: Arguments: <Xml file path> 
			  Set the xml parser content handler and call the SaxParser.java and snonymExtraction.java.

Saxparser Class:
	1. StartElement(): Arguments: String uri, String localName, String qName, Attributes attributes
			   Parse the xml  file and checks if the various tags are open are not for ex: page tag, title tag...etc and then pass the 		control to characters()

	2. characters(): Arguments: char[] ch, int start, int length
			 It takes text between the specified tag and stores into the character array. If text tag is open it calls the parseCategory() function. If title tag is open it calls isEntity() with title as argument.

	3. isEntity() : Arguments: String titleString
			It returns if the title is entity or not
	
	4. parseCategory() : Arguments: String textString, String titleString
			     It parses category in <text> tag and decides whether title is person, location or organization

	5. writeToCategoryFile(): Argument: TreeMap<String, String>
				  Create category based index of entities
	
	6. writeToGenericFile() : Argument: TreeMap<String,String>
				  Create rule based index of entities.	

synonymExtraction Class:

	1. parseSynonyms(): Argument: String textstring
			    Parses xml file to get external links, internal links and redirect links.
	
	2. extractSynonms(): Argument: String link
			     Fetches the synonyms from the extracted links.

	3. bringToMemory(): Argument:
	              	    Brings the generic entity file to memory.

	4. mergeMaps(): Argument: TreeMap<String, String>, TreeMap<String, String>
			Creates Knowledge base (KB) by merging the synonym and entity maps.

Extracting named entites from query document:

	1. offset_comp(l,r): compare function for binary search on word to offset file i,e, secondary index
 
	2. get_offset(word): returns offset of word in primary index (0-body;1-category;2-links;3-infobox;4-title)
 
	3. split(s, delim): separate s using delim
 
	4. get_category(title): extracts category using title
 
	5. get_options(query_file): takes query document and writes resultant article with links to wiki pages to "out.txt" file		 
