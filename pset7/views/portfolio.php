<h1>
<?php
//        dump($user_info, $positions);
        echo "Welcome back " . $user_info[0]["username"] . ". Your avalable balance : $" . number_format($user_info[0]["cash"], 2);

?>
</h1>


<table class ="table table-hover">
    <?php
        print("<tr>");
        print("<th>" . "symbol" . "</th>");
        print("<th>" . "name" . "</th>");
        print("<th>" . "shares" . "</th>");
        print("<th>" . "price" . "</th>");
        print("<th>" . "total value" . "</th>");
        print("</tr>");

        foreach ($positions as $position)
        {
            print("<tr>");
            print("<td>" . $position["symbol"] . "</td>");
            print("<td>" . $position["name"] . "</td>");
            print("<td>" . $position["shares"] . "</td>");
            print("<td>" . number_format($position["price"],2) . "</td>");
            print("<td> $" . number_format($position["total"],2) . "</td>");
            print("</tr>");
        }

    ?>
</table>
