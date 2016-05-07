<?php
$SPACE = ' ';
$END = '.' . PHP_EOL;
$a_name = array(
'ENL Cook Amy Winters (F)', 
'ENL Damage Control Mate Audrey Chavez (F)',
'ENL Damage Control Mate Claude Gilbert (M)',
'ENL Engineering Mate Edmund Patton (M)',
'ENL Engineering Mate Sebrina King (F)',
'ENL Gunnery Mate Janet Coleman (F)',
'ENL Gunnery Mate Levi Hicks (M)',
'ENL Gunnery Mate Sergent Randy Magnum (M)',
'ENL Soldier Barry Romero (M)',
'ENL Soldier Darnell Harmon (M)',
'ENL Soldier Jenna Graham (F)',
'ENL Soldier Kari Howell (F)',
'ENL Soldier Larry Diaz (M)',
'ENL Soldier Laurence Leonard (M)',
'ENL Soldier Myra Watts (F)',
'ENL Soldier Nael Taylor (M)',
'JG Administrator Debbie Snyder (F)',
'JG Bioengineer Joseph Jefferson (M)',
'JG Chef Emanuel Robinson (M)',
'JG Communication Samantha Andrews (F)',
'JG Navigator Mathew Barrett (M)',
'JG Nurse Hasuna Bouldermane (F)',
'JG Pilot Linda Wong (F)', 
'JG Planetary Science Sherman Terry (M)',
'JG Sensor Tech Terrance Todd (M)',
'Officer Darcasian Arren (M)', 
'Officer Lansolen (M)',
'Passenger Addison Cobb (F)',
'Passenger Laylia Anderson (F)', 
'Passenger Mallif (M)', 
);
$a_action = array(
'encountered', 
'encountered', 
'encountered', 
'encountered', 
'encountered', 
'encountered', 
'encountered', 
'disagreement', 
'disagreement', 
'disagreement',
'bonded', 
'bonded', 
'bonded', 
'group', 
'group', 
'injury', 
'fight'
);
$a_room = array('stair well', 'stair well', 'med bay', 'galley', 'galley', 'training bay', 'bunkroom', 'cargo bay', 'deck', 'deck', 'meeting room');
$a_level = array('minor', 'minor', 'minor', 'minor', 'major');
$a_part = array('hand', 'foot', 'arm', 'leg', 'chest', 'back', 'head', 'face', 'abdomen', 'groin');
$a_player = array('Musty Stones', 'Mila Arron', 'Took Cobb', 'Earendur');
$a_location = array('upper', 'lower');
$a_attitude = array('complained about XXXX', 'commented about XXXX', 'had praise for XXXX', 'inquired about XXXX', 'asked a favor', 'asked a question');
$a_behavior = array('happy', 'quiet', 'laughing', 'silent', 'complaining', 'drinking', 'joking');
echo 'Drama Generator<br/><br/>';
$l = 0;
do {
	// Get basic stuff.
	// 
	$party1 = $a_name[array_rand($a_name)];
	$action = $a_action[array_rand($a_action)];
	$level = $a_level[array_rand($a_level)];
	$thing = '';
	
	// Fight, Disagreement.
	// 
	if($action == 'fight' || $action == 'disagreement'){
		$party2 = $a_name[array_rand($a_name)];
		do {
		    $party2 = $a_name[array_rand($a_name)];
		} while ($party1 == $party2);
		$room = $a_room[array_rand($a_room)];
		if($room == 'deck') {
			$room = $a_location[array_rand($a_location)] . $SPACE . $room;
		}
		
		$thing = $party1 . $SPACE . 'had a' . $SPACE . $level . $SPACE . $action . $SPACE . 'with' . $SPACE . $party2 . $SPACE . 'in the' . $SPACE . $room . $END;
	}
	
	// Injury.
	// 
	if($action == 'injury'){
		$room = $a_room[array_rand($a_room)];
		if($room == 'deck') {
			$room = $a_location[array_rand($a_location)] . $SPACE . $room;
		}
		$part = $a_part[array_rand($a_part)];
		$location = '';
		if($part == 'arm' || $part == 'leg' || $part == 'back') {
			$location = $SPACE . $a_location[array_rand($a_location)];
		}
		$thing = $party1 . $SPACE . 'received a' . $SPACE . $level . $SPACE . $action . $SPACE . 'to the' . $location . $SPACE . $part . $SPACE . 'in the' . $SPACE . $room . $END;
	}
	
	// Bond.
	// 
	if($action == 'bonded'){
		do {
			if(rand(1,5) < 4) {
				$party2 = $a_player[array_rand($a_player)];
			} else {
				$party2 = $a_name[array_rand($a_name)];
			}
		} while ($party1 == $party2);
		$room = $a_room[array_rand($a_room)];
		if($room == 'deck') {
			$room = $a_location[array_rand($a_location)] . $SPACE . $room;
		}
		$thing = $party1 . $SPACE . 'bonded with' . $SPACE . $party2 . $SPACE . 'in the' . $SPACE . $room . $END;
	}
	
	// Encounter.
	// 
	if($action == 'encountered'){
		$player = $a_player[array_rand($a_player)];
		do {
			if(rand(1,5) < 4) {
				$party2 = $a_player[array_rand($a_player)];
			} else {
				$party2 = $a_name[array_rand($a_name)];
			}
		} while ($party1 == $party2);
		$attitude = $a_attitude[array_rand($a_attitude)];
		$room = $a_room[array_rand($a_room)];
		if($room == 'deck') {
			$room = $a_location[array_rand($a_location)] . $SPACE . $room;
		}
		$thing = $party1 . $SPACE . 'interacted with' . $SPACE . $player . $SPACE . 'and' . $SPACE . $attitude . $END;
		$thing = str_replace('XXXX',$party2,$thing);
	}

	// Meeting.
	// 
	if($action == 'group'){
		do {
				$new = $a_name[array_rand($a_name)];
		} while (substr($new,0,3) == 'ENL');
		$type = substr($party1,0,3);
		$player = $a_player[array_rand($a_player)];
		$peeps = rand(3,5);
		$names = array();
		$x = 1;
		do {
				$new = $a_name[array_rand($a_name)];
				if (!in_array($new,$names) && substr($new,0,3) == $type) {
					$names[] = $new;
					$x++;
				}
		} while ($x <= $peeps);
		$room = $a_room[array_rand($a_room)];
		if($room == 'deck') {
			$room = $a_location[array_rand($a_location)] . $SPACE . $room;
		}
		switch($type){
			case 'ENL':
				$group = 'enlisted';
				break;
			case 'JG ':
				$group = 'junior officers';
				break;
			default:
				$group = 'people';
				break;
		}
		$behavior = $a_behavior[array_rand($a_behavior)];
		$thing = $player . ' encountered a ' . $behavior . ' group of ' . $group . ' near the ' . $room . $END;
	}
	$thing = str_replace("ENL ","",$thing);
	$thing = str_replace("JG ","",$thing);
	echo '* ' . $thing . '<br/>';

	if(isset($names) && count($names) > 0){
		foreach($names as $name){
			$cleaned = str_replace("ENL ","",$name);
			$cleaned = str_replace("JG ","",$cleaned);
			echo "&nbsp;&nbsp;&nbsp;&nbsp;" . $cleaned . "<br/>";
		}
	}
	$names = array();
	echo "<br/>";
	
	$l++;
} while ($l < 3);
//$file = '/home/bhughes/git/local/pick.php.log';
//$file = 'pick.php.log';
//$fp = fopen($file, 'a');
//fwrite($fp, date("Y-m-d H:i:s") . "\t" . $thing);
//$file = 'pick.php.log';
//echo file_get_contents($file);
?>
