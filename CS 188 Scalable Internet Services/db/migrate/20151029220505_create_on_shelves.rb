class CreateOnShelves < ActiveRecord::Migration
  def change
    create_table :on_shelves do |t|
      t.integer :book_id
      t.integer :shelf_id

      t.timestamps null: false
    end
  end
end
