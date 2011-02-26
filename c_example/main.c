// Copyright 2011, Lightbox Technologies, Inc. All Rights Reserved.
#include "lightgrep_c_api.h"

#include <stdio.h>
#include <string.h>

// callback function that receives the search hit
void getHit(const LG_SearchHit* const hit) {
  printf("hit: (%llu, %llu, %u)\n", hit->Offset, hit->Length, hit->KeywordIndex);
}

static const uint32 NUM_KEYS = 3;

void searchText(char** textArray, unsigned int numStrings, LG_HCONTEXT searcher) {
  // reset the search context for re-use (not necessary on first use, but harmless)
  lg_reset_context(searcher);
  uint64 offset = 0,
         len;

  // search text as if it were a contiguous array
  unsigned int i;
  for (i = 0; i < numStrings; ++i) {
    len = strlen(textArray[i]);
    lg_search(searcher, textArray[i], textArray[i] + len, offset, getHit);
    offset += len;
  }
  // flush any remaining hits -- done once at end of file
  lg_closeout_search(searcher, getHit);
}

int main() {
  // create a parser
  LG_HPARSER    parser = lg_create_parser(0);
  LG_KeyOptions keyOpts;
  char *keys[] = {"Mary", "lamb", "[a-z]+"};
  const char* errString = "";

  // add the keywords to the parser one at a time
  int isgood = 1;
  unsigned int i;
  for (i = 0; i < NUM_KEYS; ++i) {
    if (!lg_add_keyword(parser, keys[i], i, &keyOpts, &errString)) {
      fprintf(stderr, "Parser error on keyword %d, %s: %s", i, keys[i], errString);
      isgood = 0;
      break;
    }
  }
  if (isgood) {
    // create a "program" from the parsed keywords
    LG_ProgramOptions opts;
    LG_HPROGRAM prog = lg_create_program(parser, &opts);

    // discard the parser now that we have a program
    lg_destroy_parser(parser);
    parser = 0;

    // create a search context
    LG_HCONTEXT searcher = lg_create_context(prog);

    printf("beginning search\n");

    // search texts as if they were a contiguous array
    char *mary[2] = {"Mary had a little l", "amb"},
         *twinkle[3] = {"Twin", "kle twink", "le, little star"};
    
    printf("searching 'Mary had a little lamb'\n");
    
    searchText(mary, 2, searcher);
  
    printf("searching 'Twinkle twinkle, little star'\n");
    
    searchText(twinkle, 3, searcher);

    printf("done with search\n");

    lg_destroy_context(searcher);
    lg_destroy_program(prog);
    return 0;
  }
  return 1;
}
