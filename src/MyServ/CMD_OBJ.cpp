#include "CMD_OBJ.h"
#include <stdio.h>
#include <string.h>

CMD_OBJ::CMD_OBJ()
{
	strcpy(szID,"");
	strcpy(szCmd,"");
	bIsBlock = false;
}
CMD_OBJ::~CMD_OBJ()
{

}
