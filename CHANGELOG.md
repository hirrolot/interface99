# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [unreleased]

## [0.3.0] - 2021-08-13

### Changed

 - Generate virtual tables with static linkage in `impl(Primary)` & `declImpl` [**BC**].
 - `dyn` => `DYN` to know where is a function and where is a macro.

### Added

 - Counterparts with external linkage: `externImpl(Primary)` & `externDeclImpl`.

## [0.2.0] - 2021-07-01

### Added

 - Generate `typedef struct <iface>VTable <iface>VTable;` and `typedef struct <iface> <iface>;` prior to their definitions to allow accepting `<ifaceVTable>` and `<iface>` as interface function parameters.

### Changed

 - Generate `char dummy;` only for an empty virtual table (i.e., a marker interface without superinterfaces) [**BC**].

## [0.1.0] - 2021-06-23

### Added

 - This excellent project.
