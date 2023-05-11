#define SNOW_ENABLED

#include "snow.h"
#include "../src/nominatim.h"
#include "../src/coordinates.h"
#include <math.h>

describe(test_api)
{
    it("API test")
    {
        printf("\n");

        char *query1 = "nancy";
        char *query2 = "193 avenue Paul Muller, 54600 Villers-lès-Nancy France";
        char *query3 = "jardin botanique jean marie pelt";
        char *query4 = "place stanislas";

        char *response1 = fetch_api(query1);
        assert(response1 != NULL);
        // printf(" -- QUERY:\t%s\n -- RESPONSE:\n%s\n", query1, response1);

        char *response2 = fetch_api(query2);
        assert(response2 != NULL);
        // printf(" -- QUERY:\t%s\n -- RESPONSE:\n%s\n", query2, response2);

        char *response3 = fetch_api(query3);
        assert(response3 != NULL);
        // printf(" -- QUERY:\t%s\n -- RESPONSE:\n%s\n", query3, response3);

        char *response4 = fetch_api(query4);
        assert(response4 != NULL);
        // printf(" -- QUERY:\t%s\n -- RESPONSE:\n%s\n", query4, response4);

        free(response1);
        free(response2);
        free(response3);
        free(response4);
    }
}

describe(test_nominatim)
{
    it("Conformance test")
    {
        printf("\n");

        Nominatim_t *nomin1 = get_nominatim("nancy");
        assert(nomin1 != NULL);
        // printf("---\nNAME:\t%s\nLAT:\t%f\t\tLON:\t%f\n", nomin1->name, nomin1->coord->latitude, nomin1->coord->longitude);
        assert(strcmp(nomin1->name, "Nancy, Meurthe-et-Moselle, Grand Est, France métropolitaine, France") == 0);
        assert(fabs(nomin1->coord->latitude - 48.693722) < 0.00001);
        assert(fabs(nomin1->coord->longitude - 6.183410) < 0.00001);

        Nominatim_t *nomin2 = get_nominatim("193 avenue Paul Muller, 54600 Villers-lès-Nancy France");
        assert(nomin2 != NULL);
        // printf("---\nNAME:\t%s\nLAT:\t%f\t\tLON:\t%f\n", nomin2->name, nomin2->coord->latitude, nomin2->coord->longitude);
        assert(strcmp(nomin2->name, "193, Avenue Paul Muller, Villers-lès-Nancy, Nancy, Meurthe-et-Moselle, Grand Est, France métropolitaine, 54600, France") == 0);
        assert(fabs(nomin2->coord->latitude - 48.669495) < 0.00001);
        assert(fabs(nomin2->coord->longitude - 6.155138) < 0.00001);

        Nominatim_t *nomin3 = get_nominatim("jardin botanique jean marie pelt");
        assert(nomin3 != NULL);
        // printf("---\nNAME:\t%s\nLAT:\t%f\t\tLON:\t%f\n", nomin3->name, nomin3->coord->latitude, nomin3->coord->longitude);
        assert(strcmp(nomin3->name, "Jardin Botanique Jean-Marie Pelt, Rue Victor Basch, Villers-lès-Nancy, Nancy, Meurthe-et-Moselle, Grand Est, France métropolitaine, 54600, France") == 0);
        assert(fabs(nomin3->coord->latitude - 48.660448) < 0.00001);
        assert(fabs(nomin3->coord->longitude - 6.155896) < 0.00001);

        Nominatim_t *nomin4 = get_nominatim("place stanislas");
        assert(nomin4 != NULL);
        // printf("---\nNAME:\t%s\nLAT:\t%f\t\tLON:\t%f\n", nomin4->name, nomin4->coord->latitude, nomin4->coord->longitude);
        assert(strcmp(nomin4->name, "Place Stanislas, Saint-Nicolas - Charles III - Ville vieille - Trois Maisons - Léopold, Nancy, Meurthe-et-Moselle, Grand Est, France métropolitaine, 54100, France") == 0);
        assert(fabs(nomin4->coord->latitude - 48.693524) < 0.00001);
        assert(fabs(nomin4->coord->longitude - 6.183286) < 0.00001);

        destroy_nominatim(nomin1);
        destroy_nominatim(nomin2);
        destroy_nominatim(nomin3);
        destroy_nominatim(nomin4);
    }
}

snow_main();
