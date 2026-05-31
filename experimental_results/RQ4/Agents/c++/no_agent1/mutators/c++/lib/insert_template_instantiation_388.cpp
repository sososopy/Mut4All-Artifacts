//source file
#include "../include/insert_template_instantiation_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (FD->isTemplated())
            return;
        functions.push_back(FD);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        
        if (functions.empty())
            return;

        auto templateName = TD->getNameAsString();
        std::string instantiation = "/*mut388*/ " + templateName + "<int> instance;\n";
        auto *function = functions.back();
        functions.pop_back();

        auto functionSourceRange = function->getSourceRange();
        auto functionSourceText = stringutils::rangetoStr(*(Result.SourceManager), functionSourceRange);
        functionSourceText.insert(functionSourceText.find("{") + 1, instantiation);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(functionSourceRange), functionSourceText);
    }
}
  
void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl().bind("Functions");
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}