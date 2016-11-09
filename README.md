# nemesis
nemesis is a freebsd and Mac OS X (tested through 10.11) port based on TrueCrypt, i'm slowly rebranding it to comply with the original license but that is probably my least favorite task. which is not to say i didn't have some fun with it...whereever it is my right to dictate, the 2 clause BSD license shall apply.<br>
OS - X nemesis binary here: http://mrn.sixbit.org/files/nemesis-667.dmg - this version is statically linked with wxWidgets 3.1.0 and includes a copy of a OSXFuse 3.5.3 in the DMG - if you don't have it installed already, install FUSE first WITH THE MACFUSE COMPATIBILITY LAYER ENABLED - i figure, it's about time.<br>
TAKE HEED: TAKE WARNING: TAKE HEED:<br>
<b>AES is headed for the chopping block - it doesn't belong here. call me paranoid, then read about bSafe, and if you're fine with keeping your data under one algorithm developed by the NSA, we are not in agreement, there are excellent alternatives available. Move to the Serpent-Twofish-Camellia chain, with the BLAKE-512 hash, the performance cost is negligible on modern hardware. also, expect the 256 bit hashes to be removed - and replaced, with 256 bit memory hardened versions based on (but very much NOT yescrypt) - this hash change, is imminent, as in it's my current project. AES will be removed when i replace it with another cipher, selected for use in a new combiner to replace one of the combinations involving AES. that could be a while as i haven't given it all the thought yet. but do take note, if you use the 256 bit hashes, change that up. when i break everything, i'll put a link to the last commit with support up for stragglers. if you are using Whirlpool or the BLAKE-512 hash you are good, they are not leaving. this project isn't about beating a dead horse for eternity. i've made clear my feelings on AES from the start.<br></b>
as of 11/8 the OS X and FreeBSD builds are significantly less headachey, and as a result of that work, wxWidgets-3.1.0 is included in the source tree, similar to the original configuration. by default the project builds as a statically linked version again. undefine WX_STATIC in the Makefile to use a dynamic system wide install, 3.0.2 should work fine if you already have it. on OS X, install OsxFuse 3.5.3, from the website, not ports, not brew, make sure to enable the compatibility layer during installation. ports or brew versions of OsxFuse may work but i don't know about that and i'm not going that route again without good reason.<br>
to build, on OS X or FreeBSD: go to nemesis source directory; "make wxbuild; make" and you will have an application  sitting in Main/, linked against current Fuse and wxWidgets libs, without going through a bunch of package managers and headaches and annoyance.<br>
because hot coffee, obligatory warning not to use this with windows secure boot. as it is right now it may allow some impossible permutations of hash and encryption algorithm if it builds at all and i have no means of testing so don't trash your disk and blame me. my goal isn't a windows boot application.<br>
they appear to be gone with wxWidgets 3.1.0 but it is safe to suppress and ignore display format error messages about wx.
as of now support for SHA-3 finalist BLAKE 512 is included, at 800,000 iterations.<br>
![screen shot 2016-10-14 at 7 04 42 pm](https://cloud.githubusercontent.com/assets/22229007/19405272/c637326c-9243-11e6-9ff8-188012d2c399.png)<br>
i had problems building security/truecrypt some months ago and while i was fixing the build i noticed some other things i wanted to fix, and i've always been interested in changing some things.<br>
also, i have never been a fan of AES and i decided to take a crack at replacing it with a non-NIST cipher, so I spent a day merging in Camellia and adding a cipher chain using it instead of AES, that may be more secure.<br>
![capture2](https://cloud.githubusercontent.com/assets/22229007/18573416/36651456-7b91-11e6-9128-2e220f834c5a.png)<br>
UFS has been deprecated from this build, and replaced with ext3. you'd think that's strange for a freebsd application, but support for UFS was fairly broken, and i was able to get it reading and writing ext3 via the ext2fs kernel module, so if you haven't got it loaded already, kldload ext2fs before running nemesis.<br>
once i got things working with ext3 and fat, i was able to get hidden volumes to work again. the problem was related to both the broken UFS support and some not smart behavior on behalf of mount. now, we look for an ext3 volume first, and failing that, we then look for a FAT volume, which could be the outer volume. end result, you get large file support, and hidden volumes back. not bad.<br>
![normal-innervol2](https://cloud.githubusercontent.com/assets/22229007/18756831/5e0f18f2-80bf-11e6-86ae-99b8597a2b77.png)<br>
![outer-mounted2](https://cloud.githubusercontent.com/assets/22229007/18756885/9328a940-80bf-11e6-9fb0-b482d9e11c62.png)<br>
ultimately i'm planning to rewrite as much of this as possible and address as much of the audit as possible.<br>
i have a lot of time on my hands.<br>
i'd be quite ecstatic to eventually come up with a solid near complete rewrite to give back to the community as a port.<br>
i get to learn a lot of things and my freebsd friends get an updated homage to one of my most favorite crypto applications ever<br>
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
(http://www.phrack.org), (https://www.schneier.com/blog/archives/2013/09/take_back_the_i.html), (http://cppcrypto.sourceforge.net/), (https://github.com/mjosaarinen/brutus), (http://www.eecs.harvard.edu/~alon/PAPERS/combiners/combiners.ps), (http://www.openwall.com/presentations/PHDays2014-Yescrypt/), (http://www.karljapetre.com/whirlpool/), (https://www.iacr.org/archive/fse2006/40470042/40470042.pdf), (https://github.com/tbuktu/libntru), (http://goo.gl/7Pl6Fp), (https://brage.bibsys.no/xmlui/bitstream/handle/11250/258846/426901_FULLTEXT01.pdf)<br>
