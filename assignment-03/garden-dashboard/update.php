<?php

// Checks if the page is being loaded from an URL.
if(!sizeof($_POST)) {
    header('Location: dashboard.php');
    exit;
}

if(!$_POST['update']) {
    header('Location: dashboard.php');
    exit;
}

/* Loads the json file created by the service app
used to store all of the components settings and 
the current state of the system. */
$json = file_get_contents('../garden-service/log.json');

// Decodes the json to an associative array.
$data = json_decode($json, true);


$msg = '';

foreach($data as $element) {
    // Builds element by element the final page
    if(array_key_exists('state', $element)) {
        $state = $element['state'] == "NOMINAL" ? "" : "-on";
        $msg = '
            <div id="wrapper">
                <div class="alarm">
                    <div class="idname">
                        <h3>Current state:</h3>
                        <h3 class="state-' . $element['state'] . '">' . $element['state'] . '</h3>
                    </div>
                    <div class="img">
                        <img src="./themes/imgs/alarm'. $state .'.png">
                    </div>
                </div>
            ';
    }
    
    if(array_key_exists('component', $element)) {
        $component = $element['component'];
        $hw = strpos($component['name'], "led") !== false ? "light" : "water";
        $state = $component['state'] == 0 ? 'off' : 'on';
        $img_state = $state == 'on' ? "-on" : "";

        $msg .= '
                <div class="component ' . $hw . '">
                    <div class="idname">
                        <h3>' . $component['id'] . '.</h3>
                        <h3>' . $component['name'] .'</h3>
                    </div>
                    <div class="img">
                        <img src="./themes/imgs/' . $hw . $img_state . '.png" alt="'. $hw .'">
                    </div>
                    <div class="state">
                        <p>On/Off</p>
                        <p class="checkbox ' . $state . '"></p>
                    </div>
                    <div class="intensity">
                        <p>Intensity</p>
                        <p>' . $component['intensity'] . '</p>
                    </div>
                </div>
                ';
    }
}

$msg .= '
            </div>
';

/* This msg will becone part of the final rendered
page to the end user. */
echo $msg;

?>