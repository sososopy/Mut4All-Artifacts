//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_164
 */ 
class MutatorFrontendAction_164 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(164)
private:
    class MutatorASTConsumer_164 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_164(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> baseClasses;
    };
};

//source file
#include "../include/insert_virtual_base_class_164.h"

// ========================================================================================================
#define MUT164_OUTPUT 1

void MutatorFrontendAction_164::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;
        baseClasses.insert(RD->getNameAsString());
    } else if (auto *DD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(DD->getLocation()))
            return;
        if (!DD->isCompleteDefinition() || DD->isLambda())
            return;

        for (const auto &base : baseClasses) {
            std::string virtualBaseInsertion = " : virtual public " + base;
            Rewrite.InsertTextAfterToken(DD->getBeginLoc(), virtualBaseInsertion);
        }
    }
}

void MutatorFrontendAction_164::MutatorASTConsumer_164::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDefinition()).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}