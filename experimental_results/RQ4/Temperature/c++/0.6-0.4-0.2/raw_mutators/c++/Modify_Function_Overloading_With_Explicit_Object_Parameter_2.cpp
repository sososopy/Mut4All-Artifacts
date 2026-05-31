//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_overloading_with_explicit_object_parameter_2
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
        std::vector<const clang::CXXMethodDecl *> methods;
    };
};

//source file
#include "../include/modify_function_overloading_with_explicit_object_parameter_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MD->getLocation()))
            return;

        if (MD->isOverloadedOperator() || MD->isOverloaded()) {
            methods.push_back(MD);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        for (auto *MD : methods) {
            if (FD->getNameAsString() == MD->getNameAsString()) {
                auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                           FD->getSourceRange());
                std::string mutatedDeclaration = "bool operator==(this const int&, const B&);";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedDeclaration);
                return;
            }
        }
    }
}
  
void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher methodMatcher = cxxMethodDecl().bind("Methods");
    DeclarationMatcher funcMatcher = functionDecl().bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}