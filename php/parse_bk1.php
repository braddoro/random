#!/usr/bin/php
<?php
// php parse.php -n 417723
// php parse.php -s customer -u 417723
// php parse.php -c 237394 -- big
// php parse.php -c 103486 -u 054002502 -s website
// php parse.php -c 299338
// php parse.php -c 162285 -- big
// php parse.php -c 095000130 -- one user
// php parse.php -c 299338 -u 299338
$options    = getopt("c:n:s:u:");
$customer   = isset($options['c']) ? $options['c'] : NULL;
$name       = isset($options['n']) ? $options['n'] : NULL;
$section    = isset($options['s']) ? $options['s'] : NULL;
$username   = isset($options['u']) ? $options['u'] : NULL;
$curl       = curl_init($url);
$headers = array('Accept: application/json','clientID: CMR_INTERNAL','Authorization: Basic Y21yVXNlcjpjbXJVc2Vy');
if ($name > 0) {
    $url = "https://atdconnectdev.atdconnect.com/rs/3_0/users/getUsersByUserName?userName=$name";
}
if ($customer > 0) {
    $url = "https://atdconnectdev.atdconnect.com/rs/3_0/users/getUsersByCriteria?customerNumber=$customer";
}
if ($location > 0) {
    $url = "https://atdconnectdev.atdconnect.com/rs/3_0/users/getUsersByCriteria?locationNumber=$location";
}
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
$json = curl_exec($ch);
curl_close($ch);
parse(json_decode($json,true), $section, $username);

function parse($array, $section = NULL, $username = NULL) {

    // Errors.
    //
    if($array[errorMessage]){
        foreach($array as $errors){
            if(sizeof($array[errorMessage]) == 1){

                // General error.
                //
                echo $errors . PHP_EOL;
            }else{

                // Validation error.
                //
                foreach($errors as $type => $group){
                    if ($type == $section || is_null($section)) {
                        foreach($group as $language => $message){
                            echo $type . ':' . $language . ':' . $message . PHP_EOL;
                        }
                    }
                }
            }
        }
    }
    // User array.
    //
    if($array[users]){
        foreach($array[users] as $user){
            foreach($user as $item){
                if($item[userName] == $username || is_null($username)){
                    if($section == 'user' || is_null($section)){
                        echo $item[userName]        . PHP_EOL;
                        echo $item[name]            . PHP_EOL;
                        echo $item[phone]           . PHP_EOL;
                        echo $item[fax]             . PHP_EOL;
                        echo $item[email]           . PHP_EOL;
                        echo $item[active]          . PHP_EOL;
                        echo $item[switchStores]    . PHP_EOL;
                        echo $item[role]            . PHP_EOL;
                    }
                    if($section == 'customer' || is_null($section)){
                        foreach($item[customers] as $customer){
                            foreach($customer as $number){
                                echo $number . PHP_EOL;
                            }
                        }
                    }
                    if($section == 'location' || is_null($section)){
                        foreach($item[locations] as $location){
                            foreach($location as $number){
                                echo $number . PHP_EOL;
                            }
                        }
                    }
                    if($section == 'website' || is_null($section)){
                        foreach($item[websites] as $website){
                            foreach($website as $web){
                                echo $web . PHP_EOL;
                            }
                        }
                    }
                    if($section == 'permission' || is_null($section)){
                        foreach($item[permissions] as $permission){
                            foreach($permission as $perm){
                                echo $perm[website] . ':' . $perm[value] . PHP_EOL;
                            }
                        }
                    }
                    if($section == 'configuration' || is_null($section)){
                        foreach($item[configurations] as $configuration){
                            foreach($configuration as $config){
                                echo $config[configuration] . ':' . $config[value] . PHP_EOL;
                            }
                        }
                    }
                    echo '--------------------------------------------------------' . PHP_EOL;
                }
            }
        }
    }
}
?>