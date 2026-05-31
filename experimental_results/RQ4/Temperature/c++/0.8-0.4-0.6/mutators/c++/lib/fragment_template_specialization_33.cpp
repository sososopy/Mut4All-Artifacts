//source file
#include "../include/fragment_template_specialization_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getLocation()))
            return;
        
        if (MT->hasBody()) {
            auto body = MT->getBody();
            for (auto &stmt : body->children()) {
                if (auto *exprStmt = llvm::dyn_cast<clang::ExprWithCleanups>(stmt)) {
                    if (auto *assignExpr = llvm::dyn_cast<clang::BinaryOperator>(exprStmt->getSubExpr())) {
                        if (assignExpr->isAssignmentOp()) {
                            Rewrite.RemoveText(assignExpr->getSourceRange());
                            break;
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        ofClass(classTemplateSpecializationDecl())).bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}