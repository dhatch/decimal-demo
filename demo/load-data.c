#include <stdlib.h>
#include <stdio.h>

#include <bson.h>
#include <mongoc.h>

#define RECORD_COUNT 100000

/*
 * This C program loads %RECORD_COUNT transactions into the connected database,
 * using doubles and decimals as the transaction amount.
 *
 * The database name is: demo.
 * The collection name is: sales.
 *
 * Document structure is as follows:
 *
 * {
 *      "_id": ObjectId(...),
 *      "cost_double" : 0.70,
 *      "cost_decimal" : NumberDecimal("0.70")
 * }
 */


/**
 * This function inserts %RECORD_COUNT transactions into the database.
 *
 * It also inserts a record into the revenue collection, with the structure:
 *
 * {
 *      "_id": ObjectId(...),
 *      "total_revenue_double": <sum of cost_double>,
 *      "total_revenue_decimal": <sum of cost_decimal>
 * }
 */
void
perform_insert (mongoc_collection_t *sales, mongoc_collection_t *revenue)
{
    mongoc_bulk_operation_t *bulk;
    bson_error_t error;
    bson_t *doc;
    bson_t reply;
    bool op_success;

    _Decimal128 total_decimal = 0.DL;
    double total_double = 0.;

    bulk = mongoc_collection_create_bulk_operation (sales, true, NULL);

    for (int i = 0; i < RECORD_COUNT; i++) {
        total_double += 0.70;
        total_decimal += 0.70DL;
        doc = bson_new ();
        BSON_APPEND_DOUBLE(doc, "cost_double", 0.70);
        BSON_APPEND_DECIMAL128(doc, "cost_decimal", 0.70DL);
        mongoc_bulk_operation_insert (bulk, doc);
        bson_destroy (doc);
    }

    op_success = mongoc_bulk_operation_execute (bulk, &reply, &error);

    char *str = bson_as_json (&reply, NULL);
    printf ("%s\n", str);
    bson_free (str);

    if (!op_success) {
        fprintf (stderr, "Error: %s\n", error.message);
    }

    doc = bson_new();
    BSON_APPEND_DOUBLE(doc, "total_revenue_double", total_double);
    BSON_APPEND_DECIMAL128(doc, "total_revenue_decimal", total_decimal);
    if (!mongoc_collection_insert (revenue, MONGOC_INSERT_NONE, doc, NULL, &error)) {
        printf ("%s\n", error.message);
    }

    bson_destroy(doc);

    bson_destroy (&reply);
    mongoc_bulk_operation_destroy(bulk);
}


int
main (int argc,
      char *argv[])
{
    // Initialize client and collection reference.
    mongoc_client_t *client;
    mongoc_collection_t *sales;
    mongoc_collection_t *revenue;

    mongoc_init ();

    client = mongoc_client_new ("mongodb://localhost/");
    sales = mongoc_client_get_collection(client, "test", "sales");
    revenue = mongoc_client_get_collection(client, "test", "revenue");

    perform_insert (sales, revenue);

    mongoc_collection_destroy (sales);
    mongoc_collection_destroy (revenue);
    mongoc_client_destroy (client);

    mongoc_cleanup ();

    exit(EXIT_SUCCESS);
}
