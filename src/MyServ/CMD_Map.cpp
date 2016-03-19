#include "CMD_Map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace std;


CMD_Map::CMD_Map()
{
	printf("CMD_Map created \n");		
}

CMD_Map::~CMD_Map()
{
	printf("CMD_Map destory \n");		
}

bool CMD_Map::Load(char* szXMLPath)
{

	bool bRet = false;
	printf("CMD_Map::Load\n");
	xmlChar *xpath = (xmlChar*) "/CMD_MAP_TABLE/CMD_MAP";//"//tile/@gid";
	//xmlNodeSetPtr nodeset = new xmlNodeSet();
	if(execute_xpath_expression((const char*)szXMLPath,xpath,NULL))
		bRet = true;
	else
		bRet = false;
	/*

	xmlParserCtxtPtr ctxt;
    	xmlDocPtr doc;

	ctxt = xmlNewParserCtxt();
	if (ctxt == NULL) 
	{
		fprintf(stderr, "Failed to allocate parser context\n");
		return bRet;
	}

	xmlInitParser();

	doc = xmlCtxtReadFile(ctxt, szXMLPath, NULL, XML_PARSE_DTDVALID);

	if (doc == NULL) {
		fprintf(stderr, "Failed to parse %s\n", szXMLPath);
	} 
	else 
	{
		//if (ctxt->valid == 0)
		//    fprintf(stderr, "Failed to validate %s\n", szXMLPath);

		//Do parse
		if(execute_xpath_expression(szXMLPath,"",))
		{
			//usage(argv[0]);
			return(-1);
		}


		xmlFreeDoc(doc);
	}
	xmlFreeParserCtxt(ctxt);


	xmlCleanupParser();
	xmlMemoryDump();

	*/
	return bRet;
}

/**
 * execute_xpath_expression:
 * @filename:		the input XML filename.
 * @xpathExpr:		the xpath expression for evaluation.
 * @nsList:		the optional list of known namespaces in
 *			"<prefix1>=<href1> <prefix2>=href2> ..." format.
 *
 * Parses input XML file, evaluates XPath expression and prints results.
 *
 * Returns 0 on success and a negative value otherwise.
 */
int CMD_Map::execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, const xmlChar* nsList)
{
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;

    assert(filename);
    assert(xpathExpr);

    /* Load XML document */
    doc = xmlParseFile(filename);
    if (doc == NULL) {
	fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
	return(-1);
    }

    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        return(-1);
    }

    /* Register namespaces from list (if any) */
    if((nsList != NULL) && (register_namespaces(xpathCtx, nsList) < 0)) {
        fprintf(stderr,"Error: failed to register namespaces list \"%s\"\n", nsList);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return(-1);
    }

    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return(-1);
    }

    /* Print results */
    print_xpath_nodes(xpathObj->nodesetval, stdout);

    /* Cleanup */
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return(0);
}

int CMD_Map::register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar* nsList)
{
    xmlChar* nsListDup;
    xmlChar* prefix;
    xmlChar* href;
    xmlChar* next;

    assert(xpathCtx);
    assert(nsList);

    nsListDup = xmlStrdup(nsList);
    if(nsListDup == NULL) {
	fprintf(stderr, "Error: unable to strdup namespaces list\n");
	return(-1);
    }

    next = nsListDup;
    while(next != NULL) {
	/* skip spaces */
	while((*next) == ' ') next++;
	if((*next) == '\0') break;

	/* find prefix */
	prefix = next;
	next = (xmlChar*)xmlStrchr(next, '=');
	if(next == NULL) {
	    fprintf(stderr,"Error: invalid namespaces list format\n");
	    xmlFree(nsListDup);
	    return(-1);
	}
	*(next++) = '\0';

	/* find href */
	href = next;
	next = (xmlChar*)xmlStrchr(next, ' ');
	if(next != NULL) {
	    *(next++) = '\0';
	}

	/* do register namespace */
	if(xmlXPathRegisterNs(xpathCtx, prefix, href) != 0) {
	    fprintf(stderr,"Error: unable to register NS with prefix=\"%s\" and href=\"%s\"\n", prefix, href);
	    xmlFree(nsListDup);
	    return(-1);
	}
    }

    xmlFree(nsListDup);
    return(0);
}

/**
 * print_xpath_nodes:
 * @nodes:		the nodes set.
 * @output:		the output file handle.
 *
 * Prints the @nodes content to @output.
 */
void CMD_Map::print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output) {
    xmlNodePtr cur;
    xmlNodePtr nodeName;
    xmlNodePtr nodeCMD;
    xmlNodePtr nodeValue;
    int size;
    int i;

    assert(output);
    size = (nodes) ? nodes->nodeNr : 0;

    fprintf(output, "Result (%d nodes):\n", size);
    for(i = 0; i < size; ++i) {
	assert(nodes->nodeTab[i]);

	if(nodes->nodeTab[i]->type == XML_NAMESPACE_DECL) {
	    xmlNsPtr ns;

	    ns = (xmlNsPtr)nodes->nodeTab[i];
	    cur = (xmlNodePtr)ns->next;
	    if(cur->ns) {
	        //fprintf(output, "= namespace \"%s\"=\"%s\" for node %s:%s\n",
		    //ns->prefix, ns->href, cur->ns->href, cur->name);
	    } else {
	        //fprintf(output, "= namespace \"%s\"=\"%s\" for node %s\n",
		    //ns->prefix, ns->href, cur->name);
	    }
	} else if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
	    cur = nodes->nodeTab[i];
	    if(cur->ns) {
    	        //fprintf(output, "= element node \"%s:%s\"\n",
		    //cur->ns->href, cur->name);
	    } else {
    	        //fprintf(output, "= element node \"%s\"\n",
		    //cur->name);
	    }


	    CMD_OBJ* pCMD_OBJ = new CMD_OBJ();
	    //Current Node is "CMD_MAP"
	    //xmlChar *keywordName;
	    nodeName = (xmlNodePtr)cur->children->next;
	    nodeCMD = (xmlNodePtr)nodeName->next->next;

	    nodeValue = nodeName->children;
	    //char* strName = "";
	    std::string strName((char*)nodeValue->content);
	    //fprintf(output, "= element value1 \"%s\"\n",nodeName->name);

	    strcpy(pCMD_OBJ->szID,(char*)nodeValue->content);
	    //fprintf(output, "= element value2 \"%s\"\n",nodeValue->content);



	    nodeValue = nodeCMD->children;

	    //fprintf(output, "= element value2 \"%s\"\n",nodeValue->content);
	    std::string strCMD((char*)nodeValue->content);
	    strcpy(pCMD_OBJ->szCmd,(char*)nodeValue->content);

	    //cmd_map.add(strName,strCMD);

	    //typedef pair<std::string, int>:: make_pair;
	    m_mapCMD[strName] = pCMD_OBJ;

	    //xmlChar *keywordCMD;
	} else {
	    cur = nodes->nodeTab[i];
	    //fprintf(output, "= node \"%s\": type %d\n", cur->name, cur->type);
	}
    }
}



CMD_OBJ* CMD_Map::GetCommand(char*szCmdID)
{
	std::string strName(szCmdID);
	std::map<std::string,CMD_OBJ*>::iterator i= m_mapCMD.find(strName);


	CMD_OBJ* pCMD_OBJ = NULL;
	if (i == m_mapCMD.end())
	{

	}
	else
	{
		pCMD_OBJ = (*i).second;
	}

	//CMD_OBJ* pCMD_OBJ = i->second;

	return pCMD_OBJ;
}



