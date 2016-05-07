#!/usr/bin/php
<?php
$options    = getopt("c:n:s:l:u:f:di");
$params['customer'] = isset($options['c']) ? $options['c'] : 0;
$params['debug']    = isset($options['d']) ? true : false;
$params['format']   = isset($options['f']) ? $options['f'] : 'h';
$params['location'] = isset($options['l']) ? $options['l'] : 0;
$params['name']     = isset($options['n']) ? $options['n'] : 0;
$params['section']  = isset($options['s']) ? $options['s'] : NULL;
$params['username'] = isset($options['u']) ? $options['u'] : NULL;
$params['info'] 	= isset($options['i']) ? true : false;
$params['tier'] 	= isset($options['t']) ? $options['t'] : NULL;

switch($params['tier']){
	case 'd':
		$params['url'] = 'https://atdconnectdev.atdconnect.com/rs/3_0/users/';
	break;
	case 'q':
		$params['url'] = 'https://atdconnectqa.atdconnect.com/rs/3_0/users/';
	break;
	case 'u':
		$params['url'] = 'https://testws.atdconnect.com/rs/3_0/users/';
	break;
	case 'p':
		$params['url'] = 'https://atdconnectdev.atdconnect.com/rs/3_0/users/';
	break;
	default:
		$params['url'] = 'https://ws.atdconnect.com/rs/3_0/users/';
	break;
};

if($params['debug']){
	echo $params['url'] . PHP_EOL;
}

if($params['info']) {
    echo '
Parameter Help:
c   customer    - search by customer number
l   location    - search by location number
n   name        - search by user name

u   username    - only display selected user name (not search)

f   format      - output format
                m - machine
                h - human
                n - none

t   tier      	- tier
                d - dev
                q - qa
                u - uat
				p - prd

s   section     - only display desired section
                valid values are:
                    user
                    customer
                    location
                    website
                    permission
                    configuration
                    location_data

i   info        - this message
    ' . PHP_EOL;
    exit(0);
}

