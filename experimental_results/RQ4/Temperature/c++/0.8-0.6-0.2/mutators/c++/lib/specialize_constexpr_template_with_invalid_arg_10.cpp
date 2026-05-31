//source file
#include "../include/specialize_constexpr_template_with_invalid_arg_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isConstexpr() && FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate) {
            auto templateDecl = FD->getDescribedFunctionTemplate();
            if (!templateDecl)
                return;

            std::string originalCall = templateDecl->getNameAsString();
            std::string mutatedCall = originalCall + "<1>";

            SourceLocation insertLoc = FD->getEndLoc();
            std::string mutationText = "\n/*mut10*/int x = " + mutatedCall + "();\n";

            Rewrite.InsertTextAfterToken(insertLoc, mutationText);
        }
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(isConstexpr(), hasAncestor(namespaceDecl()), hasAncestor(recordDecl())).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}