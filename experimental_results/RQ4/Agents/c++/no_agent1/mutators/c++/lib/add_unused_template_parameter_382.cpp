//source file
#include "../include/add_unused_template_parameter_382.h"

// ========================================================================================================
#define MUT382_OUTPUT 1

void MutatorFrontendAction_382::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
            return;
        
        if (FD->isTemplated()) {
            return;
        }
        
        auto funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        std::string templateParam = "template<typename UnusedType>\n";
        
        funcText.insert(0, templateParam);
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
    }
}

void MutatorFrontendAction_382::MutatorASTConsumer_382::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}