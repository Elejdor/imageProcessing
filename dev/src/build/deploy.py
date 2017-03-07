from sys import argv
import os

#config params
internals = []
externals = [ 'googletest', 'assimp' ]

intDir = '../internal/' #relative to solution dir
extDir = '../external/' #relative to solution dir
outDir = '../bin/' 		#relative to solution dir

#end config params

def ParsePaths( pathsString ):
	result = []
	currentPath = ""
	
	for c in pathsString:
		if c == '}':
			break
		elif c == ';':
			result.append( currentPath.strip() )
			currentPath = ""
		else:
			currentPath += str( c )
	return result
	
def CopyFile( src, dst ):
	print 'Copying [ %r ] to [ %r ]' % ( src, dst )
	open( dst, 'wb' ).write( open( src, 'rb' ).read() )

def IsFileNewer( a, b ):
	f1 = os.path.getmtime( a )
	f2 = os.path.getmtime( b )
	return f1 > f2

def CopyFiles( outDir, root, paths ):
	srcFiles = ParsePaths( paths )
	for srcFile in srcFiles:
		srcPath = root + srcFile
		testSrc = 'internal/test/bin/test.lib'
		if os.path.exists( srcPath ):
			dstPath = outDir + os.path.basename( srcFile )
			if not os.path.exists( dstPath ):			
				CopyFile( srcPath, dstPath )
			elif IsFileNewer( srcPath, dstPath ):
				CopyFile( srcPath, dstPath )
		else:
			print 'Source file [ %r ] does not exist' % srcPath
			
def FindConfig( config, supportedList ):
	result = 0
	for cfg in supportedList.strip( ',\n' ).split( ',' ):
		if ( cfg.strip() == config ):
			return result
		result += 1
	return -1

def CopyLibs( outDir, config, rootDir, libs ):
	for lib in libs:
		deployCfg = rootDir + lib + '/deploy.txt'
		if os.path.isfile( deployCfg ):
			file = open( deployCfg, 'r' )
			line = file.readline() #first line is list of supported configurations
			configIndex = FindConfig( config, line )
			if ( configIndex == -1 ):
				print 'Unsupported config'
			else:
				paths = file.readlines()[ configIndex ]
				#print 'Files to deploy: %r' % paths
				CopyFiles( outDir, rootDir + lib + '/', paths )
		else:
			print 'Missing deploy file [ %s ]' % os.path.abspath( deployCfg )

script, config, solutionDir = argv
outDir = solutionDir + outDir + config + '/'
intDir = solutionDir + intDir
extDir = solutionDir + extDir

lastChar = outDir[ - 1 ]
if ( lastChar == '/' ) | ( lastChar == '\\' ):
	CopyLibs( outDir, config, intDir, internals )
	CopyLibs( outDir, config, extDir, externals )
else:
	print "Directory path should end with a trailing slash [ %r ]" % outDir

