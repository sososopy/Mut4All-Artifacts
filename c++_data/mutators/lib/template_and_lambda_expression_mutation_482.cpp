//source file
#include "../include/Template_And_Lambda_Expression_Mutation_482.h"

// ========================================================================================================
#define MUT482_OUTPUT 1

void MutatorFrontendAction_482::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
            if (!LE)
                return;

            auto lambdaSourceRange = LE->getSourceRange();
            auto lambdaSourceText = stringutils::rangetoStr(*(Result.SourceManager), lambdaSourceRange);

            size_t paramsPos = lambdaSourceText.find(">");
            if (paramsPos != std::string::npos) {
                lambdaSourceText.insert(paramsPos, ", typename V");
            }

            size_t noexceptPos = lambdaSourceText.find("noexcept(");
            if (noexceptPos != std::string::npos) {
                size_t endPos = lambdaSourceText.find("})", noexceptPos);
                if (endPos != std::string::npos) {
                    lambdaSourceText.insert(endPos, " typename V::type;");
                }
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaSourceRange), lambdaSourceText);
        }
    }
}
  
void MutatorFrontendAction_482::MutatorASTConsumer_482::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher funcMatcher = functionDecl(
        hasDescendant(lambdaExpr().bind("lambdaExpr"))
    ).bind("funcDecl");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}