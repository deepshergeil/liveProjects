<?php
require_once 'Facebook/autoload.php';
date_default_timezone_set('UTC');
// define variables and set to empty values
$bid=$nameErr = $emailErr = $genderErr = $websiteErr = "";
$name = $email = $gender = $comment = $website = "";
$seq=['A1','B1','C1','D1','E1','F1','G1','H1','I1','J1'];

?>

<!DOCTYPE HTML>
<html>
<head>
    <style>
        .error {color: #FF0000;}

        h2{text-align: center;}
        #slide {background-color:  	#f7f7f7;
            width: 700px;
            height: 200px;

            margin-left: auto;
            margin-right: auto;}
        #loc{
            display: none;
        }
        h2.fb{
            font-family: Verdana;
            letter-spacing: 1px;
            font-style: italic;
        }
        #slide1 {
            width: 800px;
            margin-left: auto;
            margin-right: auto;}

        .albums{
            width: 800px;
            margin-left: auto;
            margin-right: auto;
        }
        #searchbox{
            left:60px;
            position: relative;
        }
        #detailsA {
            width: 800px;
            background-color:  	#f7f7f7;
            text-align: center;
            margin-left: auto;
            margin-right: auto;}
        #detailsP {
            width: 800px;
            background-color:  	#f7f7f7;
            text-align: center;
            margin-left: auto;
            margin-right: auto;}

        #search {
            text-align: left;
            width:100%;

        }

        #alb{
            width: 800px;
            background-color:  	#f7f7f7;
            margin-left: auto;
            margin-right: auto;
            display: none;
        }
        #pst{
            width: 800px;
            background-color:  	#f7f7f7;
            margin-left: auto;
            margin-right: auto;
            display: none;

        }
        #B1,#D1,#F1,#H1,#J1
        {
            width: 800px;
            background-color:  	#f7f7f7;
            margin-left: auto;
            margin-right: auto;
            display: none;
        }
        .noRecords{
            width: 800px;
            background-color:  	#f7f7f7;
            text-align: center;
            margin-left: auto;
            margin-right: auto;
        }
    </style>

    <script lang="javascript" type="text/javascript">
        var seq=['A1','B1','C1','D1','E1','F1','G1','H1','I1','J1'];
        function populate(s1)
        {

            var val=s1.value;
            if(val == "place")
            {

                document.getElementById('loc').style.display="block";

            }
            else{
                document.getElementById('loc').style.display="none";
            }
        }


        function albumClick()
        {
           if( document.getElementById('alb').style.display=='table')
           {
               document.getElementById('alb').style.display="none";

           }
           else
           {
               document.getElementById('alb').style.display="table";
               document.getElementById('pst').style.display="none";
           }

        }

        function postClick()
        {
            if( document.getElementById('pst').style.display=='table')
            {
                document.getElementById('pst').style.display="none";

            }
            else
            {
                document.getElementById('pst').style.display="table";
                document.getElementById('alb').style.display="none";

            }
        }

        function changeShow(id,count) {
            count++;
            var elem= seq[count];

            if (document.getElementById(elem).style.display == 'block')
            {
                document.getElementById(elem).style.display = "none";
            }
            else
                document.getElementById(elem).style.display = "block";
        }
        function fieldcheck(set) {
            if(set==1)
            {
                document.getElementById('loc').style.display="block";
            }
            else
            {document.getElementById('loc').style.display="none";}

        }
        function setToDefault1() {
            var docu = document.forms['myform'];
            console.log(docu.name.value) ;
            docu.name.value="";
            docu.location.value = " ";
            docu.distance.value = " ";
            docu.select1.value = "user";
            document.getElementById("slide1").innerHTML = "";
            document.getElementById('loc').style.display="none";
        }
        function showHide(){


        }

    </script>
