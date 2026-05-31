//source file
#include "../include/insert_complex_template_instantiation_197.h"

// ========================================================================================================
#define MUT197_OUTPUT 1

void MutatorFrontendAction_197::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClasses")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;
        templateClasses.push_back(CTD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (templateClasses.empty())
            return;
        
        size_t index = getrandom::getRandomIndex(templateClasses.size() - 1);
        const clang::ClassTemplateDecl *selectedTemplate = templateClasses[index];
        
        std::string templateName = selectedTemplate->getNameAsString();
        std::string instantiation = "\n/*mut197*/" + templateName + "<int, double> tempInstance;\n";
        
        auto funcBody = FD->getBody();
        if (funcBody) {
            auto bodyRange = funcBody->getSourceRange();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
            size_t insertPos = bodyText.find("{") + 1;
            bodyText.insert(insertPos, instantiation);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
        }
    }
}

void MutatorFrontendAction_197::MutatorASTConsumer_197::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto templateMatcher = classTemplateDecl().bind("TemplateClasses");
    auto functionMatcher = functionDecl(hasBody(compoundStmt())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}