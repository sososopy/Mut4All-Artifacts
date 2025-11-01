//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Parameter_Type_With_Incompatible_Type_47
 */ 
class MutatorFrontendAction_47 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(47)

private:
    class MutatorASTConsumer_47 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_47(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_function_parameter_type_with_incompatible_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
            return;
        
        auto callee = CE->getDirectCallee();
        if (!callee || callee->getNumParams() == 0)
            return;

        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            auto arg = CE->getArg(i);
            auto paramType = callee->getParamDecl(i)->getType();

            if (paramType->isIntegerType()) {
                Rewrite.ReplaceText(arg->getSourceRange(), "\"string\"");
            } else if (paramType->isFloatingType()) {
                Rewrite.ReplaceText(arg->getSourceRange(), "42");
            } else if (paramType->isPointerType()) {
                Rewrite.ReplaceText(arg->getSourceRange(), "nullptr");
            }
        }
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAnyParameter()))).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}