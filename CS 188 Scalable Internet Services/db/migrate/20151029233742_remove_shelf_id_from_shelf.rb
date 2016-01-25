class RemoveShelfIdFromShelf < ActiveRecord::Migration
  def change
    remove_column :shelves, :shelf_id, :integer
  end
end
