//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Virtual_Base_Class_354
 */ 
class MutatorFrontendAction_354 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(354)

private:
    class MutatorASTConsumer_354 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_354(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_virtual_base_class_354.h"

// ========================================================================================================
#define MUT354_OUTPUT 1

void MutatorFrontendAction_354::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;
        if (!BaseClass->isCompleteDefinition())
            return;
        baseClasses.insert(BaseClass->getNameAsString());
    } else if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;
        if (!DerivedClass->isCompleteDefinition())
            return;
        
        std::string insertionText = "\n/*mut354*/";
        for (const auto &baseName : baseClasses) {
            insertionText += "class " + baseName + ";\n";
        }
        
        std::string derivedName = DerivedClass->getNameAsString();
        insertionText += "class Mut354_" + derivedName + " : virtual public " + *baseClasses.begin() + " {};\n";
        
        Rewrite.InsertTextAfterToken(DerivedClass->getEndLoc(), insertionText);
    }
}

void MutatorFrontendAction_354::MutatorASTConsumer_354::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDerivedFrom(hasName("BaseClass"))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}