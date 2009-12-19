
#define ARTIST_LEN 26
#define TITLE_LEN  41

typedef struct entry {

     int time_m;

     int time_s;

     int tracks;

     struct db_entry* artist_next;

     struct db_entry* title_next;

     char title[TITLE_LEN];

     char artist[ARTist_LEN]
};

