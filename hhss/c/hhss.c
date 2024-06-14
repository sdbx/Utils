/***
 * HUTSORI.C   by Cor (24.06.02 ~ 24.06.06)
 * This program prints some "ridiculous" sentences to stdout,
 * given two data files: hsr.dat and usr.dat. Meanwhile, it
 * would be a good thing to use this program with the other
 * programs: btn and/or nsy via pipelines.
 * 
 * Usage: ./hhss NUM_SENTENCES
 * where
 *    NUM_SENTENCES = the number of sentences to print out.
 * 
 * Requirement: hsr.dat has to have at least 5 valid lines
 * and usr.dat at least 1 valid line. A valid line is a line
 * which is neither a comment nor an empty line. A comment
 * line starts with a # character. A valid line can have more
 * than one "${user}" string in it, which is to be replaced
 * with a random valid line in usr.dat.
 * 
 * P.S. hhss stands for hweng-seol-su-seol, which means
 * gibberish in Korean.
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>

#define LINES_INC_UNIT  64    /* Their size increases by this */
#define LINE_INC_UNIT  256    /* numbers, respectively.       */
#define SPLIT_INC_UNIT   8
#
#define MIN_SENTENCE_NUM  5
#define MIN_USER_NUM    1
#
#define COMMENT         '#'

char *arg_num_sentence_to_print = "argv[1]";
char *sentence_file = "hsr.dat";
char *user_file = "usr.dat";
char *user_template = "${user}";

void raise_err(char *err_msg, ...);
FILE *open_file(char *name, char *mode);
char **copy_valid_lines(FILE *fp, int *result_len_ptr);
void close_file(FILE *name);
void check_if_valid_file(char *file_name, int line_len, int min_len);
int get_num_sentence_to_print(char *str, int max_limit);
char *shuffle_once(char **lines, int final);
char *replace_template(char *line, char **usrs, int usrs_len);
void free_str_arr(char **str_arr, int arr_len);

int main(int argc, char *argv[]) {
   if (argc != 2)
      raise_err("hhss: argc != 2.");

   FILE *hsr_dat = open_file(sentence_file, "r");
   FILE *usr_dat = open_file(user_file, "r");

   int hsrs_len, usrs_len;
   char **hsrs = copy_valid_lines(hsr_dat, &hsrs_len);
   char **usrs = copy_valid_lines(usr_dat, &usrs_len);

   close_file(hsr_dat);
   close_file(usr_dat);

   check_if_valid_file(sentence_file, hsrs_len, MIN_SENTENCE_NUM);
   check_if_valid_file(user_file, usrs_len, MIN_USER_NUM);

   int num_sentence_to_print = get_num_sentence_to_print(argv[1], hsrs_len);
   char *printed_sentences[num_sentence_to_print];

   srand(time(NULL));
   for (int i = 0, final = hsrs_len - 1;
         i < num_sentence_to_print;
         i++)
   {
      char *chosen_sentence;
      
      chosen_sentence = shuffle_once(hsrs, final--);
      chosen_sentence = replace_template(chosen_sentence, usrs, usrs_len);
      printf("%s\n", chosen_sentence);
      printed_sentences[i] = chosen_sentence;
   }

   free_str_arr(hsrs, hsrs_len);
   free_str_arr(usrs, usrs_len);
   for (int i = 0; i < num_sentence_to_print; i++)
      free(printed_sentences[i]);

   return 0;
}

/* prints an error message to stderr. */
void raise_err(char *err_msg, ...) {
   va_list ap;
   
   va_start(ap, err_msg);
   vfprintf(stderr, err_msg, ap);
   va_end(ap);
   fprintf(stderr, "\n");
   exit(EXIT_FAILURE);
}

/* opens a file. */
FILE *open_file(char *name, char *mode) {
   FILE *file;
   
   file = fopen(name, mode);
   if (file == NULL)
      raise_err("hhss: failed to open %s as mode \"%s\".",
         name, mode);
   
   return file;
}

