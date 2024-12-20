#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/cmdfreak
if [ -e $BUILDDIR ]; then
    yes|rm -r $BUILDDIR
fi
mkdir -p $BUILDDIR/SOURCES


# Build
cd ../src/restapi
make all
cp cmdfreak $BUILDDIR/SOURCES
cp sample.conf $BUILDDIR/SOURCES
echo servicehost=127.0.0.1 >> $BUILDDIR/SOURCES/sample.conf
echo serviceport=8085 >> $BUILDDIR/SOURCES/sample.conf
cp sample.dat $BUILDDIR/SOURCES
cp cmdfreak.service $BUILDDIR/SOURCES
cp cmdfreak_nginx.conf $BUILDDIR/SOURCES
cp ../resource/cmdfreak.html $BUILDDIR/SOURCES
cp ../resource/cmdfreak.js $BUILDDIR/SOURCES
cp ../resource/img/favicon-16x16.png $BUILDDIR/SOURCES
cp ../resource/img/cristal_image48c.png $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/stkcommon.js $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/stkcommon.css $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp_um/stkcommon_um.js $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/jquery-3.2.0.min.js $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/bootstrap-4.4.1-dist.zip $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/IcoMoon-Free.css $BUILDDIR/SOURCES
cp ../../../YaizuComLib/src/stkwebapp/IcoMoon-Free.ttf $BUILDDIR/SOURCES
cd ../../../YaizuComLib/src/stkwebapp
make all
cp stkwebappstop $BUILDDIR/SOURCES/stopcmdfreak
cd $BUILDDIR/SOURCES
unzip $BUILDDIR/SOURCES/bootstrap-4.4.1-dist.zip
cp $BUILDDIR/SOURCES/bootstrap-4.4.1-dist/css/* $BUILDDIR/SOURCES
cp $BUILDDIR/SOURCES/bootstrap-4.4.1-dist/js/* $BUILDDIR/SOURCES


# Make SPEC file
cd $BUILDDIR
SPEC=cmdfreak.spec
cat <<EOF > ./$SPEC
Name:    cmdfreak
Version: 1.0
Release: 0.el8
Summary: CmdFreak
Requires: nginx >= 1:1.20

License: No License No Life
Source1: cmdfreak
Source2: sample.conf
Source3: cmdfreak_nginx.conf
Source4: cmdfreak.service
Source5: cmdfreak.html
Source6: jquery-3.2.0.min.js
Source7: bootstrap.css
Source8: bootstrap.css.map
Source9: bootstrap.min.css
Source10: bootstrap.min.css.map
Source11: bootstrap-grid.css
Source12: bootstrap-grid.css.map
Source13: bootstrap-grid.min.css
Source14: bootstrap-grid.min.css.map
Source15: bootstrap-reboot.css
Source16: bootstrap-reboot.css.map
Source17: bootstrap-reboot.min.css
Source18: bootstrap-reboot.min.css.map
Source23: sample.dat
Source24: stopcmdfreak
Source25: stkcommon.js
Source26: stkcommon.css
Source27: stkcommon_um.js
Source28: cmdfreak.js
Source30: bootstrap.bundle.js
Source31: bootstrap.bundle.js.map
Source32: bootstrap.bundle.min.js
Source33: bootstrap.bundle.min.js.map
Source34: bootstrap.js
Source35: bootstrap.js.map
Source36: bootstrap.min.js
Source37: bootstrap.min.js.map
Source40: IcoMoon-Free.css
Source41: IcoMoon-Free.ttf
Source42: cristal_image48c.png
Source43: favicon-16x16.png

%description
CmdFreak

%install
mkdir -p %{buildroot}/%{_bindir}
mkdir -p %{buildroot}/%{_sysconfdir}
mkdir -p %{buildroot}/%{_sysconfdir}/nginx/conf.d
mkdir -p %{buildroot}/%{_sysconfdir}/systemd/system
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/img
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
mkdir -p %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js

install -p -m 755 %{SOURCE1} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE2} %{buildroot}/%{_sysconfdir}
install -p -m 644 %{SOURCE3} %{buildroot}/%{_sysconfdir}/nginx/conf.d
install -p -m 644 %{SOURCE4} %{buildroot}/%{_sysconfdir}/systemd/system
install -p -m 644 %{SOURCE5} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE6} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE7} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE8} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE9} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE10} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE11} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE12} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE13} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE14} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE15} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE16} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE17} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE18} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css
install -p -m 644 %{SOURCE23} %{buildroot}/%{_sysconfdir}
install -p -m 755 %{SOURCE24} %{buildroot}/%{_bindir}
install -p -m 644 %{SOURCE25} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE26} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE27} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE28} %{buildroot}/%{_datarootdir}/nginx/html
install -p -m 644 %{SOURCE30} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE31} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE32} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE33} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE34} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE35} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE36} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE37} %{buildroot}/%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js
install -p -m 644 %{SOURCE40} %{buildroot}/%{_datarootdir}/nginx/html/IcoMoon-Free.css
install -p -m 644 %{SOURCE41} %{buildroot}/%{_datarootdir}/nginx/html/IcoMoon-Free.ttf
install -p -m 644 %{SOURCE42} %{buildroot}/%{_datarootdir}/nginx/html/img/cristal_image48c.png
install -p -m 644 %{SOURCE43} %{buildroot}/%{_datarootdir}/nginx/html/img/favicon-16x16.png


%files
%{_bindir}/cmdfreak
%config(noreplace) %{_sysconfdir}/sample.conf
%{_sysconfdir}/nginx/conf.d/cmdfreak_nginx.conf
%config(noreplace) %{_sysconfdir}/systemd/system/cmdfreak.service
%{_datarootdir}/nginx/html/cmdfreak.html
%{_datarootdir}/nginx/html/cmdfreak.js
%{_datarootdir}/nginx/html/stkcommon.js
%{_datarootdir}/nginx/html/stkcommon.css
%{_datarootdir}/nginx/html/stkcommon_um.js
%{_datarootdir}/nginx/html/jquery-3.2.0.min.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap.min.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap.min.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-grid.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-grid.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-grid.min.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-grid.min.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-reboot.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-reboot.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-reboot.min.css
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/css/bootstrap-reboot.min.css.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.bundle.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.bundle.js.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.bundle.min.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.bundle.min.js.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.js.map
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.min.js
%{_datarootdir}/nginx/html/bootstrap-4.4.1-dist/js/bootstrap.min.js.map
%config(noreplace) %{_sysconfdir}/sample.dat
%{_bindir}/stopcmdfreak
%{_datarootdir}/nginx/html/IcoMoon-Free.css
%{_datarootdir}/nginx/html/IcoMoon-Free.ttf
%{_datarootdir}/nginx/html/img/cristal_image48c.png
%{_datarootdir}/nginx/html/img/favicon-16x16.png


%pre
if [ \$1 = 2 ]; then
    echo "Upgrade installation (pre)"
    systemctl daemon-reload
    systemctl stop nginx.service
    systemctl stop cmdfreak.service
    while [ \`ps -ef | grep "/usr/bin/cmdfreak " | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
fi

%post
if [ \$1 = 1 ]; then
    echo "New installation (post)"
    mkdir -p %{_datadir}/cmdfreak
    echo workdir=%{_datadir}/cmdfreak >> %{_sysconfdir}/sample.conf
    setsebool httpd_can_network_connect on -P
    semanage port -a -t http_port_t -p tcp 8080
    semanage port -a -t http_port_t -p tcp 8085
    if [ -e /usr/bin/firewall-cmd ]; then
        firewall-cmd --add-port=8080/tcp --permanent
        firewall-cmd --reload
    fi
    systemctl daemon-reload
    systemctl stop nginx.service
    systemctl start nginx.service
    systemctl enable nginx.service
    systemctl start cmdfreak.service
    systemctl enable cmdfreak.service
elif [ \$1 = 2 ]; then
    echo "Upgrade installation (post)"
    systemctl daemon-reload
    systemctl start nginx.service
    systemctl start cmdfreak.service
fi

%preun
if [ \$1 = 0 ]; then
    echo "Uninstallation (preun)"
    systemctl daemon-reload
    systemctl stop nginx.service
    systemctl stop cmdfreak.service
    while [ \`ps -ef | grep "/usr/bin/cmdfreak " | grep -v grep | grep -v srvchk | wc -l\` != 0 ]
    do
        sleep 1
    done
    semanage port -d -t http_port_t -p tcp 8080
    semanage port -d -t http_port_t -p tcp 8085
    if [ -e /usr/bin/firewall-cmd ]; then
        firewall-cmd --remove-port=8080/tcp --permanent
        firewall-cmd --reload
    fi
    systemctl disable cmdfreak.service
    systemctl start nginx.service
fi

EOF


# RpmBuild
cd $BUILDDIR
rpmbuild --define "_topdir ${BUILDDIR}" -bb ./$SPEC

if [ -e ../deployment/cmdfreak-1.*.rpm ]; then
    yes|rm ../deployment/cmdfreak-1.*.rpm
fi
if [ ! -e ../deployment ]; then
    mkdir -p ../deployment
fi
cp RPMS/x86_64/cmdfreak-1.*.rpm ../deployment