</head>
<body>
<div id ="slide">
    <form method="post" name="myform" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" id="faceform" >
        <h2 class="fb">Facebook Search</h2>
        <hr>
        Keyword: <input type="text" name="name"  value="<?php echo isset($_REQUEST['name']) ? $_REQUEST['name'] : '' ?>" required >
        <span class="error">* <?php echo $nameErr;?></span>
        <br>
        Type:  &nbsp &nbsp &nbsp  <select id="select1" name="select1" onchange="populate(this)">
            <option value="user"
                <?php
                if ((isset($_REQUEST['select1']) && $_REQUEST['select1'] == 'user'))
                    echo 'selected="selected"' ?>>users</option>
            <option value="page"
                <?php
                if ((isset($_REQUEST['select1']) && $_REQUEST['select1'] == 'page'))
                    echo 'selected="selected"' ?>>pages</option>
            <option value="event"
                <?php
                if ((isset($_REQUEST['select1']) && $_REQUEST['select1'] == 'event'))
                    echo 'selected="selected"' ?>>events</option>
            <option value="place"
                <?php
                if ((isset($_REQUEST['select1']) && $_REQUEST['select1'] == 'place'))
                    echo 'selected="selected"' ?>>places</option>
            <option value="group"
                <?php
                if ((isset($_REQUEST['select1']) && $_REQUEST['select1'] == 'group'))
                    echo 'selected="selected"' ?>>groups</option>
        </select>
        <br><br>
        <div id="loc" style="<?php if ((isset($_REQUEST['select1']) && $_REQUEST['select1'] == 'place'))
        {echo 'display:block;';} else {echo 'display:none;';} ?>">
            Location:
            <input type="text" name="location"  value="<?php echo isset($_REQUEST['location']) ? $_REQUEST['location'] : '' ?>">
            Distance(meters)<input type="text" name="distance"  value="<?php echo isset($_REQUEST['distance']) ? $_REQUEST['distance'] : '' ?>">
        </div>
        <br>
        <div id="searchbox">
            <input type="submit" name="submit"  value="Search" onsubmit="showHide()" >
            <input type="button" name="reset" value="Clear" onclick="setToDefault1()">
    </form>
        </div>