$response = curlz($params);
$return = parsez($response, $params);
if($params['format'] == 'm'){var_export($return);}
//if($params['format'] == 'm'){var_dump($return);}
function curlz($params){
    if($params['name'] > ''){   $url = $params['url'] . 'getUsersByUserName?userName=' . $params['name'];}
    if($params['customer'] > 0){$url = $params['url'] . 'getUsersByCriteria?customerNumber=' . $params['customer'];}
    if($params['location'] > 0){$url = $params['url'] . 'getUsersByCriteria?locationNumber=' . $params['location'];}
if($params['debug']){
	echo($url . "\n");
}
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json','clientID: CMR_INTERNAL','Authorization: Basic Y21yVXNlcjpjbXJVc2Vy'));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    $json = curl_exec($ch);
	if($params['debug']){
		var_dump($json);
	}
	curl_close($ch);
    return $json;
}
function parsez($json, $params){
    $array = json_decode($json,true);
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
                $retval['status'] = -1;
            }else{
                foreach($errors as $type => $group){
                    if($type == $section || is_null($section)){
                        foreach($group as $language => $message){
                            echo $type . ':' . $language . ':' . $message . PHP_EOL;
                            $retval['errors'][] = $message;
                            $retval['status'] = -4;
                        }
                    }
                }
            }
        }
    }
    $users = 0;
    if($array[users]){
        foreach($array[users] as $user){
            foreach($user as $item){
                if($params['section'] == 'location_data'){
                    foreach($item[locations] as $location){
                        foreach($location as $number){
                            echo $item[userName] . "\t" . $number . PHP_EOL;
                        }
                    }
                }

                if($item[userName] == $params['username'] || is_null($params['username'])){
                    if($params['section'] == 'user' || is_null($params['section'])){
                        $css = ($item[switchStores])    ? 'Yes' : 'No';
                        $act = ($item[active])          ? 'Yes' : 'No';
                        $retval['data']['userName']     = $item[userName];
                        $retval['data']['name']         = $item[name];
                        $retval['data']['phone']        = $item[phone];
                        $retval['data']['fax']          = $item[fax];
                        $retval['data']['email']        = $item[email];
                        $retval['data']['active']       = $act;
                        $retval['data']['switchStores'] = $css;
                        $retval['data']['role']         = $item[role];
                        if($params['format'] == 'h'){
                            echo '-=[ User ]=-' . PHP_EOL;
                            echo "\t" . 'userName: ' . $item[userName]  . PHP_EOL;
                            echo "\t" . 'name....: ' . $item[name]      . PHP_EOL;
                            echo "\t" . 'phone...: ' . $item[phone]     . PHP_EOL;
                            echo "\t" . 'fax.....: ' . $item[fax]       . PHP_EOL;
                            echo "\t" . 'email...: ' . $item[email]     . PHP_EOL;
                            echo "\t" . 'active..: ' . $act             . PHP_EOL;
                            echo "\t" . 'switch..: ' . $css             . PHP_EOL;
                            echo "\t" . 'role....: ' . $item[role]      . PHP_EOL;
                        }
                    }
                    if($params['section'] == 'customer' || is_null($params['section'])){
                        foreach($item[customers] as $customer){
                            if($params['format'] == 'h'){
                                echo '-=[ Customers ]=-' . PHP_EOL;
                            }
                            foreach($customer as $number){
                                $retval['data']['customer'][] = $number;
                                if($params['format'] == 'h'){
                                    echo "\t" . $number . PHP_EOL;
                                }
                            }
                        }
                    }
                    if($params['section'] == 'location' || is_null($params['section'])){
                        foreach($item[locations] as $location){
                            if($params['format'] == 'h'){
                                echo '-=[ Locations ]=-' . PHP_EOL;
                            }
                            foreach($location as $number){
                                $retval['data']['location'][] = $number;
                                if($params['format'] == 'h'){
                                    echo "\t" . $number . PHP_EOL;
                                }
                            }
                        }
                    }
                    if($params['section'] == 'website' || is_null($params['section'])){
                        foreach($item[websites] as $website){
                            if($params['format'] == 'h'){
                                echo '-=[ Websites ]=-' . PHP_EOL;
                            }
                            foreach($website as $web){
                                $retval['data']['website'][$web] = $retval['data']['website'][$web] + 1;
                                if($params['format'] == 'h'){
                                    echo "\t" . $web . PHP_EOL;
                                }
                            }
                        }
                    }
                    if($params['section'] == 'permission' || is_null($params['section'])){
                        foreach($item[permissions] as $permission){
                            if($params['format'] == 'h'){
                                echo '-=[ Permissions ]=-' . PHP_EOL;
                            }
                            foreach($permission as $perm){
                                $retval['data']['permission'][$perm[website]] = $perm[value];
                                if($params['format'] == 'h'){
                                    echo "\t" . $perm[website] . ': ' . $perm[value] . PHP_EOL;
                                }
                            }
                        }
                    }
                    if($params['section'] == 'configuration' || is_null($params['section'])){
                        foreach($item[configurations] as $configuration){
                            if($params['format'] == 'h'){
                                echo '-=[ Configurations ]=-' . PHP_EOL;
                            }
                            foreach($configuration as $config){
                                $retval['data']['configuration'][$config[configuration]] = $config[value];
                                if($params['format'] == 'h'){
                                    echo "\t" . $config[configuration] . ': ' . $config[value] . PHP_EOL;
                                }
                            }
                        }
                    }
                    if($params['format'] == 'h'){
                        echo '----------------------------------------------------------------------'. PHP_EOL;
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
// php parse.php -l 267970
// php parse.php -c 418607
// php parse.php -c 162285
// php parse.php -c 237394 -- big
// php parse.php -c 299338 -- medium good
// php parse.php -n 417723 -- good test record
// php parse.php -c 103486 -u 054002502 -- small
// php parse.php -n 095000130 -- good test record
// php parse.php -c 162285 -- medium lots of locations
// php parse.php -c 299338 -u 299338 -- big lots of locations
?>
