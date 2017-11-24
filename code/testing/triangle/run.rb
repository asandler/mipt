File.open("tests", "r").readlines.each do |s|
    next if s[0] == "#"
    File.open("answer", "a"){|f| f.puts s }
    system("./triangle #{s.chomp} >> answer")
    system("echo ---------------------- >> answer")
    system("echo ---------------------- >> answer")
end