</div>
<div id = 'slide1'>
    <?php
    //start of php script
    echo"<br>";
    require_once 'Facebook/autoload.php';
  //  session_start();
    $fb = new Facebook\Facebook([
        'app_id' => '1821503231447458',
        'app_secret' => '7c2e8499fffb97a9a5c99ee06ffb2eea',
        'default_graph_version' => 'v2.8',
    ]);
    $fb->setDefaultAccessToken('EAAZA4pcURUaIBAEVN4YdZCpnhQGuu6Uql7yDdaEkaUD0Hd8Mll3XRMAS61DQpRWSD1akeNMU3z4Ejqm4XHZCCyDp4IZBdfzYXW6SHEZBj6KF3PthiUC2m9rfJGPVecCSTGYs08JJ1NbbhPx1zQUAw2fzSQksGkrgZD');
    $locfield=0;


    if($_SERVER['REQUEST_METHOD'] == "POST") {
        $name=$_POST['name'];
        $select1=$_POST['select1'];
        $location=$_POST['location'];
        $distance=$_POST['distance'];
        /*
        if (isset($_GET['locfield'])) {
            $set=$_GET['locfield'];
            echo '<script type="text/javascript">    fieldcheck($set);      </script>';
        }
        */

        if (empty($_POST["name"])) {
            $nameErr = "Name is required";
        } else {
            $name = test_input($_POST["name"]);
            // check if name only contains letters and whitespace
            if (!preg_match("/^[a-zA-Z ]*$/",$name)) {
                $nameErr = "Only letters and white space allowed";
            }
        }

        $type= $_POST['select1'];
        try
        {

            if($_POST['select1']=='place') //place is selected
            {
                $locfield=1;
                if(($_POST['location'])) //location is present
                {
                   // echo "abc".$location;
                    //  echo $_POST['location'];
                    $filedata = file_get_contents('https://maps.googleapis.com/maps/api/geocode/json?address='.$_POST['location'].'&key=AIzaSyAl8ALMSdANFEfLbVT1pgKr9-ucy2ZJqtc');
                    $json = json_decode($filedata, true);
                    $lat= $json['results'][0]['geometry']['location']['lat'];
                    $lng= $json['results'][0]['geometry']['location']['lng'];
                   // echo   $lat,$lng;
                    if(isset($_POST['distance'])) //distance present
                    {
                        $data ='/search?q='.$_POST["name"]."&type=".$_POST["select1"]."&center=".$lat.",". $lng."&distance=".$_POST['distance']."&fields=id,name,picture.width(700).height(700)";
                    }
                    else //no distance
                    {
                        $data ='/search?q='.$_POST["name"]."&type=".$_POST["select1"]."&center=".$lat.",". $lng."&fields=id,name,picture.width(700).height(700)";
                    }

                }
                else
                {
                    $data = '/search?q=' . $_POST["name"] . "&type=" . $_POST["select1"] . "&fields=id,name,picture.width(700).height(700)";
                }


            }
            else
            {
                if($_POST['select1']=='event')
                {
                    $data = '/search?q=' . $_POST["name"] . "&type=" . $_POST["select1"] . "&fields=id,name,picture.width(700).height(700),place";
                }
                else
                    $data = '/search?q=' . $_POST["name"] . "&type=" . $_POST["select1"] . "&fields=id,name,picture.width(700).height(700)";
            }

            $res = $fb->get($data);
            $graphEdge = $res->getGraphEdge()->asArray();
            if(count($graphEdge)!=0) {

                if ($_POST['select1'] == 'event') {

                    echo "<table bgcolor='#f7f7f7' style='width:100%' border='2'><tr><th>Profile Picture</th><th>Name</th><th>Place</th></tr>";
                    foreach ($graphEdge as $graphNode) {
                        $id = $graphNode['id'];
                        $url = $graphNode['picture']['url'];

                        if(isset($graphNode['place']))
                        $place= $graphNode['place']['name'];
                         else{
                             $place="";
                         }
                        echo "<tr><td>";
                        echo "<a href='";
                        echo $url;
                        echo "' target='_blank'>";
                        echo "<img src='";
                        echo $graphNode['picture']['url'];
                        echo "' width ='30px' height='40px'></a></td><td>";
                        echo $graphNode['name'];
                        $bid = $graphNode['id'];
                        echo "<td>";
                        echo $place;
                        echo "</td>";
                        echo "</tr>";


                    }
                    echo "</table>";
                    echo "</div>";

                } else {
                    echo "<table bgcolor='#f7f7f7' style='width:100%' border='2'><tr><th>Profile Picture</th><th>Name</th><th>Details</th></tr>";
                    foreach ($graphEdge as $graphNode) {
                        $id = $graphNode['id'];
                        $url = $graphNode['picture']['url'];
                        echo "<tr><td>";
                        echo "<a href='";
                        echo $url;
                        echo "' target='_blank'>";
                        echo "<img src='";
                        echo $graphNode['picture']['url'];
                        echo "' width ='40px' height='30px'></a></td><td>";
                        echo $graphNode['name'];
                        $bid = $graphNode['id'];
                        echo "<td>" .
                            "<a href='phpinfo.php?bill_id=" . $graphNode['id'] . "&name=" . $name . "&select1=" . $select1 . "&distance=" . $distance . "&location=" . $location . "&locfield=" . $locfield .
                            "'>Details</a>" . "</td>";
                        echo "</tr>";
                        //  echo $graphNode['id']." 1". $graphNode['picture']['url'];
                    }
                    echo "</table>";
                    echo "</div>";
                }
            }
            else{
                echo "<div class='noRecords'>No Records has been found</div>";
            }
        }
        catch(Facebook\Exceptions\FacebookResponseException $e) {
            // When Graph returns an error
            echo 'Graph returned an error: ' . $e->getMessage();
            exit;

        } catch(Facebook\Exceptions\FacebookSDKException $e) {
            // When validation fails or other local issues
            echo 'Facebook SDK returned an error: ' . $e->getMessage();
            exit;
        }


    }

    if ($_SERVER["REQUEST_METHOD"] == "GET") {
    if (isset($_GET['locfield'])) {
        $set=$_GET['locfield'];
        echo '<script type="text/javascript">    fieldcheck($set);      </script>';
    }
        if (isset($_GET['bill_id'])) {

            $bid = $_GET['bill_id'];

            $data = '/'.$bid.'?fields=id,name,picture.width(700).height(700),albums.limit(5){name,photos.limit(2){name,picture}},posts.limit(5)&access_token=EAAZA4pcURUaIBAEVN4YdZCpnhQGuu6Uql7yDdaEkaUD0Hd8Mll3XRMAS61DQpRWSD1akeNMU3z4Ejqm4XHZCCyDp4IZBdfzYXW6SHEZBj6KF3PthiUC2m9rfJGPVecCSTGYs08JJ1NbbhPx1zQUAw2fzSQksGkrgZD';
            $res = $fb->get($data); //fb sdk call
            $json = $res->getGraphNode()->asArray(); //getting graphnode
            if(isset($json['albums'])) {
                echo "<div id='detailsA'><a href='#'  onclick='albumClick();'>Albums</a></div>";
                echo "<br>";
                $count = 0;
                $num = "num";
                echo "<table id='alb' bgcolor='#f7f7f7' style='width:100%' border='2'>";
                foreach ($json['albums'] as $mydata) {
                    if(isset($mydata['photos']))
                    {
                        echo "<tr ><td ><a href='#' id=$seq[$count] onclick='changeShow(id,$count);'>" . $mydata['name'] . "</a></td></tr>";
                        $count++;

                        echo "<tr  id=$seq[$count]>";
                        echo "<td>";
                        foreach ($mydata['photos'] as $values) {
                            $id = $values['id'];
                            $datapic='/'.$id.'/picture?redirect=false';
                            $res1= $fb->get($datapic); //fb sdk call
                            $json1 = $res1->getGraphNode();
                          //  echo $json1['url'];
                            echo "<a href='";
                            echo $json1['url'];
                            echo "' target='_blank'>";
                            echo "<img src='";
                            echo $values['picture'];
                            echo "' width ='80px' height='80px ' border='2 solid white' border-color='white'>";
                            echo "</a>";
                        }
                        echo "</td>";
                        $count++;
                        echo "</tr>";
                    }
                    else{
                        echo "<tr ><td >" . $mydata['name'] . "</td></tr>";
                    }


                }
                echo "</table>";
            }
            else
            {
                echo "<div class='noRecords'>No Albums has been found</div>";
            }
            echo"<br><br>";
            if(isset($json['posts'])) {
                echo "<div id='detailsP'><a href='#' onclick='postClick();'>Posts</a></div>";
                echo "<br><br>";
                echo "<table id='pst' bgcolor='#f7f7f7' style='width:100%' border='2'>";
                echo "<tr style='text-align: left'><th>Message</th></tr>";
                foreach ($json['posts'] as $mydata)  //building posts
                {
                    if (isset($mydata['message']))
                        echo "<tr><td>" . $mydata['message'] . "</td></tr>";
                  //  else
                     //   echo "<tr><td>" . $mydata['story'] . "</td></tr>";

                }
                echo "</table>";
            }
            else{
                echo "<div class='noRecords'>No Posts has been found</div>";
            }

        }
    }
    function test_input($data) {
        $data = trim($data);
        $data = stripslashes($data);
        $data = htmlspecialchars($data);
        return $data;
    }
    //End of php script
    ?>
</div>
<?php
/*
echo "<h2>Your Input:</h2>";
echo $name;
echo "<br>";
echo $email;
echo "<br>";
echo $website;
echo "<br>";
echo $comment;
echo "<br>";
echo $gender;
*/
?>

<noscript>
</body>
</html>