/* Generated Worgle Amalgamation: do not edit manually */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef WORGLE_FULLVERSION
#define WORGLITE
#endif

/*
 * parg - parse argv
 *
 * Written in 2015-2016 by Joergen Ibsen
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the
 * public domain worldwide. This software is distributed without any
 * warranty. <http://creativecommons.org/publicdomain/zero/1.0/>
 */

#ifndef PARG_H_INCLUDED
#define PARG_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define PARG_VER_MAJOR 1        /**< Major version number */
#define PARG_VER_MINOR 0        /**< Minor version number */
#define PARG_VER_PATCH 2        /**< Patch version number */
#define PARG_VER_STRING "1.0.2" /**< Version number as a string */

/**
 * Structure containing state between calls to parser.
 *
 * @see parg_init
 */
struct parg_state {
	const char *optarg;   /**< Pointer to option argument, if any */
	int optind;           /**< Next index in argv to process */
	int optopt;           /**< Option value resulting in error, if any */
	const char *nextchar; /**< Next character to process */
};

/**
 * Structure for supplying long options to `parg_getopt_long()`.
 *
 * @see parg_getopt_long
 */
struct parg_option {
	const char *name; /**< Name of option */
	int has_arg;      /**< Option argument status */
	int *flag;        /**< Pointer to flag variable */
	int val;          /**< Value of option */
};

/**
 * Values for `has_arg` flag in `parg_option`.
 *
 * @see parg_option
 */
typedef enum {
	PARG_NOARG,  /**< No argument */
	PARG_REQARG, /**< Required argument */
	PARG_OPTARG  /**< Optional argument */
} parg_arg_num;

/**
 * Initialize `ps`.
 *
 * Must be called before using state with a parser.
 *
 * @see parg_state
 *
 * @param ps pointer to state
 */
void
parg_init(struct parg_state *ps);

/**
 * Parse next short option in `argv`.
 *
 * Elements in `argv` that contain short options start with a single dash
 * followed by one or more option characters, and optionally an option
 * argument for the last option character. Examples are '`-d`', '`-ofile`',
 * and '`-dofile`'.
 *
 * Consecutive calls to this function match the command-line arguments in
 * `argv` against the short option characters in `optstring`.
 *
 * If an option character in `optstring` is followed by a colon, '`:`', the
 * option requires an argument. If it is followed by two colons, the option
 * may take an optional argument.
 *
 * If a match is found, `optarg` points to the option argument, if any, and
 * the value of the option character is returned.
 *
 * If a match is found, but is missing a required option argument, `optopt`
 * is set to the option character. If the first character in `optstring` is
 * '`:`', then '`:`' is returned, otherwise '`?`' is returned.
 *
 * If no option character in `optstring` matches a short option, `optopt`
 * is set to the option character, and '`?`' is returned.
 *
 * If an element of argv does not contain options (a nonoption element),
 * `optarg` points to the element, and `1` is returned.
 *
 * An element consisting of a single dash, '`-`', is returned as a nonoption.
 *
 * Parsing stops and `-1` is returned, when the end of `argv` is reached, or
 * if an element contains '`--`'.
 *
 * Works similarly to `getopt`, if `optstring` were prefixed by '`-`'.
 *
 * @param ps pointer to state
 * @param argc number of elements in `argv`
 * @param argv array of pointers to command-line arguments
 * @param optstring string containing option characters
 * @return option value on match, `1` on nonoption element, `-1` on end of
 * arguments, '`?`' on unmatched option, '`?`' or '`:`' on option argument
 * error
 */
int
parg_getopt(struct parg_state *ps, int argc, char *const argv[],
            const char *optstring);

/**
 * Parse next long or short option in `argv`.
 *
 * Elements in `argv` that contain a long option start with two dashes
 * followed by a string, and optionally an equal sign and an option argument.
 * Examples are '`--help`' and '`--size=5`'.
 *
 * If no exact match is found, an unambiguous prefix of a long option will
 * match. For example, if '`foo`' and '`foobar`' are valid long options, then
 * '`--fo`' is ambiguous and will not match, '`--foo`' matches exactly, and
 * '`--foob`' is an unambiguous prefix and will match.
 *
 * If a long option match is found, and `flag` is `NULL`, `val` is returned.
 *
 * If a long option match is found, and `flag` is not `NULL`, `val` is stored
 * in the variable `flag` points to, and `0` is returned.
 *
 * If a long option match is found, but is missing a required option argument,
 * or has an option argument even though it takes none, `optopt` is set to
 * `val` if `flag` is `NULL`, and `0` otherwise. If the first character in
 * `optstring` is '`:`', then '`:`' is returned, otherwise '`?`' is returned.
 *
 * If `longindex` is not `NULL`, the index of the entry in `longopts` that
 * matched is stored there.
 *
 * If no long option in `longopts` matches a long option, '`?`' is returned.
 *
 * Handling of nonoptions and short options is like `parg_getopt()`.
 *
 * If no short options are required, an empty string, `""`, should be passed
 * as `optstring`.
 *
 * Works similarly to `getopt_long`, if `optstring` were prefixed by '`-`'.
 *
 * @see parg_getopt
 *
 * @param ps pointer to state
 * @param argc number of elements in `argv`
 * @param argv array of pointers to command-line arguments
 * @param optstring string containing option characters
 * @param longopts array of `parg_option` structures
 * @param longindex pointer to variable to store index of matching option in
 * @return option value on match, `0` for flag option, `1` on nonoption
 * element, `-1` on end of arguments, '`?`' on unmatched or ambiguous option,
 * '`?`' or '`:`' on option argument error
 */
int
parg_getopt_long(struct parg_state *ps, int argc, char *const argv[],
                 const char *optstring,
                 const struct parg_option *longopts, int *longindex);

/**
 * Reorder elements of `argv` so options appear first.
 *
 * If there are no long options, `longopts` may be `NULL`.
 *
 * The return value can be used as `argc` parameter for `parg_getopt()` and
 * `parg_getopt_long()`.
 *
 * @param argc number of elements in `argv`
 * @param argv array of pointers to command-line arguments
 * @param optstring string containing option characters
 * @param longopts array of `parg_option` structures
 * @return index of first nonoption in `argv` on success, `-1` on error
 */
int
parg_reorder(int argc, char *argv[],
             const char *optstring,
             const struct parg_option *longopts);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PARG_H_INCLUDED */
