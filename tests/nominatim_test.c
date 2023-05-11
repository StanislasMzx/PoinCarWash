#define SNOW_ENABLED

#include "snow.h"
#include "../src/nominatim.h"
#include "../src/coordinates.h"

describe(test_api)
{
    it("API test")
    {
        char *query1 = "nancy";
        char *query2 = "193 avenue Paul Muller, 54600 Villers-lès-Nancy France";
        char *query3 = "jardin botanique jean marie pelt";
        char *query4 = "place stanislas";

        char *response1 = fetch_api(query1);
        printf(" -- QUERY:\t%s\n -- RESPONSE:\n%s\n", query1, response1);

        char *response2 = fetch_api(query2);
        printf(" -- QUERY:\t%s\n -- RESPONSE:\n%s\n", query2, response2);

        char *response3 = fetch_api(query3);
        printf(" -- QUERY:\t%s\n -- RESPONSE:\n%s\n", query3, response3);

        char *response4 = fetch_api(query4);
        printf(" -- QUERY:\t%s\n -- RESPONSE:\n%s\n", query4, response4);

        free(response1);
        free(response2);
        free(response3);
        free(response4);
    }
}

// describe(test_nominatim)
// {
//     it("Conformance test")
//     {
//         Nominatim_t *nomin1 = get_nominatim("nancy");
//         printf("%s", nomin1->name);
//         coordinates_print(nomin1->coord);

//         Nominatim_t *nomin2 = get_nominatim("193 avenue Paul Muller, 54600 Villers-lès-Nancy France");
//         printf("%s", nomin2->name);
//         coordinates_print(nomin2->coord);

//         Nominatim_t *nomin3 = get_nominatim("jardin botanique jean marie pelt");
//         printf("%s", nomin3->name);
//         coordinates_print(nomin3->coord);

//         Nominatim_t *nomin4 = get_nominatim("place stanislas");
//         printf("%s", nomin4->name);
//         coordinates_print(nomin4->coord);

//         destroy_nominatim(nomin1);
//         destroy_nominatim(nomin2);
//         destroy_nominatim(nomin3);
//         destroy_nominatim(nomin4);
//     }
// }

snow_main();
