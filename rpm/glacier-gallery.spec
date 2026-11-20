Name:       glacier-gallery
Summary:    Photo Gallery for Nemo
Version:    0.2.3
Release:    1
Group:      Applications/System
License:    BSD
URL:        https://github.com/nemomobile-ux/glacier-gallery
Source0:    %{name}-%{version}.tar.bz2

Requires:   glacier-gallery-qmlplugin
Requires:   libglacierapp >= 1.0
Requires:   nemo-qml-plugin-dbus-qt6

BuildRequires:  cmake
BuildRequires:  pkgconfig(Qt6Core)
BuildRequires:  pkgconfig(Qt6Quick)
BuildRequires:  pkgconfig(Qt6Gui)
BuildRequires:  pkgconfig(Qt6Qml)
BuildRequires:  pkgconfig(libresourceqt6)
BuildRequires:  pkgconfig(tracker-sparql-3.0)
BuildRequires:  desktop-file-utils
BuildRequires:  pkgconfig(glacierapp)
BuildRequires:  qt6-qttools-linguist

Provides:   meego-handset-video > 0.2.5
Obsoletes:  meego-handset-video <= 0.2.5


%package qmlplugin
Summary: QML Plugin for gallery view
Requires:   qt6-qtquickcontrols
Requires:   qt6-qtquickcontrols-nemo
Requires:   qt6-qtmultimedia
Requires:   nemo-qml-plugin-thumbnailer-qt6
Requires:   tracker-miners

%description
Photo Gallery application using Qt Quick for Nemo Mobile.

%description qmlplugin
This plugin contains qml api for glacier gallery

%prep
%setup -q -n %{name}-%{version}

%build
mkdir -p build
cd build
cmake \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=%{_prefix} \
	-DCMAKE_INSTALL_LIBDIR=%{_lib} \
	-DCMAKE_VERBOSE_MAKEFILE=ON \
	..
cmake --build .

%install
cd build
rm -rf %{buildroot}
DESTDIR=%{buildroot} cmake --build . --target install

%files
%defattr(-,root,root,-)
%{_bindir}/glacier-gallery
%{_datadir}/applications/glacier-gallery.desktop
%{_datadir}/applications/glacier-gallery-openfile.desktop
%{_datadir}/glacier-gallery
%{_datadir}/dbus-1/services/org.nemomobile.gallery.service

%files qmlplugin
%{_libdir}/qt6/qml/org/nemomobile/gallery/*
