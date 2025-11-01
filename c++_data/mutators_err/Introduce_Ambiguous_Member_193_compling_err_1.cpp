//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Ambiguous_Member_193
 */ 
class MutatorFrontendAction_193 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(193)

private:
    class MutatorASTConsumer_193 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_193(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> processedClasses;
    };
};

//source file
#include "../include/introduce_ambiguous_member_193.h"

// ========================================================================================================
#define MUT193_OUTPUT 1

void MutatorFrontendAction_193::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        
        if (!CD->isCompleteDefinition() || processedClasses.count(CD->getNameAsString()))
            return;

        processedClasses.insert(CD->getNameAsString());

        for (auto method : CD->methods()) {
            if (method->hasBody() && !method->isOverloadedOperator()) {
                std::string newMethodDecl = "double " + method->getNameAsString() + "(double y) { return y; }";
                std::string mutation = "/*mut193*/" + newMethodDecl + "\n";
                Rewrite.InsertTextAfterToken(CD->getRBraceLoc(), mutation);
                break;
            }
        }
    }
}

void MutatorFrontendAction_193::MutatorASTConsumer_193::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}