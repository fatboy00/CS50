<?php

    // configuration
    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submission
        if (empty($_POST["username"]))
        {
            apologize("You must provide your username.");
        }
        else if (empty($_POST["password"]))
        {
            apologize("You must provide your password.");
        }
        else if ($_POST["password"] !== $_POST["confirmation"])
        {
            apologize("Your password and confirmation must match");
        }

        // insert a new user into database
        $rows = CS50::query("INSERT IGNORE INTO users (username, hash, cash) VALUES(?, ?, 1000.0000)", 
            $_POST["username"], password_hash($_POST["password"], PASSWORD_DEFAULT));
        
        // check if insert worked (is $rows equal to zero then fail)
        if ($rows !== 0)
        {
            //  get the id of the succesful insert
            $row = CS50::query("SELECT LAST_INSERT_ID() AS id");
            $id = $row[0]["id"];

            // remember that user's now logged in by storing user's ID in session
            $_SESSION["id"] = $id;

            // redirect to portfolio
            redirect("/index.php");
        }

        // else apologize
        apologize("Username already taken");
    }

?>
