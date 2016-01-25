class AddAttachmentAvatarToBooks < ActiveRecord::Migration
  def self.up
    change_table :books do |t|
      t.attachment :avatar
    end
  end

  def self.down
    remove_attachment :books, :avatar
  end
end
