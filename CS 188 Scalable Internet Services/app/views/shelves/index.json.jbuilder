json.array!(@shelves) do |shelf|
  json.extract! shelf, :id, :shelf_id, :shelf_name, :shelf_owner
  json.url shelf_url(shelf, format: :json)
end
