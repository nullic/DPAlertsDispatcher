Pod::Spec.new do |s|

  s.name         = "DPAlertsDispatcher"
  s.version      = "1.0"
  s.summary      = "Simplifies display notifications to the user"
  s.homepage     = "https://github.com/nullic/DPAlertsDispatcher"
  s.license      = "MIT"
  s.author       = { "Dmitriy Petrusevich" => "nullic@gmail.com" }
  s.platform     = :ios, "5.0"

  s.source       = { :git => "https://github.com/nullic/DPAlertsDispatcher.git", :tag => "1.1" }
  s.source_files = "DPAlertsDispatcher", "DPAlertsDispatcher/DPAlertsDispatcher/*.{h,m}"
  s.requires_arc = true

end
