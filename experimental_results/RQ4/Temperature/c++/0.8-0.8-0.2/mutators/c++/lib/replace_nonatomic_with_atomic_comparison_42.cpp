//source file
#include "../include/replace_nonatomic_with_atomic_comparison_42.h"

// ========================================================================================================
#define MUT42_OUTPUT 1

void MutatorFrontendAction_42::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("compOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BO->getBeginLoc()))
            return;

        const Expr *lhs = BO->getLHS();
        const Expr *rhs = BO->getRHS();
        
        if (const auto *DRE = dyn_cast<DeclRefExpr>(lhs)) {
            if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
                if (VD->getType()->isIntegerType() && !VD->getType()->isAtomicType()) {
                    std::string atomicVarName = "atomic_" + VD->getNameAsString();
                    
                    if (atomicVarMap.find(VD) == atomicVarMap.end()) {
                        std::string atomicDeclaration = "std::atomic<int> " + atomicVarName + " = " + VD->getNameAsString() + ";";
                        Rewrite.InsertTextBefore(BO->getBeginLoc(), atomicDeclaration + "\n");
                        atomicVarMap[VD] = atomicVarName;
                    }
                    
                    std::string comparisonLine = atomicVarName + BO->getOpcodeStr().str() + Rewrite.getRewrittenText(rhs->getSourceRange());
                    Rewrite.ReplaceText(BO->getSourceRange(), comparisonLine);
                }
            }
        }
    }
}

void MutatorFrontendAction_42::MutatorASTConsumer_42::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher compMatcher = binaryOperator(
        anyOf(
            hasOperatorName("=="),
            hasOperatorName("!="),
            hasOperatorName("<"),
            hasOperatorName(">"),
            hasOperatorName("<="),
            hasOperatorName(">=")
        )
    ).bind("compOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(compMatcher, &callback);
    matchFinder.matchAST(Context);
}