//source file
#include "../include/modify_atomic_shift_operands_676.h"

// ========================================================================================================
#define MUT676_OUTPUT 1

void MutatorFrontendAction_676::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("shiftOp")) {
        if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(BO->getExprLoc()))
            return;

        const Expr *LHS = BO->getLHS()->IgnoreParenImpCasts();
        if (const auto *DRE = dyn_cast<DeclRefExpr>(LHS)) {
            if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
                if (VD->getType()->isAtomicType()) {
                    std::string regularVarName;
                    if (atomicToRegularVarMap.find(VD) != atomicToRegularVarMap.end()) {
                        regularVarName = atomicToRegularVarMap[VD];
                    } else {
                        regularVarName = "regularVar_" + VD->getNameAsString();
                        std::string typeStr = VD->getType().getUnqualifiedType().getAsString();
                        std::string declStr = typeStr + " " + regularVarName + " = 0;\n";
                        Rewrite.InsertTextBefore(BO->getExprLoc(), declStr);
                        atomicToRegularVarMap[VD] = regularVarName;
                    }
                    std::string newExpr = regularVarName + " " + BO->getOpcodeStr().str() + " " + Rewrite.getRewrittenText(BO->getRHS()->getSourceRange());
                    Rewrite.ReplaceText(BO->getSourceRange(), newExpr);
                }
            }
        }
    }
}

void MutatorFrontendAction_676::MutatorASTConsumer_676::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher shiftMatcher = binaryOperator(hasOperatorName("<<"),
                                                   hasLHS(expr(hasType(atomicType())))).bind("shiftOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(shiftMatcher, &callback);
    matchFinder.matchAST(Context);
}