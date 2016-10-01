<?php

    // configuration
    require("../includes/config.php"); 


    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("stock_symbol.php", ["title" => "Quote"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // get the stock price from yahoo 
        $stock = lookup($_POST["symbol"]);
        // validate submission
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a Stock Symbol.");
        }
        
        else if ($stock === false) // check stock symbol is valid
        {
        //   apologize if not 
            apologize("Invalid Stock Symbol.");
        // redirect to quote.php
                
        }
        else 
        {
            // render the quote 
            render("stock_price.php", ["stock" => $stock]);
        }
            

    }


?>