#ifndef WORGLE_H
#define WORGLE_H
typedef struct worgle_d worgle_d;
typedef struct worgle_string worgle_string;
typedef struct worgle_segment worgle_segment;
typedef struct worgle_block worgle_block;
typedef struct worgle_blocklist worgle_blocklist;
typedef struct worgle_hashmap worgle_hashmap;
typedef struct worgle_file worgle_file;
typedef struct worgle_filelist worgle_filelist;
typedef struct worgle_textbuf worgle_textbuf;
typedef unsigned long worgle_long;
typedef struct worgle_orgfile worgle_orgfile;
typedef struct worgle_orglet worgle_orglet;
typedef struct worgle_orglet_header worgle_orglet_header;
typedef struct worgle_orglet_content worgle_orglet_content;
typedef struct worgle_orglet_blkref worgle_orglet_blkref;
int parse_file(worgle_d *worg, int file);
void worgle_begin_block(worgle_d *worg, worgle_string *name);
void worgle_append_file(worgle_d *worg, worgle_string *filename);
void worgle_append_string(worgle_d *worg);
void worgle_append_reference(worgle_d *worg, worgle_string *ref);
int worgle_generate(worgle_d *worg);
void worgle_init(worgle_d *worg);
void worgle_free(worgle_d *worg);
void worgle_string_reset(worgle_string *str);
void worgle_string_init(worgle_string *str);
int worgle_string_write(FILE *fp, worgle_string *str);
void worgle_segment_init(worgle_segment *s,
                        int type,
                        worgle_string *str,
                        worgle_string *filename,
                        size_t linum);
int worgle_segment_write(worgle_segment *s, worgle_hashmap *h, FILE *fp);
int worgle_segment_is_text(worgle_segment *s);
int worgle_segment_is_reference(worgle_segment *s);
void worgle_block_init(worgle_block *b);
void worgle_block_free(worgle_block *lst);
worgle_segment* worgle_block_append_segment(worgle_block *b,
                                            worgle_string *str,
                                            int type,
                                            size_t linum,
                                            worgle_string *filename);
worgle_segment* worgle_block_append_string(worgle_block *b,
                               worgle_string *str,
                               size_t linum,
                               worgle_string *filename);
worgle_segment* worgle_block_append_reference(worgle_block *b,
                                              worgle_string *str,
                                              size_t linum,
                                              worgle_string *filename);
int worgle_block_write(worgle_block *b, worgle_hashmap *h, FILE *fp);
void worgle_blocklist_init(worgle_blocklist *lst);
void worgle_blocklist_free(worgle_blocklist *lst);
void worgle_blocklist_append(worgle_blocklist *lst, worgle_block *b);
void worgle_hashmap_init(worgle_hashmap *h);
void worgle_hashmap_free(worgle_hashmap *h);
int worgle_hashmap_find(worgle_hashmap *h, worgle_string *name, worgle_block **b);
worgle_block * worgle_hashmap_get(worgle_hashmap *h, worgle_string *name);
int worgle_file_write(worgle_file *f, worgle_hashmap *h);
void worgle_filelist_init(worgle_filelist *flist);
void worgle_filelist_free(worgle_filelist *flist);
worgle_file* worgle_filelist_append(worgle_filelist *flist,
                           worgle_string *name,
                           worgle_block *top);
int worgle_filelist_write(worgle_filelist *flist, worgle_hashmap *h);
void worgle_textbuf_zero(worgle_textbuf *txt);
void worgle_textbuf_init(worgle_textbuf *txt,
                         char *buf,
                         size_t bufsize);
void worgle_textbuf_free(worgle_textbuf *txt);
worgle_long worgle_uuid_new(worgle_d *worg);
worgle_long worgle_segment_id_get(worgle_segment *s);
void worgle_segment_id_set(worgle_segment *s, worgle_long id);
worgle_long worgle_block_id_get(worgle_block *b);
void worgle_block_id_set(worgle_block *b, worgle_long id);
void worgle_file_id_set(worgle_file *f, worgle_long id);
void worgle_orglet_id_set(worgle_orglet *orglet,
                           worgle_long id);
void worgle_orgfile_init(worgle_orgfile *org);
void worgle_orgfile_free(worgle_orgfile *org);
void worgle_orgfile_append(worgle_d *worg,
                           worgle_orgfile *file,
                           worgle_orglet *orglet);
void worgle_orgfile_append_header(worgle_d *worg,
                                  worgle_string *header,
                                  int lvl);
void worgle_orgfile_append_content(worgle_d *worg,
                                   worgle_string *text);
void worgle_orgfile_append_reference(worgle_d *worg,
                                     worgle_block *blk);
void worgle_orglet_init(worgle_orglet *orglet);
void worgle_orglet_free(worgle_orglet *orglet);
int worgle_warn_unused(worgle_d *worg);
void worgle_map(worgle_d *worg, worgle_block *b, int lvl, FILE *out);
void worgle_map_files(worgle_d *worg, char *filename);
void worgle_map_a_file(worgle_d *worg, worgle_file *file, FILE *out);
#endif
#ifndef WORGLE_PRIVATE_H
struct worgle_string {
    char *str;
    size_t size;
};
enum {
SEGTYPE_TEXT,
SEGTYPE_REFERENCE
};
struct worgle_segment {
    int type;
    worgle_string str;
    size_t linum;
    worgle_string *filename;
    worgle_segment *nxt;
    worgle_long id;
};
struct worgle_block {
    int nsegs;
    worgle_segment *head;
    worgle_segment *tail;
    worgle_string name;
    int am_i_used;
    worgle_block *nxt;
    int nblocks;
    int last_seg;
    worgle_long id;
};
struct worgle_blocklist {
    int nblocks;
    worgle_block *head;
    worgle_block *tail;
};
#define HASH_SIZE 256
struct worgle_hashmap {
    worgle_blocklist blk[HASH_SIZE];
    int nwords;
};
struct worgle_file {
    worgle_string filename;
    worgle_block *top;
    worgle_file *nxt;
    worgle_long id;
};
struct worgle_filelist {
    worgle_file *head;
    worgle_file *tail;
    int nfiles;
};
struct worgle_textbuf {
    char *buf;
    size_t size;
    worgle_string filename;
};
struct worgle_orglet_header {
    worgle_string str;
    int lvl;
};
struct worgle_orglet_content {
    worgle_string text;
};
struct worgle_orglet_blkref {
    worgle_block *blk;
    int pos;
    int prev_lastseg;
    int segoff;
};
struct worgle_orglet {
    int type;
    void *ud;
    worgle_orglet *next;
    size_t linum;
    worgle_long id;
};
struct worgle_orgfile {
    worgle_orglet *head;
    worgle_orglet *tail;
    worgle_orgfile *prev;
    int size;
};
struct worgle_d {
    worgle_string block;
    #ifndef WORGLITE
    worgle_string segblock;
    #endif
    long curline;
    int block_started;
    #ifndef WORGLITE
    int new_content;
    #endif
    worgle_hashmap dict;
    worgle_filelist flist;
    worgle_textbuf *curbuf;
    worgle_textbuf *buffers;
    int nbuffers;
    worgle_block *curblock;
    size_t linum;
    worgle_orgfile *orgs;
    worgle_orgfile *curorg;
    int prog;
    worgle_long UUID_count;
};
#define WORGLE_PRIVATE_H
#endif
/*
 * parg - parse argv
 *
 * Written in 2015-2016 by Joergen Ibsen
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the
 * public domain worldwide. This software is distributed without any
 * warranty. <http://creativecommons.org/publicdomain/zero/1.0/>
 */



