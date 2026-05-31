//source file
#include "../include/replace_function_with_template_226.h"

// ========================================================================================================
#define MUT226_OUTPUT 1

void MutatorFrontendAction_226::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation() || FD->isDeleted() || FD->isDefaulted())
            return;

        auto functionName = FD->getNameAsString();
        auto returnType = FD->getReturnType().getAsString();
        std::string templateText = "template<typename T>\n" + returnType + " " + functionName + "(T param)";

        auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        size_t pos = functionSource.find(functionName);
        if (pos != std::string::npos) {
            functionSource.replace(pos, functionName.length(), templateText);
            functionSource.insert(functionSource.find("{") + 1, "\n/*mut226*/");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionSource);
        }
    }
}
  
void MutatorFrontendAction_226::MutatorASTConsumer_226::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}