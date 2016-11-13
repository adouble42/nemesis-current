# nemesis - http://mrn.sixbit.org
nemesis is a freebsd, debian linux (tested on jessie) and Mac OS X (tested through 10.11) port based on TrueCrypt, i'm slowly rebranding it to comply with the original license but that is probably my least favorite task. which is not to say i didn't have some fun with it...whereever it is my right to dictate, the 2 clause BSD license shall apply.<br>
OS - X nemesis binary release here: <b>http://mrn.sixbit.org/files/nemesis_667_fuse_bundle.dmg</b> - this version is statically linked with wxWidgets 3.1.0 and includes a bundled copy of OSXFuse 3.5.3 with MacFUSE compatibility enabled in the installer - if you don't have it installed already, just accept the defaults and install and you're good to go. if you're already running an older FUSE version or didn't install MacFUSE compatibility you may want to let it upgrade you anyway. you can disable the FUSE or the nemesis portions of the install as needed.<br>
using <b>https://github.com/adouble42/fuse-ext2</b> i've been able to read-write mount ext3fs volumes created with other versions of nemesis on OS X, and create volumes using mkfs.ext2 from the e2fsprogs version referenced in those instructions compatible with them. support isn't integrated yet directly, but to mount an ext3 volume from linux or freebsd, you would select "Do not mount" on the mount options dialog, and then mount the device listed under "properties" from the command line using <b>"fuse-ext2 \<device\> \<mount-point\> -o rw+"</b>. you'll need to dismount from the command line, and then follow up in nemesis when you're done. to create a volume, simply select "No filesystem" during the create process, mount as just described, except prior to executing "fuse-ext2", execute "/opt/gnu/sbin/mkfs.ext2 \<device\>"<br>
more news- starting with this release right now, and an upcoming binary .deb, support for linux will now be regularly maintained. debian is my reference platform, using clang and llvm to build as opposed to gcc, in order to maintain consistency. it's easy and it won't break your stuff to install them. "apt-get llvm clang build-essential" and the rest of the dependencies, and you should be executing "make wxbuild;make" in no time. regarding the issue with dm-mod vs loop mounts, it's fixed. you can enable kernel and hardware crypto in the preferences if you want it. ciphers that aren't compatible will automatically select the normal loop mount.<br>
no more dicking around with makefiles or wxwidgets, it finally is at a point of it just builds.<br>
see right here, it mounts that on a loop no problem.that's what we need. also, see what i meant about wx 3.1.0 kicking ass? it looks much better at higher res, though. this is my scrunched virtual environment.<br>
![screen shot 2016-11-10 at 12 32 22 pm](https://cloud.githubusercontent.com/assets/22229007/20188149/5ca4ce56-a745-11e6-8474-8209d4badc04.png)<br>
while you're building that or waiting for the binary .deb which is certainly en route, here, still fresh and new- the os x binary initial release:<br>
<center>BLAKE512SUM of <b>http://mrn.sixbit.org/files/nemesis_667_fuse_bundle.dmg</b>: <i>fe9ef6a1a755b2be225e28e4b520a3bc2cab731e338dac4ff7d7b197b5ae9dc2eee8896ed91c77f5bd3e8893b7ec53e6098dd6a9355a277842d3e3e30d2a1d81</i></center><br>
BLAKE hash utilities - <b>http://mrn.sixbit.org/files/BLAKE.tgz</b>
<br><br>
TAKE HEED: TAKE WARNING: TAKE HEED:<br>
<b>AES is headed for the chopping block - it doesn't belong here. call me paranoid, then read about bSafe, and if you're fine with keeping your data under one algorithm developed by the NSA, we are not in agreement, there are excellent alternatives available. Move to the Serpent-Twofish-Camellia chain, with the BLAKE-512 hash, the performance cost is negligible on modern hardware. also, expect the 256 bit hashes to be removed - and replaced, with 256 bit memory hardened versions based on (but very much NOT yescrypt) - this hash change, is imminent, as in it's my current project. AES is deprecated here in favor of Camellia. write support for it will be removed in the near future. take note, if you use the 256 bit hashes, change that up. when i break everything, i'll put a link to the last commit with support up for stragglers. if you are using Whirlpool or the BLAKE-512 hash you are good, they are not leaving. Whirlpool may add iterations in the future. this project isn't about beating a dead horse for eternity. i've made clear my feelings on AES from the start.<br></b>
it is possible that a TrueCrypt compatibility switch will be added when mounting volumes to mount volumes with old parameters and methods, in read only mode, to allow for transfer to up to date methods. the way the hashes work, we'll likely start first by checking for the reduced and non-memory hard versions followed by the hardened versions, as the hash is determined by trying them all until one works. i strongly recommend moving to the Serpent-Twofish-Camellia chain and BLAKE-512 hash as these are current, i just added them. they are the replacement i'm using for AES here.<br>
as of 11/8 the OS X and FreeBSD builds are significantly less headachey, and as a result of that work, wxWidgets-3.1.0 is included in the source tree, similar to the original configuration. by default the project builds as a statically linked version again. undefine WX_STATIC in the Makefile to use a dynamic system wide install, 3.0.2 will work fine if you already have it (tested with package "libwxgtk3.0-dev" on Debian jessie). if you go the dynamic route, it will complain about window size parameters, frequently, unless you suppress that at the initial warning. on OS X, install OsxFuse 3.5.3, from the website, not ports, not brew, make sure to enable the compatibility layer during installation. ports or brew versions of OsxFuse may work but i don't know about that and i'm not going that route again without good reason.<br>
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
#DicksOutForHarambe
![capture](https://cloud.githubusercontent.com/assets/22229007/18573392/06267938-7b91-11e6-867b-3a7a281830b4.png)
<br>
"We can make surveillance expensive again. In particular, we need open protocols, open implementations, open systems -- these will be harder for the NSA to subvert." -- bruce schneier<br>
required reading:<br>
(http://www.phrack.org), (https://www.schneier.com/blog/archives/2013/09/take_back_the_i.html), (http://cppcrypto.sourceforge.net/), (https://github.com/mjosaarinen/brutus), (http://www.eecs.harvard.edu/~alon/PAPERS/combiners/combiners.ps), (http://www.openwall.com/presentations/PHDays2014-Yescrypt/), (http://www.karljapetre.com/whirlpool/), (https://www.iacr.org/archive/fse2006/40470042/40470042.pdf), (https://github.com/tbuktu/libntru), (http://goo.gl/7Pl6Fp), (https://brage.bibsys.no/xmlui/bitstream/handle/11250/258846/426901_FULLTEXT01.pdf)<br>