/*
 * Check if state is at end of argv.
 */
static int
is_argv_end(const struct parg_state *ps, int argc, char *const argv[])
{
	return ps->optind >= argc || argv[ps->optind] == NULL;
}

/*
 * Match nextchar against optstring.
 */
static int
match_short(struct parg_state *ps, int argc, char *const argv[],
            const char *optstring)
{
	const char *p = strchr(optstring, *ps->nextchar);

	if (p == NULL) {
		ps->optopt = *ps->nextchar++;
		return '?';
	}

	/* If no option argument, return option */
	if (p[1] != ':') {
		return *ps->nextchar++;
	}

	/* If more characters, return as option argument */
	if (ps->nextchar[1] != '\0') {
		ps->optarg = &ps->nextchar[1];
		ps->nextchar = NULL;
		return *p;
	}

	/* If option argument is optional, return option */
	if (p[2] == ':') {
		return *ps->nextchar++;
	}

	/* Option argument required, so return next argv element */
	if (is_argv_end(ps, argc, argv)) {
		ps->optopt = *ps->nextchar++;
		return optstring[0] == ':' ? ':' : '?';
	}

	ps->optarg = argv[ps->optind++];
	ps->nextchar = NULL;
	return *p;
}

/*
 * Match string at nextchar against longopts.
 */
static int
match_long(struct parg_state *ps, int argc, char *const argv[],
           const char *optstring,
           const struct parg_option *longopts, int *longindex)
{
	size_t len;
	int num_match = 0;
	int match = -1;
	int i;

	len = strcspn(ps->nextchar, "=");

	for (i = 0; longopts[i].name; ++i) {
		if (strncmp(ps->nextchar, longopts[i].name, len) == 0) {
			match = i;
			num_match++;
			/* Take if exact match */
			if (longopts[i].name[len] == '\0') {
				num_match = 1;
				break;
			}
		}
	}

	/* Return '?' on no or ambiguous match */
	if (num_match != 1) {
		ps->optopt = 0;
		ps->nextchar = NULL;
		return '?';
	}

	assert(match != -1);

	if (longindex) {
		*longindex = match;
	}

	if (ps->nextchar[len] == '=') {
		/* Option argument present, check if extraneous */
		if (longopts[match].has_arg == PARG_NOARG) {
			ps->optopt = longopts[match].flag ? 0 : longopts[match].val;
			ps->nextchar = NULL;
			return optstring[0] == ':' ? ':' : '?';
		}
		else {
			ps->optarg = &ps->nextchar[len + 1];
		}
	}
	else if (longopts[match].has_arg == PARG_REQARG) {
		/* Option argument required, so return next argv element */
		if (is_argv_end(ps, argc, argv)) {
			ps->optopt = longopts[match].flag ? 0 : longopts[match].val;
			ps->nextchar = NULL;
			return optstring[0] == ':' ? ':' : '?';
		}

		ps->optarg = argv[ps->optind++];
	}

	ps->nextchar = NULL;

	if (longopts[match].flag != NULL) {
		*longopts[match].flag = longopts[match].val;
		return 0;
	}

	return longopts[match].val;
}

void
parg_init(struct parg_state *ps)
{
	ps->optarg = NULL;
	ps->optind = 1;
	ps->optopt = '?';
	ps->nextchar = NULL;
}

int
parg_getopt(struct parg_state *ps, int argc, char *const argv[],
            const char *optstring)
{
	return parg_getopt_long(ps, argc, argv, optstring, NULL, NULL);
}

int
parg_getopt_long(struct parg_state *ps, int argc, char *const argv[],
                 const char *optstring,
                 const struct parg_option *longopts, int *longindex)
{
	assert(ps != NULL);
	assert(argv != NULL);
	assert(optstring != NULL);

	ps->optarg = NULL;

	if (argc < 2) {
		return -1;
	}

	/* Advance to next element if needed */
	if (ps->nextchar == NULL || *ps->nextchar == '\0') {
		if (is_argv_end(ps, argc, argv)) {
			return -1;
		}

		ps->nextchar = argv[ps->optind++];

		/* Check for nonoption element (including '-') */
		if (ps->nextchar[0] != '-' || ps->nextchar[1] == '\0') {
			ps->optarg = ps->nextchar;
			ps->nextchar = NULL;
			return 1;
		}

		/* Check for '--' */
		if (ps->nextchar[1] == '-') {
			if (ps->nextchar[2] == '\0') {
				ps->nextchar = NULL;
				return -1;
			}

			if (longopts != NULL) {
				ps->nextchar += 2;

				return match_long(ps, argc, argv, optstring,
				                  longopts, longindex);
			}
		}

		ps->nextchar++;
	}

	/* Match nextchar */
	return match_short(ps, argc, argv, optstring);
}

/*
 * Reverse elements of `v` from `i` to `j`.
 */
static void
reverse(char *v[], int i, int j)
{
	while (j - i > 1) {
		char *tmp = v[i];
		v[i] = v[j - 1];
		v[j - 1] = tmp;
		++i;
		--j;
	}
}

/*
 * Reorder elements of `argv` with no special cases.
 *
 * This function assumes there is no `--` element, and the last element
 * is not an option missing a required argument.
 *
 * The algorithm is described here:
 * http://hardtoc.com/2016/11/07/reordering-arguments.html
 */
static int
parg_reorder_simple(int argc, char *argv[],
                    const char *optstring,
                    const struct parg_option *longopts)
{
	struct parg_state ps;
	int change;
	int l = 0;
	int m = 0;
	int r = 0;

	if (argc < 2) {
		return argc;
	}

	do {
		int nextind;
		int c;

		parg_init(&ps);

		nextind = ps.optind;

		/* Parse until end of argument */
		do {
			c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
		} while (ps.nextchar != NULL && *ps.nextchar != '\0');

		change = 0;

		do {
			/* Find next non-option */
			for (l = nextind; c != 1 && c != -1;) {
				l = ps.optind;

				do {
					c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
				} while (ps.nextchar != NULL && *ps.nextchar != '\0');
			}

			/* Find next option */
			for (m = l; c == 1;) {
				m = ps.optind;

				do {
					c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
				} while (ps.nextchar != NULL && *ps.nextchar != '\0');
			}

			/* Find next non-option */
			for (r = m; c != 1 && c != -1;) {
				r = ps.optind;

				do {
					c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
				} while (ps.nextchar != NULL && *ps.nextchar != '\0');
			}

			/* Find next option */
			for (nextind = r; c == 1;) {
				nextind = ps.optind;

				do {
					c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);
				} while (ps.nextchar != NULL && *ps.nextchar != '\0');
			}

			if (m < r) {
				change = 1;
				reverse(argv, l, m);
				reverse(argv, m, r);
				reverse(argv, l, r);
			}
		} while (c != -1);
	} while (change != 0);

	return l + (r - m);
}

