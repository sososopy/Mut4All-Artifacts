//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Return_Type_To_Generic_Lambda_123
 */ 
class MutatorFrontendAction_Add_Return_Type_To_Generic_Lambda_123 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Add_Return_Type_To_Generic_Lambda_123)

private:
    class MutatorASTConsumer_Add_Return_Type_To_Generic_Lambda_123 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Add_Return_Type_To_Generic_Lambda_123(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Return_Type_To_Generic_Lambda_123.h"

// ========================================================================================================
#define MUTAdd_Return_Type_To_Generic_Lambda_123_OUTPUT 1

void MutatorFrontendAction_Add_Return_Type_To_Generic_Lambda_123::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        auto *callOp = lambda->getCallOperator();
        if (!callOp)
            return;

        if (callOp->getReturnTypeSourceRange().isValid())
            return;

        bool allAuto = true;
        for (const auto *param : callOp->parameters()) {
            if (!param->getType()->getAs<clang::AutoType>()) {
                allAuto = false;
                break;
            }
        }
        if (!allAuto)
            return;

        auto lambdaText = stringutils::rangetoStr(*Result.SourceManager, lambda->getSourceRange());
        size_t pos = lambdaText.find(')');
        if (pos == string::npos)
            return;

        std::string mutatedText = lambdaText.substr(0, pos + 1) + " -> void" + lambdaText.substr(pos + 1);
        Rewrite.ReplaceText(lambda->getSourceRange(), mutatedText);
    }
}
  
void MutatorFrontendAction_Add_Return_Type_To_Generic_Lambda_123::MutatorASTConsumer_Add_Return_Type_To_Generic_Lambda_123::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(clang::ast_matchers::hasCaptureDefault(clang::CaptureDefaultKind::ByRef)).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}