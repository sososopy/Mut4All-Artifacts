//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_157
 */ 
class MutatorFrontendAction_157 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(157)

private:
    class MutatorASTConsumer_157 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_157(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl *> visitedClasses;
    };
};

//source file
#include "../include/Insert_Virtual_Base_Class_157.h"

// ========================================================================================================
#define MUT157_OUTPUT 1

void MutatorFrontendAction_157::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;
        if (!BaseClass->isCompleteDefinition() || BaseClass->isLambda())
            return;
        
        visitedClasses.insert(BaseClass);
    }
    else if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;
        if (!DerivedClass->isCompleteDefinition() || DerivedClass->isLambda())
            return;

        for (const auto *BaseClass : visitedClasses) {
            if (!DerivedClass->isDerivedFrom(BaseClass)) {
                std::string baseName = BaseClass->getNameAsString();
                std::string derivedName = DerivedClass->getNameAsString();
                std::string insertion = "virtual public " + baseName;
                std::string replacement = derivedName + " : " + insertion;

                auto range = DerivedClass->getSourceRange();
                std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
                size_t pos = originalText.find(derivedName);
                if (pos != std::string::npos) {
                    originalText.insert(pos + derivedName.length(), " : " + insertion);
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), originalText);
                }
            }
        }
    }
}

void MutatorFrontendAction_157::MutatorASTConsumer_157::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}