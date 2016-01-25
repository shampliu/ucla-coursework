class CreateQuotes < ActiveRecord::Migration
  def change
    create_table :quotes do |t|
      t.integer :user_id
      t.integer :book_id
      t.string :body

      t.timestamps null: false
    end
  end
end
