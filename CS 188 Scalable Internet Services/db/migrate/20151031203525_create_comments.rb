class CreateComments < ActiveRecord::Migration
  def change
    create_table :comments do |t|
      t.integer :book_id
      t.integer :author_id
      t.string :body

      t.timestamps null: false
    end
  end
end
