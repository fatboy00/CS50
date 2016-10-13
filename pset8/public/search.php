<?php

    require(__DIR__ . "/../includes/config.php");

    // numerically indexed array of places
    $places = [];

    // TODO: search database for places matching $_GET["geo"], store in $places
    $geoquery = $_GET["geo"];

    // now split out all the non-alpha numeric characters
    //    $geosplit = explode(",", trim($geoquery));  what about other non alpha numerics?
    $geosplit = preg_split("/[^a-zA-Z\d]/", $geoquery, -1, PREG_SPLIT_NO_EMPTY);

//    echo $geoquery . "\r\n";
//    print_r($geosplit);
//    echo "\r\n";
    
    $noPlaces = count($geosplit);
//    echo $noPlaces . "\r\n";

    // remove "US"  as all addresses are US
    if ( ($us = array_search("US", $geosplit)) !== false) 
    {
        unset($geosplit[$us]);
    }

//    print_r($geosplit);
//    echo "\r\n";
    
    // check if any element is a zip code (and, if so, search on that) or search on the remaining elements
    
    if ($noPlaces < 1)
    {
        echo "Please Enter: City, State, Postal Code";
    }
    else if ($noPlaces === 1)
    {
        
        if (strlen($geosplit[0]) === 5) // check for zip code
        {
            $places = CS50::query("SELECT * FROM places WHERE postal_code = ?", $geosplit[0]);
        }
        else if (strlen($geosplit[0]) === 2) // check for two letter state
        {
            $places = CS50::query("SELECT * FROM places WHERE admin_code1 = ?", strtoupper($geosplit[0]));
        }
        else // check place_name
        {
            $places = CS50::query("SELECT * FROM places WHERE place_name = ?", $geosplit[0]);
        }
        // and finally check full State name
        if (empty($places))
        {
            $places = CS50::query("SELECT * FROM places WHERE admin_name1 = ?", $geosplit[0]);
        }
    }
    else
    {
        $geostring = implode(" ", $geosplit); // reassempble into one string separated by spaces for FULLTEXT
        
        // now do this index search ting
        
        $places = CS50::query("SELECT * FROM places WHERE MATCH(postal_code, place_name, admin_name1) AGAINST (?)", $geostring);

    }
    
    
    

    // output places as JSON (pretty-printed for debugging convenience)
    header("Content-type: application/json");
    print(json_encode($places, JSON_PRETTY_PRINT));

?>