int
parg_reorder(int argc, char *argv[],
             const char *optstring,
             const struct parg_option *longopts)
{
	struct parg_state ps;
	int lastind;
	int optend;
	int c;

	assert(argv != NULL);
	assert(optstring != NULL);

	if (argc < 2) {
		return argc;
	}

	parg_init(&ps);

	/* Find end of normal arguments */
	do {
		lastind = ps.optind;

		c = parg_getopt_long(&ps, argc, argv, optstring, longopts, NULL);

		/* Check for trailing option with error */
		if ((c == '?' || c == ':') && is_argv_end(&ps, argc, argv)) {
			lastind = ps.optind - 1;
			break;
		}
	} while (c != -1);

	optend = parg_reorder_simple(lastind, argv, optstring, longopts);

	/* Rotate `--` or trailing option with error into position */
	if (lastind < argc) {
		reverse(argv, optend, lastind);
		reverse(argv, optend, lastind + 1);
		++optend;
	}

	return optend;
}
#ifndef WORGLITE
#endif
#ifndef WORGLITE
#endif

static int use_debug = 0;
static int use_warnings = 0;
static int map_source_code = 0;
static char *map_filename = NULL;
#ifndef WORGLITE
static int generate_db = 0;
static char *db_filename = NULL;
#endif
static int tangle_code = 1;
enum {
MODE_ORG,
MODE_BEGINCODE,
MODE_CODE
};
enum {
ORGLET_HEADER,
ORGLET_CONTENT,
ORGLET_BLKREF,
ORGLET_UNKNOWN
};
static int loadfile(worgle_d *worg, int file);
static int parse_name(char *line, size_t len, worgle_string *str);
#ifndef WORGLITE
static int parse_header(worgle_d *worg,
                        char *line,
                        size_t len);
#endif
static int parse_begin(char *line, size_t len, worgle_string *str);
static int check_for_reference(char *line , size_t size, worgle_string *str);
static int worgle_getline(char *fullbuf,
                  char **line,
                  size_t *pos,
                  size_t *line_size,
                  size_t buf_size);
