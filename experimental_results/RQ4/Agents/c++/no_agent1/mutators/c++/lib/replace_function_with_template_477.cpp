//source file
#include "../include/replace_function_with_template_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        
        if (!FD->hasBody() || FD->isTemplateInstantiation())
            return;

        auto functionName = FD->getNameAsString();
        auto returnType = FD->getReturnType().getAsString();
        auto params = FD->parameters();
        
        std::string templateDecl = "template<typename T>\n";
        std::string newFunctionDecl = returnType + " " + functionName + "(";
        for (size_t i = 0; i < params.size(); ++i) {
            if (i > 0) newFunctionDecl += ", ";
            newFunctionDecl += params[i]->getType().getAsString() + " " + params[i]->getNameAsString();
        }
        newFunctionDecl += ")";

        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
        std::string mutatedFunction = templateDecl + newFunctionDecl + functionBody;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedFunction);
    }
}
  
void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(cxxMethodDecl())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}