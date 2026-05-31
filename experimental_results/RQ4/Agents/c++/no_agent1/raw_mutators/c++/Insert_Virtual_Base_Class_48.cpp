//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_virtual_base_class_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(
                BaseClass->getLocation()))
            return;
        if ((!BaseClass->isStruct() && !BaseClass->isClass()) || BaseClass->isLambda())
            return;
        if (!BaseClass->isCompleteDefinition())
            return;

        std::string baseClassName = BaseClass->getNameAsString();
        if (processedClasses.find(baseClassName) != processedClasses.end())
            return;

        processedClasses.insert(baseClassName);

        if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
            if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(
                    DerivedClass->getLocation()))
                return;
            if ((!DerivedClass->isStruct() && !DerivedClass->isClass()) || DerivedClass->isLambda())
                return;
            if (!DerivedClass->isCompleteDefinition())
                return;

            std::string derivedClassName = DerivedClass->getNameAsString();
            if (processedClasses.find(derivedClassName) != processedClasses.end())
                return;

            processedClasses.insert(derivedClassName);

            std::string mutationText = " : virtual public " + baseClassName;
            Rewrite.InsertTextAfterToken(DerivedClass->getLocation(), mutationText);
        }
    }
}

void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}