<?php
$command = "vlc --one-instance --playlist-enqueue ../Podcasts";
system( $command . " &> /tmp/error ");

//foreach (new DirectoryIterator('../Podcasts') as $fileInfo) {
//    if($fileInfo->isDot()) continue;
//    echo $fileInfo->getFilename() . "\n";
//    $name = $fileInfo->getPathname();
//    $command = "vlc --one-instance --playlist-enqueue $name";
//    system( $command . " &> /tmp/error ");
//
//    //#vlc --one-instance --playlist-enqueue The_Nerdist-20-586-Paul_Reubens.mp3
//    //echo $fileInfo->getFilename() . "\n";
//    //echo $fileInfo->getBasename() . "\n";
//    //$extension = pathinfo($fileInfo->getFilename(), PATHINFO_EXTENSION);
//    //echo("$extension\n");
//}
?>