//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_non_constant_struct_access_in_constexpr_21
 */ 
class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
    class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
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
                    if (!FD->isConstexpr()) {
                        // Perform mutation on the source code text by applying string replacement
                        Rewrite.ReplaceText(ME->getSourceRange(), "50"); // Replace with a constant value
                    }
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