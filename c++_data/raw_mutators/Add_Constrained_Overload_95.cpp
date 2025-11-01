//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constrained_Overload_95
 */ 
class MutatorFrontendAction_95 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(95)

private:
    class MutatorASTConsumer_95 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_95(Rewriter &R) : TheRewriter(R) {}
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
        const CXXMethodDecl *targetMethod = nullptr;
        const CXXRecordDecl *targetClass = nullptr;
    };
};

//source file
#include "../include/add_constrained_overload_95.h"

// ========================================================================================================
#define MUT95_OUTPUT 1

void MutatorFrontendAction_95::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methods")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isOverloadedOperator() || MT->isDeleted() || MT->isDefaulted())
            return;

        targetMethod = MT;
        targetClass = dyn_cast<CXXRecordDecl>(MT->getParent());

        if (!targetClass || !targetClass->hasDefinition())
            return;

        // Get the source code text of target node
        auto methodSource = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string newConstraint = "\nvoid f() requires sizeof(T) > 4 { /*mut95*/ }\n";
        std::string insertText = "template<typename T>\nstruct " + targetClass->getNameAsString() + " {\n" + newConstraint + methodSource + "\n};\n";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(targetClass->getSourceRange(), insertText);
    }
}

void MutatorFrontendAction_95::MutatorASTConsumer_95::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isTemplateInstantiation()).bind("methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}