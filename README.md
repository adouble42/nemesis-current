# nemesis
nemesis is a freebsd port based on TrueCrypt.<br>
i had problems building security/truecrypt last week and while i was fixing the build i noticed some other things i wanted to fix.<br>
also, i have never been a fan of AES and i decided to take a crack at replacing it with a non-NIST cipher, so I spent a day merging in Camellia and adding a cipher chain using it instead of AES that while slow as balls is probably more secure.<br>
![capture2](https://cloud.githubusercontent.com/assets/22229007/18573416/36651456-7b91-11e6-9128-2e220f834c5a.png)<br>
UFS has been deprecated from this build, and replaced with ext3. you'd think that's strange for a freebsd application, but support for UFS was fairly broken, and i was able to get it reading and writing ext3 via the ext2fs kernel module, so if you haven't got it loaded already, kldload ext2fs before running nemesis.<br>
once i got things working with ext3 and fat, i was able to get hidden volumes to work again. the problem was related to both the broken UFS support and some not smart behavior on behalf of mount. now, we look for an ext3 volume first, and failing that, we then look for a FAT volume, which could be the outer volume. end result, you get large file support, and hidden volumes back. not bad.<br>
this version has also been updated to use a shared library version of wxWidgets 3.0.2 as opposed to the 2.8 packaged with the original TC. i'm considering making that static and including a wxWidgets port with this package as well, as the wxgtk-3.0.2 in the ports tree doesn't seem to fully install itself. you're going to need to install that and make sure this can find wx-config or wx-config-3.0 to build. i'm probably not going to make that change back to static any time soon because it would drive me insane to rebuild wxWidgets every time i change something here, and i just don't love you all that much.<br>
ultimately i'm planning to rewrite as much of this as possible and address as much of the audit as possible.<br>
i have a lot of time on my hands.<br>
i'd like to add Skein and ditch most of the hashes, try to protect the key in memory, perhaps.<br>
last thing - the main goal here was to get this to work on freebsd so i'm not going out of my way to maintain anything that gets in the way of that end.<br>
i'd be quite ecstatic to eventually come up with a solid near complete rewrite to give back to the community as a port.<br>
i get to learn a lot of things and my freebsd friends get an updated homage to one of my most favorite crypto applications ever
this is already fun for me.<br>
don't ruin it.
<br>
this is meant to be built via a freebsd port, which i've also been providing via the adouble42/nemesis repo, but at the moment the port lags behind some features, i'm going to update it when i get some more done here. you can just put a tarball of it in /usr/ports/distfiles and modify the Makefile from the port appropriately, make NO_CHECKSUM=yes from the port directory. you need a working wx<br>
<br>
#DicksOutForHarambe

![capture](https://cloud.githubusercontent.com/assets/22229007/18573392/06267938-7b91-11e6-867b-3a7a281830b4.png)

things to add/influences - skein hashes
(https://www.schneier.com/academic/skein/)
all of the cpp crypto you could ever use.
(http://cppcrypto.sourceforge.net/)
