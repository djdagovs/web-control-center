#include "libxml/xmlIO.h"
#include "libxml/xinclude.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <hash_map>
#include <map>
#include <string>
#include "CMD_OBJ.h"

using namespace std;


//#include <ext/hash_map>
//namespace std { using namespace __gnu_cxx; }



class CMD_Map
{	
public:
	CMD_Map();
	~CMD_Map();

	map<string, CMD_OBJ*> m_mapCMD;
	bool Load(char* szXMLPath);
	int execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, const xmlChar* nsList);
	int register_namespaces(xmlXPathContextPtr xpathCtx, const xmlChar* nsList);
	void print_xpath_nodes(xmlNodeSetPtr nodes, FILE* output);

	CMD_OBJ* GetCommand(char*szCmdID);
};
