#include <bson.h>
#include <mongoc.h>
#include <stdio.h>

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
    sales_collection = mongoc_client_get_collection (client, "demo", "sales");
    query = bson_new ();
    cursor = mongoc_collection_find (sales_collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

    double cost_sum_double = 0;
    _Decimal128 cost_sum_decimal = 0;
    while (mongoc_cursor_next (cursor, &doc)) {
        bson_iter_init(&iter, doc);
        bson_iter_find(&iter, "cost_double");
        cost_sum_double += bson_iter_double(&iter);
        bson_iter_find(&iter, "cost_decimal");
        cost_sum_decimal += bson_iter_decimal128(&iter);
    }

    printf("Cost sum (double): %f", cost_sum_double);
    printf("Cost sum (decimal): %f", cost_sum_decimal);

    bson_destroy (query);
    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (sales_collection);
    mongoc_client_destroy (client);

    return 0;
}
