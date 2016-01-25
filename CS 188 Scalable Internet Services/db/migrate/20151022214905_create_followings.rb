class CreateFollowings < ActiveRecord::Migration
  def change
    create_table :followings do |t|
      t.integer :person_id
      t.integer :follower_id

      t.timestamps null: false
    end
  end
end
