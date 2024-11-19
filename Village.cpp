#include "Village.h"

Village::Village() {
	categoryName = "Village";
	people = data.config["StartingResources"]["Village"];
}