/* reads arg_num_sentence_to_print. */
int get_num_sentence_to_print(char *str, int max_limit) {
   char *indicator;
   int num;

   errno = 0;
   num = strtol(str, &indicator, 10);
   if (errno == ERANGE)
      raise_err("hhss: %s out of range -> %s.", arg_num_sentence_to_print, str);
   if (indicator == str)
      raise_err("hhss: no conversion can be performed.");
   if (num < MIN_SENTENCE_NUM)
      raise_err("hhss: %s = %d needs to be >= %d.",
         arg_num_sentence_to_print, num, MIN_SENTENCE_NUM);
   else if (num > max_limit)
      raise_err("hhss: %s = %d needs to be <= %d, "
         "the number of valid lines in %s.",
         arg_num_sentence_to_print, num, max_limit, sentence_file);

   return num;
}

char *create_new_line(int *last_char_idx_ptr, int *max_char_num_ptr);
void store_current_line(
   char ***lines_ptr, char *line, int *lines_cur_len_ptr, int *lines_max_len_ptr);
void store_current_char(
   char **line_ptr, char ch, int *last_char_idx_ptr, int *max_char_num_ptr);

/* copies valid lines from a data file. */
char **copy_valid_lines(FILE *fp, int *result_len_ptr) {
   char **lines;
   int lines_cur_len = 0;
   int lines_max_len = LINES_INC_UNIT;

   lines = malloc(lines_max_len * sizeof(char *));
   if (lines == NULL)
      raise_err("hhss: failed to malloc.");
   
   bool at_line_start = true;
   bool on_comment = false;
   int ch;
   int last_char_idx = 0;
   int max_char_num = LINE_INC_UNIT;
   char *line = create_new_line(&last_char_idx, &max_char_num);

   while ((ch = getc(fp)) != EOF) {
      switch (ch) {
         case '\n':
            store_current_line(
               &lines, line, &lines_cur_len, &lines_max_len);
            if (strlen(line) != 0)
               line = create_new_line(&last_char_idx, &max_char_num);
            if (!at_line_start) at_line_start = true;
            if (on_comment) on_comment = false;
            break;
         case '\r':
            break;
         case COMMENT:
            if (at_line_start) {
               on_comment = true;
               at_line_start = false;
               break;
            }
         default:
            if (on_comment) break;
            if (at_line_start) at_line_start = false;
            store_current_char(
               &line, ch, &last_char_idx, &max_char_num);
      }
   }
   store_current_line(  /* for a file not to be ended with \n. */
      &lines, line, &lines_cur_len, &lines_max_len);

   *result_len_ptr = lines_cur_len;
   return lines;
}

/* creates a new line and initializes
   two variables related to it. */
char *create_new_line(int *last_char_idx_ptr, int *max_char_num_ptr) {
   char *line;
   
   line = malloc(LINE_INC_UNIT);
   if (line == NULL)
      raise_err("hhss: failed to malloc.");
   line[0] = '\0';
   *last_char_idx_ptr = 0;
   *max_char_num_ptr = 0;

   return line;
}

char **lengthen_lines(char **lines, int lines_max_len);

/* stores a current line (namely, char *) to char ** variable. */
void store_current_line(
   char ***lines_ptr,
   char *line,
   int *lines_cur_len_ptr,
   int *lines_max_len_ptr
) {
   if (strlen(line) == 0) return;
   if (*lines_cur_len_ptr == *lines_max_len_ptr) {
      *lines_max_len_ptr += LINES_INC_UNIT;
      *lines_ptr = lengthen_lines(*lines_ptr, *lines_max_len_ptr);
   }
   (*lines_ptr)[(*lines_cur_len_ptr)++] = line;
}

/* makes a char ** variable have a longer space. */
char **lengthen_lines(char **lines, int lines_max_len) {
   char **lines_reallocated;
   
   lines_reallocated = realloc(lines, lines_max_len * sizeof(char *));
   if (lines == NULL)
      raise_err("hhss: failed to realloc.");
   
   return lines_reallocated;
}

