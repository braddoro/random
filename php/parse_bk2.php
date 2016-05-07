#!/usr/bin/php
<?php
$options    = getopt("c:n:s:u:d");
$customer   = isset($options['c']) ? $options['c'] : NULL;
$debug      = isset($options['d']) ? true : false;
$name       = isset($options['n']) ? $options['n'] : NULL;
$section    = isset($options['s']) ? $options['s'] : NULL;
$username   = isset($options['u']) ? $options['u'] : NULL;
$params     = array('section' => $section, 'username' => $username, 'debug' => $debug);
$curl       = curl_init($url);
if($name > 0){$url = "https://atdconnectdev.atdconnect.com/rs/3_0/users/getUsersByUserName?userName=$name";}
if($customer > 0){$url = "https://atdconnectdev.atdconnect.com/rs/3_0/users/getUsersByCriteria?customerNumber=$customer";}
if($location > 0){$url = "https://atdconnectdev.atdconnect.com/rs/3_0/users/getUsersByCriteria?locationNumber=$location";}
if($debug){echo $url . PHP_EOL; echo '--------------------------------------------------------' . PHP_EOL;}
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json','clientID: CMR_INTERNAL','Authorization: Basic Y21yVXNlcjpjbXJVc2Vy'));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
$json = curl_exec($ch);
curl_close($ch);
parse(json_decode($json,true), $params);
function parse($array, $params) {
    if($array[errorMessage]){
        foreach($array as $errors){
            if(sizeof($array[errorMessage]) == 1){
                echo $errors . PHP_EOL;
            }else{
                foreach($errors as $type => $group){
                    if($type == $section || is_null($section)){
                        foreach($group as $language => $message){
                            echo $type . ':' . $language . ':' . $message . PHP_EOL;
                        }
                    }
                }
            }
        }
    }
    $users=0;
    if($array[users]){
        foreach($array[users] as $user){
            foreach($user as $item){
                if($item[userName] == $params['username'] || is_null($params['username'])){
                    if($params['section'] == 'user' || is_null($params['section'])){
                        echo 'userName:' . $item[userName]        . PHP_EOL;
                        echo 'name....:' . $item[name]            . PHP_EOL;
                        echo 'phone...:' . $item[phone]           . PHP_EOL;
                        echo 'fax.....:' . $item[fax]             . PHP_EOL;
                        echo 'email...:' . $item[email]           . PHP_EOL;
                        echo 'active..:' . $item[active]          . PHP_EOL;
                        echo 'switch..:' . $item[switchStores]    . PHP_EOL;
                        echo 'role....:' . $item[role]            . PHP_EOL;
                    }
                    if($params['section'] == 'customer' || is_null($params['section'])){
                        foreach($item[customers] as $customer){
                            foreach($customer as $number){echo 'customer:' . $number . PHP_EOL;}
                        }
                    }
                    if($params['section'] == 'location' || is_null($params['section'])){
                        foreach($item[locations] as $location){
                            foreach($location as $number){echo 'location:' . $number . PHP_EOL;}
                        }
                    }
                    if($params['section'] == 'website' || is_null($params['section'])){
                        foreach($item[websites] as $website){
                            foreach($website as $web){echo 'website.:' . $web . PHP_EOL;}
                        }
                    }
                    if($params['section'] == 'permission' || is_null($params['section'])){
                        foreach($item[permissions] as $permission){
                            foreach($permission as $perm){echo 'perms...:' . $perm[website] . ':' . $perm[value] . PHP_EOL;}
                        }
                    }
                    if($params['section'] == 'configuration' || is_null($params['section'])){
                        foreach($item[configurations] as $configuration){
                            foreach($configuration as $config){echo 'config..:' . $config[configuration] . ':' . $config[value] . PHP_EOL;}
                        }
                    }
                    $users++;
                    echo '--------------------------------------------------------' . PHP_EOL;
                }
            }
        }
        if($params['debug']) {echo "Users: $users" . PHP_EOL;}
    }
}
// php parse.php -n 095000130 -- good test record
// php parse.php -n 417723 -- good test record
// php parse.php -c 237394 -- big
// php parse.php -c 103486 -u 054002502 -- small
// php parse.php -c 299338 -- medium
// php parse.php -c 162285 -- medium lots of locations
// php parse.php -c 299338 -u 299338 -- big lots of locations
?>