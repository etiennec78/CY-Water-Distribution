#ifndef DATA_H
#define DATA_H

typedef enum {
    SOURCE,
    SPRING,
    POND,
    CATCHMENT,
    WELL,
    RESURGENCE,
    INTAKE,
    BOREHOLE,
    FOUNTAIN,
    RESERVOIR,
    FORAGE,
    WELL_FIELDS,
    TANK,
    UNIT,
    MODULE,
    FACILITY_COMPLEX,
    STORAGE,
    JUNCTION,
    CUST,
} FacilityType;

typedef struct {
    FacilityType upstream;
    char* upstreamID;

    FacilityType facility;
    char* facilityID;
    
    int annualAmount;
    float leakPercentage;
} Capture;

 typedef struct {
    FacilityType facility;
    char* facilityID;
    
    int annualAmount;
} Factory;

 typedef struct {
    FacilityType facility;
    char* facilityID;

    FacilityType downstream;
    char* downstreamID;

    float leakPercentage;
} Storage;

 typedef struct {
    FacilityType facility;        //usine qui a traité l'eau de ce tronçon
    char* facilityID;

    FacilityType downstream;
    char* downstreamID;

    FacilityType junction;
    char* junctionID;

    float leakPercentage;
} principalDistribution;

 typedef struct {
    FacilityType facility;        //usine qui a traité l'eau de ce tronçon
    char* facilityID;

    FacilityType downstream;
    char* downstreamID;

    FacilityType junction;
    char* junctionID;

    float leakPercentage;
} secondaryDistribution;


typedef struct {
    FacilityType facility;        //usine qui a traité l'eau de ce tronçon
    char* facilityID;

    FacilityType downstream;
    char* downstreamID;

    FacilityType junction;
    char* junctionID;

    float leakPercentage;
} connexion;

#endif