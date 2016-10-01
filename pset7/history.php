<?php

    // configuration
    require("../includes/config.php"); 

    // Query Database for username
    $user_info = CS50::query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    $transactions_info = CS50::query("SELECT * FROM history WHERE user_id = ?", $_SESSION["id"]);


    // now pass all data to history_form.php
    render("history_form.php", ["title"  => "Transaction History", "transactions" => $transactions_info, "user_info" => $user_info]);

?>
