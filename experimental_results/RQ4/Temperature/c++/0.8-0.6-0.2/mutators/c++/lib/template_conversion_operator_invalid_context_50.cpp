//source file
#include "../include/template_conversion_operator_invalid_context_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
        if (!method || !Result.Context->getSourceManager().isWrittenInMainFile(method->getLocation()))
            return;

        if (currentTemplateClass && method->getParent() == currentTemplateClass) {
            if (method->hasBody()) {
                auto bodyRange = method->getBody()->getSourceRange();
                std::string bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
                // Perform mutation on the source code text by introducing an invalid conversion
                std::string mutation = "0, operator int();";
                bodyText.insert(bodyText.find("{") + 1, "/*mut50*/" + mutation + "\n");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
            }
        }
    }
    else if (auto *templateClass = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        currentTemplateClass = templateClass->getTemplatedDecl();
    }
}
  
void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto templateClassMatcher = classTemplateDecl(has(cxxRecordDecl(has(cxxMethodDecl(isDefaulted()))))).bind("TemplateClass");
    auto methodMatcher = cxxMethodDecl(ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("Method");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateClassMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.matchAST(Context);
}