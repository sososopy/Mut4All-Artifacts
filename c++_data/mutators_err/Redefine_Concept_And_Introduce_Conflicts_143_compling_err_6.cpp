//header file
#pragma once
#include "Mutator_base.h"

/**
 * Redefine_Concept_And_Introduce_Conflicts_143
 */ 
class MutatorFrontendAction_143 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(143)

private:
    class MutatorASTConsumer_143 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_143(Rewriter &R) : TheRewriter(R) {}
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
        const ConceptDecl *OriginalConcept = nullptr;
        const FunctionTemplateDecl *FunctionTemplate = nullptr;
    };
};

//source file
#include "../include/redefine_concept_and_introduce_conflicts_143.h"

// ========================================================================================================
#define MUT143_OUTPUT 1

void MutatorFrontendAction_143::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
        //Filter nodes in header files
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
            return;
        
        // Record the original concept declaration
        OriginalConcept = CD;
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplates")) {
        //Filter nodes in header files
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
            return;
        
        // Record the function template using the concept
        FunctionTemplate = FTD;
    }

    if (OriginalConcept && FunctionTemplate) {
        //Get the source code text of target node
        auto conceptSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                     OriginalConcept->getSourceRange());
        auto functionSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                      FunctionTemplate->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        conceptSource += " && sizeof(T) > 1"; // Add a conflicting condition to the concept
        functionSource.insert(functionSource.find("{") + 1, "\n/*mut143*/requires MyConcept<U> && sizeof(T) < sizeof(U);\n");

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(OriginalConcept->getSourceRange()), conceptSource);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FunctionTemplate->getSourceRange()), functionSource);
    }
}
  
void MutatorFrontendAction_143::MutatorASTConsumer_143::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher conceptMatcher = clang::ast_matchers::conceptDecl().bind("Concepts");
    DeclarationMatcher functionTemplateMatcher = clang::ast_matchers::functionTemplateDecl().bind("FunctionTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.matchAST(Context);
}