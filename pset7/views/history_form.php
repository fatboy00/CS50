<h1>
<?php
        echo "Welcome back " . $user_info[0]["username"] . ". Your avalable balance : $" . number_format($user_info[0]["cash"], 2);

?>
</h1>


<table class ="table table-hover">
    <?php
        print("<tr>");
        print("<th>" . "Buy/Sell" . "</th>");
        print("<th>" . "Date/Time" . "</th>");
        print("<th>" . "Stock" . "</th>");
        print("<th>" . "Number" . "</th>");
        print("<th>" . "Price" . "</th>");
        print("</tr>");

        foreach ($transactions as $transaction)
        {
            print("<tr>");
            print("<td>" . $transaction["type"] . "</td>");
            print("<td>" . $transaction["date"] . "</td>");
            print("<td>" . $transaction["symbol"] . "</td>");
            print("<td>" . $transaction["shares"] . "</td>");
            print("<td>" . number_format($transaction["price"],2) . "</td>");
            print("</tr>");
        }

    ?>
</table>
