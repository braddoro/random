<?php
$algos = hash_algos();
$max = sizeof(hash_algos());
$options = getopt("s:dt:");
$algo = rand(0,$max-1);
$hash = $algos[$algo];
$size = isset($options['s']) ? $options['s'] : rand(8,15);
$type = isset($options['t']) ? $options['t'] : $algo;
$disp = isset($options['d']) ? $options['d'] : false;
if(isset($options['d'])) {
    print_r($algos);
    exit(0);
}
if(isset($options['t'])) {
    if(array_key_exists($type, $algos)) {
        $hash = $algos[$type];
    } else {
        die("The key given is not a valid hash type. \n");
    }
}
$randomString = substr(str_shuffle("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), 0, $size);
echo("$size - $randomString \n");
echo("$type - $hash \n");
$output = hash($hash,$randomString);
echo("$output\n\n");

foreach (new DirectoryIterator('../Podcasts') as $fileInfo) {
    if($fileInfo->isDot()) continue;
    echo $fileInfo->getFilename() . "\n";
    $name = $fileInfo->getPathname();

    //$command = "vlc --playlist-enqueue $name";
    //system( $command . " &> /tmp/error ");

    //echo $fileInfo->getFilename() . "\n";
    //echo $fileInfo->getBasename() . "\n";
    //$extension = pathinfo($fileInfo->getFilename(), PATHINFO_EXTENSION);
    //echo("$extension\n");

}
//#vlc --one-instance --playlist-enqueue The_Nerdist-20-586-Paul_Reubens.mp3
?>