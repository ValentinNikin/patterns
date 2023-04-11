// Паттерн Строитель (Builder)

#include <string>
#include <memory>

// Класс продукта
class Report {
public:
    virtual ~Report() = default;
    virtual void saveReportToFile(const std::string& filePath) = 0;
};

class JsonReport : public Report {
public:
    void saveReportToFile(const std::string& filePath) override {}
};

class XmlReport : public Report {
public:
    void saveReportToFile(const std::string& filePath) override {}
};

// Вспомогательный класс
class JobResult {
public:
    std::string filename;
    std::string sourcePath;
    std::string status;
    uint32_t countTests;
    uint32_t countPassedTests;
    uint32_t countFailedTests;
    uint32_t countSkippedTests;
};

// Абстрактный класс строителя
class ReportBuilder {
public:
    virtual ~ReportBuilder() = default;

    virtual void addTitle(const std::string& title) {}
    virtual void addDateInterval(const std::string& from, const std::string& to) {}
    virtual void addCreator(const std::string& creator) {}
    virtual void addJobResult(const JobResult& result) {}

    virtual std::unique_ptr<Report> build() = 0;
};

// Строитель JSON отчетов
class JsonReportBuilder : public ReportBuilder {
public:
    JsonReportBuilder() : _report(new JsonReport()) {}

    void addTitle(const std::string& title) override {
        // TODO: some logic
    }

    void addDateInterval(const std::string& from, const std::string& to) override {
        // TODO: some logic
    }

    void addCreator(const std::string& creator) override {
        // TODO: some logic
    }

    void addJobResult(const JobResult& result) override {
        // TODO: some logic
    }

    std::unique_ptr<Report> build() override {
        auto newReport = std::make_unique<JsonReport>();
        _report.swap(newReport);
        return newReport;
    }

private:
    std::unique_ptr<JsonReport> _report;
};

// Строитель XML отчетов
class XmlReportBuilder : public ReportBuilder {
public:
    XmlReportBuilder() : _report(new XmlReport()) {}

    void addTitle(const std::string& title) override {
        // TODO: some logic
    }

    void addDateInterval(const std::string& from, const std::string& to) override {
        // TODO: some logic
    }

    void addCreator(const std::string& creator) override {
        // TODO: some logic
    }

    void addJobResult(const JobResult& result) override {
        // TODO: some logic
    }

    std::unique_ptr<Report> build() override {
        auto newReport = std::make_unique<XmlReport>();
        _report.swap(newReport);
        return newReport;
    }

private:
    std::unique_ptr<XmlReport> _report;
};

// Класс директора
class ReportGenerator {
public:
    std::unique_ptr<Report> generate(const std::shared_ptr<ReportBuilder>& builder) {
        builder->addTitle("Report");
        builder->addDateInterval("10.20.2022", "01.25.2023");
        builder->addCreator("Report generator");
        builder->addJobResult(JobResult{});
        builder->addJobResult(JobResult{});
        builder->addJobResult(JobResult{});

        return builder->build();
    }
};

int main(int, char *[]) {
    auto jsonReportBuilder = std::make_shared<JsonReportBuilder>();
    auto reportGenerator = std::make_unique<ReportGenerator>();

    auto report = reportGenerator->generate(jsonReportBuilder);
    report->saveReportToFile("Some file");

    return 0;
}