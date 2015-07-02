// Update all of our sales to include tax
db.sales.update({}, {$mul: {cost_double : 1.05}})
db.sales.update({}, {$mul: {cost_decimal : NumberDecimal("1.05")}})
db.sales.find()
(db.sales.findOne()).cost_double.toFixed(2)
(db.sales.findOne()).cost_decimal

// We owe this amount of money
untaxed_revenue_double = db.revenue.findOne().total_revenue_double
untaxed_revenue_decimal = db.revenue.findOne().total_revenue_decimal

db.revenue.update({}, {$mul: {total_revenue_double : 1.05}})
db.revenue.update({}, {$mul: {total_revenue_decimal : NumberDecimal("1.05")}})

taxed_revenue_double = db.revenue.findOne().total_revenue_double
taxed_revenue_decimal = db.revenue.findOne().total_revenue_decimal

