import json
file_path = "/Users/alanlee/Downloads/internship_data.json"
with open(file_path, "r") as f:
    data = json.load(f)

companies  = data["companies"]
# print("number of companies: {}".format(len(companies)))
# print("company: {}".format(companies[0]))

# Filter out companies with invalid monthlySalary or set it to 0 if it's not numeric
filtered_companies = {}
for company in companies:
    if company['company'] not in filtered_companies:
        filtered_companies[company['company']] = 0
    if company.get("title") != "Software Engineer":
        continue
    if not isinstance(company.get("monthlySalary", 0), (int, float)):
        company["monthlySalary"] = 0
    if not isinstance(company.get("hourlySalary", 0), (int, float)):
        company["hourlySalary"] = 0
    if company.get("monthlySalary", 0) >= company.get("hourlySalary", 0) * 8 * 30 + 1:
        company["monthlySalary"] = company["hourlySalary"] = 0
    filtered_companies[company['company']] = max(filtered_companies[company['company']], company.get("monthlySalary", 0))
    # filtered_companies.append(company)

# Sort companies by monthlySalary in descending order
sorted_companies = dict(sorted(filtered_companies.items(), key=lambda x: x[1], reverse=True))
# sorted_companies = sorted(filtered_companies,  key=lambda x: x.get("monthlySalary", 0), reverse=True)

import sys
if len(sys.argv) < 2:
    tot_cnt = 300
else:
    tot_cnt = int(sys.argv[1])
    print("tot_cnt: {}".format(tot_cnt))

print(f"Company")
cnt = 0
for company, monthly_salary in sorted_companies.items():
    if cnt == tot_cnt:
        break
    cnt += 1
    print(f"{company}")
# print("Company: Jane Street, Monthly Salary: {}".format(sorted_companies["Jane Street"]))


# print("company: {}".format(sorted_companies[0]))

# Print the sorted companies
# for i, company in enumerate(sorted_companies):
#     if i >= 5000:
#         break
#     print(f"{company['company']},{company['monthlySalary']}/month,{company.get('title', '')},{company.get('tags', '')},{company.get('link', '')}")