//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Array_Size_In_Ternary_Operand_498
 */ 
class MutatorFrontendAction_498 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(498)

private:
    class MutatorASTConsumer_498 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_498(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Change_Array_Size_In_Ternary_Operand_498.h"

// ========================================================================================================
#define MUT498_OUTPUT 1

void MutatorFrontendAction_498::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *condOp = Result.Nodes.getNodeAs<clang::ConditionalOperator>("condOp")) {
        if (!condOp || !Result.Context->getSourceManager().isWrittenInMainFile(condOp->getBeginLoc()))
            return;

        const Expr *trueExpr = condOp->getTrueExpr();
        const Expr *falseExpr = condOp->getFalseExpr();

        const VarDecl *arr1 = nullptr;
        const VarDecl *arr2 = nullptr;

        if (const auto *trueDeclRef = dyn_cast<DeclRefExpr>(trueExpr)) {
            if (const auto *vd = dyn_cast<VarDecl>(trueDeclRef->getDecl())) {
                if (vd->hasAttr<ConstexprAttr>() && vd->getType()->isConstantArrayType()) {
                    arr1 = vd;
                }
            }
        }

        if (const auto *falseDeclRef = dyn_cast<DeclRefExpr>(falseExpr)) {
            if (const auto *vd = dyn_cast<VarDecl>(falseDeclRef->getDecl())) {
                if (vd->hasAttr<ConstexprAttr>() && vd->getType()->isConstantArrayType()) {
                    arr2 = vd;
                }
            }
        }

        if (arr1 && arr2) {
            SourceLocation loc = arr2->getLocation();
            SourceManager &SM = Result.Context->getSourceManager();
            if (!SM.isWrittenInMainFile(loc))
                return;

            unsigned new_size = 1;

            SourceRange declRange = arr2->getSourceRange();
            std::string originalDecl = stringutils::rangetoStr(*Result.SourceManager, declRange);

            size_t startPos = originalDecl.find('[');
            if (startPos != std::string::npos) {
                size_t endPos = originalDecl.find(']', startPos);
                if (endPos != std::string::npos) {
                    std::string newSizeStr = std::to_string(new_size);
                    std::string newDecl = originalDecl.substr(0, startPos + 1) + newSizeStr + originalDecl.substr(endPos);

                    Rewrite.ReplaceText(declRange, newDecl);
                }
            }
        }
    }
}

void MutatorFrontendAction_498::MutatorASTConsumer_498::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    using namespace clang::ast_matchers;
    DeclarationMatcher matcher = conditionalOperator(
        anyOf(
            hasTrueExpr(declRefExpr(to(varDecl(hasAttr<ConstexprAttr>(), hasType(arrayType()))))),
            hasFalseExpr(declRefExpr(to(varDecl(hasAttr<ConstexprAttr>(), hasType(arrayType())))))
        )
    ).bind("condOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}