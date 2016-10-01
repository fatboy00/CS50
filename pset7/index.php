<?php

    // configuration
    require("../includes/config.php"); 

    // Query Database for username
    $user_info = CS50::query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    $holdings_info = CS50::query("SELECT * FROM stocks WHERE user_id = ?", $_SESSION["id"]);

    // create an array called positions which stores all the info about the shares
    $combined_info = [];
    foreach ($holdings_info as $holding_info)
    {
        $stock = lookup($holding_info["symbol"]);
        if ($stock !== false)
        {
		    $combined_info[] = [
		    "name" => $stock["name"],
		    "price" => $stock["price"],
		    "shares" => $holding_info["shares"],
		    "symbol" => $holding_info["symbol"],
		    "total" => sprintf("%.2f", $holding_info["shares"]*$stock["price"])
		  ];
            
        }
    }

    // now pass all data to portfolio.php
    render("portfolio.php", ["title"  => "Positions", "positions" => $combined_info, "user_info" => $user_info]);

?>


/**
 * 
 * 
 * This was one way to do it!
 * 
    // create a new array that combines the previous array
    $combined_info = [
        "user_info" => $user_info,
        "holdings_info" => $holdings_info,
        ];
//    dump($user_info,$holdings_info, $combined_info);
//    dump($combined_info["user_info"], $combined_info["user_info"][0], $combined_info["user_info"][0]["username"] );


    // render portfolio
    render("portfolio.php", ["combined_info" => $combined_info]);


*
* 
* 
*/
