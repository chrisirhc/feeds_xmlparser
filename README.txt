$Id$

A very simple Feeds plugin to parse XML files. The module uses the
XML parsing interface of http://drupal.org/project/xml_parser to
return a formatted PHP array.

Example
=======

1. Enable the Feeds, Feeds UI and Feeds XML Parser modules.
2. Create a new feeds importer, and select the XML Parser as parser.
3. Set a processor (e.g. node processor) and provide mapping. If you
use the example.xml of this module, you can do something like

  Item XPath : //book

	title => Title
	@id => GUID (unique)
	publish_date => Published date
	description => Body
	
4. Go to /import, upload the file example.xml and import. If everything
goes well, you should have 2 nodes with data from the XML.
