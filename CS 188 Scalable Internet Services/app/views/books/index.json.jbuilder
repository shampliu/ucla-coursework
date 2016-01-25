json.array!(@books) do |book|
  json.extract! book, :id
  json.url book_url(book, format: :json)
end
