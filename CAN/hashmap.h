#ifndef __HASHMAP_H__
#define __HASHMAP_H__


typedef struct SHashmapList 
{
  char * szKey;
  void * pData;
  struct SHashmapList * ptNext;
} THashmapList;

typedef unsigned int (* FHash)(const char * szKey, unsigned int uArraySize);

typedef struct SHashmap 
{
  unsigned int uArraySize;
  THashmapList ** aptHashmapList;
  FHash HashFunction;
} THashmap;


THashmap * NEW_Hashmap(unsigned int uArraySize, FHash HashFunction);
void CLEAR_Hashmap(THashmap * ptHashmap, unsigned int uArraySize);
void DESTOY_Hashmap(THashmap ** pptHashmap);

void Hashmap_AddElement(THashmap * ptHashmap, const char * szKey, void * pData);
void * Hashmap_GetElement(THashmap * ptHashmap, const char * szKey);
void Hashmap_DeleteElement(THashmap * ptHashmap, const char * szKey);

unsigned int Hash_Firstchar(const char * szKey, unsigned int uArraySize);
unsigned int Hash_Charsum(const char * szKey, unsigned int uArraySize);

#endif//__HASHMAP_H__
