//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_variadic_expansion_in_derived_classes_697
 */ 
class MutatorFrontendAction_697 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(697)

private:
    class MutatorASTConsumer_697 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_697(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> candidateClasses;
    };
};

//source file
#include "../include/template_variadic_expansion_in_derived_classes_697.h"

// ========================================================================================================
#define MUT697_OUTPUT 1

void MutatorFrontendAction_697::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;
        
        if (!DerivedClass->isCompleteDefinition())
            return;
        
        // Get the source code text of target node
        std::string classText = stringutils::rangetoStr(*(Result.SourceManager), DerivedClass->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        auto insertionPoint = classText.find_last_of('}');
        if (insertionPoint != std::string::npos) {
            std::string mutationText = "\npublic:\n    using NonVirtualBase::dummy;\n    void modify() {\n        this->Base2::m = 1;\n        this->Base3::m = 2;\n        this->Base1::m = 3;\n    }\n";
            classText.insert(insertionPoint, mutationText);
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DerivedClass->getSourceRange()), classText);
    }
}

void MutatorFrontendAction_697::MutatorASTConsumer_697::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isTemplateInstantiation(), hasAnyBase(isVirtualBaseClass())).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}