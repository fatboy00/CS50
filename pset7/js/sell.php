<?php

    // configuration
    require("../includes/config.php"); 


    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("sell_form.php", ["title" => "Sell"]);
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
            // check if the session owner has the stock
            // who is the user?
            $user_info = CS50::query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
            
            // what stock does the user have?
            $positions = CS50::query("SELECT * FROM stocks WHERE user_id = ?", $_SESSION["id"]);

            // Loop through to check that session owner owns stock
            // set flag to false
            $update = false;
            foreach ($positions as $position)
            {
                if ($position["symbol"] === $stock["symbol"])
                {
                    //  update database
                    
                    //  remove stock
                    // calculate cash amount
                    $addcash = sprintf("%.2f",$position["shares"]*$stock["price"]);                 
                    // add cash to account (database)
                    CS50::query("UPDATE users SET cash = ? WHERE id = ?", $user_info[0]["cash"] + $addcash, $user_info[0]["id"]);
                    // delete the stock from database of stocks
                    $update = CS50::query("DELETE FROM stocks WHERE user_id = ? AND symbol = ?", $user_info[0]["id"], $stock["symbol"]);
                    // and update the history
                                // insert into history
                    CS50::query("INSERT INTO history (user_id, type, symbol, shares, price, date) VALUES(?, 'sell', ?, ? ,?, 
                        Now())",$user_info[0]["id"], $stock["symbol"], $position["shares"], $stock["price"]); 
                }
                
            }
            if ($update == false)
            {
                //   apologize as session person does not own stock
                apologize("You have no holdings in this stock.");
            }
            // else redirect to portfolio
                redirect("/");


        }
            

    }


?>
