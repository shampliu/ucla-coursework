class Book < ActiveRecord::Base
	has_attached_file :avatar, styles: { medium: "300x300>", thumb: "100x100>" }, default_url: "http://img06.deviantart.net/cdc3/i/2015/107/7/3/gold_vintage_book_cover__exclusive_stock_by_somadjinn-d7jk6bp.jpg"
	validates_attachment_content_type :avatar, content_type: /\Aimage\/.*\Z/
	has_many :comments, dependent: :destroy

	include PublicActivity::Model
	tracked owner: Proc.new{ |controller, model| controller.current_user }

  def self.random
    Book.offset(rand(Book.count)).first
  end
end
