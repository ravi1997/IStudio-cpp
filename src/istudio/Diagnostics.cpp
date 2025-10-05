#include "istudio/Diagnostics.h"
#include <iostream>

namespace istudio {
    void DiagnosticEngine::report(DiagnosticSeverity severity, std::string message) {
        Diagnostic diag;
        diag.severity = severity;
        diag.message = std::move(message);
        diagnostics_.push_back(std::move(diag));
    }
    
    void DiagnosticEngine::reportError(std::string message) {
        report(DiagnosticSeverity::Error, std::move(message));
    }
    
    void DiagnosticEngine::reportWarning(std::string message) {
        report(DiagnosticSeverity::Warning, std::move(message));
    }
    
    void DiagnosticEngine::reportInfo(std::string message) {
        report(DiagnosticSeverity::Info, std::move(message));
    }
}