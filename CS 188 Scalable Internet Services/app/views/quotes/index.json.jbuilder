json.array!(@quotes) do |quote|
  json.extract! quote, :id, :user_id, :book_id, :body
  json.url quote_url(quote, format: :json)
end
