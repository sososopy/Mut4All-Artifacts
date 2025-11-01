//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Incorrect_This_Parameter_In_Defaulted_Operator_197
 */ 
class MutatorFrontendAction_197 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(197)

private:
    class MutatorASTConsumer_197 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_197(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incorrect_This_Parameter_In_Defaulted_Operator_197.h"

// ========================================================================================================
#define MUT197_OUTPUT 1

void MutatorFrontendAction_197::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("OperatorMethod")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isOverloadedOperator() && MT->isDefaulted()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
            size_t pos = declaration.find("operator");
            if (pos != std::string::npos) {
                std::string incorrectThisParam = "this const int&, ";
                declaration.insert(pos + 8, incorrectThisParam);
                declaration += " = default;";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
            }
        }
    }
}

void MutatorFrontendAction_197::MutatorASTConsumer_197::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperator(), isDefaulted()).bind("OperatorMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}