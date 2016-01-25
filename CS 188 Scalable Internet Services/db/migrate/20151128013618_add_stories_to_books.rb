class AddStoriesToBooks < ActiveRecord::Migration
  def change
    add_column :books, :story, :string
  end
end
