json.array!(@on_shelves) do |on_shelf|
  json.extract! on_shelf, :id, :book_id, :shelf_id
  json.url on_shelf_url(on_shelf, format: :json)
end
