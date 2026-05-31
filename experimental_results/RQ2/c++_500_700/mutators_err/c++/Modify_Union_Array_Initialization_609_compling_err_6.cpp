//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Decl.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_union_array_initialization_609
 */ 
class MutatorFrontendAction_609 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(609)

private:
    class MutatorASTConsumer_609 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_609(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_union_array_initialization_609.h"

// ========================================================================================================
#define MUT609_OUTPUT 1

void MutatorFrontendAction_609::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::VarDecl>("unionVar")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UD->getLocation()))
            return;
        
        if (const clang::InitListExpr *initList = llvm::dyn_cast<clang::InitListExpr>(UD->getInit())) {
            if (initList->getNumInits() > 0) {
                auto firstInit = initList->getInit(0);
                if (firstInit->getType()->isArrayType()) {
                    std::string newInit = "{3.14}";
                    Rewrite.ReplaceText(initList->getSourceRange(), newInit);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_609::MutatorASTConsumer_609::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(isUnionType()), hasInitializer(initListExpr())).bind("unionVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}