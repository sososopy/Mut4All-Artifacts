//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_191
 */ 
class MutatorFrontendAction_191 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(191)

private:
    class MutatorASTConsumer_191 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_191(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_virtual_base_class_191.h"

// ========================================================================================================
#define MUT191_OUTPUT 1

void MutatorFrontendAction_191::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;
        if (!Base->isCompleteDefinition() || Base->isLambda())
            return;
        baseClasses.insert(Base->getNameAsString());
    } else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        if (!Derived->isCompleteDefinition() || Derived->isLambda())
            return;

        if (!baseClasses.empty()) {
            std::string virtualBaseInsertion = " : virtual " + *baseClasses.begin();
            std::string derivedDecl = stringutils::rangetoStr(*(Result.SourceManager), Derived->getSourceRange());
            if (derivedDecl.find(":") == std::string::npos) {
                derivedDecl.insert(derivedDecl.find("{"), virtualBaseInsertion);
            } else {
                derivedDecl.insert(derivedDecl.find("{"), ", virtual " + *baseClasses.begin());
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Derived->getSourceRange()), derivedDecl);
        }
    }
}

void MutatorFrontendAction_191::MutatorASTConsumer_191::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isClass()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isClass(), isDerivedFrom(anything())).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}