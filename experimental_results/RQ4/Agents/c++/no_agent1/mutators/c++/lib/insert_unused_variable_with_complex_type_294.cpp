//source file
#include "../include/insert_unused_variable_with_complex_type_294.h"

// ========================================================================================================
#define MUT294_OUTPUT 1

void MutatorFrontendAction_294::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            std::string complexType = "std::tuple<int, std::map<std::string, std::vector<double>>>";
            std::string unusedVar = complexType + " unusedVar_294;";
            std::string bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            bodyText.insert(bodyText.find("{") + 1, "\n/*mut294*/" + unusedVar + "\n");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}
  
void MutatorFrontendAction_294::MutatorASTConsumer_294::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}