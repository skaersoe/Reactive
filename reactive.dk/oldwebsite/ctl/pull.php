<?

if ( $_POST['payload'] ) {
	echo exec('git pull origin master');	
}
?>