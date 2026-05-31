//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <vector>

/**
 * Replace_Operator_Overload_With_Template_99
 */ 
class MutatorFrontendAction_99 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(99)

private:
    class MutatorASTConsumer_99 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_99(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXMethodDecl *> operatorMethods;
    };
};

//source file
#include "../include/replace_operator_overload_with_template_99.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT99_OUTPUT 1

void MutatorFrontendAction_99::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *OM = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("OperatorMethods")) {
        if (!OM || !Result.Context->getSourceManager().isWrittenInMainFile(
                       OM->getLocation()))
            return;
        if (OM->isOverloadedOperator()) {
            operatorMethods.push_back(OM);
            auto methodName = OM->getNameAsString();
            auto returnType = OM->getReturnType().getAsString();
            std::string templateReplacement = "template <typename T>\nT operator" + methodName + "(const T& lhs, const T& rhs) {\n";
            templateReplacement += "    /*mut99*/ return lhs; // Placeholder logic\n}\n";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(OM->getSourceRange()), templateReplacement);
        }
    }
}
  
void MutatorFrontendAction_99::MutatorASTConsumer_99::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isOverloadedOperator()).bind("OperatorMethods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}