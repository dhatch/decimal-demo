#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <string.h>

char * dec_str(_Decimal128 decimal) {
  char str[BSON_DEC128_STRING];
  bson_dec128_t dec;
  bson_decimal128_to_dec128(decimal, &dec);
  bson_dec128_to_string(&dec, str);
  return strdup(str);
}

int
main (int   argc,
      char *argv[])
{
    mongoc_client_t *client;
    mongoc_collection_t *sales_collection;
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_t *query;
    char *str;
    bson_iter_t iter;

    mongoc_init ();

    client = mongoc_client_new ("mongodb://localhost:27017/");
    sales_collection = mongoc_client_get_collection (client, "test", "sales");
    query = bson_new ();
    cursor = mongoc_collection_find (sales_collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

    _Decimal128 cost_sum_decimal = 0;
    while (mongoc_cursor_next (cursor, &doc)) {
        bson_iter_init(&iter, doc);
        bson_iter_find(&iter, "cost_decimal");
        _Decimal128 decimal = bson_iter_decimal128(&iter);
        if (((int)(decimal * 1000.DL)) % 10 >= 5) {
          decimal = ((_Decimal128)((int)(decimal * 100.DL + 1.DL))) / 100.DL;
        } else {
          decimal = ((_Decimal128)((int)(decimal * 100.DL))) / 100.DL;
        }
        bson_dec128_t dec_r;
        char dec_str_r[BSON_DEC128_STRING];
        bson_decimal128_to_dec128(decimal, &dec_r);
        bson_dec128_to_string(&dec_r, dec_str_r);
        cost_sum_decimal += decimal;
    }

    bson_dec128_t dec;
    char dec_str[BSON_DEC128_STRING];
    bson_decimal128_to_dec128(cost_sum_decimal, &dec);
    bson_dec128_to_string(&dec, dec_str);
    printf("Cost sum (decimal): %s\n", dec_str);

    bson_destroy (query);
    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (sales_collection);
    mongoc_client_destroy (client);

    return 0;
}
