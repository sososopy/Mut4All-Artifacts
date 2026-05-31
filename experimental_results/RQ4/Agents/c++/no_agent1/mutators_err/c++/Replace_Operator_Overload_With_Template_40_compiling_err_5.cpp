//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Replace_Operator_Overload_With_Template_40
 */ 

class MutatorFrontendAction_40 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(40)
private:
    class MutatorASTConsumer_40 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_40(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_operator_overload_with_template_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *OD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("OperatorOverload")) {
        if (!OD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       OD->getLocation()))
            return;

        if (OD->isOverloadedOperator()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       OD->getSourceRange());

            std::string templateCode = "template<typename T>\nT operator" 
                                       + OD->getNameAsString() + "(const T& lhs, const T& rhs) {\n"
                                       + "    /*mut40*/ return lhs; \n"
                                       + "}\n";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(OD->getSourceRange()), templateCode);
        }
    }
}

void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperator()).bind("OperatorOverload");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}