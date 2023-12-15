%{!?directory:%define directory /usr}

%define buildroot %{_tmppath}/%{name}

Name:          tclopenal
Summary:       Tcl bindings for OpenAL (open audio library)
Version:       0.7
Release:       0
License:       LGPL-2.1-or-later
Group:         Development/Libraries/Tcl
Source:        %{name}-%{version}.tar.gz
URL:           https://github.com/ray2501/tclopenal
BuildRequires: autoconf
BuildRequires: make
BuildRequires: tcl-devel >= 8.4
BuildRequires: openal-soft-devel
Requires:      tcl >= 8.4
Requires:      libopenal1
BuildRoot:     %{buildroot}

%description
Tcl bindings for OpenAL (open audio library).

%prep
%setup -q -n %{name}-%{version}

%build
./configure \
	--prefix=%{directory} \
	--exec-prefix=%{directory} \
	--libdir=%{directory}/%{_lib}
make 

%install
make DESTDIR=%{buildroot} pkglibdir=%{tcl_archdir}/%{name}%{version} install

%clean
rm -rf %buildroot

%files
%defattr(-,root,root)
%{tcl_archdir}
