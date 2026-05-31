//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_defaulted_operator_with_incorrect_this_parameter_2
 */ 
class MutatorFrontendAction_2 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(2)

private:
    class MutatorASTConsumer_2 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_2(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_defaulted_operator_with_incorrect_this_parameter_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("OperatorMethod")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
            return;

        if (MT->isUserProvided() && MT->isOverloadedOperator()) {
            std::string methodName = MT->getNameAsString();
            std::string structName = MT->getParent()->getNameAsString();
            
            std::string incorrectOperator = "bool " + structName + "::" + methodName + "(this const int&, const " + structName + "&) = default;";
            incorrectOperator = "/*mut2*/" + incorrectOperator;

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), incorrectOperator);
        }
    }
}

void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperator()).bind("OperatorMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}