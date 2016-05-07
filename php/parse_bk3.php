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
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json','clientID: CMR_INTERNAL','Authorization: Basic Y21yVXNlcjpjbXJVc2Vy'));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
$json = curl_exec($ch);
curl_close($ch);
$return = parse(json_decode($json,true), $params);
var_dump($return);
function parse($array, $params){
    $retval = array(
    	data            => array(),
      	status          => 0,
      	errors          => array(),
      	errorMessage	=> NULL,
   		totalRows       => 0,
   		startRow        => 0,
   		endRow          => 0
    );
    if($array[errorMessage]){
        foreach($array as $errors){
            if(sizeof($array[errorMessage]) == 1){
                echo $errors . PHP_EOL;
                $retval['errorMessage'] = $errors;
            }else{
                foreach($errors as $type => $group){
                    if($type == $section || is_null($section)){
                        foreach($group as $language => $message){
                            echo $type . ':' . $language . ':' . $message . PHP_EOL;
                            $retval['errors'][] = $message;
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
                        $retval['data']['userName']     = $item[userName];
                        $retval['data']['name']         = $item[name];
                        $retval['data']['phone']        = $item[phone];
                        $retval['data']['fax']          = $item[fax];
                        $retval['data']['email']        = $item[email];
                        $retval['data']['active']       = $item[active];
                        $retval['data']['switchStores'] = $item[switchStores];
                        $retval['data']['role']         = $item[role];
                    }
                    if($params['section'] == 'customer' || is_null($params['section'])){
                        foreach($item[customers] as $customer){
                            foreach($customer as $number){
                                $retval['data']['customer'][] = $number;
                            }
                        }
                    }
                    if($params['section'] == 'location' || is_null($params['section'])){
                        foreach($item[locations] as $location){
                            foreach($location as $number){
                                $retval['data']['location'][] = $number;
                            }
                        }
                    }
                    if($params['section'] == 'website' || is_null($params['section'])){
                        foreach($item[websites] as $website){
                            foreach($website as $web){
                                $retval['data']['website'][$web] = $retval['data']['website'][$web] + 1;
                            }
                        }
                    }
                    if($params['section'] == 'permission' || is_null($params['section'])){
                        foreach($item[permissions] as $permission){
                            foreach($permission as $perm){
                                $retval['data']['permission'][$perm[website]] = $perm[value];
                            }
                        }
                    }
                    if($params['section'] == 'configuration' || is_null($params['section'])){
                        foreach($item[configurations] as $configuration){
                            foreach($configuration as $config){
                                $retval['data']['configuration'][$config[configuration]] = $config[value];
                            }
                        }
                    }
                    $users++;
                }
            }
        }
        $retval['totalRows'] = $users;
        $retval['endRow'] = $users;
    }
    return $retval;
}
// php parse.php -c 162285
// php parse.php -c 237394 -- big
// php parse.php -c 299338 -- medium
// php parse.php -n 417723 -- good test record
// php parse.php -c 103486 -u 054002502 -- small
// php parse.php -n 095000130 -- good test record
// php parse.php -c 162285 -- medium lots of locations
// php parse.php -c 299338 -u 299338 -- big lots of locations
?>