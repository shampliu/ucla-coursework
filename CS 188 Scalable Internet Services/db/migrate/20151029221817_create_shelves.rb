class CreateShelves < ActiveRecord::Migration
  def change
    create_table :shelves do |t|
      t.integer :shelf_id
      t.string :shelf_name
      t.integer :shelf_owner

      t.timestamps null: false
    end
  end
end
