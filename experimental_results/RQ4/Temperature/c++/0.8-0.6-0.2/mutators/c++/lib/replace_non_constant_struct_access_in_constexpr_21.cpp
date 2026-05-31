//source file
#include "../include/Mutator_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("ConstexprCall")) {
        // Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                CE->getBeginLoc()))
            return;

        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            if (auto *ME = dyn_cast<MemberExpr>(CE->getArg(i))) {
                if (auto *FD = dyn_cast<FieldDecl>(ME->getMemberDecl())) {
                    // Perform mutation on the source code text by applying string replacement
                    Rewrite.ReplaceText(ME->getSourceRange(), "50"); // Replace with a constant value
                }
            }
        }
    }
}
  
void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(
        hasDescendant(memberExpr(hasType(recordDecl().bind("StructType"))))
    ).bind("ConstexprCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}