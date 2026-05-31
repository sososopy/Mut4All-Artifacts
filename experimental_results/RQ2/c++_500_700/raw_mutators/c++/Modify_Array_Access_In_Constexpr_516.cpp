//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_array_access_in_constexpr_516
 */ 
class MutatorFrontendAction_516 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(516)

private:
    class MutatorASTConsumer_516 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_516(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::map<const clang::VarDecl *, unsigned> arraySizes;
    };
};

//source file
#include "../include/modify_array_access_in_constexpr_516.h"

// ========================================================================================================
#define MUT516_OUTPUT 1

void MutatorFrontendAction_516::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::ConstantExpr>("ConstExprAccess")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getExprLoc()))
            return;

        if (auto *DRE = dyn_cast<DeclRefExpr>(CE->getSubExpr())) {
            if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
                if (arraySizes.find(VD) != arraySizes.end()) {
                    unsigned size = arraySizes[VD];
                    std::string mutatedAccess = DRE->getNameInfo().getAsString() + "[" + std::to_string(size) + "]";
                    Rewrite.ReplaceText(CE->getSourceRange(), mutatedAccess);
                }
            }
        }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstExprArray")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (auto *AT = VD->getType()->getAsArrayTypeUnsafe()) {
            if (auto *CAT = dyn_cast<ConstantArrayType>(AT)) {
                arraySizes[VD] = CAT->getSize().getZExtValue();
            }
        }
    }
}

void MutatorFrontendAction_516::MutatorASTConsumer_516::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto arrayMatcher = varDecl(hasType(constantArrayType()), isConstexpr()).bind("ConstExprArray");
    auto accessMatcher = constantExpr(hasDescendant(declRefExpr(to(varDecl(hasType(constantArrayType()), isConstexpr()))))).bind("ConstExprAccess");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(accessMatcher, &callback);
    matchFinder.matchAST(Context);
}