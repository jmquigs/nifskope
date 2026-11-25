#ifndef RESOLVEPATH_H
#define RESOLVEPATH_H

#include <QString>

//! @file resolvepath.h Case-insensitive resource path resolution

/*! Resolve @p rel ('/'-separated) under @p base.
 *
 * Returns the on-disk path, or an empty string if not found.
 * On Windows this is a plain QDir::exists() check. Elsewhere, if the exact
 * path does not exist, each path component is matched case-insensitively,
 * since game assets and the paths stored in nifs are case-insensitive.
 */
QString resolvePathCI( const QString & base, const QString & rel );

//! Clear the directory listing cache used by resolvePathCI()
void resolvePathCIClearCache();

#endif // RESOLVEPATH_H
