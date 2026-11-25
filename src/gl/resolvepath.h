#ifndef RESOLVEPATH_H
#define RESOLVEPATH_H

#include <QLoggingCategory>
#include <QString>

//! @file resolvepath.h Case-insensitive resource path resolution

//! Texture/material lookup logging, off by default. Enable with --debug-textures
//! or QT_LOGGING_RULES="nifskope.texture.info=true".
Q_DECLARE_LOGGING_CATEGORY( lcTexture )

/*! Resolve @p rel ('/'-separated) under @p base.
 *
 * Returns the on-disk path, or an empty string if not found.
 * On Windows this is a plain QDir::exists() check. Elsewhere, if the exact
 * path does not exist, each path component is matched case-insensitively,
 * since game assets and the paths stored in nifs are case-insensitive.
 *
 * If @p detail is given, it is set to a short description of how the path
 * was matched or why it was not found, for logging.
 */
QString resolvePathCI( const QString & base, const QString & rel, QString * detail = nullptr );

//! Clear the directory listing cache used by resolvePathCI()
void resolvePathCIClearCache();

#endif // RESOLVEPATH_H
