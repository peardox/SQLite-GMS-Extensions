//Game Maker SQLite DLL
//Copyright (c) 2008 IsmAvatar <cmagicj@nni.com>
//Feel free to modify and/or redistribute, as your copy is *your* property, and not mine!

#define export __declspec (dllexport)
#define MAX 5
#define NULL 0
#define get(o) s ## o[(int) (o - 1)]
#include "sqlite3.h"

//Make SQLite object
//gcc -m32 -c -Wno-deprecated-declarations sqlite3.c

//Build DLL
//gcc -m32 -c -DBUILD_DLL hat.c
//gcc -m32 -shared -o hat.dll hat.o sqlite3.o

//Build EXE
//gcc -Wno-deprecated-declarations -o hat.exe hat.c sqlite3.o

sqlite3 *sdb[MAX];
sqlite3_stmt *sqry[MAX];

export double sopen(char *fn) {
 int i;
 for (i = 0; i < MAX; i++)
  if (sdb[i] == NULL)
   break;
 if (i == MAX) return 0.0;
 int r = sqlite3_open(fn,&sdb[i]);
 if (r) {
  sqlite3_close(sdb[i]);
  return (double) -r;
 }
 return (double) (i + 1);
}

export double sclose(double db) {
 double d = (double) -sqlite3_close(get(db));
 get(db) = NULL;
 return d;
}

export double serrcode(double db) {
 return (double) sqlite3_errcode(get(db));
}

export const char *serrmsg(double db) {
 return sqlite3_errmsg(get(db));
}

export double squery(double db, char *sql) {
 int j;
 for (j = 0; j < MAX; j++)
  if (sqry[j] == NULL)
   break;
 if (j == MAX) return 0.0;
 int r = sqlite3_prepare_v2(get(db),sql,-1,&sqry[j],NULL);
 if (r) {
  sqlite3_finalize(sqry[j]);
  return (double) -r;
 }
 return (double) (j + 1);
}

export double sfree(double qry) {
 sqlite3_finalize(get(qry));
 get(qry) = NULL;
 return 0.0;
}

export double sstep(double qry) {
 int i = sqlite3_step(get(qry));
 if (i == 100) return 1.0;
 if (i == 101) return 0.0;
 return (double) -i;
}

export double sreset(double qry) {
 return (double) -sqlite3_reset(get(qry));
}

export double sdouble(double qry, double col) {
 return sqlite3_column_double(get(qry),(int)col);
}

export const char *stext(double qry, double col) {
 return sqlite3_column_text(get(qry),(int)col);
}

export double schanges(double db) {
 return (double) sqlite3_changes(get(db));
}

export double slastid(double db) {
 return (double) sqlite3_last_insert_rowid(get(db));
}

#if 1
int main() {return 0;}
#else
void eqry(double db, char *sql) {
 printf("> %s\n",sql);
 double q = squery(db,sql);
 if (q < 0) {
  printf("Error %.f: %s\n",-q,serrmsg(db));
  return;
 }
 double r = sstep(q);
 if (r < 0) printf("Error %.f: %s\n",-r,serrmsg(db));
 else printf("Done. %.f rows affected.\n",schanges(db));
 sfree(q);
}

int main() {
 double d = sopen("db.db");
 if (d < 0) {
  printf("Cannot open database. Error %.f\n",d);
  return 0;
 }
 printf("Opened database %.f\n",d);
 eqry(d,"DROP TABLE IF EXISTS t1");
 eqry(d,"CREATE TABLE t1 (a INTEGER PRIMARY KEY, b INTEGER)");
 eqry(d,"INSERT INTO t1 VALUES (1,123)");
 eqry(d,"INSERT INTO t1 VALUES (2,50)");
 eqry(d,"INSERT INTO t1 VALUES (NULL,75)");
 printf("Last id: %.f\n",slastid(d)); //3

 printf("> SELECT * FROM t1 ORDER BY b\n");
 double q = squery(d,"SELECT * FROM t1 ORDER BY b");
 if (q < 0) {
  printf("Error %.f: %s\n",-q,serrmsg(d));
  sclose(d);
  return 0;
 }

 int c = 0;
 double r;
 while ((r = sstep(q)) != 0.0) {
  printf("%.f %.f\n",sdouble(q,0),sdouble(q,1));
  c++;
 }

 printf("Done. %d rows fetched.\nAll queries finished. Terminating\n",c);
 sfree(q);
 sclose(d);
 return 0;
}
#endif
