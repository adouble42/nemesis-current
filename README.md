# nemesis
nemesis is a freebsd port based on TrueCrypt, i'm slowly rebranding it to comply with the original license but that is probably my least favorite task. which is not to say i didn't have some fun with it...whereever it is my right to dictate, the 2 clause BSD license shall apply.<br>
building on OS X el capitan as well, now. also, i have unmerged changes from an ubuntu build in a separate and as of now less functional branch.<br>
because hot coffee, obligatory warning not to use this with windows secure boot. as it is right now it may allow some impossible permutations of hash and encryption algorithm if it builds at all and i have no means of testing so don't trash your disk and blame me. my goal isn't a windows boot application.<br>
as of now support for SHA-3 finalist BLAKE 512 is included, at 800,000 iterations.<br>
![screen shot 2016-10-14 at 7 04 42 pm](https://cloud.githubusercontent.com/assets/22229007/19405272/c637326c-9243-11e6-9ff8-188012d2c399.png)
i had problems building security/truecrypt last week and while i was fixing the build i noticed some other things i wanted to fix, and i've always been interested in changing some things.<br>
also, i have never been a fan of AES and i decided to take a crack at replacing it with a non-NIST cipher, so I spent a day merging in Camellia and adding a cipher chain using it instead of AES that while slow as balls is probably more secure.<br>
![capture2](https://cloud.githubusercontent.com/assets/22229007/18573416/36651456-7b91-11e6-9128-2e220f834c5a.png)<br>
UFS has been deprecated from this build, and replaced with ext3. you'd think that's strange for a freebsd application, but support for UFS was fairly broken, and i was able to get it reading and writing ext3 via the ext2fs kernel module, so if you haven't got it loaded already, kldload ext2fs before running nemesis.<br>
once i got things working with ext3 and fat, i was able to get hidden volumes to work again. the problem was related to both the broken UFS support and some not smart behavior on behalf of mount. now, we look for an ext3 volume first, and failing that, we then look for a FAT volume, which could be the outer volume. end result, you get large file support, and hidden volumes back. not bad.<br>
![normal-innervol2](https://cloud.githubusercontent.com/assets/22229007/18756831/5e0f18f2-80bf-11e6-86ae-99b8597a2b77.png)<br>
![outer-mounted2](https://cloud.githubusercontent.com/assets/22229007/18756885/9328a940-80bf-11e6-9fb0-b482d9e11c62.png)<br>
this version has also been updated to use a shared library version of wxWidgets 3.0.2 as opposed to the 2.8 packaged with the original TC. the wxgtk-3.0.2 in the ports tree doesn't seem to fully install itself, you will need to  make sure this can find wx-config 
or wx-config-3.0 to build. i'm probably not going to make that change back to static any time soon because it would drive me insane to rebuild wxWidgets every time i change something here, and i just don't love you all that much.<br>
ultimately i'm planning to rewrite as much of this as possible and address as much of the audit as possible.<br>
i have a lot of time on my hands.<br>
i'd like to add Skein and ditch most of the hashes, try to protect the key in memory, perhaps. instead of taking a CRC of the keyfile/password in cases where there is a key, i'd prefer to use a hash instead of that CRC. that sort of change would break backwards compatibility with the original, but i guess adding read support that allowed for the old way of mounting old volumes would be doable, with perhaps a migration wizard. <br>
last thing - the main goal here was to get this to work on freebsd so i'm not going out of my way to maintain anything that gets in the way of that end.<br>
i'd be quite ecstatic to eventually come up with a solid near complete rewrite to give back to the community as a port.<br>
i get to learn a lot of things and my freebsd friends get an updated homage to one of my most favorite crypto applications ever
this is already fun for me.<br>
don't ruin it.
<br>
<br>
this builds via gmake on freebsd<br>
you need a working wx-config for wx-gtk2-3.0.2 specified in the Makefile<br>
in case you are having trouble you can git clone https://github.com/adouble42/wxWidgets-3-patched somewhere else, cd to there, run ./autogen.sh;./configure;make<br>
you're welcome<br>
<br>
<br>
#DicksOutForHarambe
![capture](https://cloud.githubusercontent.com/assets/22229007/18573392/06267938-7b91-11e6-867b-3a7a281830b4.png)
<br>
"We can make surveillance expensive again. In particular, we need open protocols, open implementations, open systems -- these will be harder for the NSA to subvert." -- bruce schneier<br>
required reading:<br>
(http://www.phrack.org), (https://www.schneier.com/blog/archives/2013/09/take_back_the_i.html), (http://cppcrypto.sourceforge.net/), (https://github.com/mjosaarinen/brutus),  (http://www.openwall.com/presentations/PHDays2014-Yescrypt/), (http://www.karljapetre.com/whirlpool/), (https://www.iacr.org/archive/fse2006/40470042/40470042.pdf), (https://github.com/tbuktu/libntru), (http://goo.gl/7Pl6Fp), (https://brage.bibsys.no/xmlui/bitstream/handle/11250/258846/426901_FULLTEXT01.pdf)<br>
