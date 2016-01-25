json.array!(@comments) do |comment|
  json.extract! comment, :id, :book_id, :author_id, :body
  json.url comment_url(comment, format: :json)
end
