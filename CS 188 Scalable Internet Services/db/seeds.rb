# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)

PublicActivity.enabled = false

User.delete_all
Book.delete_all
Following.delete_all
Comment.delete_all
Shelf.delete_all
OnShelf.delete_all

RECORD_COUNT_FACTOR = 3000

(1..RECORD_COUNT_FACTOR).each do |seed_id|

  puts "Working on user #{seed_id}"


  user = User.new
  user.first = "first#{seed_id}"
  user.last = "last#{seed_id}"
  user.email = "seeduser#{seed_id}@seeduser.com"
  user.password = "password"
  user.save!

  current_user = user

  shelf= Shelf.new
  shelf.shelf_name = "seedshelf#{seed_id}"
  shelf.shelf_owner = user.id
  shelf.save!

  book = Book.new
  book.title = "seedbook#{seed_id}"
  book.save!
end

puts "done adding users. following & commenting"

User.all.each do |user|

  puts "doing commenting and following for user #{user.id}"

  (RECORD_COUNT_FACTOR / 100.0).round.times do
    user.following.build(:person_id => User.random.id).save
  end

  comment = Comment.new
  random_book_id = Book.random.id
  comment.book_id = random_book_id
  comment.author_id = user.id
  comment.body = "seedcommentby#{user.id}onbook#{random_book_id}"
  comment.save!
end

puts "shelving books"

Shelf.all.each do |shelf|

  puts "placing books on shelf #{shelf.id}"

  (RECORD_COUNT_FACTOR / 100.0 / 5.0).round.times do
    on_shelf = OnShelf.new
    on_shelf.shelf_id = shelf.id
    on_shelf.book_id = Book.random.id
    on_shelf.save!
  end
end

puts "Done!"

