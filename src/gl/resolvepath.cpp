#include "resolvepath.h"

#include <QDir>

Q_LOGGING_CATEGORY( lcTexture, "nifskope.texture", QtWarningMsg )

#ifndef Q_OS_WIN
#include <QHash>
#include <QStringList>

namespace
{
//! Directory path -> entry names. Avoids re-listing large texture trees on every lookup.
QHash<QString, QStringList> dirCache;

const QStringList & listDir( const QString & path )
{
	auto it = dirCache.find( path );
	if ( it == dirCache.end() ) {
		it = dirCache.insert( path,
			QDir( path ).entryList( QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System ) );
	}
	return it.value();
}
}
#endif

QString resolvePathCI( const QString & base, const QString & rel, QString * detail )
{
	auto setDetail = [detail]( const QString & d ) {
		if ( detail )
			*detail = d;
	};

	QDir dir( base );
	if ( dir.exists( rel ) ) {
		setDetail( "exact match" );
		return dir.filePath( rel );
	}

	if ( !dir.exists() ) {
		setDetail( "folder does not exist" );
		return QString();
	}

#ifdef Q_OS_WIN
	setDetail( "not found" );
	return QString();
#else
	QString current = dir.absolutePath();

	for ( const QString & part : rel.split( '/' ) ) {
		if ( part.isEmpty() || part == "." )
			continue;

		if ( part == ".." ) {
			QDir up( current );
			if ( !up.cdUp() ) {
				setDetail( QString( "cannot go up from %1" ).arg( current ) );
				return QString();
			}
			current = up.absolutePath();
			continue;
		}

		// Plain string comparison rather than QDir name filters, which would treat
		// characters like '[' and '*' in texture names as wildcards.
		QString match;
		for ( const QString & entry : listDir( current ) ) {
			if ( entry.compare( part, Qt::CaseInsensitive ) == 0 ) {
				match = entry;
				break;
			}
		}

		if ( match.isEmpty() ) {
			setDetail( QString( "no entry matching \"%1\" in %2" ).arg( part, current ) );
			return QString();
		}

		current = QDir( current ).filePath( match );
	}

	setDetail( "case-insensitive match" );
	return current;
#endif
}

void resolvePathCIClearCache()
{
#ifndef Q_OS_WIN
	dirCache.clear();
#endif
}
