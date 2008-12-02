
#This reads in the command line arguments and sets options.
proc commandLine {args} {
	global options
	set options(n) 10
	set options(c) "vim"

	foreach {arg value} $args {
		if {[string equal $arg "-c"]} {
			set options(c) $value
		} elseif {[string equal $arg "-n"]} {
			set options(n) $value
		} else {
			puts "Don't Recognize Argument $arg"
		}
	}
}

commandLine $argv

set COMMANDS [list]

for {set i 0} {$i<$options(n)} {incr i} {
	if {[eof stdin]} {
		break
	}
	set line [gets stdin]
	lappend COMMANDS $line
	puts "$i- $line"
}

puts "Choice: "
set r [gets stdin]

puts "Choice was $r"