static void append_filename(worgle_d *worg, char *filename);
#ifndef WORGLITE
static void tangle_to_db(worgle_d *worg);
#endif
static int loadfile(worgle_d *worg, int file)
{
    FILE *fp;
    char *filename;
    worgle_textbuf *txt;
    size_t size;
    char *buf;
    txt = &worg->buffers[file];
    filename = txt->filename.str;
    fp = fopen(filename, "r");
    
    if(fp == NULL) {
        fprintf(stderr, "Could not find file %s\n", filename);
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    buf = calloc(1, size);
    worgle_textbuf_init(&worg->buffers[file], buf, size);
    fseek(fp, 0, SEEK_SET);
    fread(buf, size, 1, fp);
    fclose(fp);
    return 1;
}
int parse_file(worgle_d *worg, int file)
{
    char *buf;
    size_t size;
    worgle_textbuf *curbuf;
    worgle_string str;
    char *line;
    size_t pos;
    size_t read;
    int mode;
    int rc;
    int status;

    curbuf = &worg->buffers[file];
    buf = curbuf->buf;
    size = curbuf->size;
    worg->curbuf = curbuf;
#ifndef WORGLITE
    worg->curorg = &worg->orgs[file];
    if (file > 0) {
        worg->curorg->prev = &worg->orgs[file - 1];
    } else {
        worg->curorg->prev = NULL;
    }
#endif
    worgle_string_init(&str);
    line = NULL;
    pos = 0;
    mode = MODE_ORG;
    rc = 0;
    status = 0;
    worg->linum = 0;
    while (1) {
        worg->linum++;
        status = worgle_getline(buf, &line, &pos, &read, size);
        if(!status) break;
        if(mode == MODE_ORG) {
            #ifndef WORGLITE
            if (generate_db) {
                if (read >= 2) {
                    if (parse_header(worg, line, read)) {
                        continue;
                    }
                }
            }
            #endif
            if(read >= 7) {
                if(!strncmp(line, "#+NAME", 6)) {
            #ifndef WORGLITE
                    if (generate_db) {
                        worgle_orgfile_append_content(worg, &worg->segblock);
                        worgle_string_reset(&worg->segblock);
                    }
            #endif
                    if(line[6] != ':') {
                        fprintf(stderr,
                                "line %lu: expected ':'\n",
                                worg->linum);
                        rc = 1;
                        break;
                    }
                    mode = MODE_BEGINCODE;
                    parse_name(line, read, &str);
                    worgle_begin_block(worg, &str);
            #ifndef WORGLITE
                    continue;
            #endif
                }
            }
            #ifndef WORGLITE
            if (generate_db) {
                #ifndef WORGLITE
                if (worg->new_content) {
                    worg->new_content = 0;
                    worgle_string_reset(&worg->segblock);
                    worg->segblock.str = line;
                }
                worg->segblock.size += read;
                #endif
            }
            #endif
        } else if(mode == MODE_CODE) {
            if(read >= 9) {
                if(!strncmp(line, "#+END_SRC", 9)) {
                    mode = MODE_ORG;
                    worg->block_started = 0;
                    worgle_append_string(worg);
            #ifndef WORGLITE
                    worg->new_content = 1;
            #endif
                    continue;
                }
            }
            
            if(check_for_reference(line, read, &str)) {
                worgle_append_string(worg);
                worgle_append_reference(worg, &str);
                worg->block_started = 1;
                worgle_string_reset(&worg->block);
                continue;
            }
            
            worg->block.size += read;
            
            if(worg->block_started) {
                worg->block.str = line;
                worg->block_started = 0;
                worg->curline = worg->linum;
            }
        } else if(mode == MODE_BEGINCODE) {
            if (read >= 11) {
                if(!strncmp (line, "#+BEGIN_SRC",11)) {
                    mode = MODE_CODE;
                    worg->block_started = 1;
                    worgle_string_reset(&worg->block);
                    if (parse_begin(line, read, &str) == 2) {
                        worgle_append_file(worg, &str);
                    }
                    continue;
                } else {
                    fwrite(line, read, 1, stderr);
                    fprintf(stderr,
                            "line %lu: Expected #+BEGIN_SRC\n",
                            worg->linum);
                    rc = 1;
                    break;
                }
            }
            fprintf(stderr,
                    "line %lu: Expected #+BEGIN_SRC\n",
                    worg->linum);
            rc = 1;
        }
    }
    return rc;
}
static int parse_name(char *line, size_t len, worgle_string *str)
{
    size_t n;
    size_t pos;
    int mode;

    line+=7;
    len-=7;
    /* *namelen = 0; */
    str->size = 0;
    str->str = NULL;
    if(len <= 0) return 1;
    pos = 0;
    mode = 0;
    for(n = 0; n < len; n++) {
        if(mode == 2) break;
        switch(mode) {
            case 0:
                if(line[n] == ' ') {

                } else {
                    str->str = &line[n];
                    str->size++;
                    pos++;
                    mode = 1;
                }
                break;
            case 1:
                if(line[n] == 0xa) {
                    mode = 2;
                    break;
                }
                pos++;
                str->size++;
                break;
            default:
                break;
        }
    }
    /* *namelen = pos; */
    return 1;
}
void worgle_begin_block(worgle_d *worg, worgle_string *name)
{
    worg->curblock = worgle_hashmap_get(&worg->dict, name);
    if (worgle_block_id_get(worg->curblock) == 0) {
        worgle_block_id_set(worg->curblock, worgle_uuid_new(worg));
    }
    worg->curblock->nblocks++;
#ifndef WORGLITE
    worgle_orgfile_append_reference(worg, worg->curblock);
#endif
}
#ifndef WORGLITE
static int parse_header(worgle_d *worg,
                        char *line,
                        size_t len)
{
    int mode;
    int rc;
    size_t s;
    char *header;
    worgle_string str;
    int lvl;
    mode = 0;

    if(line[0] != '*') return 0;

    rc = 0;
    worgle_string_init(&str);
    lvl = 1;
    for (s = 1; s < len; s++) {
        if (mode == 2) break;
        switch (mode) {
            case 0:
                if (line[s] == '*') {
                    lvl++;
                } else if (line[s] == ' '){
                    mode = 1;
                } else {
                    mode = 2;
                    rc = 0;
                }
                break;
            case 1:
                rc = 1;
                mode = 2;
                header = &line[s];
                str.str = header;
                str.size = len - s;
                str.size -= line[len - 1] == '\n';
                worgle_orgfile_append_content(worg, &worg->segblock);
                worgle_string_reset(&worg->segblock);
                worgle_orgfile_append_header(worg,
                                             &str,
                                             lvl);
                worg->new_content = 1;
                break;
        }
    }
    return rc;
}
#endif
static int parse_begin(char *line, size_t len, worgle_string *str)
{
    size_t n;
    int mode;
    int rc;

    line += 11;
    len -= 11;

    if(len <= 0) return 0;


    mode = 0;
    n = 0;
    rc = 1;
    str->str = NULL;
    str->size = 0;
    while(n < len) {
        switch(mode) {
            case 0: /* initial spaces after BEGIN_SRC */
                if(line[n] == ' ') {
                    n++;
                } else {
                    mode = 1;
                }
                break;
            case 1: /* look for :tangle */
                if(line[n] == ' ') {
                    mode = 0;
                    n++;
                } else {
                    if(line[n] == ':') {
                        if(!strncmp(line + n + 1, "tangle", 6)) {
                            n+=7;
                            mode = 2;
                            rc = 2;
                        }
                    }
                    n++;
                }
                break;
            case 2: /* save file name, spaces after tangle */
                if(line[n] != ' ') {
                    str->str = &line[n];
                    str->size++;
                    mode = 3;
                }
                n++;
                break;
            case 3: /* read up to next space or line break */
                if(line[n] == ' ' || line[n] == '\n') {
                    mode = 4;
                } else {
                    str->size++;
                }
                n++;
                break;
            case 4: /* countdown til end */
                n++;
                break;
        }
    }

    return rc;
}
void worgle_append_file(worgle_d *worg, worgle_string *filename)
{
    worgle_file *f;
    f = worgle_filelist_append(&worg->flist, filename, worg->curblock);
    worgle_file_id_set(f, worgle_uuid_new(worg));
}
void worgle_append_string(worgle_d *worg)
{
    worgle_segment *seg;
    if(worg->curblock == NULL) return;
    if(worg->curline < 0) return;
    seg = worgle_block_append_string(worg->curblock,
                                     &worg->block,
                                     worg->curline,
                                     &worg->curbuf->filename);
    worgle_segment_id_set(seg, worgle_uuid_new(worg));
    worg->curblock->last_seg = seg->id;
}
void worgle_append_reference(worgle_d *worg, worgle_string *ref)
{
    worgle_segment *seg;
    if(worg->curblock == NULL) return;
    seg = worgle_block_append_reference(worg->curblock,
                                        ref,
                                        worg->linum,
                                        &worg->curbuf->filename);
    worgle_segment_id_set(seg, worgle_uuid_new(worg));
    worg->curblock->last_seg = seg->id;
}
static int check_for_reference(char *line , size_t size, worgle_string *str)
{
    int mode;
    size_t n;
    mode = 0;

    str->size = 0;
    str->str = NULL;
    for(n = 0; n < size; n++) {
        if(mode < 0) break;
        switch(mode) {
            case 0: /* spaces */
                if(line[n] == ' ') continue;
                else if(line[n] == '<') mode = 1;
                else mode = -1;
                break;
            case 1: /* second < */
                if(line[n] == '<') mode = 2;
                else mode = -1;
                break;
            case 2: /* word setup */
                str->str = &line[n];
                str->size++;
                mode = 3;
                break;
            case 3: /* the word */
                if(line[n] == '>') {
                    mode = 4;
                    break;
                }
                str->size++;
                break;
            case 4: /* last > */
                if(line[n] == '>') mode = 5;
                else mode = -1;
                break;
        }
    }

    return (mode == 5);
}
static int worgle_getline(char *fullbuf,
                  char **line,
                  size_t *pos,
                  size_t *line_size,
                  size_t buf_size)
{
    size_t p;
    size_t s;
    *line_size = 0;
    p = *pos;
    *line = &fullbuf[p];
    s = 0;
    while(1) {
        s++;
        if(p >= buf_size) return 0;
        if(fullbuf[p] == '\n') {
            *pos = p + 1;
            *line_size = s;
            return 1;
        }
        p++;
    }
}
int worgle_generate(worgle_d *worg)
{
    return worgle_filelist_write(&worg->flist, &worg->dict);
}
void worgle_init(worgle_d *worg)
{
worgle_string_init(&worg->block);
#ifndef WORGLITE
worgle_string_init(&worg->segblock);
#endif
worg->curline = -1;
worg->block_started = 0;
#ifndef WORGLITE
worg->new_content = 1;
#endif
worgle_hashmap_init(&worg->dict);
worgle_filelist_init(&worg->flist);
worg->curbuf = NULL;
worg->buffers = NULL;
worg->nbuffers = 0;
worg->curblock = NULL;
worg->linum = 0;
worg->orgs = NULL;
worg->curorg = NULL;
worg->prog = 0;
worg->UUID_count = 1;
}
void worgle_free(worgle_d *worg)
{
    int i;
    worgle_hashmap_free(&worg->dict);
    worgle_filelist_free(&worg->flist);
    for(i = 0; i < worg->nbuffers; i++) {
        worgle_textbuf_free(&worg->buffers[i]);
    }
    if(worg->nbuffers > 0) free(worg->buffers);
    if(worg->orgs != NULL && worg->nbuffers > 0) {
        for(i = 0; i < worg->nbuffers; i++) {
            worgle_orgfile_free(&worg->orgs[i]);
        }
        free(worg->orgs);
    }
}
void worgle_string_reset(worgle_string *str)
{
    str->str = NULL;
    str->size = 0;
}
void worgle_string_init(worgle_string *str)
{
    worgle_string_reset(str);
}
int worgle_string_write(FILE *fp, worgle_string *str)
{
    return fwrite(str->str, 1, str->size, fp);
}
void worgle_segment_init(worgle_segment *s,
                        int type,
                        worgle_string *str,
                        worgle_string *filename,
                        size_t linum)
{
   s->type = type;
   s->str = *str;
   s->filename = filename;
   s->linum = linum;
   s->nxt = NULL;
   s->id = 0;
}
int worgle_segment_write(worgle_segment *s, worgle_hashmap *h, FILE *fp)
{
    worgle_block *b;
    if(s->type == SEGTYPE_TEXT) {
        if(use_debug) {
            fprintf(fp, "#line %lu \"", s->linum);
            worgle_string_write(fp, s->filename);
            fprintf(fp, "\"\n");
        }
        worgle_string_write(fp, &s->str);
    } else {
        if(!worgle_hashmap_find(h, &s->str, &b)) {
            fprintf(stderr, "Warning: could not find reference segment '");
            worgle_string_write(stderr, &s->str);
            fprintf(stderr, "'\n");
            if(use_warnings == 2) {
                return 0;
            } else {
                return 1;
            }
        }
        return worgle_block_write(b, h, fp);
    }

    return 1;
}
int worgle_segment_is_text(worgle_segment *s)
{
    return s->type == SEGTYPE_TEXT;
}

int worgle_segment_is_reference(worgle_segment *s)
{
    return s->type == SEGTYPE_REFERENCE;
}
void worgle_block_init(worgle_block *b)
{
    b->nsegs = 0;
    b->head = NULL;
    b->tail = NULL;
    b->nxt = NULL;
    b->am_i_used = 0;
    worgle_string_init(&b->name);
    b->nblocks = 0;
    b->last_seg = -1;
    b->id = 0;
}
void worgle_block_free(worgle_block *lst)
{
    worgle_segment *s;
    worgle_segment *nxt;
    int n;
    s = lst->head;
    for(n = 0; n < lst->nsegs; n++) {
        nxt = s->nxt;
        free(s);
        s = nxt;
    }
}
worgle_segment* worgle_block_append_segment(worgle_block *b,
                                            worgle_string *str,
                                            int type,
                                            size_t linum,
                                            worgle_string *filename)
{
    worgle_segment *s;
    s = malloc(sizeof(worgle_segment));
    if(b->nsegs == 0) {
        b->head = s;
        b->tail = s;
    }
    worgle_segment_init(s, type, str, filename, linum);
    b->tail->nxt = s;
    b->tail = s;
    b->nsegs++;
    /* TODO: remove this */
    /* b->last_seg = s->id; */
    return s;
}
worgle_segment* worgle_block_append_string(worgle_block *b,
                                           worgle_string *str,
                                           size_t linum,
                                           worgle_string *filename)
{
    return worgle_block_append_segment(b, str, SEGTYPE_TEXT, linum, filename);
}
worgle_segment* worgle_block_append_reference(worgle_block *b,
                                              worgle_string *str,
                                              size_t linum,
                                              worgle_string *filename)
{
    return worgle_block_append_segment(b, str, SEGTYPE_REFERENCE,
                                       linum, filename);
}
int worgle_block_write(worgle_block *b, worgle_hashmap *h, FILE *fp)
{
    worgle_segment *s;
    int n;
    s = b->head;
    b->am_i_used = 1;
    for(n = 0; n < b->nsegs; n++) {
        if(!worgle_segment_write(s, h, fp)) return 0;
        s = s->nxt;
    }

    return 1;
}
void worgle_blocklist_init(worgle_blocklist *lst)
{
    lst->head = NULL;
    lst->tail = NULL;
    lst->nblocks = 0;
}
void worgle_blocklist_free(worgle_blocklist *lst)
{
    worgle_block *b;
    worgle_block *nxt;
    int n;
    b = lst->head;
    for(n = 0; n < lst->nblocks; n++) {
        nxt = b->nxt;
        worgle_block_free(b);
        free(b);
        b = nxt;
    }
}
void worgle_blocklist_append(worgle_blocklist *lst, worgle_block *b)
{
    if(lst->nblocks == 0) {
        lst->head = b;
        lst->tail = b;
    }
    lst->tail->nxt = b;
    lst->tail = b;
    lst->nblocks++;
}
void worgle_hashmap_init(worgle_hashmap *h)
{
    int n;
    h->nwords = 0;
    for(n = 0; n < HASH_SIZE; n++) {
        worgle_blocklist_init(&h->blk[n]);
    }
}
void worgle_hashmap_free(worgle_hashmap *h)
{
    int n;
    for(n = 0; n < HASH_SIZE; n++) {
        worgle_blocklist_free(&h->blk[n]);
    }
}
static int hash(const char *str, size_t size)
{
    unsigned int h = 5381;
    size_t i = 0;

    for(i = 0; i < size; i++) {
        h = ((h << 5) + h) ^ str[i];
        h %= 0x7FFFFFFF;
    }

    return h % HASH_SIZE;
}
int worgle_hashmap_find(worgle_hashmap *h, worgle_string *name, worgle_block **b)
{
    int pos;
    worgle_blocklist *lst;
    int n;
    worgle_block *blk;
    pos = hash(name->str, name->size);
    lst = &h->blk[pos];

    blk = lst->head;
    for(n = 0; n < lst->nblocks; n++) {
        if(name->size == blk->name.size) {
            if(!strncmp(name->str, blk->name.str, name->size)) {
                *b = blk;
                return 1;
            }
        }
        blk = blk->nxt;
    }
    return 0;
}
worgle_block * worgle_hashmap_get(worgle_hashmap *h, worgle_string *name)
{
    worgle_block *b;
    worgle_blocklist *lst;
    int pos;

    if(worgle_hashmap_find(h, name, &b)) return b;
    pos = hash(name->str, name->size);
    b = NULL;
    b = malloc(sizeof(worgle_block));
    worgle_block_init(b);
    b->name = *name;
    lst = &h->blk[pos];
    worgle_blocklist_append(lst, b);
    return b;
}
int worgle_file_write(worgle_file *f, worgle_hashmap *h)
{
    FILE *fp;
    char tmp[128];
    size_t n;
    size_t size;
    int rc;

    if(f->filename.size > 128) size = 127;
    else size = f->filename.size;
    for(n = 0; n < size; n++) tmp[n] = f->filename.str[n];
    tmp[size] = 0;

    fp = fopen(tmp, "w");

    rc = worgle_block_write(f->top, h, fp);

    fclose(fp);
    return rc;
}
void worgle_filelist_init(worgle_filelist *flist)
{
    flist->head = NULL;
    flist->tail = NULL;
    flist->nfiles = 0;
}
void worgle_filelist_free(worgle_filelist *flist)
{
    worgle_file *f;
    worgle_file *nxt;
    int n;
    f = flist->head;
    for(n = 0; n < flist->nfiles; n++) {
        nxt = f->nxt;
        free(f);
        f = nxt;
    }
}
worgle_file* worgle_filelist_append(worgle_filelist *flist,
                           worgle_string *name,
                           worgle_block *top)
{
    worgle_file *f;
    f = malloc(sizeof(worgle_file));
    f->filename = *name;
    f->top = top;
    f->nxt = NULL;
    f->id = 0;

    if(flist->nfiles == 0) {
        flist->head = f;
        flist->tail = f;
    }
    flist->tail->nxt = f;
    flist->tail = f;
    flist->nfiles++;
    return f;
}
int worgle_filelist_write(worgle_filelist *flist, worgle_hashmap *h)
{
    worgle_file *f;
    int n;

    f = flist->head;
    for(n = 0; n < flist->nfiles; n++) {
        if(!worgle_file_write(f, h)) return 0;
        f = f->nxt;
    }

    return 1;
}
void worgle_textbuf_zero(worgle_textbuf *txt)
{
    txt->buf = NULL;
    worgle_string_init(&txt->filename);
    txt->size = 0;
}
void worgle_textbuf_init(worgle_textbuf *txt,
                         char *buf,
                         size_t bufsize)
{
    txt->buf = buf;
    txt->size = bufsize;
}
void worgle_textbuf_free(worgle_textbuf *txt)
{
    if(txt->buf != NULL) free(txt->buf);
    worgle_textbuf_zero(txt);
}
worgle_long worgle_uuid_new(worgle_d *worg)
{
    return worg->UUID_count++;
}
worgle_long worgle_segment_id_get(worgle_segment *s)
{
    return s->id;
}

void worgle_segment_id_set(worgle_segment *s, worgle_long id)
{
    s->id = id;
}
worgle_long worgle_block_id_get(worgle_block *b)
{
    return b->id;
}

void worgle_block_id_set(worgle_block *b, worgle_long id)
{
    b->id = id;
}
void worgle_file_id_set(worgle_file *f, worgle_long id)
{
    f->id = id;
}
void worgle_orglet_id_set(worgle_orglet *orglet,
                           worgle_long id)
{
    orglet->id = id;
}
void worgle_orgfile_init(worgle_orgfile *org)
{
    org->head = NULL;
    org->tail = NULL;
    org->size = 0;
    org->prev = NULL;
}
void worgle_orgfile_free(worgle_orgfile *org)
{
    worgle_orglet *ent;
    worgle_orglet *nxt;
    int n;
    if (org->size > 0) {
        ent = org->head;
        for (n = 0; n < org->size; n++) {
            nxt = ent->next;
            worgle_orglet_free(ent);
            ent = nxt;
        }
    }
}
void worgle_orgfile_append(worgle_d *worg,
                           worgle_orgfile *file,
                           worgle_orglet *orglet)
{
    if (file->size <= 0) {
        file->head = orglet;
        if (file->prev != NULL) {
            file->prev->tail->next = file->head;
        }
        file->tail = orglet;
    }

    file->tail->next = orglet;
    file->tail = orglet;
    file->size++;
    worgle_orglet_id_set(orglet, worgle_uuid_new(worg));
}
void worgle_orgfile_append_header(worgle_d *worg,
                                  worgle_string *header,
                                  int lvl)
{
    worgle_orglet *orglet;
    worgle_orglet_header *h;
    worgle_orgfile *f;
    size_t linum;

    f = worg->curorg;
    linum = worg->linum;

    orglet = calloc(1, sizeof(worgle_orglet));
    h = calloc(1, sizeof(worgle_orglet_header));

    h->str = *header;
    h->lvl = lvl;
    worgle_orglet_init(orglet);

    orglet->type = ORGLET_HEADER;
    orglet->ud = h;
    orglet->linum = linum;
    worgle_orgfile_append(worg, f, orglet);
}
void worgle_orgfile_append_content(worgle_d *worg,
                                   worgle_string *text)
{
    worgle_orglet *orglet;
    worgle_orglet_content *c;
    worgle_orgfile *f;
    size_t linum;

    if (text->size == 0) return;
    if (text->size == 1) return;

    f = worg->curorg;
    linum = worg->linum;

    orglet = calloc(1, sizeof(worgle_orglet));
    c = calloc(1, sizeof(worgle_orglet_content));

    c->text = *text;

    /* printf("CONTENT(%d): '", c->text.size); */
    /* fwrite(c->text.str, 1, c->text.size, stdout); */
    /* printf("'\n"); */

    worgle_orglet_init(orglet);

    orglet->type = ORGLET_CONTENT;
    orglet->ud = c;
    orglet->linum = linum;
    worgle_orgfile_append(worg, f, orglet);
}
void worgle_orgfile_append_reference(worgle_d *worg,
                                     worgle_block *blk)
{
    worgle_orglet *orglet;
    worgle_orglet_blkref *br;
    worgle_orgfile *f;
    size_t linum;

    f = worg->curorg;
    linum = worg->linum;

    orglet = calloc(1, sizeof(worgle_orglet));
    br = calloc(1, sizeof(worgle_orglet_blkref));

    br->blk = blk;
    br->pos = blk->nblocks;
    br->prev_lastseg = blk->last_seg;
    br->segoff = blk->nsegs;
    worgle_orglet_init(orglet);

    orglet->type = ORGLET_BLKREF;
    orglet->ud = br;
    orglet->linum = linum;
    worgle_orgfile_append(worg, f, orglet);
}
void worgle_orglet_init(worgle_orglet *orglet)
{
    orglet->type = ORGLET_UNKNOWN;
    orglet->ud = NULL;
    orglet->next = NULL;
    orglet->linum = 0;
    orglet->id = 0;
}
void worgle_orglet_free(worgle_orglet *orglet)
{
    free(orglet->ud);
    free(orglet);
}
int worgle_warn_unused(worgle_d *worg)
{
    worgle_hashmap *dict;
    worgle_block *blk;
    worgle_blocklist *lst;
    int n;
    int b;
    int rc;

    dict = &worg->dict;
    rc = 0;

    for(n = 0; n < HASH_SIZE; n++) {
        lst = &dict->blk[n];
        blk = lst->head;
        for(b = 0; b < lst->nblocks; b++) {
            if(blk->am_i_used == 0) {
                fprintf(stderr, "Warning: block '");
                worgle_string_write(stderr, &blk->name);
                fprintf(stderr, "' unused.\n");
                fprintf(stderr, "First declared in ");
                worgle_string_write(stderr,
                                    blk->head->filename);
                fprintf(stderr, ", line %lu\n",
                        blk->head->linum);
                if(use_warnings == 2) rc = 1;
            }
            blk = blk->nxt;
        }
    }
    return rc;
}
static void append_filename(worgle_d *worg, char *filename)
{
    worgle_string *str;
    int pos;
    pos = worg->nbuffers;
    worg->nbuffers++;
    if(worg->nbuffers == 1) {
        worg->buffers = calloc(1, sizeof(worgle_textbuf));
    } else {
        worg->buffers = realloc(worg->buffers,
                                sizeof(worgle_textbuf) *
                                worg->nbuffers);
    }
    str = &worg->buffers[pos].filename;
    str->str = filename;
    str->size = strlen(filename);
#ifndef WORGLITE
    if(worg->nbuffers == 1) {
        worg->orgs = calloc(1, sizeof(worgle_orgfile));
    } else {
        worg->orgs = realloc(worg->orgs,
                             sizeof(worgle_orgfile) *
                             worg->nbuffers);
    }
    if(worg->nbuffers > 0) {
        worgle_orgfile_init(&worg->orgs[worg->nbuffers - 1]);
    }
#endif
}
#ifndef WORGLITE
static void tangle_to_db(worgle_d *worg)
{
    sqlite3 *db;
    int rc;

    if(db_filename == NULL) return;

    rc = sqlite3_open(db_filename, &db);
    if(rc) {
        fprintf(stderr,
                "Could not open database: %s",
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    worgle_db_clear(db, worg->prog);
    worgle_db_schemas(worg, db);
    worgle_db_generate(worg, db);
    sqlite3_close(db);
}
#endif
void worgle_map(worgle_d *worg, worgle_block *b, int lvl, FILE *out)
{
    int i;
    worgle_segment *s;
    worgle_block *newblk;
    worgle_hashmap *h;

    h = &worg->dict;
    if(lvl != 0) {
        for(i = 0; i <= lvl; i++) {
            fputc('*', out);
        }
        fputc(' ', out);
        worgle_string_write(out, &b->name);
        fputc('\n', out);
    }

    s = b->head;

    newblk = NULL;

    for(i = 0; i < b->nsegs; i++) {
        if(s->type == SEGTYPE_TEXT) {
            if(s->str.size > 0) {
                worgle_string_write(out, s->filename);
                fprintf(out, ":%lu\n", s->linum);
                fprintf(out, "#+NAME: ");
                worgle_string_write(out, &b->name);
                fprintf(out, "_%d\n", i);
                fprintf(out, "#+BEGIN_SRC\n");
                worgle_string_write(out, &s->str);
                fprintf(out, "#+END_SRC");
            }
            fprintf(out, "\n");
        } else if(worgle_hashmap_find(h, &s->str, &newblk)) {
            worgle_map(worg, newblk, lvl + 1, out);
        }
        s = s->nxt;
    }
}
void worgle_map_files(worgle_d *worg, char *filename)
{
    int n;
    worgle_file *f;
    FILE *fp;

    fp = fopen(filename, "w");
    if(fp == NULL) return;

    f = worg->flist.head;

    fprintf(fp, "#+TITLE: Code Map\n");
    for(n = 0; n < worg->flist.nfiles; n++) {
        worgle_map_a_file(worg, f, fp);
        f = f->nxt;
    }

    fclose(fp);
}

void worgle_map_a_file(worgle_d *worg, worgle_file *file, FILE *out)
{
    fprintf(out, "* ");
    worgle_string_write(out, &file->filename);
    fprintf(out, "\n");
    worgle_map(worg, file->top, 0, out);
}

int main(int argc, char *argv[])
{
    worgle_d worg;
    char *filename;
    int rc;
    int i;
    struct parg_state ps;
    int c;
    worgle_init(&worg);
    filename = NULL;
    if(argc < 2) {
        fprintf(stderr, "Usage: %s filename.org\n", argv[0]);
        return 1;
    }
    parg_init(&ps);
    while ((c = parg_getopt(&ps, argc, argv, "gW:m:d:p:n")) != -1) {
        switch(c) {
            case 1:
                filename = (char *)ps.optarg;
                append_filename(&worg, (char *)ps.optarg);
                break;
            case 'g':
                use_debug = 1;
                break;
            case 'W':
                if(!strncmp(ps.optarg, "soft", 4)) {
                    use_warnings = 1;
                } else if(!strncmp(ps.optarg, "error", 5)) {
                    use_warnings = 2;
                } else {
                    fprintf(stderr, "Unidentified warning mode '%s'\n", ps.optarg);
                    return 1;
                }
                break;
            case 'm':
                map_source_code = 1;
                map_filename = (char *)ps.optarg;
                break;
            case 'd':
    #ifndef WORGLITE
                generate_db = 1;
                db_filename = (char *)ps.optarg;
    #else
                fprintf(stderr, "Database flag (-d) is  not enabled\n");
                return 1;
    #endif
                break;
            case 'p':
                worg.prog = atoi(ps.optarg);
                break;
            case 'n':
                tangle_code = 0;
                break;
            default:
                fprintf(stderr, "Unknown option -%c\n", c);
                return 1;
        }
    }
    if(filename == NULL) {
        fprintf(stderr, "No filename specified\n");
        return 1;
    }
    rc = 0;
    for(i = 0; i < worg.nbuffers; i++) {
        rc = loadfile(&worg, i);
        if(!rc) goto cleanup;
    }
    for (i = 0; i < worg.nbuffers; i++) {
        rc = parse_file(&worg, i);
        if (rc) goto cleanup;
    }
    #ifndef WORGLITE
    if (generate_db) {
        worgle_orgfile_append_content(&worg, &worg.segblock);
    }
    #endif
    if(!rc && tangle_code) if(!worgle_generate(&worg)) rc = 1;
    if(!rc && use_warnings) rc = worgle_warn_unused(&worg);
    if(map_source_code && map_filename != NULL) {
        worgle_map_files(&worg, map_filename);
    }
    #ifndef WORGLITE
    if(generate_db) tangle_to_db(&worg);
    #endif
    cleanup:
    worgle_free(&worg);
    return rc;
}
