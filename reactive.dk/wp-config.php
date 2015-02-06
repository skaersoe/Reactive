<?php
/**
 * The base configurations of the WordPress.
 *
 * This file has the following configurations: MySQL settings, Table Prefix,
 * Secret Keys, and ABSPATH. You can find more information by visiting
 * {@link http://codex.wordpress.org/Editing_wp-config.php Editing wp-config.php}
 * Codex page. You can get the MySQL settings from your web host.
 *
 * This file is used by the wp-config.php creation script during the
 * installation. You don't have to use the web site, you can just copy this file
 * to "wp-config.php" and fill in the values.
 *
 * @package WordPress
 */

// ** MySQL settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define('DB_NAME', 'reactiveweb');

/** MySQL database username */
define('DB_USER', 'reactiveweb');

/** MySQL database password */
define('DB_PASSWORD', 'oliverkat');

/** MySQL hostname */
define('DB_HOST', 'localhost');

/** Database Charset to use in creating database tables. */
define('DB_CHARSET', 'utf8');

/** The Database Collate type. Don't change this if in doubt. */
define('DB_COLLATE', '');

/**#@+
 * Authentication Unique Keys and Salts.
 *
 * Change these to different unique phrases!
 * You can generate these using the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}
 * You can change these at any point in time to invalidate all existing cookies. This will force all users to have to log in again.
 *
 * @since 2.6.0
 */
define('AUTH_KEY',         '{X>Qu*|/LVA&gj2+Tmv.}zFNkHb|}`sXvD6XmtNJ}w$1bI(1js-7^P/ZsYDSn{F_');
define('SECURE_AUTH_KEY',  'qyCUb_9/%@59$&dHKZHc(i /$T}rqMtgz;+Gp?8)2Vp}-K;98?9^^F:_Ej84 ~Q ');
define('LOGGED_IN_KEY',    'b}UDbu_!F{%B#>fI/?OTG/#>!.rMs.hW2:gAOjm5{o$P|}~i-Kt5 6>H/bjMb%rm');
define('NONCE_KEY',        'Tvv3;| 4/<CGwX#.xTaRfLP=]*V(z3>X}bGFp;[YIr7t[@#D1q0Wgs>|:(4(YP#R');
define('AUTH_SALT',        'Vpd@72~{bdx,+-eqqpOo,I%:9]5fx=iSh)>fs|me :PGeG`34mE3NCeOlg|n*{e-');
define('SECURE_AUTH_SALT', '`lo]0`o11rA2P$x9 zMG7C^kMvx~}7r~gX:KLO+b]_m*x97Nv)guV%$`Sh9XU1>w');
define('LOGGED_IN_SALT',   'iqD3-^ep0!=-ikqv(mQMnNTgaFtmD+t,Qt8>{>>F3]{+!N[%&Nx}vS&k0x90fyt6');
define('NONCE_SALT',       'bb>L#bt!rNd7?c$%5m<W>.(?kRBR?ghQKfbZmw?>sKg;,#snJ8CSpiCk57L:wjF ');

/**#@-*/

/**
 * WordPress Database Table prefix.
 *
 * You can have multiple installations in one database if you give each a unique
 * prefix. Only numbers, letters, and underscores please!
 */
$table_prefix  = 'wp_';

/**
 * For developers: WordPress debugging mode.
 *
 * Change this to true to enable the display of notices during development.
 * It is strongly recommended that plugin and theme developers use WP_DEBUG
 * in their development environments.
 */
define('WP_DEBUG', false);

/* That's all, stop editing! Happy blogging. */

/** Absolute path to the WordPress directory. */
if ( !defined('ABSPATH') )
	define('ABSPATH', dirname(__FILE__) . '/');

/** Sets up WordPress vars and included files. */
require_once(ABSPATH . 'wp-settings.php');
