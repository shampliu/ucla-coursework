class Following < ActiveRecord::Base
  belongs_to :person, :class_name => 'User'
  belongs_to :follower, :class_name => 'User'
end
