<?php

    // configuration
    require("../includes/config.php"); 


    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("buy_form.php", ["title" => "Buy"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // get the stock price from yahoo 
        
        $stock_name = strtoupper($_POST["symbol"]);
        
        $stock = lookup($stock_name); // make sure its converted to upper case as well.
        $number = $_POST["number"];
        
        
        // validate submission
        if (empty($_POST["symbol"])) // stock to sell entered
        {
            apologize("You must provide a Stock Symbol.");
        }
        else if (empty($_POST["number"])) // shares to sell entered
        {
            apologize("You must provide a whole number of shares to purchase.");
        }
        else if (!preg_match("/^\d+$/", $_POST["number"])) // whole number?
        {
            apologize("Invalid Amount.  Provide whole number of shares.");
        }
        else if ($stock === false) // check stock symbol is valid
        {
            apologize("Invalid Stock Symbol.");
        }
        else 
        {
            // check if the session owner has enough money
            // who is the user?
            $user_info = CS50::query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
            $neededcash = sprintf("%.2f",$number * $stock["price"]);                 
            if ($user_info[0]["cash"] < $neededcash) // check there is sufficient funds
            {
                //   apologize as session person does not have da cash!
                apologize("You have insufficent funds");
            }
            // now add shares to portfolio

            CS50::query("INSERT INTO stocks (user_id, symbol, shares) VALUES(?, ?, ?)
                ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)",$user_info[0]["id"], $stock["symbol"], $number);
            // INSERT INTO stocks (user_id, symbol, shares) VALUES($user_info[0]["id"], '$stock["symbol"]', $number) ON DUPLICATE KEY UPDATE shares = shares + VALUES($number)

            // and subtract cash....
            CS50::query("UPDATE users SET cash = ? WHERE id = ?", $user_info[0]["cash"] - $neededcash, $user_info[0]["id"]);

            // insert into history
            CS50::query("INSERT INTO history (user_id, type, symbol, shares, price, date) VALUES(?, 'buy', ?, ? ,?, 
                Now())",$user_info[0]["id"], $stock["symbol"], $number, $stock["price"]); 

            // redirect to portfolio
            redirect("/");
        }
    }
?>
