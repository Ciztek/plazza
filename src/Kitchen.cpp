#include "KitchenCatalog.hpp"

#include "Kitchen.hpp"

Kitchen::Kitchen(const KitchenCalatog &catalog, size_t ncook)
  : _catalog(catalog), _ncook(ncook)
{
}
