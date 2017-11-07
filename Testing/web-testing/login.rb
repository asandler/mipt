require 'watir-webdriver'

tests = File.open(ARGV[0], 'r').readlines.map{|s| (s[0] == '#') ? nil : s.chomp}.compact.map{|s| s.split(/,/)}

tests.each do |login, password|
    b = Watir::Browser.new
    b.goto 'http://rcd.hflabs.ru/rcd/admin/dictionaries/'
    b.text_field(:name => 'j_username').set login
    b.text_field(:name => 'j_password').set password
    b.button(:type => 'submit').click
    if b.text.index("Вход в систему")
        puts "Didn't log in"
    else
        puts "Logged in"
    end

    b.close
end
