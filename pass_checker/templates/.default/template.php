<?php

//$StylesIfCracked = array(FALSE => 'ok-box', TRUE => 'not-ok-box');
$StylesIfCracked = array(FALSE => 'box ok', TRUE => 'box not-ok');
$Style = $StylesIfCracked[$arResult['password_cracked']];

?>
<div class="<?=$Style?>">
	<ul>
		<li> Username is "<?=$arResult['username']?>"
		<li> Salt is <?=$arResult['salt']?>
		<li> Hash is <?=$arResult['hash']?>
		<li> Password was <?= ($arResult['password_cracked']) ? '' : 'not' ?> cracked
		<?
		if ($arResult['password_cracked']) {
			echo '<li> Password is "' . $arResult['password'] . '"';
			} else {
			echo '<li> Your password is safe for now';
			}
		?>
	</ul>
</div>






