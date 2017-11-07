require 'watir-webdriver'

tests = File.open(ARGV[0], 'r').readlines.map{|s| (s[0] == '#') ? nil : s.chomp}.compact.map{|s| s.split(/,/)}

tests.each do |group,name,descr,expected,add|
    b = Watir::Browser.new
    b.goto 'http://rcd.hflabs.ru/rcd/admin/dictionaries/'
    b.text_field(:name => 'j_username').set 'admin'
    b.text_field(:name => 'j_password').set '1'
    b.button(:type => 'submit').click

    b.button(:title => "Добавить справочник").wait_until_present
    b.button(:title => "Добавить справочник").click
    b.text_field(:id => "dictionaries-add-name").wait_until_present
    b.text_field(:id => "dictionaries-add-name").set name
    b.text_field(:id => "dictionaries-add-description").set descr
    b.button(:class => "btn dropdown-toggle btn-default").click
#    b.input(:type => "text", :class => "input-block-level form-control").to_subtype.set group
    flag = false
    b.spans(:class => "text").each do |sp|
        small_text = sp.small(:class => "muted").text
        if group + small_text == sp.text and sp.visible?
            sp.click
            flag = true
        end
    end
    if not flag
        puts "FAILED (group doesn't exist)"
        b.close
        next
    end
    begin
        Watir::Wait.until{ b.button(:id => "dictionaries-add-button-submit").enabled? }
        b.button(:id => "dictionaries-add-button-submit").click
    rescue Watir::Wait::TimeoutError 
        puts "FAILED"
        b.close
        next
    end

    begin
        b.button(:title => "Добавить перекодировку").wait_until_present
    rescue Watir::Wait::TimeoutError 
        puts "FAILED"
        b.close
        next
    end

    puts "OK"
    b.close
end
