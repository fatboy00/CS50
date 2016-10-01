<?php

    // configuration
    require("../includes/config.php"); 


    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("deposit_form.php", ["title" => "Deposit"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // get the stock price from yahoo 
        
        $amount = $_POST["amount"];
        
        
        // validate submission
        if (empty($_POST["amount"])) // stock to sell entered
        {
            apologize("You must provide an amount to deposit.");
        }
        else if (!is_numeric($_POST["amount"])) // must be a number
        {
            apologize("You must provide a numeric amount to deposit.");
        }
        else 
        {
            // who is the user?
            $user_info = CS50::query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);

            // and add cash....
            CS50::query("UPDATE users SET cash = ? WHERE id = ?", $user_info[0]["cash"] + $amount, $user_info[0]["id"]);

            // insert into history
            CS50::query("INSERT INTO history (user_id, type, symbol, shares, price, date) VALUES(?, 'dep', 'cash', ? ,'1', 
                Now())",$user_info[0]["id"], $amount); 

            // redirect to portfolio
            redirect("/");
        }
    }
?>
