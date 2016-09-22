# nemesis
nemesis is a freebsd port based on TrueCrypt.<br>
i had problems building security/truecrypt yesterday and while i was fixing the build i noticed some things i wanted to fix.<br>
it doesn't get all goofy about creating new volumes and claim there's no disk space.<br>
also, i have never been a fan of AES and i decided to take a crack at replacing it with a non-NIST cipher, that isn't known to be vulnerable as AES is, so I spent the day merging in Camellia and adding a cipher chain using it instead of AES that while slow as balls is probably vastly more secure.<br>
this is a labor of love.<br>
ultimately i'm planning to rewrite as much of this as possible and address as much of the audit as possible.<br>
i have a lot of time on my hands.<br>
i'd like to add Skein and ditch most of the hashes, try to protect the key in memory, perhaps.<br>
last thing - the main goal here was to get this to work on freebsd so i'm not going to maintain anything that gets in the way of that end. in fact, i'm probably going to start by gutting as much non freebsd related cruft as possible.<br>
i'd be quite ecstatic to eventually come up with a solid near complete rewrite to give back to the community as a port.<br>
i get to learn a lot of things and my freebsd friends get an updated homage to one of my most favorite crypto applications ever
this is already fun for me.<br>
don't ruin it.
<br>
this repo is set up so you can just clone it in /usr/ports/security and then <br>
cd nemesis<br>
mv *gz /usr/ports/distfiles<br>
make<br>
it'll pull in wxWidgets and what have you. <br>
if you just want my working copy check out nemesis-current. you could probably run make extract on this port and then dump nemesis-current in WORKSRC and build whatever is there. if this is excessive, tough luck, i'm new to git commits and i like to keep a lot of backups.<br>
<br>
#DicksOutForHarambe

![capture](https://cloud.githubusercontent.com/assets/22229007/18573392/06267938-7b91-11e6-867b-3a7a281830b4.png)
![capture2](https://cloud.githubusercontent.com/assets/22229007/18573416/36651456-7b91-11e6-9128-2e220f834c5a.png)

things to add/influences - skein hashes
(https://www.schneier.com/academic/skein/)
all of the cpp crypto you could ever use.
(http://cppcrypto.sourceforge.net/)
