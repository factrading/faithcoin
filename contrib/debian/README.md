
Debian
====================
This directory contains files used to package facd/fac-qt
for Debian-based Linux systems. If you compile facd/fac-qt yourself, there are some useful files here.

## fac: URI support ##


fac-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install fac-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your facqt binary to `/usr/bin`
and the `../../share/pixmaps/fac128.png` to `/usr/share/pixmaps`

fac-qt.protocol (KDE)

