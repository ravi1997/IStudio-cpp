#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <string>
#include <vector>
#include <optional>

namespace istudio {

struct SourceLocation {
    std::size_t line;
    std::size_t column;
};

struct SourceRange {
    SourceLocation begin;
    SourceLocation end;
};

enum class DiagnosticSeverity {
    Info,
    Warning,
    Error,
    Fatal
};

struct Diagnostic {
    DiagnosticSeverity severity;
    std::string message;
    std::optional<SourceRange> range;
};

class DiagnosticEngine {
public:
    void report(DiagnosticSeverity severity, std::string message);
    void reportError(std::string message);
    void reportWarning(std::string message);
    void reportInfo(std::string message);
    
    std::vector<Diagnostic> getDiagnostics() const { return diagnostics_; }

private:
    std::vector<Diagnostic> diagnostics_;
};

} // namespace istudio

#endif // DIAGNOSTICS_H