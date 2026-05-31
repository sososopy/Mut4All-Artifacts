//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_283
 */ 
class MutatorFrontendAction_283 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(283)

private:
    class MutatorASTConsumer_283 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_283(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl *> baseClasses;
    };
};

//source file
#include "../include/insert_virtual_base_class_283.h"

// ========================================================================================================
#define MUT283_OUTPUT 1

void MutatorFrontendAction_283::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const CXXRecordDecl *baseClass = Result.Nodes.getNodeAs<CXXRecordDecl>("BaseClass")) {
        if (!baseClass || !Result.Context->getSourceManager().isWrittenInMainFile(baseClass->getLocation()))
            return;
        baseClasses.insert(baseClass);
    } else if (const CXXRecordDecl *derivedClass = Result.Nodes.getNodeAs<CXXRecordDecl>("DerivedClass")) {
        if (!derivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(derivedClass->getLocation()))
            return;
        if (!derivedClass->isCompleteDefinition())
            return;

        for (const auto *baseClass : baseClasses) {
            if (derivedClass->isDerivedFrom(baseClass)) {
                std::string baseName = baseClass->getNameAsString();
                std::string derivedText = stringutils::rangetoStr(*(Result.SourceManager), derivedClass->getSourceRange());
                std::string virtualBaseInsert = " : virtual public " + baseName;
                if (derivedText.find(':') != std::string::npos) {
                    size_t pos = derivedText.find(':');
                    derivedText.insert(pos, virtualBaseInsert);
                } else {
                    size_t pos = derivedText.find('{');
                    derivedText.insert(pos, virtualBaseInsert);
                }
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(derivedClass->getSourceRange()), derivedText);
            }
        }
    }
}

void MutatorFrontendAction_283::MutatorASTConsumer_283::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isClass()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isClass(), hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}