char *lengthen_line(char *line, int max_char_num);

/* stores a current character (namely, char) to char * variable. */
void store_current_char(
   char **line_ptr,
   char ch,
   int *last_char_idx_ptr,
   int *max_char_num_ptr
) {
   if (*last_char_idx_ptr == *max_char_num_ptr) {
      *max_char_num_ptr += LINE_INC_UNIT;
      *line_ptr = lengthen_line(*line_ptr, *max_char_num_ptr);
   }
   (*line_ptr)[(*last_char_idx_ptr)++] = ch;  
}

/* makes a char * variable have a longer space. */
char *lengthen_line(char *line, int max_char_num) {
   char *line_reallocated;

   line_reallocated = realloc(line, max_char_num * sizeof(char));
   if (line_reallocated == NULL)
      raise_err("hhss: failed to realloc.");

   return line_reallocated;
}

/* closes a file. */
void close_file(FILE *name) {
   if (fclose(name) == EOF)
      raise_err("hhss: failed to close %s.\n", name);
}

/* sees whether a file satisfies the requirement. */
void check_if_valid_file(char *file_name, int line_len, int min_len) {
   if (line_len < min_len)
      raise_err("hhss: it is required for %s to have "
         "valid lines more than or equal to %d.",
         file_name, min_len);
}

unsigned int rand_range(unsigned int min, unsigned int max);

/* shuffles once; returns a random char * from char **. */
char *shuffle_once(char **lines, int final) {
   int rv = rand_range(0, final);
   char *chosen;
   
   if (rv == final)
      return chosen = lines[final];
   else {
      chosen = lines[rv];
      lines[rv] = lines[final];
      lines[final] = chosen;
      return chosen;
   }
}

/* returns a random positive integer; min <= x <= max. */
unsigned int rand_range(unsigned int min, unsigned int max) {
   if (max == 0)
      return 0;
   return min + rand() / (RAND_MAX / max + 1);
}

char **split(char *str, char *delim_str, int *result_len_ptr);

/* replaces ${} and returns a processed sentence. */
char *replace_template(char *line, char **usrs, int usrs_len) {
   int parts_len;
   char **parts;
   
   parts = split(line, user_template, &parts_len);

   char *prev_name = NULL;
   int final = usrs_len - 1;
   int result_str_len = 0;

   for (int i = 0; i < parts_len; i++) {
      if (strlen(parts[i]) == 0) {
         free(parts[i]);

         char *temp;

         do {
            temp = shuffle_once(usrs, final--);
            if (final == -1)
               final = usrs_len - 1;
         } while (prev_name == temp);
         prev_name = temp;

         char *user_name;
         int temp_len = strlen(temp);

         user_name = malloc(temp_len + 1);
         if (user_name == NULL)
            raise_err("hhss: failed to malloc.");
         
         strncpy(user_name, temp, temp_len);
         user_name[temp_len] = '\0';
         parts[i] = user_name;
      }
      result_str_len += strlen(parts[i]);
   }

   char *result_str;
   
   result_str = malloc(result_str_len + 1);
   if (result_str == NULL)
      raise_err("hhss: failed to malloc.");
   
   for (int i = 0, offset = 0; i < parts_len; i++) {
      int len = strlen(parts[i]);
      strncpy(result_str + offset, parts[i], len);
      offset += len;
   }
   result_str[result_str_len] = '\0';

   for (int i = 0; i < parts_len; i++)
      free(parts[i]);
   free(parts);

   return result_str;
}

/* splits a string, replacing ${} with an empty string ("\0"),
   and returns the result as char **. */
char **split(char *str, char *delim_str, int *result_len_ptr) {
   char **arr;
   int arr_len = SPLIT_INC_UNIT;

   arr = malloc(arr_len * sizeof(char *));
   if (arr == NULL)
      raise_err("hhss: failed to malloc.");

   int delim_str_len = strlen(delim_str);
   int i = 0;

   for (int ini = 0, fin = 0; /* initial, final */
        ini < strlen(str);
        i++)
   {
      char *ptr;
      char *current = str + ini;

      ptr = strstr(current, delim_str);
      if (ptr == NULL)
         fin = strlen(str);
      else
         if (ptr == current)
            fin = ini; /* Thus len becomes to 0. */
         else
            fin = ptr - str;

      char *part;
      int len = fin - ini;

      part = malloc(len + 1);
      if (part == NULL)
         raise_err("hhss: failed to malloc.");
      
      strncpy(part, current, len);
      part[len] = '\0';

      if (i == arr_len) {
         arr_len += SPLIT_INC_UNIT;
         arr = realloc(arr, arr_len * sizeof(char *));
         if (arr == NULL)
            raise_err("hhss: failed to realloc.");
      }
      
      arr[i] = part;

      if (ptr == NULL)
         ini = fin;
      else
         if (ptr == current)
            ini += delim_str_len;
         else
            ini = fin;
   }

   *result_len_ptr = i;
   return arr;
}

void free_str_arr(char **str_arr, int arr_len) {
   for (int i = 0; i < arr_len; i++)
      free(str_arr[i]);
   free(str_arr);
}

/* Another implementation:
   processes a line, one character at a time. */
#if 0
#define TEMPLATE_BEGIN  '$'

void handle_user_template(
   char **usrs, char **prev_ptr, int final_value,
   char **template_replaced_sentence_ptr,
   int *template_replaced_sentence_last_char_idx_ptr,
   int *template_replaced_sentence_max_char_idx_ptr);

char *replace_template(char *line, char **usrs, int usrs_len) {
   char *template_replaced_sentence;
   int template_replaced_sentence_last_char_idx = 0;
   int template_replaced_sentence_max_char_idx = LINE_INC_UNIT;
   char *ptr;
   char *prev = NULL;

   template_replaced_sentence = malloc(template_replaced_sentence_max_char_idx);
   if (template_replaced_sentence == NULL)
      raise_err("hhss: failed to malloc.");
   for (int i = 0; i < strlen(line); /* intentionally left blank */ ) {
      if (line[i] != TEMPLATE_BEGIN) {
         store_current_char(
            &template_replaced_sentence,
            line[i],
            &template_replaced_sentence_last_char_idx,
            &template_replaced_sentence_max_char_idx);
         i++;
      }
      else {
         ptr = strstr(line + i, user_template);
         if (ptr != NULL) {
            handle_user_template(
               usrs, &prev, usrs_len - 1,
               &template_replaced_sentence,
               &template_replaced_sentence_last_char_idx,
               &template_replaced_sentence_max_char_idx);
            i += strlen(user_template);
         }
         else {
            store_current_char(
               &template_replaced_sentence,
               line[i],
               &template_replaced_sentence_last_char_idx,
               &template_replaced_sentence_max_char_idx);
            i++;
         }
      }
   }
   store_current_char(
      &template_replaced_sentence,
      '\0',
      &template_replaced_sentence_last_char_idx,
      &template_replaced_sentence_max_char_idx);

   return template_replaced_sentence;
}

void handle_user_template(
   char **usrs,
   char **prev_ptr,
   int final_value,
   char **template_replaced_sentence_ptr,
   int *template_replaced_sentence_last_char_idx_ptr,
   int *template_replaced_sentence_max_char_idx_ptr)
{
   char *user_name;
   int final = final_value;

   do {
      user_name = shuffle_once(usrs, final--);
      if (final == -1)
         final = final_value;
   } while (*prev_ptr == user_name);
   *prev_ptr = user_name;

   for (int i = 0; i < strlen(user_name); i++)
      store_current_char(
         template_replaced_sentence_ptr,
         user_name[i],
         template_replaced_sentence_last_char_idx_ptr,
         template_replaced_sentence_max_char_idx_ptr);
}
#endif